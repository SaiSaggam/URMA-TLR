/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#include "show_ipsec.h"
#include "config_def.h"
#include "config_mgr.h"
#include "unittest_support.h"
#include "strlcpy.h"
#include "tlr_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <string.h>
#include <ctype.h>

STATIC_FUNCTION int skip_lines_until(FILE * fp, char *match, char **line, size_t *len)
{
    int result;

    do
    {
        if (getline(line, len, fp) <= 0)
        {
            result = -1;
            goto done;
        }
        if (*line != NULL && strstr(*line, match) != NULL)
        {
            result = 0;
            goto done;
        }
    }
    while (*line != NULL);

    result = -1;

done:
    return result;
}

/*
 * Parse the line:
 *      ipsec1[2]: ESTABLISHED 23 minutes ago, 10.52.18.180[10.52.18.180]...10.52.18.109[10.52.18.109]
 *      Here the ESTABLISHED keyword indicates the tunnel is up, we then parse out the peer IP
 *      address, which is the IP address after the "..."
 */
STATIC_FUNCTION int get_uptime_and_peer_ip(FILE * const fp, tunnel_status_t * ptr_tunnel, char *line, size_t *len)
{
    int ret=-1, bytes_to_copy, i;
    char *ptr_start, *ptr_end;

    if ((ptr_start = strstr(line, "ESTABLISHED")) == NULL)
    {
        ptr_tunnel->is_tunnel_up = false;
        goto done;
    }

    while (!STRING_IS_EMPTY(ptr_start) && !isdigit(*ptr_start))
    {
        ptr_start++;
    }

    if ((ptr_end = strstr(ptr_start, "ago")) == NULL)
    {
        goto error;
    }

    i = ptr_end - ptr_start;

    bytes_to_copy = (i > sizeof ptr_tunnel->up_time-1) ? sizeof ptr_tunnel->up_time-1 : i;
    memcpy(ptr_tunnel->up_time, ptr_start, bytes_to_copy);

    // Next get the peer IP address.
    if ((ptr_start = strstr(ptr_end, "...")) == NULL)
    {
         goto error;
    }
    ptr_start += 4; // XXX - why 4? Skipping passed a ' ' after the "..."? Perhaps the string above should be "... "

    if ((ptr_end = strstr(ptr_start, "[")) == NULL)
    {
         goto error;
    }
    ptr_start--;

    i = ptr_end - ptr_start;
    bytes_to_copy = (i > sizeof ptr_tunnel->peer_ip-1) ? sizeof ptr_tunnel->peer_ip-1 : i;
    memcpy(ptr_tunnel->peer_ip, ptr_start, bytes_to_copy);

done:
    ret = 0;
error:
    return ret;
}

// Parse the line:
//       ipsec1[2]: IKEv1 SPIs: 2a49367b12f3fb59_i* 93fa258257b64cca_r, pre-shared key reauthentication in 27 minutes
// We need to parse out the '27 minutes' which is the renegotation time
STATIC_FUNCTION int get_key_renegotion_time(FILE * const fp, tunnel_status_t * ptr_tunnel, char *line, size_t *len)
{
    int ret=-1;
    char *ptr_start;

    if (skip_lines_until(fp, "IKEv1 SPIs", &line, len) < 0)
    {
        // TODO: check for IKEv2
        goto error;
    }

    ptr_tunnel->ike_version = 1;


    if ((ptr_start = strstr(line, "in")) == NULL)
    {
        goto error;
    }

    while (!isdigit(*ptr_start)) ptr_start++;

    if (strstr(ptr_start, "rekeying disabled") == NULL)
    {
        strncpy(ptr_tunnel->rekeying_in, ptr_start, sizeof ptr_tunnel->rekeying_in);
        ptr_tunnel->rekeying_in[strlen(ptr_tunnel->rekeying_in) - 1] = '\0'; // strip off newline
    }
    else
    {
        strcpy(ptr_tunnel->rekeying_in, "Rekeying disabled");
    }

    ret = 0;
error:
    return ret;
}

// Map ipsec status output to the strings in commands.conf
typedef struct encryption_strings_t
{
    char *ipsec_name;
    char *tlr_name;
} encryption_strings;

