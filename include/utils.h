#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod);
uint64_t modinv(uint64_t a, uint64_t m);

uint64_t bruteforce_dlog(uint64_t g, uint64_t y, uint64_t p);

#endif // UTILS_H