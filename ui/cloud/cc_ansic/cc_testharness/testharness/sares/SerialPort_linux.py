import time

class SerialPortClass(object):
    port = None
    baudrate = None
    handler = None

    def __init__(self, port, baudrate):
        self.handler = SerialPort(port, speed = baudrate)

    def write(self, command):
        return self.handler.write(command)

    def open(self):
        return (True,"")

    def setSessionOpenTimeout(self, timeout):
            return True

    def readAll(self):
        return self.handler.read(int(self.handler.inWaiting()))

    def readUntil(self, searchMessages, searchTimeout):
        readedOutput = ""
        limitTimeout = time.time() + searchTimeout
        while (time.time() < limitTimeout):
            readedOutput += self.readAll()
            for __i in range(len(searchMessages)):
                if (searchMessages[__i] in readedOutput):
                    return (__i, readedOutput)
        return (-1, readedOutput)

# -*- coding: iso-8859-1 -*-

##########################################################################
# USPP Library (Universal Serial Port Python Library)
#
# Copyright (C) 2006 Isaac Barona <ibarona@gmail.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##########################################################################

#------------------------------------------------------------------------
# Project:   USPP Library (Universal Serial Port Python Library)
# Name:      SerialPort_linux.py
# Purpose:   Handle low level access to serial port in linux.
#
# Author:    Isaac Barona Martinez <ibarona@gmail.com>
# Copyright: (c) 2006 by Isaac Barona Martnez
# Licence:   LGPL
#
# Created:   26 June 2001
# History:
# 20 January 2002 : Damien Geranton <dgeranton@voila.fr>
#  - NCCS worry fixed. We must not use TERMIOS
#  - inWaiting call fixed.
# 09 Sept 2005: Douglas Jones <dfj23@drexel.edu>
#  - readline method.
#
#-------------------------------------------------------------------------

"""
SerialPort_linux.py - Handle low level access to serial port in linux.

See also uspp module docstring.


"""

import os
from termios import *
import fcntl
import exceptions
import struct
import array
import threading
import signal

class SerialPortException(exceptions.Exception):
    """Exception raise in the SerialPort methods"""
    def __init__(self, args=None):
        self.args = args


def handler(signum, frame, timeout = 3, port = ""):
    aux = signum
    aux = frame
    del aux
    _error = '[timeout %s] Unable to open the serial port% s\n' % \
                                                        (timeout, port)
    raise SerialPortException((_error,))


"""
Classes to shared data for threads
"""
class shared_data():
    def __init__(self, value = None):
        self.shared_value = value

class do_eval_func(threading.Thread):
    def __init__(self, funct, sd_handler):
        threading.Thread.__init__(self)
        self.funct = funct
        self.sd_handler = sd_handler
    def run(self):
        self.sd_handler.shared_value = eval(self.funct)


def _open_port(dev):
    return os.open(dev, os.O_RDWR)

def _do_open_port(dev):
    return [_open_port(dev), ""]


def _open_port_with_thread_timeout(dev):
    sd_handler = shared_data(value = [-1, "Aborting uspp by timeout"])
    cmf = "_do_open_port('%s')" % dev
    ef_handel = do_eval_func(cmf, sd_handler)
    ef_handel.start()
    ef_handel.join(6)
    # Take the result after the timeout or after the thread finishes
    r = sd_handler.shared_value
    del sd_handler
    del ef_handel
    if r[0] == -1:
        handler("", "", 6, dev)
        return -1
    else:
        return r[0]


