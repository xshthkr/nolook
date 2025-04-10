#include <utils.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main () {

        // test 1
        uint64_t a = 3;
        uint64_t m = 11;
        uint64_t result = modinv(a, m);
        // printf("result: %lu\n", result);
        assert(result == 4);

        // test 2
        a = 123456789;
        m = 1000000009;
        result = modinv(a, m);
        // printf("result: %lu\n", result);
        assert(result == 734806636);

        // test 3
        a = 0;  // 0 is not invertible in mod 1
        m = 1;
        result = modinv(a, m);
        // printf("result: %lu\n", result);
        assert(result == 0);

        printf("All tests passed.\n");
        return 0;
}