import time

class FileReader(object):
    path = None
    handler = None

    def __init__(self, path):
        self.path = path

    def write(self, command):
        return False

    def open(self):
        self.handler = open(self.path, 'r')
        return (True,"")

    def close(self):
        self.handler.close()

    def setSessionOpenTimeout(self, timeout):
        return True

    def readAll(self):
        content = self.handler.readlines()
        return "".join(content)

    def readUntil(self, searchMessages, searchTimeout):
        readedOutput = ""
        limitTimeout = time.time() + searchTimeout
        while (time.time() < limitTimeout):
            readedOutput += self.readAll()
            for __i in range(len(searchMessages)):
                if (searchMessages[__i] in readedOutput):
                    return (__i, readedOutput)
        return (-1, readedOutput)