// Strings which map how 'ipsec statusall' displays them and how
// we display them in the 'show ipsec' output.
static const  encryption_strings encryption_mapping[] =
{
        {"AES_CBC_128", "aes128"},
        {"AES_CBC_192", "aes192"},
        {"AES_CBC_256", "aes256"},
        {"HMAC_SHA1", "sha1"},
        {"HMAC_SHA1_96", "sha1"},
        {"HMAC_SHA1_256", "sha1"},
        {"HMAC_SHA_96", "sha96"},
        {"HMAC_SHA_256", "sha256"},
        {"HMAC_SHA_384", "sha384"},
        {"MODP_768", "modp768"},
        {"MODP_1536", "modp1536"},
        {"MODP_2048", "modp2048"},
        {"MODP_3072", "modp3072"},
        {"MODP_4096", "modp4096"},
        {"MODP_6144", "modp6144"},
        {"MODP_8192", "modp8192"},
        {NULL, NULL}
};

STATIC_FUNCTION char *get_encryption_name(char *enc)
{
    char *name=NULL, *ptr;
    int i;

    // Remove any newline or commas (which indicates the end of the encryption types)
    if (((ptr = strstr(enc, ",")) != NULL) || ((ptr = strstr(enc, "\n")) != NULL))
    {
        *ptr =0x0;
    }

    for (i=0; encryption_mapping[i].ipsec_name != NULL; i++)
    {
        if (strcmp(enc, encryption_mapping[i].ipsec_name) == 0)
        {
            name = encryption_mapping[i].tlr_name;
            break;
        }
    }
    // If we couldn't find the name, just use the one ipsec statusall prints out
    if (encryption_mapping[i].ipsec_name == NULL)
    {
        name = enc;
    }

    return name;
}



// Parse the line:
//    IKE proposal: AES_CBC_128/HMAC_SHA1_96/PRF_HMAC_SHA1/MODP_1536
// Parse out the IKE encryption types
STATIC_FUNCTION int get_key_negotiation_info(FILE * const fp, tunnel_status_t * ptr_tunnel, char *line, size_t *len)
{
    int ret=-1;
    char *ptr_start;

    if (skip_lines_until(fp, "proposal:", &line, len) < 0)
    {
        goto error;
    }

    if ((ptr_start = strstr(line, "proposal:")) == NULL)
    {
        goto error;
    }
    while (!isupper(*ptr_start)) ptr_start++;

    // Convert the encryption types to what TLR prints out.
    if (ptr_tunnel->ike_version == 1)
    {
        strcat(ptr_tunnel->key_negotiation, "IKEv1, ");
    }
    else
    {
        strcat(ptr_tunnel->key_negotiation, "IKEv2, ");
    }
    {
        char *enc;
        //char buffer[32];

        enc = strtok(ptr_start, "/");

        if (enc == NULL)
        {
            // We have only one encryption value
            strncpy(ptr_tunnel->key_negotiation, ptr_start, sizeof ptr_tunnel->key_negotiation);
            ptr_tunnel->key_negotiation[strlen(ptr_tunnel->key_negotiation) - 1] = '\0';
        }

        while (enc != NULL)
        {
            char *name;

            if (strstr(enc, "PRF_") == 0) // skip the pseudo-random function
            {
                name = get_encryption_name(enc);
                strcat(ptr_tunnel->key_negotiation, name);
                enc = strtok(NULL, "/");
                if (enc != NULL)
                {
                    strcat(ptr_tunnel->key_negotiation, ", ");
                }
            }
            else
            {
                enc = strtok(NULL, "/");
            }

        }
    }

    ret = 0;
error:
    return ret;
}

STATIC_FUNCTION void convert_to_upper_case(char * const str)
{
    char * pch = str;

    while(*pch != '\0')
    {
        if (islower(*pch))
        {
            *pch = toupper(*pch);
        }
        pch++;
    }
}

static bool parse_hex_word(char const * * const line, char * const destination, size_t const destination_size)
{
    char const * pch = *line;
    char const * ptr_end;
    size_t const size = destination_size - 1; /* subtract 1 for NUL terminator */
    size_t bytes_to_copy;
    bool found_hex_word;

    while (*pch != '\0' && !isxdigit(*pch))
    {
        pch++;  // Find the first hex digit.
    }
    if (*pch == '\0')
    {
        found_hex_word = false;
        goto done;
    }

    ptr_end = pch;
    while (isxdigit(*ptr_end))
    {
        ptr_end++;
    }

    bytes_to_copy = ptr_end - pch;
    if (bytes_to_copy > size)
    {
        bytes_to_copy = size;
    }

    memcpy(destination, pch, bytes_to_copy);
    *line = ptr_end;
    destination[bytes_to_copy] = '\0';
    convert_to_upper_case(destination);
    found_hex_word = true;

done:

    return found_hex_word;
}

