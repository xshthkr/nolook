#include <zkp.h>
#include <utils.h>

#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

zkp_params_t zkp_init_params(uint64_t p, uint64_t q, uint64_t g) {
        zkp_params_t params;
        params.p = p;
        params.q = q;
        params.g = g;
        return params;
}

uint64_t zkp_gen_publickey(zkp_params_t* params, uint64_t x) {
        return modexp(params->g, x, params->p);
}

uint64_t zkp_gen_commitment(zkp_params_t* params, uint64_t r) {
        return modexp(params->g, r, params->p);
}

uint64_t zkp_random(uint64_t lower, uint64_t upper) {
        if (lower > upper) {
                uint64_t temp = lower;
                lower = upper;
                upper = temp;
        }

        uint64_t range = upper - lower + 1;
        uint64_t rand_val;
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd < 0) {
                perror("Failed to open /dev/urandom");
                exit(EXIT_FAILURE);
        }

        uint16_t limit = UINT_MAX - (UINT_MAX % range);

        do {
                if (read(fd, &rand_val, sizeof(rand_val)) != sizeof(rand_val)) {
                perror("Failed to read random bytes");
                close(fd);
                exit(EXIT_FAILURE);
                }
        } while ((uint16_t)rand_val >= limit);

        close(fd);
        return lower + ((uint16_t)rand_val % range);
}

uint64_t zkp_gen_response(zkp_params_t* params, uint64_t r, uint64_t c, uint64_t x) {
        return (r + ((c * x) % params->q)) % params->q;
}

uint8_t zkp_verify(zkp_params_t* params, uint64_t y, uint64_t t, uint64_t c, uint64_t s) {
        uint64_t left = modexp(params->g, s, params->p);
        uint64_t right = modmul(t, modexp(y, c, params->p), params->p);
        return left == right;
}

void zkp_print_params(zkp_params_t* params) {
        printf("p: %lu\n", params->p);
        printf("q: %lu\n", params->q);
        printf("g: %lu\n", params->g);
}

void zkp_print_alice(zkp_alice_t* alice) {
        printf("Alice's private key (x): %lu\n", alice->x);
        printf("Alice's public key (y): %lu\n", alice->y);
        printf("Alice's random value (r): %lu\n", alice->r);
        printf("Alice's commitment (t): %lu\n", alice->t);
        printf("Alice's response (s): %lu\n", alice->s);
        printf("Alice's challenge niZKP (c): %lu\n", alice->c);
}

void zkp_print_bob(zkp_bob_t* bob) {
        printf("Bob's challenge (c): %lu\n", bob->c);
        printf("Bob's verification result: %s\n", bob->result ? "Valid" : "Invalid");
}

void zkp_init_alice(zkp_alice_t* alice, zkp_params_t* params) {
        alice->params = params;
        alice->x = 0;
        alice->y = 0;
        alice->c = 0;
        alice->r = 0;
        alice->t = 0;
        alice->s = 0;
}

void zkp_init_bob(zkp_bob_t* bob, zkp_params_t* params) {
        bob->params = params;
        bob->c = 0;
        bob->result = 0;
}

uint64_t zkp_hash(zkp_params_t* params, uint64_t y, uint64_t t) {

        // hash with domain separation
        // stream = "zkp|" + params->g + "|" + y + "|" + t

        size_t stream_size = 3 * sizeof(uint64_t) + 6 * sizeof(uint8_t);
        uint8_t* stream = malloc(stream_size);
        if (stream == NULL) {
                perror("Failed to allocate memory for stream");
                exit(EXIT_FAILURE);
        }
        size_t offset = 0;

        const char* prefix = "zkp|";
        size_t prefix_len = 4 * sizeof(uint8_t);
        memcpy(stream + offset, prefix, prefix_len);
        offset += prefix_len;

        const char* separator = "|";
        size_t separator_len = sizeof(uint8_t);

        memcpy(stream + offset, &params->g, sizeof(uint64_t));
        offset += sizeof(uint64_t);

        memcpy(stream + offset, separator, separator_len);
        offset += separator_len;

        memcpy(stream + offset, &y, sizeof(uint64_t));
        offset += sizeof(uint64_t);

        memcpy(stream + offset, separator, separator_len);
        offset += separator_len;

        memcpy(stream + offset, &t, sizeof(uint64_t));
        offset += sizeof(uint64_t);

        uint64_t hash = sha256_uint64(stream, 3 * sizeof(uint64_t));
        free(stream);
        return hash;       
}