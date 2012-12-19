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
#include <errno.h>
#include <arpa/inet.h>
/*#include <sys/types.h>
  #include <asm/types.h> */
#include <linux/types.h>
#include "md5.h"
#include "seq_create.h"
#include "../src/utils.h"

extern unsigned char const net_secret_char[];
static u32 *net_secret = (u32 *)net_secret_char;

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

  printf("Value of net_secret[15] is %x\n", net_secret[15]);  //compare to printk output

  /* do real work */
  unsigned short src_port, dest_port;
  char *src_ip_addr, *dest_ip_addr;
  u32 saddr, daddr;
  u16 sport, dport;
  __u32 seq_num;
  //struct sockaddr_in cli_addr, serv_addr;

  if(argc != 5){
    fprintf(stderr, "Error.  Usage: src-ip, src-port dest-ip dest-port\n");
    return -1;
  }else{
    /* check arguments for validity */
    /* src ip addr */
    src_ip_addr = argv[1];
    if(!isValidIpAddress(src_ip_addr)){
      fprintf(stderr, "Input src-ip address invalid.\n");
      return -1;
    }
    /* src port */
    if((sscanf(argv[2], "%hd", &src_port)) < 0){
      fprintf(stderr, "Input src-port must be a short integer. %s\n", 
	      strerror(errno));
      return -1;
    }
    /* dest ip addr */
    dest_ip_addr = argv[3];
    if(!isValidIpAddress(dest_ip_addr)){
      fprintf(stderr, "Input dest-ip address invalid.\n");
      return -1;
    }
    /* dest port */
    if((sscanf(argv[4], "%hd", &dest_port)) < 0){
      fprintf(stderr, "Input dest-port must be a short integer. %s\n", 
	      strerror(errno));
      return -1;
    }

  }

  /* initialize values */
  saddr =  inet_addr(src_ip_addr);
  sport = htons(src_port);
  daddr =  inet_addr(dest_ip_addr);
  dport = htons(dest_port);

  /* verify values: */
  printf("saddr after init:\t%x\n", saddr);
  
  seq_num = secure_tcp_sequence_number(saddr, daddr, sport, dport);
  
  printf("sequence number without clock:\t%d\n", seq_num);
  printf("sequence number (hex)  without clock:\t%x\n", seq_num);
  
  return 0;
}