// Parse the line:
//       ipsec1{2}:  INSTALLED, TUNNEL, reqid 2, ESP in UDP SPIs: 8138efd3_i e797ed5d_o
// Read the security associations which start after 'SPIs'
STATIC_FUNCTION int get_esp_sa(FILE * const fp, tunnel_status_t * const ptr_tunnel, char * line, size_t * const len)
{
    int ret = -1;
    char const * ptr_start;

    if (skip_lines_until(fp, "INSTALLED", &line, len) < 0)
    {
        goto error;
    }

    if ((ptr_start = strstr(line, "SPIs:")) == NULL)
    {
        goto error;
    }

    if (!parse_hex_word(&ptr_start, ptr_tunnel->inbound_esp_sa, sizeof ptr_tunnel->inbound_esp_sa))
    {
        fprintf(stderr, "failed to parse inbound word");
        goto error;
    }

    if (!parse_hex_word(&ptr_start, ptr_tunnel->outbound_esp_sa, sizeof ptr_tunnel->outbound_esp_sa))
    {
        fprintf(stderr, "failed to parse outbound word");
        goto error;
    }

    ret = 0;
error:
    return ret;
}


// Parse the line:
//    ipsec1{2}:  AES_CBC_128/HMAC_SHA1_96, 0 bytes_i, 0 bytes_o, rekeying in 25 minutes
// also can have the form:
// AES_CBC_128/HMAC_SHA1_96, 168 bytes_i (2 pkts, 3s ago), 168 bytes_o (2 pkts, 3s ago), rekeying in 16 seconds
// Parse out the authentication information and the rekeying time.
STATIC_FUNCTION int get_encryption_info(FILE * const fp, tunnel_status_t * ptr_tunnel, char *line, size_t *len)
{
    int ret=-1, bytes_to_copy, i;
    char *ptr_start, *ptr_end;
    char buffer[256];
    size_t temp;

    if (skip_lines_until(fp, "rekeying", &line, len) < 0)
    {
        // TODO: check for IKEv2
        goto error;
    }

    ptr_tunnel->ike_version = 1;

    if ((ptr_start = strstr(line, "}:")) == NULL)
    {
        goto error;
    }

    // Next is the 'ENCRYPTION_ALGORITHM/INTEGRITY_ALGORITHM'
    // ENCRYPTION_ALGORITHM consists of the encryption_algorithm_name/encr_size
    while (!isalpha(*ptr_start)) ptr_start++;

    strlcpy(buffer, ptr_start, sizeof buffer);

    // Convert the encryption types to what TLR prints out.
    {
        char *enc;
        //char buffer[32];

        enc = strtok(buffer, "/");

        if (enc == NULL)
        {
            if ((ptr_end = strstr(ptr_start, ",")) == NULL)
            {
                goto error;
            }

            i = ptr_end - ptr_start;

            bytes_to_copy = (i > sizeof ptr_tunnel->esp_ciphers-1) ? sizeof ptr_tunnel->esp_ciphers-1: i;
            memcpy(ptr_tunnel->esp_ciphers , ptr_start, bytes_to_copy);

        }

        while (enc != NULL)
        {
            char *name;

            if (strstr(enc, "PRF_") == 0) // skip the pseudo-random function
            {
                name = get_encryption_name(enc);
                strcat(ptr_tunnel->esp_ciphers, name);
            }
            enc = strtok(NULL, "/");
            if (enc != NULL)
            {
                strcat(ptr_tunnel->esp_ciphers, ", ");
            }
        }
    }
    if ((ptr_start = strstr(ptr_start, ",")) == NULL)
    {
        goto error;
    }
    // Next get the number of bytes received
    while (!isdigit(*ptr_start)) ptr_start++;
    ptr_end = ptr_start;

    while (isdigit(*ptr_end)) ptr_end++;

    i = ptr_end - ptr_start;

    bytes_to_copy =  (i > sizeof ptr_tunnel->bytes_in-1) ? sizeof ptr_tunnel->bytes_in-1 : i;
    memcpy(ptr_tunnel->bytes_in , ptr_start, bytes_to_copy);

    // Next get the number of bytes transmitted
    while (!isdigit(*ptr_start)) ptr_start++;
    ptr_end = ptr_start;

    while (isdigit(*ptr_end)) ptr_end++;

    i = ptr_end - ptr_start;

    bytes_to_copy =  (i > sizeof ptr_tunnel->bytes_out-1) ? sizeof ptr_tunnel->bytes_out-1 : i;
    memcpy(ptr_tunnel->bytes_out , ptr_start, bytes_to_copy);

    if ((ptr_start = strstr(line, "in")) == NULL)
    {
        goto error;
    }

    while (!isdigit(*ptr_start)) ptr_start++;

    strlcpy(ptr_tunnel->renegotiating_in, ptr_start, sizeof ptr_tunnel->renegotiating_in);
    /* remove the '\n' at the end of the line */
    temp = strlen(ptr_tunnel->renegotiating_in);
    if (temp > 0)
    {
        ptr_tunnel->renegotiating_in[temp - 1] = '\0';
    }

    ret = 0;
error:
    return ret;
}


