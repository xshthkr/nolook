#include <zkp.h>
#include <utils.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main () {

        zkp_alice_t alice;
        zkp_bob_t bob;
        zkp_params_t params = zkp_init_params(23, 11, 2);
        zkp_init_alice(&alice, &params);
        zkp_init_bob(&bob, &params);

        // zkp_print_params(&params);
        // zkp_print_alice(&alice);
        // zkp_print_bob(&bob);

        // step 0: setup params
        // alice picks secret x in Zq
        // she computes y = g^x mod p
        // she shares p, q, g, y with bob
        alice.x = 6; // private key
        alice.y = zkp_gen_publickey(&params, alice.x); // public key

        // printf("\nstep 0: alice generates public key\n");
        // zkp_print_alice(&alice);
        // zkp_print_bob(&bob);
        
        // step 1: alice generates a commitment
        // she picks a random value r in Zq
        // she computes t = g^r mod p
        // she sends t to bob
        alice.r = zkp_random(1, params.q - 1);
        alice.t = zkp_gen_commitment(&params, alice.r);

        // printf("\nstep 1: alice generates commitment\n");
        // zkp_print_alice(&alice);
        // zkp_print_bob(&bob);

        // step 2: bob generates a challenge
        // he picks a random value c in Zq
        // he sends c to alice
        bob.c = zkp_random(1, params.q - 1);

        // printf("\nstep 2: bob generates challenge\n");
        // zkp_print_alice(&alice);
        // zkp_print_bob(&bob);

        // step 3: alice generates a response
        // she computes s = r + cx mod q
        // she sends s to bob
        alice.s = zkp_gen_response(&params, alice.r, bob.c, alice.x);

        // printf("\nstep 3: alice generates response\n");
        // zkp_print_alice(&alice);
        // zkp_print_bob(&bob);

        // step 4: bob verifies the proof
        // he checks the following equation
        // g^s mod p == t * y^c mod p
        bob.result = zkp_verify(&params, alice.y, alice.t, bob.c, alice.s);

        // printf("\nstep 4: bob verifies proof\n");
        // zkp_print_alice(&alice);
        // zkp_print_bob(&bob);

        assert(bob.result == 1);
        printf("All tests passed.\n");
        return 0;

}