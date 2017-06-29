# -*- coding: utf-8 -*-
import ConfigParser
import os
import fileinput, string, sys
from optparse import OptionParser

def replace_string(file_name, src_str, dst_str):
  infile = open(file_name, "r")
  text = infile.read()
  infile.close()
  outfile = open(file_name, "w")
  outfile.write(text.replace(src_str, dst_str))
  outfile.close()

def remove_errors(file_name):
    infile = open(file_name, "r")
    text = infile.read()
    infile.close()
    if text.find('//#error') == -1: # Don't run if #error is already commented out.
        #print "Replacing #error in [%s]" % file_name
        outfile = open(file_name, "w")
        outfile.write(text.replace('#error', '//#error'))
        outfile.close()

def update_field(define, line, value):
  elements = line.split()
  if elements[0] != '#define':
    return line

  modified_line = elements[0] + '    ' + elements[1] + '    '
  if define.is_string:
    modified_line += '"' + str(value) + '"' + '\n'  
  else:
    modified_line += str(value) + '\n'
  return modified_line

class DefineConfig(object):

  def __init__(self, define, config_key, is_string=False):
    self.define = define
    self.config_key = config_key
    self.is_string = is_string

DEFINES = [ DefineConfig('CONNECTOR_DEVICE_TYPE', 'device_type', is_string=True),
            DefineConfig('CONNECTOR_CLOUD_URL',   'connector_host',  is_string=True),
            DefineConfig('CONNECTOR_TX_KEEPALIVE_IN_SECONDS',  'tx_keepalive'),
            DefineConfig('DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS', 'tx_keepalive'),
            DefineConfig('CONNECTOR_RX_KEEPALIVE_IN_SECONDS',  'rx_keepalive'),
            DefineConfig('DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS', 'rx_keepalive'),
            DefineConfig('CONNECTOR_WAIT_COUNT',               'wait_count'),
            DefineConfig('DEVICE_WAIT_COUNT',              'wait_count'),
            DefineConfig('CONNECTOR_VENDOR_ID',                'vendor_id'),
            DefineConfig('CONNECTOR_MSG_MAX_TRANSACTION',      'max_transaction'),
            DefineConfig('CONNECTOR_MSG_MAX_TRANSACTIONS',     'max_transaction')]

def update_config_header(header_file, **config):

  found_enable = False
  infile = open(header_file, "r")
  new_lines = []

  for line in infile:
    # Enable compile time data passing so these values are used.
    if 'ENABLE_COMPILE_TIME_DATA_PASSING' in line:
      if found_enable == False:
        found_enable = True
        elements = line.split()
        if elements[0] == '#if':
          new_lines.append("#define   ENABLE_COMPILE_TIME_DATA_PASSING   1\n")
      new_lines.append(line)
      continue

    injected = False
    for define in DEFINES:
      if define.define in line:
        if config.has_key(define.config_key):
          new_lines.append(update_field(define, line, config[define.config_key]))
          injected = True
        break

    if not injected:
      new_lines.append(line)

  infile.close()
  outfile = open(header_file, "wb")
  for line in new_lines:
    line = line.replace('\r\n', '\n')
    outfile.write(line)
  outfile.close()


def update_connector_config_c(src_file, **device_config):

  infile = open(src_file, "r")
  new_lines = []

  for line in infile:
    injected = False
    for define in DEFINES:
      if define.define in line:
        if device_config.has_key(define.config_key):
          new_lines.append(update_field(define, line, device_config[define.config_key]))
          injected = True
        break
    
    if not injected:
      new_lines.append(line)

  infile.close()
  outfile = open(src_file, "wb")
  for line in new_lines:
    line = line.replace('\r\n', '\n')
    outfile.write(line)
  outfile.close()

def update_platform_h(platform_file, target_ssl_cert='public/certificates/Digi_Int-ca-cert-public.crt'):
  ssl_cert_text = '#define APP_SSL_CA_CERT   "../../../certificates/Digi_Int-ca-cert-public.crt"'
  replace_text  = '#define APP_SSL_CA_CERT   "%s"' % target_ssl_cert

  infile = open(platform_file, 'r')
  data = infile.read()
  infile.close()
  data = data.replace(ssl_cert_text, replace_text)
  outfile = open(platform_file, 'w')
  outfile.write(data)
  outfile.close()

def update_config_c(cnfg_file, **device_config):

  old_mac = '{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}'
  old_vendor_id = '0x00000000'
  old_device_type = '"Linux Application"'
  old_imei = '"000000-00-000000-0"'
  old_esn = '"00000000"'
  old_meid = '"00000000000000"'
  # For backwards compatibility of old tests, replace 'my.idigi.com'
  # or 'developer.idigi.com' with target server.
  old_server_urls = ['"login.etherios.com"', '"my.idigi.com"', '"developer.idigi.com"']

  # Setup Vendor ID
  vendor_id = device_config['vendor_id'] if device_config.has_key('vendor_id') else 0
  # hex_string = '%08X' % vendor_id
  # new_vendor_id  = '{%s}' % ', '.join([ '0x%s' % hex_string[x:x+2] for x in xrange(0,len(hex_string), 2)])
  new_vendor_id = '0x%08X' % vendor_id
  
  # Setup Device ID
  device_id = device_config['device_type'] if device_config.has_key('device_type') else 'ECC DVT'

  # Setup iDigi Server
  connector_host = device_config['connector_host'] if device_config.has_key('connector_host') else 'test.etherios.com'
  new_server_url  = '"' + connector_host + '"'

  # Setup Device Type
  new_device_type = '"' + device_id + '"'

  if( device_config.has_key('device_mac') and device_config['device_mac'] != None):
    # Setup MAC Address
    mac_addr = device_config['device_mac']
    new_mac = '{' + '0x' + mac_addr[0] + mac_addr[1] + ', '
    new_mac += '0x' + mac_addr[2] + mac_addr[3] + ', '
    new_mac += '0x' + mac_addr[4] + mac_addr[5] + ', '
    new_mac += '0x' + mac_addr[7] + mac_addr[8] + ', '
    new_mac += '0x' + mac_addr[9] + mac_addr[10] + ', '
    new_mac += '0x' + mac_addr[11] + mac_addr[12] + '}'
    # Update File with new MAC Address
    replace_string(cnfg_file, old_mac, new_mac)

  if( device_config.has_key('device_imei') and device_config['device_imei'] != None):
    # Setup IMEI
    imei_id = device_config['device_imei']
    new_imei_id = '"' + imei_id + '"'
    # Update File with new IMEI
    replace_string(cnfg_file, old_imei, new_imei_id)

  if( device_config.has_key('device_esn') and device_config['device_esn'] != None):
    # Setup ESN
    esn_id = device_config['device_esn']
    new_esn_id = '"' + esn_id + '"'
    # Update File with new ESN
    replace_string(cnfg_file, old_esn, new_esn_id)

  if( device_config.has_key('device_meid') and device_config['device_meid'] != None):
    # Setup MEID
    meid_id = device_config['device_meid']
    new_meid_id = '"' + meid_id + '"'
    # Update File with new MEID
    replace_string(cnfg_file, old_meid, new_meid_id)


  replace_string(cnfg_file, old_vendor_id, new_vendor_id)
  replace_string(cnfg_file, old_device_type, new_device_type)
  for old_server_url in old_server_urls:
    replace_string(cnfg_file, old_server_url, new_server_url)

def update_config_source(cnfg_file, **device_config):
  update_connector_config_c(cnfg_file, **device_config)
  update_config_c(cnfg_file, **device_config)