/*
 * Parse the line:
 *         ipsec1{2}:   100.100.102.0/24 === 100.100.101.0/24
 *  Read the subnet information
 */
STATIC_FUNCTION int get_networks(FILE * const fp, tunnel_status_t * ptr_tunnel, char *line, size_t *len)
{
    int ret=-1, bytes_to_copy, i;
    char *ptr_start, *ptr_end;

    if (skip_lines_until(fp, "===", &line, len) < 0)
    {
        goto error;
    }

    if ((ptr_start = strstr(line, "}:")) == NULL)
    {
        goto error;
    }

    while (!isdigit(*ptr_start)) ptr_start++;

    ptr_end = ptr_start;
    while (!isspace(*ptr_end)) ptr_end++;
    ptr_end++;

    i = ptr_end - ptr_start;

    bytes_to_copy = (i > sizeof ptr_tunnel->local_cidr_network-1) ? sizeof ptr_tunnel->local_cidr_network-1: i;
    memcpy(ptr_tunnel->local_cidr_network , ptr_start, bytes_to_copy);

    if ((ptr_start = strstr(ptr_end, "===")) == NULL)
    {
        goto error;
    }

    while (!isdigit(*ptr_start)) ptr_start++;

    strncpy(ptr_tunnel->remote_cidr_network, ptr_start, sizeof ptr_tunnel->remote_cidr_network);

    // Strip off the newline
    ptr_tunnel->remote_cidr_network[strlen(ptr_tunnel->remote_cidr_network) - 1] = '\0';

    ret = 0;
error:
    return ret;
}

/*
 * Parse the output from the 'ipsec statusall <tunnel_name> command'
 */
int read_tunnel_info_from_ipsec_status(FILE * const fp, tunnel_status_t * ptr_tunnel)
{
    int ret=-1;
    char *line=NULL;
    size_t len = 0;
    char *ipsec_string=NULL;
    extern int asprintf(char **ret, const char *format, ...);

    if (asprintf(&ipsec_string, "%s[", ptr_tunnel->name) < 0)
    {
        goto error;
    }

    // Find the start of the information for this tunnel
    if (skip_lines_until(fp, ipsec_string, &line, &len) < 0)
    {
       ptr_tunnel->is_tunnel_up = false;
       goto done;
    }

    ptr_tunnel->is_tunnel_up = true;

    if (get_uptime_and_peer_ip(fp, ptr_tunnel, line, &len) < 0)
    {
        goto error;
    }

    if (!ptr_tunnel->is_tunnel_up || !ptr_tunnel->is_tunnel_enabled)
    {
        goto done;
    }

    if (get_key_renegotion_time(fp, ptr_tunnel, line, &len) < 0)
    {
         goto error;
    }

    if (get_key_negotiation_info(fp, ptr_tunnel, line, &len) < 0)
    {
        goto error;
    }

    if (get_esp_sa(fp, ptr_tunnel, line, &len) < 0)
    {
        goto error;
    }

    if (get_encryption_info(fp, ptr_tunnel, line, &len) < 0)
    {
        goto error;
    }

    if (get_networks(fp, ptr_tunnel, line, &len) < 0)
    {
        goto error;
    }

done:
    ret = 0;
error:
    free(ipsec_string);
    free(line);
    return ret;
}

