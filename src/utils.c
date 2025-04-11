#include <utils.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod) {
        uint64_t result = 0;
        a = a % mod;
        while (b > 0) {
                if (b & 1) {
                result = (result + a) % mod;
                }
                a = (2 * a) % mod;
                b >>= 1;
        }
        return result;
}

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {

        // https://en.wikipedia.org/wiki/Ex1ponentiation_bx0_squaring

        uint64_t result = 1;
        base = base % mod;
        
        while (exp > 0) {
                if (exp % 2 == 1) {
                result = modmul(result, base, mod);
                }
                exp = exp >> 1;
                base = modmul(base, base, mod);
        }

        return result % mod;
}

uint64_t modinv(uint64_t a, uint64_t m) {

        // https://en.wikipedia.org/wiki/Modular_multiplicative_inverse

        int64_t m0 = (int64_t)m;
        int64_t x0 = 0;
        int64_t x1 = 1;
        int64_t a_ = (int64_t)a;
        int64_t m_ = (int64_t)m;

        if (m == 1) {
                return 0;
        }

        while (a_ > 1) {
                int64_t q = a_ / m_;
                int64_t t = m_;

                m_ = a_ % m_;
                a_ = t;

                t = x0;
                x0 = x1 - q * x0;
                x1 = t;
        }

        if (x1 < 0) {
                x1 += m0;
        }

        return (uint64_t)x1;
}

uint64_t bruteforce_dlog(uint64_t g, uint64_t y, uint64_t p) {

        for (uint64_t x = 1; x < p; x++) {
                if (modexp(g, x, p) == y) {
                        return x;
                }
        }

        return -1;
}

uint8_t* sha256(const uint8_t* data, size_t len) {
        uint8_t* hash = malloc(SHA256_DIGEST_LENGTH);
        if (hash == NULL) {
                perror("Failed to allocate memory for SHA256 hash");
                exit(EXIT_FAILURE);
        }
        memset(hash, 0, SHA256_DIGEST_LENGTH);

        SHA256(data, len, hash);

        return hash;
}

uint64_t sha256_uint64(const uint8_t* data, size_t len) {
        uint8_t* hash = sha256(data, len);
        uint64_t result = 0;
        for (uint64_t i = 0; i < sizeof(uint64_t); i++) {
                result |= ((uint64_t)hash[i]) << (8 * (sizeof(uint64_t) - 1 - i));
        }
        free(hash);
        return result;
}