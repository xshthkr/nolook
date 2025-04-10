#include <utils.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main() {

        // test 1
        uint64_t base = 2;
        uint64_t exp = 10;
        uint64_t mod = 1000;
        uint64_t result = modexp(base, exp, mod);
        // printf("result: %lu\n", result);
        assert(result == 24);

        // test 2 (large values)
        base = 123456789;
        exp = 987654321;
        mod = 1000000009;
        result = modexp(base, exp, mod);
        // printf("result: %lu\n", result);
        assert(result == 917281796);

        // test 3 (edge case)
        base = 0;
        exp = 0;
        mod = 1;
        result = modexp(base, exp, mod);
        // printf("result: %lu\n", result);
        assert(result == 0);

        printf("All tests passed.\n");
        return 0;
}