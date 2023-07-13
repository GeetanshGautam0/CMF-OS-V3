#include "random32.h"

#define TAUSWORTHE(s,a,b,c,d) ((s&c)<<d) ^ (((s <<a) ^ s)>>b)
#define RAND_MAX	32767


unsigned int __random32(struct rnd_state* state) {
    state->s1 = TAUSWORTHE(state->s1, 13, 19, 4294967294UL, 12);
    state->s2 = TAUSWORTHE(state->s2, 2, 25, 4294967288UL, 4);
    state->s3 = TAUSWORTHE(state->s3, 3, 11, 4294967280UL, 17);

    return (state->s1 ^ state->s2 ^ state->s3);
}


char mRand(unsigned int state) {
    state = state * 1664525 + 1013904223;
    return state >> 24;
}

static unsigned long int next = 1; 

int rand(void)
{ 
    next = next * 1103515245 + 12345; 
    return (unsigned int)(next/65536) % RAND_MAX; 
} 

int srand(unsigned int seed) 
{ 
    next = seed; 
    return rand();
} 
