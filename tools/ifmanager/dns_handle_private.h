/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/
#ifndef __DNS_HANDLE_PRIVATE_H__
#define __DNS_HANDLE_PRIVATE_H__


/* needed by unit tests, so don't make this file scope */
typedef enum  { DEFAULT_ROUTE, ANYTHING_ROUTE } ROUTE_TYPE;

#if defined(UNIT_TEST)
void trim_trailing_white(char * const str);
bool extract_metric_and_iface(char * * saved_strok_context, int * const pmetric, char * * const ifname);
bool get_interface_and_metric_from_line(char * const line, ROUTE_TYPE route_type, char * * const ifname, int * metric);
#endif

#endif /* __DNS_HANDLE_PRIVATE_H__ */
