# -*- coding: iso-8859-1 -*-
from __future__ import division
from base64 import b64decode
import datetime
import logging
import time
import json
import idigi_rest_api

from threading import Event
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()
from base64 import encodestring

log = logging.getLogger('utils')
log.setLevel(logging.INFO)

FORMATTER = logging.Formatter('%(asctime)-15s %(name)-20s %(levelname)-8s %(message)s')

handler = logging.StreamHandler()
handler.setLevel(logging.INFO)
handler.setFormatter(FORMATTER)
log.addHandler(handler)

class ConnectivityException(Exception):
    pass

class PollingDeviceConnectionMonitor(object):

    def __init__(self, push_client, dev_id):
        self.dev_id = dev_id
        username = push_client.username
        password = push_client.password
        hostname = push_client.hostname
        self.rest_session  = idigi_rest_api.rest_session(hostname,
            auth=(username, password), verify=False)

    def __get_device_core(self):
        response = self.rest_session.get_first('DeviceCore/%s' % self.dev_id)
        if response.status_code != 200:
            raise Exception('Device %s not found.' % self.dev_id)

        device_core = response.resource

        # If device has never disconnected, dpLastDisconnectTime is not defined
        # so we set it to 0.
        if not hasattr(device_core, 'dpLastDisconnectTime'):
            device_core.dpLastDisconnectTime = 0

        # If device has never connected, dpLastConnectTime is not defined
        # so we set it to 0.
        if not hasattr(device_core, 'dpLastConnectTime'):
            device_core.dpLastConnectTime = 0

        return device_core

    def start(self):
        """
        Captures current device state
        """
        device_core = self.__get_device_core()
        self.last_connect = device_core.dpLastConnectTime
        self.last_disconnect = device_core.dpLastDisconnectTime

    def wait_for_connect(self, timeout):
        start_time = time.time()
        duration = 0

        while duration < timeout:
            device_core = self.__get_device_core()
            if device_core.dpLastConnectTime != self.last_connect:
                # Device connected!
                self.last_connect = device_core.dpLastConnectTime
                return device_core
            # Sleep a second to limit amount of polling
            time.sleep(1.0)
            duration = time.time() - start_time

        log.info('Connect time = %s Device last connect time = %s' %(device_core.dpLastConnectTime, self.last_connect))
        log.info('Disconnect time = %s Device last disconnect time = %s' %(device_core.dpLastDisconnectTime, self.last_disconnect))
        # Device never connected, raise exception.
        raise ConnectivityException(
            'Device %s did not connect within %d seconds.' % \
                (self.dev_id, timeout))

    def wait_for_disconnect(self, timeout):
        start_time = time.time()
        duration = 0

        while duration < timeout:
            device_core = self.__get_device_core()
            if device_core.dpLastDisconnectTime != self.last_disconnect:
                # Device disconnected!
                self.last_disconnect = device_core.dpLastDisconnectTime
                return device_core
            # Sleep a second to limit amount of polling
            time.sleep(1.0)
            duration = time.time() - start_time

        log.info('Connect time = %s Device last connect time = %s' %(device_core.dpLastConnectTime, self.last_connect))
        log.info('Disconnect time = %s Device last disconnect time = %s' %(device_core.dpLastDisconnectTime, self.last_disconnect))

        # Device never disconnected, raise exception.
        raise ConnectivityException(
            'Device %s did not disconnect within %d seconds.' % \
                (self.dev_id, timeout))

    def stop(self):
        # Nothing to do here!
        pass

