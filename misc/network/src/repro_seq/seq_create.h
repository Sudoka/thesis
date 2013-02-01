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

/* net_secret_char[] generated from kernel read -> net_sec_zero_input_jiffies.txt
 *   ----(convert_net_sec.pl)---> net_sec_zero_input_jiffies_bytes.txt
 */
unsigned char const net_secret_char[] = {"\x08\x50\x43\xa4\x0c\x87\xc6\xc3\x70\xf6\xcd\xf8\x38\x8f\xea\xc2\xe5\x9a\x78\xa1\xb8\x9e\x6f\x04\xf0\x55\xc4\x30\xf7\x3e\x74\x73\xe2\xdc\xab\xf2\xa9\xb5\x5e\xdd\x01\x89\x92\x4d\xed\xe8\xdc\x04\x3e\xd5\x00\xf9\x43\xab\x80\x07\xd9\xba\xb8\x93\x24\xdc\x7f\xcf"};

#endif /* SEQ_CREATE_H */
