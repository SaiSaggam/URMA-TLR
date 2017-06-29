#!/usr/bin/python

import sys
import re
import os
import itertools
import magic
import subprocess
import tempfile
import argparse
from curses.ascii import isprint

fd, tmp = tempfile.mkstemp()
os.close(fd)
command = ['/usr/bin/spell', '-d', 'validate.dict', tmp]

def leave(message):
    sys.stderr.write(message)
    if args.stop_on_error:
        sys.exit(2)

def spellcheck(text):
    with open(tmp, 'w') as check:
        check.write(text)

    spell = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = spell.communicate()
    if stderr:
        leave(stderr)

    return ', '.join(sorted(set(filter(lambda w: not w.isupper(), stdout.split()))))

class counters():
    def __init__(self):
        self.data = {}

    def count(self):
        return sum(self.data.values())

    def incr(self, name, amount=1):
        assert(amount > 0)
        self.data[name] = self.data.setdefault(name, 0) + amount

    def remove(self, name):
        if name in self.data:
            del self.data[name]

    def add(self, other):
        for name in other.data.keys():
            self.incr(name, other.data[name])

    def items(self):
        result = []
        for name, value in self.data.items():
            result.append('{}: {}'.format(name, value))

        return ', '.join(result)

class parse_any(object):
    def __init__(self, path, allow_tabs=False, window_size=2):
        class sliding_window(object):
            def __init__(self, size):
                self.size = size
                self.reset()

            def reset(self):
                self.win = '\0' * self.size

            def add(self, char):
                assert(len(char) == 1)
                self.win = self.win[1:] + char
                assert(len(self.win) == self.size)

            def matches(self, needle):
                assert(len(needle) <= len(self.win))
                return self.win.endswith(needle)

        self.path = path
        self.allow_tabs = allow_tabs
        self.first_line = True;
        self.text = ''
        self.error = counters()
        self.url_preface = 'http://'
        self.in_url = False;
        self.window = sliding_window(max(window_size, len(self.url_preface)))
        if args.verbose:
            self.output(None)

    def is_valid(self, window):
        raise NotImplementedError

    def output(self, message):
        if self.first_line:
            print '{}:'.format(self.path)
            self.first_line = False
        if message:
            print message

    def eol(self, white_count, line):
        if white_count > 0:
            print line.number, white_count
            self.error.incr('eol_ws')
            white_count = 0

        text = ''.join(line.content)
        if (len(text) != 0) and (not text.isspace()):
            if args.show_by == 'line':
                words = spellcheck(text)
                if words:
                    self.output('{}: {}'.format(line.number, words))
                    self.error.incr('spell', len(words))
            else:
                self.text = '\n'.join([self.text, text])

        del line.content[:]
        line.number += 1

        self.window.reset()

    def space(self, white_count, line):
        if white_count == 0:
            line.content.append(' ')
        white_count += 1

    def report(self):
        if args.show_by == 'file':
            words = spellcheck(self.text)
            if words:
                self.output(words)
                self.error.incr('spell', len(words))

        errors = (self.error.count() != 0)
        if args.show_by == 'summary':
            global summary
            summary.text = '\n'.join([summary.text, self.text])
            summary.error.add(self.error)
        else:
            if errors:
                self.output(self.error.items())

        return (not errors)

    def parse(self):
        def tab(white_count, line):
            self.space(white_count, line)
            if not self.allow_tabs:
                self.error.incr('tab')

        def carriage_return(white_count, line):
            self.error.incr('eol-cr')
            self.eol(white_count, line)

        def line_feed(white_count, line):
            if not self.window.matches('\r\n'):
                self.eol(white_count, line)

        class line():
            def invalidate(self, string):
                remove = -len(string)
                if (string == ''.join(self.content[remove:])):
                    del self.content[remove:]

        line = line()
        line.number = 1
        line.content = []

        white_count = 0
        whitespace = {' ': self.space, '\t': tab, '\r': carriage_return, '\n': line_feed}
        for char in open(self.path, 'rb').read():
            self.window.add(char)
            try:
                whitespace[char](white_count, line)
                self.in_url = False
            except KeyError:
                if not isprint(char):
                    self.error.incr('non-ascii')
                if self.window.matches(self.url_preface):
                    line.invalidate(self.url_preface[:-1])
                    self.in_url = True
                if not self.in_url and self.is_valid(self.window):
                    line.content.append(char)
                white_count = 0
        if line.content:
            self.eol(white_count, line)
        return self.report()

# TODO: cannot handle multi-line comments (a '\' at the end-of-line) or detect when hash is inside a string
class parse_hash_comment(parse_any):
    def __init__(self, path, allow_tabs=False, window_size=2):
        super(parse_hash_comment, self).__init__(path, allow_tabs, window_size)
        self.in_hash_comment = False

    def is_valid(self, window):
        if window.matches('#') and not window.matches('\\#'):
            self.in_hash_comment = True

        return self.in_hash_comment

    def eol(self, white_count, line):
        super(parse_hash_comment, self).eol(white_count, line)
        self.in_hash_comment = False

