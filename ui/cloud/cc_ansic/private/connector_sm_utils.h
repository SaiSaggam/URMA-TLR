/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

static uint16_t const crc_table[] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

#define SmCRC16(crcval, newchar) crcval = ((crcval) >> 8) ^ crc_table[((crcval) ^ (newchar)) & 0x00ff]

STATIC uint16_t sm_calculate_crc16(uint16_t crc, uint8_t const * const data, size_t const bytes)
{
    size_t i;

    for (i = 0; i < bytes; i++)
        SmCRC16(crc, data[i]);

    return crc;
}

#if (defined CONNECTOR_TRANSPORT_SMS)
/* Base85 encoding lookup table. */
static uint8_t const encode85_table[] =
{
    '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*',
    '+', ',', '-', '.', '/',  '0', '1',  '2', '3', '4',
    '5', '6', '7', '8', '9',  ':', ';',  '<', '=', '>',
    '?', '@', 'A', 'B', 'C',  'D', 'E',  'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M',  'N', 'O',  'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W',  'X', 'Y',  'Z', '_', 'a',
    'b', 'c', 'd', 'e', 'f',  'g', 'h',  'i', 'j', 'k',
    'l', 'm', 'n', 'o', 'p',  'q', 'r',  's', 't', 'u',
    'v', 'w', 'x', 'y', 'z'
};

/* Base85 decoding lookup table. */
static uint8_t const decode85_table[] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
    7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0,  0,  0,  0,  58, 0,  59, 60, 61,
    62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
    82, 83, 84, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


static unsigned long const pow85[] =
{
    85*85*85*85, 85*85*85, 85*85, 85, 1
};

STATIC int sm_encode85(uint8_t * dest, size_t dest_len, uint8_t const * const src, size_t const src_len)
{
    uint8_t buf[5];
    uint32_t tuple = 0;
    size_t src_count = 0;
    size_t dest_count = 0;
    size_t count = 0;
    size_t i;
    unsigned char *s;

    while (src_count < src_len)
    {
        unsigned const c = (*(src + src_count)) & 0xFF;

        switch (count++) {
        case 0:
            tuple |= (c << 24);
            break;
        case 1:
            tuple |= (c << 16);
            break;
        case 2:
            tuple |= (c <<  8);
            break;
        case 3:
            tuple |= c;
            i = 5;
            s = buf;
            do {
                *s++ = tuple % 85;
                tuple /= 85;
            } while (--i > 0);
            i = count;
            do {
                *dest++ = encode85_table[*--s];
                dest_count++;
                if (dest_count >= dest_len) {
                    return dest_count;
                }
            } while (i-- > 0);

            tuple = 0;
            count = 0;
            break;
        }
        src_count++;
    }

    /* Cleanup any remaining bytes... */
    if (count > 0) {
        i = 5;
        s = buf;
        do {
            *s++ = tuple % 85;
            tuple /= 85;
        } while (--i > 0);
        i = count;
        do {
            *dest++ = encode85_table[*--s];
            dest_count++;
            if (dest_count >= dest_len) {
                return dest_count;
            }
        } while (i-- > 0);
    }

    return dest_count;
}

STATIC int sm_decode85(uint8_t * dest, size_t dest_len, uint8_t const * const src, size_t const src_len)
{
    unsigned long tuple = 0;
    int c;
    size_t count = 0;
    size_t src_count = 0;
    size_t dest_count = 0;

    ASSERT_GOTO(dest_len >= (src_len * 4)/5, error);
    while (src_count < src_len) {
        c = (*(src + src_count)) & 0xFF;
        tuple += decode85_table[c] * pow85[count++];
        if (count == 5) {
            *dest++ = tuple >> 24;
            *dest++ = tuple >> 16;
            *dest++ = tuple >>  8;
            *dest++ = tuple;
            dest_count += 4;
            count = 0;
            tuple = 0;
        }
        src_count++;
    }

    /* Cleanup any remaining bytes... */
    if (count > 0) {
        count--;
        tuple += pow85[count];

        switch (count) {
        case 4:
            *dest++ = tuple >> 24;
            *dest++ = tuple >> 16;
            *dest++ = tuple >>  8;
            *dest++ = tuple;
            dest_count += 4;
            break;
        case 3:
            *dest++ = tuple >> 24;
            *dest++ = tuple >> 16;
            *dest++ = tuple >>  8;
            dest_count += 3;
            break;
        case 2:
            *dest++ = tuple >> 24;
            *dest++ = tuple >> 16;
            dest_count += 2;
            break;
        case 1:
            *dest++ = tuple >> 24;
            dest_count++;;
            break;
        }
    }

error:
    return dest_count;
}
#endif