
import fileinput, re, string, sys, os
from optparse import OptionParser

if __name__ == '__main__':
  parser = OptionParser(usage = "usage: %prog file_name src_str dst_str")
  (options, args) = parser.parse_args()

  infile = open(args[0], "r")
  text = infile.read()
  infile.close()

  myStrVer=str.split(args[1], '.')
  if len(myStrVer) == 4:
     # only come here if there is a 4 tuple tag
     myHexVer = "0x%02x%02x%02x%02x" % (int(myStrVer[0]), int(myStrVer[1]), int(myStrVer[2]), int(myStrVer[3]))
     myReplacement = "#define CONNECTOR_VERSION    " + "UINT32_C(" + myHexVer + ")"
     mySearchPattern = '#define\s+CONNECTOR_VERSION\s+0x\w+'

     newtext = re.sub(mySearchPattern, myReplacement, text, 1) 

     print ">> Writing " + myReplacement + " instead of " + mySearchPattern + " to " + args[0] 
     outfile = open(args[0], "w")
     outfile.write(newtext)
     outfile.close()
