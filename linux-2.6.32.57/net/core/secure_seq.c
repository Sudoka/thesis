#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cryptohash.h>
#include <linux/module.h>
#include <linux/cache.h>
#include <linux/random.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/string.h>

#include <net/secure_seq.h>

static u32 net_secret[MD5_MESSAGE_BYTES / 4] ____cacheline_aligned;

static int __init net_secret_init(void)
{
	get_random_bytes(net_secret, sizeof(net_secret));
        
        /* dcashman change - print out net_secret */
        printk(KERN_DEBUG "BEGIN NET_SECRET:\n");
	int i=0;
	unsigned char *byte_ptr;
	for(i=0; i < sizeof(net_secret)/4; i++){
	  byte_ptr = (unsigned char *) &net_secret[i];
	  int j=0;
	  for(j=0; j < sizeof(net_secret[i]); j++){
	    printk(KERN_DEBUG "\\x%x", *(byte_ptr+j));
	  }
	}
	printk(KERN_DEBUG "\nEND NET_SECRET\n");

	return 0;
}
late_initcall(net_secret_init);

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
        u64 temp_addition = (ktime_to_ns(ktime_get_real()) >> 6);
	u64 temp_addition2 = (jiffies_to_ns() >> 6);
	printk(KERN_DEBUG "TIME Value of ktime_to_ns at seq_scale %llu\n", temp_addition);
	printk(KERN_DEBUG "TIME Value of jiffies_to_ns at seq_scale %llu\n", temp_addition2);
	printk(KERN_DEBUG "Value of seq+ns reading 64-bit: %llu\n", seq+temp_addition);
	printk(KERN_DEBUG "Value of seq+ns reading 32-bit: %u\n", seq+temp_addition);

        return seq /* + (ktime_to_ns(ktime_get_real()) >> 6) */;
}

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
__u32 secure_tcpv6_sequence_number(__be32 *saddr, __be32 *daddr,
				   __be16 sport, __be16 dport)
{
	u32 secret[MD5_MESSAGE_BYTES / 4];
	u32 hash[MD5_DIGEST_WORDS];
	u32 i;

	memcpy(hash, saddr, 16);
	for (i = 0; i < 4; i++)
		secret[i] = net_secret[i] + daddr[i];
	secret[4] = net_secret[4] +
		(((__force u16)sport << 16) + (__force u16)dport);
	for (i = 5; i < MD5_MESSAGE_BYTES / 4; i++)
		secret[i] = net_secret[i];

	md5_transform(hash, secret);

	return seq_scale(hash[0]);
}
EXPORT_SYMBOL(secure_tcpv6_sequence_number);

u32 secure_ipv6_port_ephemeral(const __be32 *saddr, const __be32 *daddr,
			       __be16 dport)
{
	u32 secret[MD5_MESSAGE_BYTES / 4];
	u32 hash[MD5_DIGEST_WORDS];
	u32 i;

	memcpy(hash, saddr, 16);
	for (i = 0; i < 4; i++)
		secret[i] = net_secret[i] + (__force u32) daddr[i];
	secret[4] = net_secret[4] + (__force u32)dport;
	for (i = 5; i < MD5_MESSAGE_BYTES / 4; i++)
		secret[i] = net_secret[i];

	md5_transform(hash, secret);

	return hash[0];
}
#endif

#ifdef CONFIG_INET
__u32 secure_ip_id(__be32 daddr)
{
	u32 hash[MD5_DIGEST_WORDS];

	hash[0] = (__force __u32) daddr;
	hash[1] = net_secret[13];
	hash[2] = net_secret[14];
	hash[3] = net_secret[15];

	md5_transform(hash, net_secret);

	return hash[0];
}

__u32 secure_ipv6_id(const __be32 daddr[4])
{
	__u32 hash[4];

	memcpy(hash, daddr, 16);
	md5_transform(hash, net_secret);

	return hash[0];
}

__u32 secure_tcp_sequence_number(__be32 saddr, __be32 daddr,
				 __be16 sport, __be16 dport)
{

	/* dcashman change - determing byte order */
	printk(KERN_DEBUG "TCP_SEQ BYTE ORDER TEST\n");
        printk(KERN_DEBUG "saddr normal: %x\n", saddr);
        printk(KERN_DEBUG "saddr htonl: %x\n", htonl(saddr));
	printk(KERN_DEBUG "saddr ntohl: %x\n", ntohl(saddr));
	printk(KERN_DEBUG "net_secret[15]: %x\n", net_secret[15]);
        /* dcashman end change */

	u32 hash[MD5_DIGEST_WORDS];

	hash[0] = (__force u32)saddr;
	hash[1] = (__force u32)daddr;
	hash[2] = ((__force u16)sport << 16) + (__force u16)dport;
	hash[3] = net_secret[15];

	md5_transform(hash, net_secret);
        
	/*dcashman change - print seq num? */
	printk(KERN_DEBUG "Sequence num: %x\n", hash[0]);
	printk(KERN_DEBUG "Sequence num ntohl: %x\n", htonl(hash[0]));
	/*dcashman change end */
	return seq_scale(hash[0]);
}

u32 secure_ipv4_port_ephemeral(__be32 saddr, __be32 daddr, __be16 dport)
{
	u32 hash[MD5_DIGEST_WORDS];

	hash[0] = (__force u32)saddr;
	hash[1] = (__force u32)daddr;
	hash[2] = (__force u32)dport ^ net_secret[14];
	hash[3] = net_secret[15];

	md5_transform(hash, net_secret);

	return hash[0];
}
EXPORT_SYMBOL_GPL(secure_ipv4_port_ephemeral);
#endif

#if defined(CONFIG_IP_DCCP) || defined(CONFIG_IP_DCCP_MODULE)
u64 secure_dccp_sequence_number(__be32 saddr, __be32 daddr,
				__be16 sport, __be16 dport)
{
	u32 hash[MD5_DIGEST_WORDS];
	u64 seq;

	hash[0] = (__force u32)saddr;
	hash[1] = (__force u32)daddr;
	hash[2] = ((__force u16)sport << 16) + (__force u16)dport;
	hash[3] = net_secret[15];

	md5_transform(hash, net_secret);

	seq = hash[0] | (((u64)hash[1]) << 32);
	seq += ktime_to_ns(ktime_get_real());
	seq &= (1ull << 48) - 1;

	return seq;
}
EXPORT_SYMBOL(secure_dccp_sequence_number);

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
u64 secure_dccpv6_sequence_number(__be32 *saddr, __be32 *daddr,
				  __be16 sport, __be16 dport)
{
	u32 secret[MD5_MESSAGE_BYTES / 4];
	u32 hash[MD5_DIGEST_WORDS];
	u64 seq;
	u32 i;

	memcpy(hash, saddr, 16);
	for (i = 0; i < 4; i++)
		secret[i] = net_secret[i] + daddr[i];
	secret[4] = net_secret[4] +
		(((__force u16)sport << 16) + (__force u16)dport);
	for (i = 5; i < MD5_MESSAGE_BYTES / 4; i++)
		secret[i] = net_secret[i];

	md5_transform(hash, secret);

	seq = hash[0] | (((u64)hash[1]) << 32);
	seq += ktime_to_ns(ktime_get_real());
	seq &= (1ull << 48) - 1;

	return seq;
}
EXPORT_SYMBOL(secure_dccpv6_sequence_number);
#endif
#endif