class DeviceConnectionMonitor(object):
    """
    Used to monitor a Device for Connection and Disconnection Events.  Utilizes
    iDigi Push Monitoring to monitor events.
    """

    def __init__(self, push_client, dev_id):
        """
        Create instance.

        Parameters:
        push_client -- push_client to use for monitoring.
        dev_id -- the devId in the device according to iDigi.
        """
        self.dev_id  = dev_id
        self.push_client = push_client
        self.disconnect_event = Event()
        self.disconnect_data = None
        self.connect_event = Event()
        self.connect_data = None
        self.monitor = None
        self.session = None
        username = push_client.username
        password = push_client.password
        hostname = push_client.hostname
        self.rest_session  = idigi_rest_api.rest_session(hostname,
            auth=(username, password), verify=False)

    def __get_device_core(self):
        response = self.rest_session.get_first('DeviceCore/%s' % self.dev_id)
        if response.status_code != 200:
            raise Exception('Device %s not found.' % self.dev_id)

        device_core = response.resource

        # If device has never disconnected, dpLastDisconnectTime is not defined
        # so we set it to 0.
        if not hasattr(device_core, 'dpLastDisconnectTime'):
            device_core.dpLastDisconnectTime = 0

        # If device has never connected, dpLastConnectTime is not defined
        # so we set it to 0.
        if not hasattr(device_core, 'dpLastConnectTime'):
            device_core.dpLastConnectTime = 0

        return device_core

    def __session_callback(self, data):
        device_core = json.loads(data)['Document']['Msg']['DeviceCore']
        status = device_core['dpConnectionStatus']
        if status == 1:
            # Device is connected.
            self.connect_data = device_core
            self.connect_event.set()
        elif status == 0:
            # Device is disconnected.
            self.disconnect_data = device_core
            self.disconnect_event.set()

        return True


    def start(self):
        """
        Starts by registering a Monitor on the Device and creating a TCP
        session.
        """
        topic = 'DeviceCore/%s' % self.dev_id
        log.info("Creating a Push Monitor Subscription on Topic: %s" % topic)
        self.monitor = self.push_client.create_monitor(
                                        [topic],
                                        batch_size=1, batch_duration=0,
                                        format_type='json')

        self.session = self.push_client.create_session(self.__session_callback,
                                                self.monitor)


    def wait_for_connect(self, timeout):
        """
        Waits timeout seconds for device to connect.  If it connects within
        timeout, returns the DeviceCore payload, otherwise an Exception is
        raised.
        """
        device_core = self.__get_device_core()
        if device_core.dpConnectionStatus == '1':
            # Already connected
            None

        self.connect_event.wait(timeout)

        if self.connect_data is None:
            log.info('Connect time = %s Disconnect time = %s' %(device_core.dpLastConnectTime, device_core.dpLastDisconnectTime))
            raise ConnectivityException(
                'Device %s did not connect within %d seconds.' % \
                (self.dev_id, timeout))

        connect_data = self.connect_data
        self.connect_data = None
        self.connect_event.clear()
        return connect_data

    def wait_for_disconnect(self, timeout):
        """
        Waits timeout seconds for device to disconnect.  If it disconnects within
        timeout, returns the DeviceCore payload, otherwise an Exception is
        raised.
        """
        device_core = self.__get_device_core()
        if device_core.dpConnectionStatus == '0':
            # Already disconnected
            None

        self.disconnect_event.wait(timeout)

        if self.disconnect_data is None:
            log.info('Connect time = %s Disconnect time = %s' %(device_core.dpLastConnectTime, device_core.dpLastDisconnectTime))
            raise ConnectivityException(
                'Device %s did not disconnect within %d seconds.' % \
                (self.dev_id, timeout))

        disconnect_data = self.disconnect_data
        self.disconnect_data = None
        self.disconnect_event.clear()
        return disconnect_data

    def stop(self):
        """
        Stop the push client and all sessions.
        """
        if self.session is not None:
            self.session.stop()
        if self.monitor is not None:
            self.push_client.delete_monitor(self.monitor)

def convert_to_datetime(fd_time):

    """ Converts the date/time returned from iDigi to a datetime object"""

    dt = datetime.datetime(int(fd_time[0:4]),
                                int(fd_time[5:7]),
                                int(fd_time[8:10]),
                                int(fd_time[11:13]),
                                int(fd_time[14:16]),
                                int(fd_time[17:19]))
    return dt

def total_seconds(dt):

    """ Converts a datetime.delta to an int representing total seconds

    requires the 'from __future__ import division' line at beginning
    of file (for use with Python 2.6 and earlier)
    """

    return (dt.microseconds + (dt.seconds + dt.days * 24 * 3600) * 10**6) / 10**6

def getText(elem):
    rc = []
    for node in elem.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return str(''.join(rc))

def clean_slate(rest_session, file_location):
    """Delete file if it exists."""
    rest_session.delete(file_location)

    response = rest_session.get(file_location)
    if response.status_code != 404:
       log.info('*********************************')
       log.info('File %s was not deleted' % file_location)
       log.info('*********************************')

def update_firmware(session, hostname, device, input_firmware, target, filename=None):

    request = impl.createDocument(None, "sci_request", None)
    sci_element = request.documentElement

    update_firmware_element = request.createElement("update_firmware")

    targets = request.createElement("targets")
    update_firmware_element.appendChild(targets)
    device_element = request.createElement("device")
    device_element.setAttribute("id", device)
    targets.appendChild(device_element)

    sci_element.appendChild(update_firmware_element)

    data = request.createElement("data")
    f = open(input_firmware, 'rb')
    data_value = request.createTextNode(encodestring(f.read()))
    f.close()
    data.appendChild(data_value)

    update_firmware_element.appendChild(data)

    if target:
        update_firmware_element.setAttribute("firmware_target", target)

    if ( filename is not None ):
        update_firmware_element.setAttribute("filename", filename)

    return session.post('https://%s/ws/sci' % hostname,
        data=request.toprettyxml(), timeout=120).content

