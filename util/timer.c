/**
  ******************************************************************************
  * @file timer.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-19 22:29:53
  * @brief  This file provides all the timer functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/time.h>

#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#include <glib.h>
#include <gio/gio.h>

#include "glib.h"
#include "timer.h"
#include "diagnosis.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SECS_PER_HOUR   (60 * 60)
#define SECS_PER_DAY    (SECS_PER_HOUR * 24)
#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

static const unsigned short int mon_yday[2][13] = {
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

#define is_leap_year(y) \
    ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * General functions 
 *******************************************************************************/

/***************************************************
 * Function : timer_millisecond_get
 * Author : leon.xie
 * Creat Date : 2018/01/23  17:31:41
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int timer_millisecond_get(guint64 *timer)
{
	struct timeval tv;
	if(NULL == timer)
	{
		ASSERT(0);
		return FAIL;
	}
	
	if (gettimeofday(&tv, NULL) < 0)
	{
		ASSERT(0);
		return FAIL;
	}
	*timer = (tv.tv_sec * 1000ull) + (tv.tv_usec / 1000ull);
	
    return SUCC;
}


/***************************************************
 * Function : timer_fields_get
 * Author : leon.xie
 * Creat Date : 2018/01/23  17:41:38
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
void timer_fields_get(guint64 timer, struct tm *fields)
{
	if(0 == timer
	|| NULL == fields)
	{
		ASSERT(0);
		return;
	}
	/* This code is taken from GLibC under terms of LGPLv2+ */
	/* Remove the 'offset' or GMT manipulation since we don't care. See
	 * commit id '3ec12898' comments regarding localtime.
	 */
	long int days, rem, y;
	const unsigned short int *ip;
	unsigned long long whenSecs = timer / (1000ull);

	days = whenSecs / SECS_PER_DAY;
	rem = whenSecs % SECS_PER_DAY;

	fields->tm_hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	fields->tm_min = rem / 60;
	fields->tm_sec = rem % 60;
	/* January 1, 1970 was a Thursday.	*/
	fields->tm_wday = (4 + days) % 7;
	if (fields->tm_wday < 0)
		fields->tm_wday += 7;
	y = 1970;

	while (days < 0 || days >= (is_leap_year(y) ? 366 : 365)) {
		/* Guess a corrected year, assuming 365 days per year.	*/
		long int yg = y + days / 365 - (days % 365 < 0);

	  /* Adjust DAYS and Y to match the guessed year.  */
	  days -= ((yg - y) * 365
			   + LEAPS_THRU_END_OF(yg - 1)
			   - LEAPS_THRU_END_OF(y - 1));
	  y = yg;
	}
	fields->tm_year = y - 1900;

	fields->tm_yday = days;
	ip = mon_yday[is_leap_year(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	fields->tm_mon = y;
	fields->tm_mday = days + 1;
}


/***************************************************
 * Function : yin_timer_millisecond2string
 * Author : leon.xie
 * Creat Date : 2018/01/23  17:38:35
 * Description : none
 * In-Parameter : as below
 * Return : as below
 * Modify : none
 **************************************************/
int timer_millisecond2string(guint64 timer,gchar *buf)
{
	if(0 == timer
	|| NULL == buf)
	{
		ASSERT(0);
		return FAIL;
	}
	
    struct tm fields;

    timer_fields_get(timer, &fields);

    fields.tm_year += 1900;
    fields.tm_mon += 1;
	if (snprintf(buf, TIME_STRING_BUFLEN,
					 "%4d-%02d-%02d %02d:%02d:%02d.%03d",
					 fields.tm_year, fields.tm_mon, fields.tm_mday,
					 fields.tm_hour, fields.tm_min, fields.tm_sec,
					 (int) (timer % 1000)) >= TIME_STRING_BUFLEN) 
	{
			errno = ERANGE;
			ASSERT(0);
			return FAIL;
	}
	
    return SUCC;
}

#ifdef  __cplusplus
}
#endif


