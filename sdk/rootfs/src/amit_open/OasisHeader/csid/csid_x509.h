/*
 * @file Member ID Definition of Group X509.
 *
 * Moderator: Austin
 * Group ID: 0x00750000/0x80750000
 */

#ifndef _CSID_X509_H
#define _CSID_X509_H

#include "csid_gid.h"

#define _CFG_X509(x)       (CSID_GIDC_X509|(x))
#define _STA_X509(x)       (CSID_GIDS_X509|(x))

/* Extra Definitions */
//X.509

#define CSID_S_X509_ALTERED			  					_STA_X509(0x0000) //T=u32, ui altered of X509
#define CSID_S_X509_ACTIDX                        		_STA_X509(0x0001) //T=u32, idx of X509 certificate
#define CSID_S_X509_ACTION                        		_STA_X509(0x0002) //T=u32, D=0,X509 action
#define CSID_S_X509_STATUS                        		_STA_X509(0x0003) //T=u32, D=0,X509 status, 1:ok 2:file error
#define CSID_S_X509_DEL_INDEX							_STA_X509(0x0004) //T=str, X509 delete index
#define CSID_S_X509_SCEP_STATUS							_STA_X509(0x0005) //T=u32, D=0,scep status, 1:ok 2:file error
#define CSID_S_X509_SCEP_ALTERED	  					_STA_X509(0x0006) //T=u32, ui altered of X509 scep
#define CSID_S_X509_SCEP_INDEX	 	 					_STA_X509(0x0007) //T=u32, ui index of X509 scep
#define CSID_S_X509_SCEP_ACTION	 	 					_STA_X509(0x0008) //T=u32, ui action of X509 scep, 1:getca 2:enroll
#define CSID_S_X509_SCEP_NAME	 	 					_STA_X509(0x0009) //T=str, certificate name of X509 scep


#define CSID_C_X509_EN                            		_CFG_X509(0x0000) //T=u32, D=0,X509 Enable
#define CSID_C_X509_NORESET								_CFG_X509(0x0001) //T=u32, D=0,X509 no reset
#define CSID_C_X509_ROOTCA_CRL							_CFG_X509(0x0002) //T=str, X509 rootca crl 
#define CSID_C_X509_ROOTCA_CRLNUMBER					_CFG_X509(0x0003) //T=str, X509 rootca crl number
#define CSID_C_X509_ROOTCA_SERIAL						_CFG_X509(0x0004) //T=str, X509 rootca serial
#define CSID_C_X509_ROOTCA_INDEX						_CFG_X509(0x0005) //T=str, X509 rootca index

#define CSID_C_X509_LOCAL_CERT_EN                       _CFG_X509(0x0100) //T=u32, D=0, X509 certificate Enable,0:default
#define CSID_C_X509_LOCAL_NAME                          _CFG_X509(0x0200) //T=str, X509 certificate name
#define CSID_C_X509_LOCAL_SELF_SIGN_EN                  _CFG_X509(0x0300) //T=u32, D=0, Self sign,0:default
#define CSID_C_X509_LOCAL_PASSWORD    	              	_CFG_X509(0x0400) //T=str, X509 local certificate PASSWORD
#define CSID_C_X509_LOCAL_CERT                          _CFG_X509(0x0500) //T=str, X509 local certificate, 0:root CA, 1~255:Local Certificates
#define CSID_C_X509_LOCAL_CERT_2                        _CFG_X509(0x0600) //T=str, X509 local certificate 2
#define CSID_C_X509_LOCAL_PRIVATE_KEY    	            _CFG_X509(0x0700) //T=str, X509 local certificate private key
#define CSID_C_X509_LOCAL_PRIVATE_KEY_2  	            _CFG_X509(0x0800) //T=str, X509 local certificate private key 2

#define CSID_C_X509_LOCAL_ISSUER                        _CFG_X509(0x0900) //T=str, X509 local certificate issuer
#define CSID_C_X509_LOCAL_VALID_TO                      _CFG_X509(0x0A00) //T=str, X509 local certificate valid to
#define CSID_C_X509_LOCAL_SUBJECT                       _CFG_X509(0x0B00) //T=str, X509 local certificate subject

