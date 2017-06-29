#!/bin/sh
# set -x
##############################################################################
# Copyright (c) 2016 Digi International Inc., All Rights Reserved
#
# This software contains proprietary and confidential information of Digi
# International Inc.  By accepting transfer of this copy, Recipient agrees
# to retain this software in confidence, to prevent disclosure to others,
# and to make no use of this software other than that for which it was
# delivered.  This is an unpublished copyrighted work of Digi International
# Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
# prohibited.
#
# Restricted Rights Legend
#
# Use, duplication, or disclosure by the Government is subject to
# restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
# Technical Data and Computer Software clause at DFARS 252.227-7031 or
# subparagraphs (c)(1) and (2) of the Commercial Computer Software -
# Restricted Rights at 48 CFR 52.227-19, as applicable.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
##############################################################################


# CSID define for O3G
# PDP context type
CSID_C_3G_MULTIWAN_IP_TYPE=0x001e22b0

IP_TYPE_IPV4=0
IP_TYPE_IPV6=1
IP_TYPE_IPV4V6=2

default_ip_type=$IP_TYPE_IPV4

wrcsman "$CSID_C_3G_MULTIWAN_IP_TYPE $default_ip_type"