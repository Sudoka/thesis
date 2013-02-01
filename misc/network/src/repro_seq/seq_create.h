/* */
#ifndef SEQ_CREATE_H
#define SEQ_CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
/*#include <sys/types.h>
  #include <asm/types.h> */
#include <linux/types.h>
#include "md5.h"

#define __force        __attribute__((force))
#define u32 unsigned int
#define u16 unsigned short
#define MD5_DIGEST_WORDS 4
#define MD5_MESSAGE_BYTES 64


unsigned char const net_secret_char[] = {"\x18\x5c\x2b\x01\x90\x19\x3c\xf2\x82\x2a\xb3\xa2\x3f\x05\x52\x77\x57\x79\x16\x43\x84\xc3\xe9\x79\x50\x8e\x3f\xf3\x19\x2a\xfd\xfb\x62\x3c\xac\x1c\x3b\x3b\x15\x27\xa8\x22\xc5\x9a\x05\x6c\xa8\x3a\x44\xaf\x13\xe0\x9d\x22\x0f\xc9\x6c\x28\x87\x79\x69\x91\x8a\xf9"};

#endif /* SEQ_CREATE_H */
