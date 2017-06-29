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


import base64
import httplib
import xml.dom.minidom

log = logging.getLogger('cc.utils')
#log.setLevel(logging.INFO)

#FORMATTER = logging.Formatter('%(asctime)-15s %(name)-20s %(levelname)-8s %(message)s')

#handler = logging.StreamHandler()
#handler.setLevel(logging.INFO)
#handler.setFormatter(FORMATTER)
#log.addHandler(handler)

class ConnectivityException(Exception):
    pass



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



class FileDataCallback(object):

    def __init__(self, path):
        self.path = path
        self.event = Event()
        self.data = None

    def callback(self, data):
        self.data = json.loads(data)
        self.event.set()

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

        instance.assertNotEqual(None, cb.data, "Data not received for %s within wait time %d." % (file_location, wait_time))

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
