# MurmurHash 3 (Apache Cassandra Variant)

This repository contains an implementation in C++ of the variant
of the MurmurHash3 non-cryptographic hashing function used by
Apache Cassandra for partitioning.

This is incompatible with the [reference
implementation](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp) of
MurmurHash3. If you want a fast, well-characterised non-cryptographic
hash function, use that (or something compatible with it).

To be clear: **you should only use this code if you need to interoperate with
Cassandra's partitioning**.

This code only includes the implementation of the `MurmurHash3_x64_128`
algorithm as this is the algorithm used by Cassandra.

## What's the difference?
Cassandra's [Java implementation of Murmur3
](https://github.com/apache/cassandra/blob/trunk/src/java/org/apache/cassandra/utils/MurmurHash.java)
bafflingly seems to have been written and released without any attempt
to test the output against the reference implementation, a fact
[sheepishly admitted in the
comments](https://github.com/apache/cassandra/blob/trunk/src/java/org/apache/cassandra/utils/MurmurHash.java#L28-L29).

All intermediate variables which are `uint64_t` in the reference
implementation are Java's (signed) `long` type in the Cassandra
implementation. (To be fair, `unsigned long`s weren't available until Java
8, but this doesn't really excuse calling it "Murmur3" everywhere.)

## Test Vectors
Generally, when implementing a hash algorithm it is helpful to test your
implementation against some examples (test vectors).

To assist with anyone else who ends up in my situation in the future,
I have provided [1000 test vectors](/test_vectors.txt), generated by
Cassandra's Java implementation. They are in the format `<input
key>\t<hash value 1>\t<hash value 2>\n`.

This implementation has been tested against these vectors.