#define CSID_C_X509_LOCAL_VALID_DAYS                    _CFG_X509(0x0C00) //T=u32, D=3650, X509 certificate valid days
#define CSID_C_X509_LOCAL_KEY_SIZE                      _CFG_X509(0x0D00) //T=u32, D=1024, X509 certificate key size
#define CSID_C_X509_LOCAL_SUBJECT_COUNTRY               _CFG_X509(0x0E00) //T=str, X509 local certificate subject country
#define CSID_C_X509_LOCAL_SUBJECT_STATE                 _CFG_X509(0x0F00) //T=str, X509 local certificate subject state
#define CSID_C_X509_LOCAL_SUBJECT_LOCATION              _CFG_X509(0x1000) //T=str, X509 local certificate subject location
#define CSID_C_X509_LOCAL_SUBJECT_ORGANIZATION          _CFG_X509(0x1100) //T=str, X509 local certificate subject organization
#define CSID_C_X509_LOCAL_SUBJECT_ORGANIZATION_UNIT     _CFG_X509(0x1200) //T=str, X509 local certificate subject organization
#define CSID_C_X509_LOCAL_SUBJECT_COMMON_NAME           _CFG_X509(0x1300) //T=str, X509 local certificate subject common name
#define CSID_C_X509_LOCAL_SUBJECT_EMAIL                 _CFG_X509(0x1400) //T=str, X509 local certificate subject email
#define CSID_C_X509_LOCAL_CHALLENGEPASSWORD             _CFG_X509(0x1500) //T=str, X509 certificate request challengePassword
#define CSID_C_X509_LOCAL_UNSTRUCTURED_NAME             _CFG_X509(0x1600) //T=str, X509 certificate request unstructured name
#define CSID_C_X509_LOCAL_KEY_TYPE			            _CFG_X509(0x1700) //T=u32, D=0, X509 generate key type, 0:rsa, 1:dsa
#define CSID_C_X509_LOCAL_DIGEST_ALG		            _CFG_X509(0x1800) //T=u32, D=0, X509 request digest algorithm, 0:sha1, 1:md5

#define CSID_C_X509_TRUSTED_CA_EN                       _CFG_X509(0x2000) //T=u32, D=0, X509 trusted ca certificate Enable,0:default
#define CSID_C_X509_TRUSTED_CA_NAME                     _CFG_X509(0x2100) //T=str, X509 trusted ca certificate name
#define CSID_C_X509_TRUSTED_CA_CERT                     _CFG_X509(0x2200) //T=str, X509 trusted ca certificate
#define CSID_C_X509_TRUSTED_CA_CERT_2                   _CFG_X509(0x2300) //T=str, X509 trusted ca certificate 2
#define CSID_C_X509_TRUSTED_CA_ISSUER                   _CFG_X509(0x2400) //T=str, X509 trusted ca certificate issuer
#define CSID_C_X509_TRUSTED_CA_VALID_TO                 _CFG_X509(0x2500) //T=str, X509 trusted ca certificate valid to
#define CSID_C_X509_TRUSTED_CA_SUBJECT                  _CFG_X509(0x2600) //T=str, X509 trusted ca certificate subject

#define CSID_C_X509_TRUSTED_CLIENT_EN                   _CFG_X509(0x3000) //T=u32, D=0,X509 trusted client certificate Enable,0:default
#define CSID_C_X509_TRUSTED_CLIENT_NAME                 _CFG_X509(0x3100) //T=str, X509 trusted client certificate name
#define CSID_C_X509_TRUSTED_CLIENT_CERT                 _CFG_X509(0x3200) //T=str, X509 trusted client certificate
#define CSID_C_X509_TRUSTED_CLIENT_CERT_2               _CFG_X509(0x3300) //T=str, X509 trusted client certificate 2
#define CSID_C_X509_TRUSTED_CLIENT_ISSUER               _CFG_X509(0x3400) //T=str, X509 trusted client certificate issuer
#define CSID_C_X509_TRUSTED_CLIENT_VALID_TO             _CFG_X509(0x3500) //T=str, X509 trusted client certificate valid to
#define CSID_C_X509_TRUSTED_CLIENT_SUBJECT              _CFG_X509(0x3600) //T=str, X509 trusted client certificate subject

