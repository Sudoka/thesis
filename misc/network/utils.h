/* utils.h - a collection of simple utility functions to assist with
 *   learning and debugging network-related scripts.
 * Dan Cashman
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>

/* from pg 204 in hacking book by jon erikson */
void dump_bytes(const unsigned char *data_buffer, const unsigned int length);

/* stack overflow ans.  Checks string to see if it is a valid address */
bool isValidIpAddress(char *ipAddress);


#endif  //UTILS_H
