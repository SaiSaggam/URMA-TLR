import os
import ddp_if

def callback_fn(xml_str):
    print xml_str

def test_init():
    token = os.environ.get('CONFIG_MANAGER_API_TOKEN')
    ddp_if.init(token, callback_fn)
    print token

if __name__ == '__main__':
    test_init()
