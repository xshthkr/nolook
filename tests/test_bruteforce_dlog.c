#include <utils.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main () {

        // test 1
        uint64_t g = 2;
        uint64_t y = 8;
        uint64_t p = 11;
        uint64_t x = bruteforce_dlog(g, y, p);
        // printf("x: %lu\n", x);
        assert(x == 3);

        // test 2
        g = 123456789;
        y = 5538589;
        p = 10000000;
        x = bruteforce_dlog(g, y, p);
        // printf("x: %lu\n", x);
        assert(x == 71);

        printf("All tests passed.\n");
}