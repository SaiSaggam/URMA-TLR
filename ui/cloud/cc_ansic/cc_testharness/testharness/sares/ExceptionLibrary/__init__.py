import os

__all__ = []
for _file in os.listdir(os.path.dirname(os.path.realpath(__file__))):
    if not _file.startswith('_') and not _file.startswith('.'):
        # Is a *.py file
        if _file.endswith('.py'):
            __all__.append(_file[:-3])
            #  We can not use rstrip, it has a bug if the char before the '.' is 'y'
            #__all__.append(_file.rstrip('.py'))
        # Is a directory, probably is a module
        elif os.path.isdir(os.path.join(os.path.realpath(__file__), _file)):
            __all__.append(_file)

for elem in __all__:
    exec("import %s" % elem)
    exec('reload(%s)' % elem)


class Error(Exception):
    def __init__(self, message):
        self.error = message
    def __str__(self):
        return str("ExceptionLibraryError: %s" % self.error)
