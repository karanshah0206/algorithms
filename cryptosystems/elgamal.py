import random

# Function to check if a number is prime
def is_prime(n, k=5):
    if n <= 1:
        return False
    elif n <= 3:
        return True
    elif n % 2 == 0:
        return False

    # Write n as (2^r)*d + 1
    d = n - 1
    r = 0
    while d % 2 == 0:
        d //= 2
        r += 1

    # Witness loop
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

# Function to find a primitive root modulo p
def find_primitive_root(p):
    if not is_prime(p):
        return None
    for g in range(2, p):
        if pow(g, p - 1, p) == 1:
            return g
    return None

# Function to generate public and private keys
def generate_keys(p):
    g = find_primitive_root(p)
    if g is None:
        return None, None, None

    # Private key (a)
    a = random.randint(2, p - 2)
    
    # Public key (A)
    A = pow(g, a, p)

    return g, A, a

# Function to encrypt a message
def encrypt(message, p, g, A):
    k = random.randint(2, p - 2)
    c1 = pow(g, k, p)
    c2 = (message * pow(A, k, p)) % p
    return c1, c2

# Function to decrypt a message
def decrypt(ciphertext, p, a):
    c1, c2 = ciphertext
    m = (c2 * pow(c1, p - 1 - a, p)) % p
    return m

# Example usage
p = 647 # Choose a prime number
g, A, a = generate_keys(p) # Generate keys
if g is None:
    print("Failed to generate keys")
else:
    print("Public key (p, g, A):", (p, g, A))
    print("Private key (a):", a)

    plaintext = 10 # Message to be encrypted

    # Encryption
    ciphertext = encrypt(plaintext, p, g, A)
    print("Ciphertext:", (ciphertext))

    # Decryption
    decrypted_text = decrypt(ciphertext, p, a)
    print("Decrypted text:", decrypted_text)
