/*
  This is a maximally equidistributed combined Tausworthe generator
  based on code from GNU Scientific Library 1.5 (30 Jun 2004)

   x_n = (s1_n ^ s2_n ^ s3_n)

   s1_{n+1} = (((s1_n & 4294967294) <<12) ^ (((s1_n <<13) ^ s1_n) >>19))
   s2_{n+1} = (((s2_n & 4294967288) << 4) ^ (((s2_n << 2) ^ s2_n) >>25))
   s3_{n+1} = (((s3_n & 4294967280) <<17) ^ (((s3_n << 3) ^ s3_n) >>11))

   The period of this generator is about 2^88.

   From: P. L'Ecuyer, "Maximally Equidistributed Combined Tausworthe
   Generators", Mathematics of Computation, 65, 213 (1996), 203--213.

   This is available on the net from L'Ecuyer's home page,

   http://www.iro.umontreal.ca/~lecuyer/myftp/papers/tausme.ps
   ftp://ftp.iro.umontreal.ca/pub/simulation/lecuyer/papers/tausme.ps

   There is an erratum in the paper "Tables of Maximally
   Equidistributed Combined LFSR Generators", Mathematics of
   Computation, 68, 225 (1999), 261--269:
   http://www.iro.umontreal.ca/~lecuyer/myftp/papers/tausme2.ps

        ... the k_j most significant bits of z_j must be non-
        zero, for each j. (Note: this restriction also applies to the
        computer code given in [4], but was mistakenly not mentioned in
        that paper.)

   This affects the seeding procedure by imposing the requirement
   s1 > 1, s2 > 7, s3 > 15.

*/

#include <linux/types.h>
#include <linux/percpu.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/random.h>

/* dacashman change - keep track of the number of calls */
static int state_change_num = 0;


struct rnd_state {
	u32 s1, s2, s3;
};

static DEFINE_PER_CPU(struct rnd_state, net_rand_state);

static u32 __random32(struct rnd_state *state)
{
#define TAUSWORTHE(s,a,b,c,d) ((s&c)<<d) ^ (((s <<a) ^ s)>>b)

	state->s1 = TAUSWORTHE(state->s1, 13, 19, 4294967294UL, 12);
	state->s2 = TAUSWORTHE(state->s2, 2, 25, 4294967288UL, 4);
	state->s3 = TAUSWORTHE(state->s3, 3, 11, 4294967280UL, 17);

	/* dacashman change */
	printk(KERN_DEBUG "RANDOM - __random32 called. Change num %d.  Value: %x\t%x\t%x\n", ++state_change_num, state->s1, state->s2, state->s3);

	return (state->s1 ^ state->s2 ^ state->s3);
}

/*
 * Handle minimum values for seeds
 */
static inline u32 __seed(u32 x, u32 m)
{
        /* dacashman change */
	printk(KERN_DEBUG "RANDOM - __seed init called.\n");

	return (x < m) ? x + m : x;
}

/*
 *	random32 - pseudo random number generator
 *
 *	A 32 bit pseudo-random number is generated using a fast
 *	algorithm suitable for simulation. This algorithm is NOT
 *	considered safe for cryptographic use.
 */
u32 random32(void)
{
	unsigned long r;
	struct rnd_state *state = &get_cpu_var(net_rand_state);
	r = __random32(state);
	put_cpu_var(state);

	/* dacashman change */
	printk(KERN_DEBUG "RANDOM - random32 called.  Value: %x\t%x\t%x\n", state->s1, state->s2, state->s3);

	return r;
}
EXPORT_SYMBOL(random32);

/**
 *	srandom32 - add entropy to pseudo random number generator
 *	@seed: seed value
 *
 *	Add some additional seeding to the random32() pool.
 */
void srandom32(u32 entropy)
{

  /* dacashman change */
  struct rnd_state *state_cpy;


	int i;
	/*
	 * No locking on the CPUs, but then somewhat random results are, well,
	 * expected.
	 */
	for_each_possible_cpu (i) {
		struct rnd_state *state = &per_cpu(net_rand_state, i);
		state->s1 = __seed(state->s1 ^ entropy, 1);
		state_cpy = state;
	}

	/* dacashman change */
	printk(KERN_DEBUG "RANDOM - srandom32 called with seed %d. Change num %d Value: %x\t%x\t%x\n", entropy,++state_change_num, state_cpy->s1, state_cpy->s2, state_cpy->s3);
	dump_stack();
}
EXPORT_SYMBOL(srandom32);

/*
 *	Generate some initially weak seeding values to allow
 *	to start the random32() engine.
 */
static int __init random32_init(void)
{

  /* dacashman change */
  struct rnd_state *state_cpy;


	int i;
	
	for_each_possible_cpu(i) {
		struct rnd_state *state = &per_cpu(net_rand_state,i);

#define LCG(x)	((x) * 69069)	/* super-duper LCG */
		state->s1 = __seed(LCG(i + jiffies), 1);
		state->s2 = __seed(LCG(state->s1), 7);
		state->s3 = __seed(LCG(state->s2), 15);

		/* "warm it up" */
		__random32(state);
		__random32(state);
		__random32(state);
		__random32(state);
		__random32(state);
		__random32(state);


		/* dacashman change */
		state_cpy = state;
	}

	/* dacashman change */
	printk(KERN_DEBUG "RANDOM - random32 init called.  Value: %x\t%x\t%x\n", state_cpy->s1, state_cpy->s2, state_cpy->s3);
	return 0;
}
core_initcall(random32_init);

/*
 *	Generate better values after random number generator
 *	is fully initalized.
 */
static int __init random32_reseed(void)
{
	int i;

	for_each_possible_cpu(i) {
		struct rnd_state *state = &per_cpu(net_rand_state,i);
		u32 seeds[3];

		get_random_bytes(&seeds, sizeof(seeds));
		state->s1 = __seed(seeds[0], 1);
		state->s2 = __seed(seeds[1], 7);
		state->s3 = __seed(seeds[2], 15);

		/* mix it in */
		__random32(state);
		/* dacashman change */
	printk(KERN_DEBUG "RANDOM - random32_reseed called.  Value: %x\t%x\t%x\n", state->s1, state->s2, state->s3);
	state_change_num = 0;

	}
	return 0;
}
late_initcall(random32_reseed);
