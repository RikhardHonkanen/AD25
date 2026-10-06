# Diffie–Hellman

Diffie–Hellman is the classic key-exchange protocol that lets two parties agree on a shared secret over an insecure channel. It’s built on the math of modular exponentiation with prime numbers.

## How it works with primes

1- Public parameters: Both sides agree on a large prime p and a generator g where 2 ≤ g ≤ p-2. Usually g = 2 or 5.

2- Private keys: Alice picks secret a, Bob picks secret b.

3- Public keys: Alice computes A = g^a mod p and sends it. Bob computes B = g^b mod p and sends it.

4- Shared secret: Alice computes s = B^a mod p. Bob computes s = A^b mod p. Both get the same s because (g^b)^a = (g^a)^b mod p.

Security relies on the discrete log problem: given p, g, g^a mod p, it's hard to find a when p is large, ∼2048+ bits.
