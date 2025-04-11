#include <zkp.h>
#include <utils.h>

#include <stdint.h>
#include <stdio.h>

uint8_t sim_nizkp(uint64_t p, uint64_t q, uint64_t g, uint64_t x);
uint8_t sim_izkp(uint64_t p, uint64_t q, uint64_t g, uint64_t x);

int main() {

        uint64_t p = 467;
        uint64_t q = 233;
        uint64_t g = 2;
        uint64_t x = 123;

        uint8_t result = sim_nizkp(p, q, g, x);
        // uint8_t = sim_izkp(p, q, g, x);

        printf("Authentication %s\n", result ? "succeeded" : "failed");
}


uint8_t sim_nizkp(uint64_t p, uint64_t q, uint64_t g, uint64_t x) {
        zkp_alice_t alice;
        zkp_params_t params = zkp_init_params(p, q, g);
        printf("Parameters Initialized:\n");
        zkp_print_params(&params);

        zkp_init_alice(&alice, &params);
        alice.x = x;
        alice.y = zkp_gen_publickey(&params, alice.x);
        printf("Alice Initialized:\n");
        printf("Alice's private key: %lu\n", alice.x);

        alice.r = zkp_random(1, params.q - 1);
        alice.t = zkp_gen_commitment(&params, alice.r);
        printf("Alice's commitment: %lu\n", alice.t);

        alice.c = zkp_hash(&params, alice.y, alice.t) % params.q;
        printf("Alice's hash challenge: %lu\n", alice.c);

        alice.s = zkp_gen_response(&params, alice.r, alice.c, alice.x);
        printf("Alice's response: %lu\n", alice.s);

        printf("Sending commitment and response to verifier...\n");

        uint64_t c = zkp_hash(&params, alice.y, alice.t) % params.q;
        printf("Verifier's hash challenge: %lu\n", c);

        printf("Verifying response...\n");

        uint8_t result = zkp_verify(&params, alice.y, alice.t, c, alice.s);
        return result;
}

uint8_t sim_izkp(uint64_t p, uint64_t q, uint64_t g, uint64_t x) {
        zkp_alice_t alice;
        zkp_bob_t bob;

        zkp_params_t params = zkp_init_params(p, q, g);
        printf("Parameters Initialized:\n");
        zkp_print_params(&params);

        zkp_init_alice(&alice, &params);
        zkp_init_bob(&bob, &params);

        alice.x = x;
        alice.y = zkp_gen_publickey(&params, alice.x);
        printf("Alice Initialized:\n");
        printf("Alice's private key: %lu\n", alice.x);

        alice.r = zkp_random(1, params.q - 1);
        alice.t = zkp_gen_commitment(&params, alice.r);
        printf("Alice's commitment: %lu\n", alice.t);

        printf("Sending commitment to Bob...\n");

        bob.c = zkp_random(1, params.q - 1);
        printf("Bob's random challenge: %lu\n", bob.c);

        printf("Sending challenge to Alice...\n");

        alice.s = zkp_gen_response(&params, alice.r, bob.c, alice.x);
        printf("Alice's response: %lu\n", alice.s);

        printf("Sending response to Bob...\n");

        printf("Sending commitment and response to verifier...\n");

        printf("Bob verifying response...\n");

        bob.result = zkp_verify(&params, alice.y, alice.t, bob.c, alice.s);
        return bob.result;
}