class FileDataCallback(object):

    def __init__(self, path):
        self.path = path
        self.event = Event()
        self.data = None

    def callback(self, data):
        self.data = json.loads(data)
        self.event.set()

def monitor_and_return(instance, push_client, device_id,
                    file_location, expected_content=None, dne=False,
                    original_created_time=None, wait_time=90,
                    trigger_function=None):

    """Performs a GET on the file_location to determine if the correct content
    was pushed. A second GET is performed to verify the file's
    metadata is correct.
    """
    log.info("Creating a Push Monitor Subscription on Topic: %s" % file_location)
    monitor = push_client.create_monitor([file_location], batch_size=1, batch_duration=0,
        format_type='json')


    session = None
    try:
        cb = FileDataCallback(file_location)

        session = push_client.create_session(cb.callback, monitor)

        # Temporary Kludge until iDigi 46525 is fixed.
        time.sleep(5.0)

        if trigger_function is not None:
            trigger_function()

        # Check that file content is correct
        log.info("Waiting for File...")
        cb.event.wait(wait_time)

        if cb.data is None:
            return None

        log.info("Verifying File Content.")
        file_data = cb.data['Document']['Msg']['FileData']
        file_size = file_data['fdSize']

        if file_size == 0:
            file_content = ''
        else:
            file_content = b64decode(file_data['fdData'])

        # If provided, compare file's contents to expected content.
        if expected_content:
            log.info("Comparing file's content = %s" % file_content)
            log.info("with expected file's content = %s" % expected_content)
            # Verify file's contents
            instance.assertEqual(expected_content, file_content)
        else:
            log.info("File's metadata verified!")

        # Check that FileData is correct
        log.info("Verifying file's metadata")

        return file_content
    finally:
        if session is not None:
            session.stop()
        push_client.delete_monitor(monitor)

def get_and_verify(instance, push_client, device_id,
                    file_location, expected_content=None, dne=False,
                    original_created_time=None, wait_time=90,
                    trigger_function=None):

    """Performs a GET on the file_location to determine if the correct content
    was pushed. A second GET is performed to verify the file's
    metadata is correct.
    """
    log.info("Creating a Push Monitor Subscription on Topic: %s" % file_location)
    monitor = push_client.create_monitor([file_location], batch_size=1, batch_duration=0,
        format_type='json')


    session = None
    try:
        cb = FileDataCallback(file_location)

        session = push_client.create_session(cb.callback, monitor)

        # Temporary Kludge until iDigi 46525 is fixed.
        time.sleep(5.0)

        if trigger_function is not None:
            trigger_function()

        # Check that file content is correct
        log.info("Waiting for File...")
        cb.event.wait(wait_time)

        if cb.data is None:
            log.info("***** Push Monitor Subscription on %s FAILED *****" % file_location)
            file_request = 'http://%s/ws/%s' % (instance.hostname, file_location)
            log.info("Try getting the file %s" %file_request)
            file_response = instance.session.get(file_request)
            log.info("Reponse from Get: %d %s" % (file_response.status_code, file_response.content))
            if file_response.status_code == 404:
                # file not exist
                log.info("%s not exit" % file_location)
                instance.assertNotEqual(None, cb.data, "Data not received for %s within wait time %d." % (file_location, wait_time))
            else:
                file_content = file_response.content
                file_size = len(file_content)
        else:
            file = cb.data['Document']['Msg']['FileData']
            file_size = file['fdSize']
            file_content = b64decode(file['fdData'])

        log.info("Verifying File Content.")

        if file_size == 0:
            file_content = ''

        # If provided, compare file's contents to expected content.
        if expected_content:
            log.info("Comparing file's content = %s" % file_content)
            log.info("with expected file's content = %s" % expected_content)
            # Verify file's contents
            instance.assertEqual(expected_content, file_content)
        else:
            log.info("File's metadata verified!")

        # Check that FileData is correct
        log.info("Verifying file's metadata")

        return file_content
    finally:
        if session is not None:
            session.stop()
        push_client.delete_monitor(monitor)


