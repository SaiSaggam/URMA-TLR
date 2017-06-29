import ddp_if
from datetime import datetime

token = "!digi#ddpv1!"

def callback_fn(xml_str):
    #print xml_str + '\n'
    test = 0


def test_init():
    dt1 = datetime.now()
    result = ddp_if.init(token, callback_fn)
    dt2 = datetime.now()
    print("Before: %d.%d\n" %(dt1.second, dt1.microsecond))
    print("After: %d.%d\n" %(dt2.second, dt2.microsecond))
    print("Delta: %d\n" %((dt2.second - dt1.second) * 1000000 + dt2.microsecond - dt1.microsecond))

def test_set():
    result = ddp_if.set(token, "admin", "eth", 1, "description", "Setting from Web1", True)
    print ("set result %d" %result)

def test_poll():
    ddp_if.poll(token, "eth", 1, 1, callback_fn)    

if __name__ == '__main__':
    test_init()
    #test_set()
    #test_poll()
