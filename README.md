# NOLOOK

A simulation of interactive and non-interactive zero knowledge proof protocols (Schnorr's Protocol and Flat-Shamir heuristic) for password authentication.

## Features

- Interactive and Non-interactive ZKP modes
- Rejection of invalid/tampered proofs
- Domain separation in hashing (`zkp|`) to avoid collisions
- Reads from `/dev/urandom` for secure randomness

## Installation and Usage

Clone the repository and build the project.

```bash
git clone https://github.com/xshthkr/nolook.git
cd nolook
make
```

The binary will be in the `bin` directory.

```bash
./bin/nolook
```

> [!CAUTION]
> There is a suspected bug where an overflow causes the verification to yield invalid. Will be fixing that soon.

## Technical Details

### Interactive ZKP (Schnorr's Protocol)

1. **Commitment**: Alice picks random `r`, sends `t = g^r mod p` to Bob
2. **Challenge**: Bob replies with random `c`
3. **Response**: Alice computes `s = (r - c·x) mod q` and sends it
4. **Verification**: Bob checks `g^s ?= t·y^c mod p`

### Non-Interactive ZKP (Fiat-Shamir Heuristic)

There is no Bob interacting with Alice. Challenge `c` is derived via:

```c
  c = Hash("zkp|" + g "|" + y + "|" + t) % q;
```

The rest of the protocl remains the same.

## Requirements

- Linux (uses `/dev/urandom`)
- OpenSSL (`libssl-dev` or `openssl` headers) for SHA256
- C99 or later
- 64-bit system (uses `uint64_t`)

## References

- **Schnorr's Original Paper**: [Efficient Signature Generation by Smart Cards](https://publikationen.ub.uni-frankfurt.de/opus4/frontdoor/deliver/index/docId/4280/file/schnorr.pdf)
- **Secure Remote Password protocol**: [Stanford](http://srp.stanford.edu/)
