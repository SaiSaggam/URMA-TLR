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

#include <sys/types.h>
#include <ctype.h>
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define     SUPPORT_JSON        0
#define     DEBUG_PARMS         0

#if SUPPORT_JSON
#include <json/json.h>
#endif

#include "digiutils.h"

static int 	use_json_format=0;
static char  const * const month_str[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" } ;
static char const * const date_err = "Invalid date. Should be in the format of DD:MM:YYYY\n";
static char const * const time_err = "Invalid time. Should be in the 24 hour format of HH[:MM[:SS]]\n";


int set_date_time ( struct tm *ptm_time );
static void json_format_print (struct tm  *pt);
static int set_rtc (struct tm *ptm_time);

int digi_date_main (int argc, char **argv)
{
  char *fvalue = NULL;
  int   index;
  int   c, param_nb;
  char  *param1 = NULL;
  char  *param2 = NULL;
  time_t    tm;
  struct tm     tm_time;

  opterr = 0;

#if DEBUG_PARMS
    printf ( "argc:%d\n", argc );
        {
        int     i;

        for (i=0; i<argc; i++ )
            printf ( "arg:%d:%s\n", i, argv[i] );
        }
#endif


  while ((c = getopt (argc, argv, "t:i:f:u:")) != -1)
    {
    switch (c)
      {
      case 'f':
        fvalue = optarg;
        break;
      case '?':
        if (optopt == 'f')
          fprintf (stdout, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stdout, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stdout, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      case 'u':
    	  /* user */
    	  break;
      }
    }

  if ( fvalue != NULL )
    {
    if ( strcmp(fvalue,"json") == 0 )
        {
        use_json_format = 1;
        }
    }

  for (index = optind, param_nb=0; index < argc; index++, param_nb++)
    {
    switch ( param_nb )
        {
        case 0:  param1=argv[index];    break;
        case 1:  param2=argv[index];    break;
        // Only two parameters for our date command
        }
    }

   if ( param1 != NULL  )
    {
    int     num_scan;
    int     tm_hour,tm_min,tm_sec;
    
    // Time being set.
    // param1 is of the form...HH:MM:SS
    time(&tm);
    memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
    tm_time.tm_isdst = -1;  // -1 when you don't know (and we are about to set)
    num_scan = sscanf(param1, "%2d:%2d:%2d", &tm_hour,&tm_min,&tm_sec);
    if ( num_scan >= 1 )
        tm_time.tm_hour = tm_hour;
    if ( num_scan >= 2 )
        tm_time.tm_min = tm_min;
    if ( num_scan >= 3 )
        tm_time.tm_sec = tm_sec;

    if ( num_scan != 0 )
        {
        // And now check if we've been given a date as well.
        if ( param2 != NULL )
            {
            num_scan = sscanf(param2, "%2d:%2d:%d", &tm_time.tm_mday, &tm_time.tm_mon,&tm_time.tm_year);   
            if ( num_scan == 3 )
                {
                if (tm_time.tm_mon )
                    tm_time.tm_mon--;       // So our mon is in range 0-11
                if ( tm_time.tm_year >= 1900 )
                    tm_time.tm_year -= 1900;
                }
            else
                {
                fputs ( date_err, stdout );
                return 1;
                }
            }
         if ( set_date_time ( &tm_time ) )
           return 1;

         // Set HW RTC also
         if ( set_rtc ( &tm_time ) )
           return 1;
        }
    else
        {
        fputs ( time_err, stdout );
        return 1;
        }
    }
  
    // display of current date/time
    time(&tm);
    memcpy(&tm_time, localtime(&tm), sizeof(tm_time));    

    if ( tm_time.tm_mon >= 0 && tm_time.tm_mon <= 11 )
        {
        if ( use_json_format == 0 )
            {
            printf ( "system time: %02d:%02d:%02d, %02d %s %d\n",  tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,    
                tm_time.tm_mday, month_str [tm_time.tm_mon], tm_time.tm_year+1900 );
            }
        else
            {
            json_format_print (&tm_time );
            }
        }
    return 0;
}



int set_date_time ( struct tm *ptm_time )
{
    time_t    tm;

// Check the given date/time makes sense and if so then set it into the system
// Return 0 if all ok.
    if ( (ptm_time->tm_hour >= 0 && ptm_time->tm_hour < 24) &&
         (ptm_time->tm_min >= 0 && ptm_time->tm_min < 60) &&
        (ptm_time->tm_sec >= 0 && ptm_time->tm_sec < 60) )
        {
        if ( (ptm_time->tm_mday >= 1 && ptm_time->tm_mday <= 31) &&
             (ptm_time->tm_mon >=0 && ptm_time->tm_mon <= 11 ) )
            {
            // all ok lets set it..
            tm = mktime(ptm_time);
            if ( stime(&tm) < 0 )
                {
                fprintf ( stdout, "Cannot set time/date - need root access.\n" );
                return 1;
                }
            }
        else
            {
            fputs ( date_err, stdout );
            return 1;
            }
        }
    else
        {
        fputs ( time_err, stdout );
        return 1;
        }

    return 0;
}

static int set_rtc (struct tm *ptm_time)
{
    // i2c_rtc set commands arguments:
    //    i2c_rtc s <year-month-day weekday hour:minute:second>
    //
    // Note, we don't use the weekday argument, so set it to constant 1
    return system_command("i2c_rtc s %d-%d-%d 1 %d:%d:%d",
        ptm_time->tm_year + 1900, ptm_time->tm_mon + 1, ptm_time->tm_mday,
        ptm_time->tm_hour, ptm_time->tm_min, ptm_time->tm_sec);
}

static void json_format_print (struct tm  *pt)
{
#if SUPPORT_JSON
    json_object     *jobj = json_object_new_object();
    json_object     *jobjtime = json_object_new_object();
    json_object     *jobjdate = json_object_new_object();
    json_object     *jhour = json_object_new_int(pt->tm_hour);
    json_object     *jmin = json_object_new_int(pt->tm_min);
    json_object     *jsec = json_object_new_int(pt->tm_sec);
    json_object     *jmday = json_object_new_int(pt->tm_mday);
    json_object     *jmon = json_object_new_int(pt->tm_mon+1);
    json_object     *jyear = json_object_new_int(pt->tm_year);

    json_object_object_add(jobjtime, "hour", jhour);
    json_object_object_add(jobjtime, "min", jmin);
    json_object_object_add(jobjtime, "sec", jsec);
    
    json_object_object_add(jobjdate, "day", jmday);
    json_object_object_add(jobjdate, "month", jmon);
    json_object_object_add(jobjdate, "year", jyear);

    json_object_object_add(jobj, "time", jobjtime);
    json_object_object_add(jobj, "date", jobjdate);

    printf ( "%s\n", json_object_to_json_string(jobj) );
    json_object_put(jobj);
#endif

}



