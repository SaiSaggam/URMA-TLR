#!/usr/bin/python
import ddp_if
import sys, getopt

def main(argv):
   group_name = argv[0]
   instance = int(argv[1])
   element_name = argv[2]
   element_value = argv[3]
   token = os.environ.get('CONFIG_MANAGER_API_TOKEN')

   result = ddp_if.set(token, "urma", group_name, instance, element_name, element_value, True)
   if result:
      print "Success"
   else:
      print "Failed"

if __name__ == "__main__":
   main(sys.argv[1:])