class SerialPort:
    """Encapsulate methods for accesing to a serial port."""

    BaudRatesDic={
        110: B110,
        300: B300,
        600: B600,
        1200: B1200,
        2400: B2400,
        4800: B4800,
        9600: B9600,
        19200: B19200,
        38400: B38400,
        57600: B57600,
        115200: B115200
        }
    buf = array.array('h', '\000'*4)

    def __init__(self, dev, timeout=None, speed=None, mode='232', params=None):
        """Open the serial port named by the string 'dev'

        'dev' can be any of the following strings: '/dev/ttyS0', '/dev/ttyS1',
        ..., '/dev/ttySX' or '/dev/cua0', '/dev/cua1', ..., '/dev/cuaX'.

        'timeout' specifies the inter-byte timeout or first byte timeout
        (in miliseconds) for all subsequent reads on SerialPort.
        If we specify None time-outs are not used for reading operations
        (blocking reading).
        If 'timeout' is 0 then reading operations are non-blocking. It
        specifies that the reading operation is to return inmediately
        with the bytes that have already been received, even if
        no bytes have been received.

        'speed' is an integer that specifies the input and output baud rate to
        use. Possible values are: 110, 300, 600, 1200, 2400, 4800, 9600,
        19200, 38400, 57600 and 115200.
        If None a default speed of 9600 bps is selected.

        'mode' specifies if we are using RS-232 or RS-485. The RS-485 mode
        is half duplex and use the RTS signal to indicate the
        direction of the communication (transmit or recive).
        Default to RS232 mode (at moment, only the RS-232 mode is
        implemented).

        'params' is a list that specifies properties of the serial
        communication.
        If params=None it uses default values for the number of bits
        per byte (8), the parity (NOPARITY) and the number of stop bits (1)
        else params is the termios package mode array to use for
        initialization.

        """
        self.__devName, self.__timeout, self.__speed=dev, timeout, speed
        self.__mode=mode
        self.__params=params

        """
        IHF: Two fixed:
            1. Avoid endless block using a signal alarm. Used sigalarm class
            2. Fixed recurrent failure. Under some conditions,
                normally when serial port was used in minicom session
                To fix the second problem, reconfigure clocal with stty
                  This reconfigure fails for a ttyUSBX
                    [-]clocal
                    disable modem control signals
        """
        _th = True
        try:
            signal.signal(signal.SIGALRM, handler)
            signal.alarm(3)
        except (ValueError):
            #ValueError('signal only works in main thread',)
            _th = False
        # Try to do something similar openning a thread
        #  If clocal is enabled, it is not able to open the port
        os.system("stty -F %s clocal" % dev)

        # This can be blocked endless
        if _th:
            self.__handle = _open_port(dev)
            signal.alarm(0)
        else:
            self.__handle = _open_port_with_thread_timeout(dev)
        self.__configure()


    def __del__(self):
        """Close the serial port and restore its initial configuration

        To close the serial port we have to do explicity: del s
        (where s is an instance of SerialPort)
        """

        ## IHF: If the device could not be oppened, this call will fail
        ##  due to an attibute error. Catch it and exit (nothing to close)
        try:
            tcsetattr(self.__handle, TCSANOW, self.__oldmode)
        except AttributeError:
            return

        try:
            os.close(self.__handle)
        except IOError:
            raise SerialPortException(('Unable to close port\n',))


    def __configure(self):
        """Configure the serial port.

        Private method called in the class constructor that configure the
        serial port with the characteristics given in the constructor.
        """
        if not self.__speed:
            self.__speed=9600

        # Save the initial port configuration
        self.__oldmode=tcgetattr(self.__handle)
        if not self.__params:
            # self.__params is a list of attributes of the file descriptor
            # self.__handle as follows:
            # [c_iflag, c_oflag, c_cflag, c_lflag, c_ispeed, c_ospeed, cc]
            # where cc is a list of the tty special characters.
            self.__params=[]
            # c_iflag
            self.__params.append(IGNPAR)
            # c_oflag
            self.__params.append(0)
            # c_cflag
            self.__params.append(CS8|CLOCAL|CREAD)
            # c_lflag
            self.__params.append(0)
            # c_ispeed
            self.__params.append(SerialPort.BaudRatesDic[self.__speed])
            # c_ospeed
            self.__params.append(SerialPort.BaudRatesDic[self.__speed])
            cc=[0]*NCCS
        if self.__timeout==None:
            # A reading is only complete when VMIN characters have
            # been received (blocking reading)
            cc[VMIN]=1
            cc[VTIME]=0
        elif self.__timeout==0:
            # Non-blocking reading. The reading operation returns
            # inmeditately, returning the characters waiting to
            # be read.
            cc[VMIN]=0
            cc[VTIME]=0
        else:
            # Time-out reading. For a reading to be correct
            # a character must be recieved in VTIME*100 seconds.
            cc[VMIN]=0
            cc[VTIME]=self.__timeout/100
        self.__params.append(cc)               # c_cc

        tcsetattr(self.__handle, TCSANOW, self.__params)


    def fileno(self):
        """Return the file descriptor for opened device.

        This information can be used for example with the
        select funcion.
        """
        return self.__handle


    def __read1(self):
        """Read 1 byte from the serial port.

        Generate an exception if no byte is read and self.timeout!=0
        because a timeout has expired.
        """
        byte = os.read(self.__handle, 1)
        if len(byte)==0 and self.__timeout!=0: # Time-out
            raise SerialPortException(('Timeout',))
        else:
            return byte


    def read(self, num=1):
        """Read num bytes from the serial port.

        Uses the private method __read1 to read num bytes. If an exception
        is generated in any of the calls to __read1 the exception is reraised.
        """
        s=''
        for i in range(num):
            s=s+SerialPort.__read1(self)

        return s


    def readline(self):
        """Read a line from the serial port.  Returns input once a '\n'
        character is found.
        Douglas Jones (dfj23@drexel.edu) 09/09/2005.
        """

        s = ''
        while not '\n' in s:
            s = s+SerialPort.__read1(self)

        return s


    def write(self, s):
        """Write the string s to the serial port"""

        os.write(self.__handle, s)


    def inWaiting(self):
        """Returns the number of bytes waiting to be read"""
        data = struct.pack("L", 0)
        data=fcntl.ioctl(self.__handle, TIOCINQ, data)
        return struct.unpack("L", data)[0]

    def outWaiting(self):
        """Returns the number of bytes waiting to be write
        mod. by J.Grauheding
        result needs some finetunning
        """
        rbuf=fcntl.ioctl(self.__handle, TIOCOUTQ, self.buf)
        return rbuf

    def getlsr(self):
        """Returns the status of the UART LSR Register
        J.Grauheding
        """
        rbuf=fcntl.ioctl(self.__handle, TIOCSERGETLSR, self.buf)
        return ord(rbuf[0])

    def get_temt(self):
        """Returns the Tranmitterbuffer Empty Bit of LSR Register
        J.Grauheding
        test result against TIOCSER_TEMT
        """
        rbuf=fcntl.ioctl(self.__handle, TIOCSERGETLSR, self.buf)
        return ord(rbuf[0]) & TIOSER_TEMT


    def flush(self):
        """Discards all bytes from the output or input buffer"""
        tcflush(self.__handle, TCIOFLUSH)

    def rts_on(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, SerialPort.buf)
        SerialPort.buf[1] = ord(rbuf[3]) | TIOCM_RTS
        rbuf = fcntl.ioctl(self.__handle, TIOCMSET, SerialPort.buf)
        return rbuf

    def rts_off(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, self.buf)
        self.buf[1]=ord(rbuf[3]) & ~TIOCM_RTS
        rbuf = fcntl.ioctl(self.__handle, TIOCMSET, self.buf)
        return rbuf

    def dtr_on(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, SerialPort.buf)
        SerialPort.buf[1] = ord(rbuf[3]) | TIOCM_DTR
        rbuf = fcntl.ioctl(self.__handle, TIOCMSET, SerialPort.buf)
        return rbuf

    def dtr_off(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, self.buf)
        self.buf[1]=ord(rbuf[3]) & ~TIOCM_DTR
        rbuf = fcntl.ioctl(self.__handle, TIOCMSET, self.buf)
        return rbuf

    def cts(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, self.buf)
        return ord(rbuf[3]) & TIOCM_CTS

    def cd(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, self.buf)
        return ord(rbuf[3]) & TIOCM_CAR

    def dsr(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, self.buf)
        return ord(rbuf[2]) & (TIOCM_DSR>>8)

    def ri(self):
        """ J.Grauheding """
        rbuf = fcntl.ioctl(self.__handle, TIOCMGET, self.buf)
        return ord(rbuf[3]) & TIOCM_RNG
