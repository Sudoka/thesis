/* utils.h - a collection of simple utility functions to assist with
 *   learning and debugging network-related scripts.
 * Dan Cashman
 */

#ifndef UTILS_H
#define UTILS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* from pg 204 in hacking book by jon erikson */
void dump_bytes(const unsigned char *data_buffer, const unsigned int length);

#endif  //UTILS_H
