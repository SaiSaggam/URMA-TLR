import os, fnmatch

def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename




import xml.etree.ElementTree

def mergeXMLfiles(rootPath):

    mergeElemenTree = None

    for filename in find_files(rootPath, '*.xml'):
        print 'Found XML source:', filename

        # Get the root element
        testResultElementTree = xml.etree.ElementTree.parse(filename).getroot() # <test_result>

        if (mergeElemenTree is None):
            # Save the first XML structure
            mergeElemenTree = testResultElementTree
        else:
            # Get <test_descriptor> element
            testDescriptorElementTree = testResultElementTree.find("test_descriptor") # <test_descriptor>
            # Append to the <test_result> ElementTree the new <test_descriptor> ElementTree
            mergeElemenTree.append( testDescriptorElementTree )



    # Create a new output file
    outputFilePath = os.path.join(rootPath, 'final_report.xml')
    f = open(outputFilePath,'w')
    f.write( '<?xml version="1.0" encoding="ISO-8859-1"?>\n')
    f.write( '<!-- Created by Cloud Connector Test Framework -->\n')
    f.write( '<?xml-stylesheet type="text/xsl" href="test_report.xsl"?>\n')
    f.write( xml.etree.ElementTree.tostring(mergeElemenTree) )
    f.close()

    print("Output file with merged XML files saved in '%s'" % outputFilePath)





import sys

def Usage():
    print 'Usage: merge_result_xml_files.py <Result path>'
    print '    Merge all result XML files from the provided path'
    print '    where:'
    print '        <Result path> is the root directory where the results are stored'


def main(argv):
    #process arguments
    count = len(argv);
    if (count < 1):
        Usage()
    else:
        # Get parameters from command line
        rootPath = argv[0]

        # Merge all result files from root path
        mergeXMLfiles(rootPath)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
