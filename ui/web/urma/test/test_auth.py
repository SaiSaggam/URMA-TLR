import os
import ddp_if

def test_auth():
    token = os.environ.get('CONFIG_MANAGER_API_TOKEN')
    print "checking valid user..."
    result = ddp_if.authenticate(token, "user1", "test1")
    print ("access: %d" %result)
    print "checkong intruder..."
    result = ddp_if.authenticate(token, "user1", "test")
    print ("access: %d" %result)

if __name__ == '__main__':
    test_auth()
