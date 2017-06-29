import ddp_if
import time
from datetime import datetime

token = "!digi#ddpv1!"

def callback_fn(xml_str):
    print xml_str + '\n'
    dt1 = datetime.now()
    print("Time: %d" %dt1.second)

def test_poll():
    # return ddp_if.poll(token, "eth", 1, 2, callback_fn)
    # ddp_if.poll(token, "firmware_files", 1, 0, callback_fn)
    return ddp_if.poll(token, "firmware_status", 1, 1, callback_fn)

def test_cancel_poll(id):
    result = ddp_if.cancel_poll(token, id)
    if result:
        print("Cancel success\n")
    else:
        print("Cancel failed\n")

def test_fw_download():
    result = ddp_if.update_firmware(token, "urma", "lr54-0.0.0.1.bin", 1)
    if result:
        print("update success\n")
    else:
        print("update failed\n")

if __name__ == '__main__':
    thread_id = test_poll()
    print("Thread id: %ld" %thread_id)
    test_fw_download()
    time.sleep(2)
    test_cancel_poll(thread_id)
    print "Done"

