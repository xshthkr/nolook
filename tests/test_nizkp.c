#include <zkp.h>
#include <utils.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main () {

        zkp_alice_t alice;
        zkp_params_t params = zkp_init_params(23, 11, 2);
        zkp_init_alice(&alice, &params);

        // zkp_print_params(&params);
        // zkp_print_alice(&alice);

        // step 0: setup params
        // alice picks secret x in Zq
        // she computes y = g^x mod p
        // she shares p, q, g, y publicly
        alice.x = 6; // private key
        alice.y = zkp_gen_publickey(&params, alice.x); // public key

        // printf("\nstep 0: alice generates public key\n");
        // zkp_print_alice(&alice);
        
        // step 1: alice generates a commitment
        // she picks a random value r in Zq
        // she computes t = g^r mod p
        // she shares t publicly
        alice.r = zkp_random(1, params.q - 1);
        alice.t = zkp_gen_commitment(&params, alice.r);

        // printf("\nstep 1: alice generates commitment\n");
        // zkp_print_alice(&alice);

        // step 2: alice generates a challenge
        // she computes c by hashing y and t
        alice.c = zkp_hash(&params, alice.y, alice.t) % params.q;

        // printf("\nstep 2: alice generates challenge\n");
        // zkp_print_alice(&alice);

        // step 3: alice generates a response
        // alice shares s publicly
        alice.s = zkp_gen_response(&params, alice.r, alice.c, alice.x);

        // printf("\nstep 3: alice generates response\n");
        // zkp_print_alice(&alice);

        // step 4: verifier verifies the proof
        // he checks the following equation
        // g^s mod p == t * y^c mod p
        uint64_t c = zkp_hash(&params, alice.y, alice.t) % params.q;
        // tampering to test
        // c = (c + 1) % params.q;
        // alice.y = (alice.y + 1) % params.p;
        // alice.t = (alice.t + 1) % params.p;
        // alice.s = (alice.s + 1) % params.q;
        uint8_t result = zkp_verify(&params, alice.y, alice.t, c, alice.s);

        assert(result == 1);
        printf("All tests passed.\n");
        return 0;

}