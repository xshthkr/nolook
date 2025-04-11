#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod);
uint64_t modinv(uint64_t a, uint64_t m);
uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod);

uint64_t bruteforce_dlog(uint64_t g, uint64_t y, uint64_t p);

uint8_t* sha256(const uint8_t* data, size_t len);
uint64_t sha256_uint64(const uint8_t* hash, size_t len);

#endif // UTILS_H