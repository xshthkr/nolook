#ifndef ZKP_H
#define ZKP_H

#include <stdint.h>

typedef struct {
        uint64_t p; // prime
        uint64_t q; // prime
        uint64_t g; // generator
} zkp_params_t;

typedef struct {
        uint64_t x, y;
        uint64_t r, t, s, c;
        zkp_params_t* params;
} zkp_alice_t;

typedef struct {
        uint64_t c;
        uint8_t result;
        zkp_params_t* params;
} zkp_bob_t;

zkp_params_t    zkp_init_params(uint64_t p, uint64_t q, uint64_t g);
void            zkp_init_alice(zkp_alice_t* alice, zkp_params_t* params);
void            zkp_init_bob(zkp_bob_t* bob, zkp_params_t* params);
void            zkp_print_params(zkp_params_t* params);
void            zkp_print_alice(zkp_alice_t* alice);
void            zkp_print_bob(zkp_bob_t* bob);
uint64_t        zkp_gen_publickey(zkp_params_t* params, uint64_t x);
uint64_t        zkp_gen_commitment(zkp_params_t* params, uint64_t r);
uint64_t        zkp_random(uint64_t min, uint64_t max);
uint64_t        zkp_hash(zkp_params_t* params, uint64_t y, uint64_t t);
uint64_t        zkp_gen_response(zkp_params_t* params, uint64_t r, uint64_t c, uint64_t x);
uint8_t         zkp_verify(zkp_params_t* params, uint64_t y, uint64_t t, uint64_t c, uint64_t s);

#endif // ZKP_H