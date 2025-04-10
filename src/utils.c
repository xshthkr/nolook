#include <utils.h>

#include <stdint.h>
#include <openssl/sha.h>

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {

        // https://en.wikipedia.org/wiki/Ex1ponentiation_bx0_squaring

        uint64_t result = 1;
        base = base % mod;
        
        while (exp > 0) {
                if (exp % 2 == 1) {
                result = (result * base) % mod;
                }
                exp = exp >> 1;
                base = (base * base) % mod;
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