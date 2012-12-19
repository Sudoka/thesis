/* seq_create.c - a script to replicate the sequence number creation inside of  
 *   the kernel.  Creating a sequence number requires 6 pieces of information:
 *     net_secret[XXX] - initialized at boot time from entropy store
 *     md5 func - the specific md5 function used in linux
 *     src_addr - the source ip addr (in network byte order?)
 *     src_port - the source port (in network byte order?)
 *     dest_addr - the destination ip_addr (in network byte orde?)
 *     dest_port - the destination port (in network byte order?)
 * Dan Cashman
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
/*#include <sys/types.h>
  #include <asm/types.h> */
#include <linux/types.h>
#include "md5.h"
#include "seq_create.h"


extern u32 net_secret[];

/* exact copy of scaling function from net/core/secure.c */
static u32 seq_scale(u32 seq)
{
	/*
	 *	As close as possible to RFC 793, which
	 *	suggests using a 250 kHz clock.
	 *	Further reading shows this assumes 2 Mb/s networks.
	 *	For 10 Mb/s Ethernet, a 1 MHz clock is appropriate.
	 *	For 10 Gb/s Ethernet, a 1 GHz clock should be ok, but
	 *	we also need to limit the resolution so that the u32 seq
	 *	overlaps less than one time per MSL (2 minutes).
	 *	Choosing a clock of 64 ns period is OK. (period of 274 s)
	 */
        /* dcashman change - got rid of time component */
        return seq /* + (ktime_to_ns(ktime_get_real()) >> 6) */;
}

/* exact copy of tcp_sequence number function from net/core/secure.c */
__u32 secure_tcp_sequence_number(u32 saddr, u32 daddr,
				 u16 sport, u16 dport)
{
	u32 hash[MD5_DIGEST_WORDS];

	hash[0] = saddr;
	hash[1] = daddr;
	hash[2] = (sport << 16) + dport;
	hash[3] = net_secret[15];

	md5_transform(hash, net_secret);

	return seq_scale(hash[0]);
}

int main(int argc, char *argv[]){
  printf("Your \"random\" sequence number is 4!\n");

  return 0;
}