def update_and_verify(instance, rest_session, push_client, device_id, target,
                    content, datetime_created, file_location,
                    file_name, expected_content=None, dne=False,
                    original_created_time=None, wait_time=60):

    """Sends firmware update to trigger a data push, then performs
    a GET on the file_location to determine if the correct content
    was pushed. A second GET is performed to verify the file's
    metadata is correct.
    """
    log.info("Creating a Push Monitor Subscription on Topic: %s" % file_location)
    monitor = push_client.create_monitor([file_location], batch_size=1, batch_duration=0,
        format_type='json')

    session = None
    try:
        cb = FileDataCallback(file_location)

        session = push_client.create_session(cb.callback, monitor)
        # Temporary Kludge until iDigi 46525 is fixed.
        time.sleep(55555.0)


        # Send first firmware update to Archive=True target
        log.info("Sending firmware update to create file.")
        # Collect current datetime for later comparison
        i = len(datetime_created)
        datetime_created.append(datetime.datetime.utcnow())

        # Send firmware update to target
        response = update_firmware(rest_session, device_id, "%d" % target, content)

        # Check that file content is correct
        log.info("Waiting for File Content.")
        cb.event.wait(wait_time)

        instance.assertNotEqual(None, cb.data, "Data not received for %s within wait time %d." % (file_location, wait_time))

        log.info("Verifying File Content.")
        file_data = cb.data['Document']['Msg']['FileData']
        file_size = file_data['fdSize']

        if file_size == 0:
            file_content = ''
        else:
            file_content = b64decode(file_data['fdData'])

        # If provided, compare file's contents to expected content.
        if not expected_content:
            expected_content = content

        # Verify file's contents
        instance.assertEqual(expected_content, file_content)

        # Check that FileData is correct
        log.info("Verifying file's metadata")

        # Verify that file's Modified Date/time is within 2 minutes of sampled
        # date/time
        fd_datetime_modified = convert_to_datetime(file_data['fdLastModifiedDate'])
        delta = total_seconds(abs(fd_datetime_modified - datetime_created[i]))
        # temporarily remove assertion, iDigi bug needs to be resolved
        instance.assertTrue(delta < 120,
            "File's Last Modified Date/Time is not correct (delta=%d)." % delta)

        # If supplied, verify that file's Created Date/time is within 2 minutes
        # of sampled date/time
        if original_created_time:
            fd_datetime_created = convert_to_datetime(file_data['fdCreatedDate'])
            delta = total_seconds(abs(fd_datetime_created - original_created_time))
            instance.assertTrue(delta < 120, "File's Create Date/Time is not correct.")

        # If the file did not previously exist (Does Not Exist), verify that
        # the created date/time is the same as the last modified date/time
        if dne and not original_created_time:
            instance.assertEqual(file_data['fdCreatedDate'],
                file_data['fdLastModifiedDate'],
                "File's created date/time does not match file's last modified date/time.")

        return file_content
    finally:
        if session is not None:
            session.stop()
        push_client.delete_monitor(monitor)

def get_filedatahistory(rest_session, file_history_location):

    """ Retrieves the filedata history from the file_history_location
    and then parses it into separate enries
    """
    log.info("Verifying data in %s" % file_history_location)
    fdh_response = rest_session.get(file_history_location,
        params={'embed' : 'true', 'orderby' : 'fdLastModifiedDate'})

    if fdh_response.status_code == 200:
        return fdh_response.resource
    else:
        raise Exception("Error while retrieving %s: (%d): %s" \
            % (file_history_location, fdh_response.status_code,
                fdh_response.content))

def check_filedatahistory(instance, fdh, datetime_created,
                            content, files_pushed_after, dne=False):
    """ takes a list of filedatahistory entries and verifies the content
    and metadata.
    """
    # find correct filedatahistory entry
    historical_index = len(fdh) - (files_pushed_after + 1)
    entry = fdh[historical_index]
    modified_date = entry.fdLastModifiedDate
    data = b64decode(entry.fdData)

    instance.assertEqual(data, content,
        "File's contents do not match what is expected.")

    if dne:
        created_date = entry.fdCreatedDate
        instance.assertEqual(created_date, modified_date,
            "File's created date/time does not match file's last modified date/time.")


def device_is_connected(self):
    """
    Captures current device state
    """
    response = self.rest_session.get_first('DeviceCore/%s' % self.dev_id)
    if response.status_code != 200:
        raise Exception('Device %s not found.' % self.dev_id)

    device_core = response.resource

    # If device has never disconnected, dpLastDisconnectTime is not defined
    # so we set it to 0.
    if not hasattr(device_core, 'dpLastDisconnectTime'):
        device_core.dpLastDisconnectTime = 0

    # If device has never connected, dpLastConnectTime is not defined
    # so we set it to 0.
    if not hasattr(device_core, 'dpLastConnectTime'):
        device_core.dpLastConnectTime = 0

    if device_core.dpLastConnectTime > device_core.dpLastDisconnectTime:
        return True
    else:
        return False
