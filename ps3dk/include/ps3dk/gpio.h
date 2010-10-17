/*
	libps3dk - An open-source PS3 SDK
	GPIO handling

Copyright (C) 2010              Alex Marshall "trap15" <trap15@raidenii.net>

# This code is licensed to you under the terms of the MIT license;
# see file LICENSE or http://www.opensource.org/licenses/mit-license.php
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <ps3dk/syscall.h>

static inline unsigned long int gpio_get(void)
{
	unsigned long int gpio;
	DO_SYSCALL(SYSCALL_GPIO_GET, 0, NULL,  1, &gpio);
	return gpio;
}

static inline void gpio_set(unsigned long int gpio)
{
	DO_SYSCALL(SYSCALL_GPIO_SET, 1, &gpio, 0, NULL);
}

#endif /* _GPIO_H_ */