class parse_c(parse_any):
    def __init__(self, path):
        self.cpp_comment_count = 0

        self.include_preface = '#include'
        super(parse_c, self).__init__(path, window_size=len(self.include_preface))

        self.in_c_comment = False
        self.in_cpp_comment = False
        self.in_quote = False
        self.is_include = False

    def is_valid(self, window):
        if self.in_c_comment:
            if window.matches('*/'):
                self.in_c_comment = False
        elif self.in_cpp_comment:
            pass # good until end-of-line
        elif self.in_quote:
            if window.matches('"') and not window.matches('\\"'):
                self.in_quote = False
                self.is_include = False
        elif window.matches(self.include_preface):
            self.is_include = True
        else:
            if window.matches('/*'):
                self.in_c_comment = True
            elif window.matches('\\\\'):
                self.in_cpp_comment = True
                self.error.incr('cpp-\\\\')
            elif window.matches('"') and not window.matches('\\"'):
                self.in_quote = True
            else:
                pass

        return self.in_c_comment or self.in_cpp_comment or (self.in_quote and not self.is_include)

    def eol(self, white_count, line):
        super(parse_c, self).eol(white_count, line)
        self.in_cpp_comment = False
        self.is_include = False

    def report(self):
        super(parse_c, self).report()

class parse_python(parse_hash_comment):
    def __init__(self, path):
        super(parse_python, self).__init__(path, window_size=4)
        self.quote_type = None
        self.quote_match = {
            'single': ("'", "\\'"),
            'double': ('"', '\\"'),
            '3singles': ("'''", "\\'''"),
            '3doubles': ('"""', '\\"""')
        }
        self.quote_types = sorted(self.quote_match.keys()) # The 3's need to come first, the match needs to be greedy.

    def is_valid(self, window):
        def matches(window, quote_type):
            success, failure = self.quote_match[quote_type]
            return window.matches(success) and not window.matches(failure)

        in_comment = False
        if self.quote_type:
            if matches(window, self.quote_type):
                self.quote_type = None
        elif super(parse_python, self).is_valid(window):
            in_comment = True
        else:
            for quote_type in self.quote_types:
                if matches(window, quote_type):
                    self.quote_type = quote_type
                    break

        return in_comment or self.quote_type

class parse_rci(parse_hash_comment):
    def __init__(self, path):
        super(parse_rci, self).__init__(path)
        self.in_quote = False

    def is_valid(self, window):
        in_comment = False
        if self.in_quote:
            if window.matches('"') and not window.matches('\\"'):
                self.in_quote = False
        elif super(parse_rci, self).is_valid(window):
            in_comment = True
        elif window.matches('"') and not window.matches('\\"'):
            self.in_quote = True
        else:
            pass

        return in_comment or self.in_quote

class parse_text(parse_any):
    def is_valid(self, window):
        return True

class parse_makefile(parse_hash_comment):
    def __init__(self, path):
        super(parse_makefile, self).__init__(path, allow_tabs=True)

parse = {
    r'\.c$': parse_c,
    r'\.c_aux$': parse_c,
    r'\.c.optional$': parse_c,
    r'\.h$': parse_c,
    r'\.py$': parse_python,
    r'\.rci$': parse_rci,
    r'/Readme.txt$': parse_text,
    r'/[mM]akefile$': parse_makefile
}
skip = [
    r'\.css$',
    r'\.d$',
    r'\.html$',
    r'\.js$',
    r'\.map$',
    r'\.xml$',
    r'/Doxyfile$'
]

def match(haystack, needle):
    result = None
    for rx in haystack:
        if rx.search(needle):
            result = rx.pattern
            break
    return result

include = map(re.compile, parse.keys())
def included(path):
    return match(include, path)

exclude = map(re.compile, skip)
def excluded(path):
    return match(exclude, path)

mage = magic.Magic()
def is_human(path):
    magic = mage.from_file(path)

    try:
        magic.index('ASCII')
        magic.index('text')
        human = True
    except ValueError:
        human = False

    return human

def validate_file(path):
    valid = included(path) in parse
    if not valid:
        if not is_human(path):
            if args.verbose:
                sys.stderr.write('non-human file: {}\n'.format(path))
        elif excluded(path):
            if args.verbose:
                sys.stderr.write('excluded file: {}\n'.format(path))
        else:
            leave('unknown file: {}\n'.format(path))

    return valid

def validate_dir(dir):
    files = set()
    for entry in os.listdir(dir):
        path = os.path.join(dir, entry)
        if os.path.isdir(path):
            files |= validate_dir(path)
        else:
            if validate_file(path):
                files.add(path)
    return files

default_scan = ['doxygen', 'private', 'public']
parser = argparse.ArgumentParser(description='Validates tabbing, line endings, and spelling.')
parser.add_argument('--verbose', action='store_true', help='show all files even if skipped or no errors')
parser.add_argument('--show-by', help='show spelling errors by line, file, or summary [default: file]', choices=['line', 'file', 'summary'], default='file', metavar='TYPE')
parser.add_argument('--stop-on-error', action='store_true', help='stop on first error')
parser.add_argument('scan', nargs='*', help='files or directories to scan [default: {}]'.format(' '.join(default_scan)), default=default_scan, metavar='PATH')
args = parser.parse_args()

files = set()
for path in args.scan:
    try:
        if os.path.isdir(path):
            files |= validate_dir(path)
        else:
            if validate_file(path):
                files.add(path)
    except IOError:
        leave('bad path: {}\n'.format(path))

if args.show_by == 'summary':
    global summary
    def summary(): pass
    summary.text = ''
    summary.error = counters()

exit_code = 0
for path in sorted(files):
    if not parse[included(path)](path).parse():
        exit_code = 1
        if args.stop_on_error:
            break

if args.show_by == 'summary':
    words = spellcheck(summary.text)
    if words:
        print words

    if summary.error.count():
        print summary.error.items()

sys.exit(exit_code)