#define CSID_C_X509_ISSUED_CLIENT_NAME                 	_CFG_X509(0x4001) //T=str, X509 certificate name
#define CSID_C_X509_ISSUED_CLIENT_REQ                 	_CFG_X509(0x4002) //T=str, X509 certificate
#define CSID_C_X509_ISSUED_CLIENT_REQ_2               	_CFG_X509(0x4003) //T=str, X509 certificate 2
#define CSID_C_X509_ISSUED_CLIENT_CERT                 	_CFG_X509(0x4004) //T=str, X509 certificate
#define CSID_C_X509_ISSUED_CLIENT_CERT_2               	_CFG_X509(0x4005) //T=str, X509 certificate 2
#define CSID_C_X509_ISSUED_CLIENT_ISSUER               	_CFG_X509(0x4006) //T=str, X509 certificate issuer
#define CSID_C_X509_ISSUED_CLIENT_VALID_TO             	_CFG_X509(0x4007) //T=str, X509 certificate valid to
#define CSID_C_X509_ISSUED_CLIENT_SUBJECT              	_CFG_X509(0x4008) //T=str, X509 certificate subject
	
#define CSID_C_X509_IPSEC_CERT_EN                 		_CFG_X509(0x5000) //T=u32, D=0, X509 certificate Enable,0:default
#define CSID_C_X509_IPSEC_L_CERT                  		_CFG_X509(0x5100) //T=str, X509 certificate name for local ipsec.conf
#define CSID_C_X509_IPSEC_R_CERT                  		_CFG_X509(0x5200) //T=str, X509 certificate name for remote ipsec.conf

#define CSID_C_X509_SCEP_ENABLE	                  		_CFG_X509(0x6000) //T=u32, D=0, scep enable, 0:default
#define CSID_C_X509_SCEP_GETCA_ENABLE                 	_CFG_X509(0x6010) //T=u32, D=0, scep getca enable, 0:default
#define CSID_C_X509_SCEP_GETCA_SRV_INDEX	            _CFG_X509(0x6020) //T=u32, scep enrollment client interface
#define CSID_C_X509_SCEP_GETCA_INTERFACE         		_CFG_X509(0x6030) //T=u32, D=0, scep getca client interface
#define CSID_C_X509_SCEP_GETCA_SVR_IP              		_CFG_X509(0x6040) //T=str, scep getca server ip
#define CSID_C_X509_SCEP_GETCA_SVR_PORT         		_CFG_X509(0x6050) //T=u32, D=0, scep getca server port
#define CSID_C_X509_SCEP_GETCA_PATH	                  	_CFG_X509(0x6060) //T=str, scep getca path
#define CSID_C_X509_SCEP_GETCA_APPLICATION	            _CFG_X509(0x6070) //T=str, D="pkiclient.exe", scep getca application
#define CSID_C_X509_SCEP_GETCA_CA_IDENTIFIER	        _CFG_X509(0x6080) //T=str, scep getca ca identifier

#define CSID_C_X509_SCEP_ENROLL_ENABLE	                _CFG_X509(0x7000) //T=u32, D=0, scep enrollment enable, 0:default
#define CSID_C_X509_SCEP_ENROLL_SRV_INDEX	            _CFG_X509(0x7100) //T=u32, scep enrollment client interface
#define CSID_C_X509_SCEP_ENROLL_INTERFACE	            _CFG_X509(0x7200) //T=u32, D=0, scep enrollment client interface
#define CSID_C_X509_SCEP_ENROLL_CA_IDENTIFIER	        _CFG_X509(0x7300) //T=str, scep enrollment ca identifier
#define CSID_C_X509_SCEP_ENROLL_CA_CERT	                _CFG_X509(0x7400) //T=str, scep enrollment ca certificate
#define CSID_C_X509_SCEP_ENROLL_CA_ENC_CERT	            _CFG_X509(0x7500) //T=str, scep enrollment ca encryption certificate
#define CSID_C_X509_SCEP_ENROLL_CA_SIGN_CERT	        _CFG_X509(0x7600) //T=str, scep enrollment ca signature certificate
#define CSID_C_X509_SCEP_ENROLL_REQ_CERT	            _CFG_X509(0x7700) //T=str, scep enrollment certificate signing request
#define CSID_C_X509_SCEP_ENROLL_REENROLL_ENABLE	        _CFG_X509(0x7800) //T=u32, D=0, scep enrollment re-enroll enable


#endif //ifndef _CSID_X509_H


