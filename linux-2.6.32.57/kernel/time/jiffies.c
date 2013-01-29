/***********************************************************************
* linux/kernel/time/jiffies.c
*
* This file contains the jiffies based clocksource.
*
* Copyright (C) 2004, 2005 IBM, John Stultz (johnstul@us.ibm.com)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
************************************************************************/
#include <linux/clocksource.h>
#include <linux/jiffies.h>
#include <linux/init.h>

/* The Jiffies based clocksource is the lowest common
 * denominator clock source which should function on
 * all systems. It has the same coarse resolution as
 * the timer interrupt frequency HZ and it suffers
 * inaccuracies caused by missed or lost timer
 * interrupts and the inability for the timer
 * interrupt hardware to accuratly tick at the
 * requested HZ value. It is also not reccomended
 * for "tick-less" systems.
 */
#define NSEC_PER_JIFFY	((u32)((((u64)NSEC_PER_SEC)<<8)/ACTHZ))

/* Since jiffies uses a simple NSEC_PER_JIFFY multiplier
 * conversion, the .shift value could be zero. However
 * this would make NTP adjustments impossible as they are
 * in units of 1/2^.shift. Thus we use JIFFIES_SHIFT to
 * shift both the nominator and denominator the same
 * amount, and give ntp adjustments in units of 1/2^8
 *
 * The value 8 is somewhat carefully chosen, as anything
 * larger can result in overflows. NSEC_PER_JIFFY grows as
 * HZ shrinks, so values greater than 8 overflow 32bits when
 * HZ=100.
 */
#define JIFFIES_SHIFT	8

/* dacashman - evaluating the variation of jiffies */
static jiffies_read_count = 0;

static cycle_t jiffies_read(struct clocksource *cs)
{
  /* dacashman - print out jiffies value at each read to see difference and frequency. Might need to acquire lock here */
        jiffies_read_count++;
	if(jiffies_read_count < 10 || jiffies_read_count % 100 == 0){
	  dump_stack();
	  printk(KERN_DEBUG "JIFFIES_READ: read #%d long value: %llu\n", jiffies_read_count, jiffies);
	  printk(KERN_DEBUG "JIFFIES_READ: read #%d long jiffies_64 value: %llu\n", jiffies_read_count, jiffies_64);
	  //printk(KERN_DEBUG "JIFFIES_READ: read #%d long jiffies_64 proper read: %llu\n", jiffies_read_count, get_jiffies_64());
	  printk(KERN_DEBUG "JIFFIES_READ unsigned 32bit read #%d value: %u, %x\n", jiffies_read_count, jiffies, jiffies);
	  printk(KERN_DEBUG "JIFFIES_READ unsigned 32bit 2nd read #%d value: %u, %x\n", jiffies_read_count, jiffies, ((__u32)(jiffies)));

	  printk(KERN_DEBUG "JIFFIES NSEC_PER_JIFFY value: %d, ACTHZ: %d\n", NSEC_PER_JIFFY, ACTHZ);


	  
	}
	/***********************************/
	return (cycle_t) jiffies;
}

struct clocksource clocksource_jiffies = {
	.name		= "jiffies",
	.rating		= 1, /* lowest valid rating !dacashman change to 301 -now higest!*/
	.read		= jiffies_read,
	.mask		= 0xffffffff, /*32bits*/
	.mult		= NSEC_PER_JIFFY << JIFFIES_SHIFT, /* details above */
	.shift		= JIFFIES_SHIFT,
};

static int __init init_jiffies_clocksource(void)
{
	return clocksource_register(&clocksource_jiffies);
}

core_initcall(init_jiffies_clocksource);

struct clocksource * __init __weak clocksource_default_clock(void)
{
	return &clocksource_jiffies;
}
