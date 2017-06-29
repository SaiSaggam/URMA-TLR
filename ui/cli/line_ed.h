/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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

#ifndef _LINE_ED_H_
#define _LINE_ED_H_

/***********************************/
/* Key definitions. ASCII assumed. */
/***********************************/

/* Control keys */

#define CTRL_A      ('A' - 'A' + 1)
#define CTRL_B      ('B' - 'A' + 1)
#define CTRL_C      ('C' - 'A' + 1)
#define CTRL_D      ('D' - 'A' + 1)
#define CTRL_E      ('E' - 'A' + 1)
#define CTRL_F      ('F' - 'A' + 1)
#define CTRL_G      ('G' - 'A' + 1)
#define CTRL_H      ('H' - 'A' + 1)
#define CTRL_I      ('I' - 'A' + 1)
#define CTRL_J      ('J' - 'A' + 1)
#define CTRL_K      ('K' - 'A' + 1)
#define CTRL_L      ('L' - 'A' + 1)
#define CTRL_M      ('M' - 'A' + 1)
#define CTRL_N      ('N' - 'A' + 1)
#define CTRL_O      ('O' - 'A' + 1)
#define CTRL_P      ('P' - 'A' + 1)
#define CTRL_Q      ('Q' - 'A' + 1)
#define CTRL_R      ('R' - 'A' + 1)
#define CTRL_S      ('S' - 'A' + 1)
#define CTRL_T      ('T' - 'A' + 1)
#define CTRL_U      ('U' - 'A' + 1)
#define CTRL_V      ('V' - 'A' + 1)
#define CTRL_W      ('W' - 'A' + 1)
#define CTRL_X      ('X' - 'A' + 1)
#define CTRL_Y      ('Y' - 'A' + 1)
#define CTRL_Z      ('Z' - 'A' + 1)
#define ESC         0x1B
#define TAB         CTRL_I
#define BSP         CTRL_H
#define ENTER       CTRL_M

/* The following O/S dependent routines are assumed to exist */

int  line_edit (cli_t *const context);
void cursor_to (cli_t *const context, int pos);
int line_input (cli_t * const context);

int  os_init(cli_t * const context);
void os_end(cli_t * const context);
int  os_getc  (cli_t *const context);
void os_putc  (char c, cli_t *const context);
void os_flush (cli_t *const context);
void calculate_next_logout_secs (cli_t * const cli);

#endif /* _LINE_ED_H_ */

