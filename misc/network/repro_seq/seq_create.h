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

u32 net_secret[]= {0x01 };
unsigned char net_secret_char[] = {}

#endif /* SEQ_CREATE_H */
