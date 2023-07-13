#pragma once

struct rnd_state {
    unsigned int s1, s2, s3;
};

unsigned int __random32(struct rnd_state* state);
char mRand(unsigned int state);
int rand();
int srand(unsigned int seed);