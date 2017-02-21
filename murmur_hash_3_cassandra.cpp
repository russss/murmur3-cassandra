#include <stdint.h>

inline int64_t rotsl64(int64_t x, int8_t r) {
    return (x << r) | ((uint64_t)x >> (64 - r));
}

#define ROTSL64(x,y) rotsl64(x,y)
#define BIG_SIGNED_CONSTANT(x) (x##LL)

inline int64_t sfmix(int64_t k) {
    k ^= (uint64_t)k >> 33;
    k *= BIG_SIGNED_CONSTANT(0xff51afd7ed558ccd);
    k ^= (uint64_t)k >> 33;
    k *= BIG_SIGNED_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= (uint64_t)k >> 33;

    return k;
}

void MurmurHash3_x64_128_cassandra(const void *key, const int len,
                         const int64_t seed, void *out) {
    const uint8_t *data = (const uint8_t *)key;
    const int nblocks = len / 16;

    int64_t h1 = seed;
    int64_t h2 = seed;

    const int64_t c1 = BIG_SIGNED_CONSTANT(0x87c37b91114253d5);
    const int64_t c2 = BIG_SIGNED_CONSTANT(0x4cf5ad432745937f);

    //----------
    // body

    const int64_t *blocks = (const int64_t *)(data);

    for (int i = 0; i < nblocks; i++) {
        int64_t k1 = blocks[i * 2 + 0];
        int64_t k2 = blocks[i * 2 + 1];

        k1 *= c1;
        k1    = ROTSL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = ROTSL64(h1, 27);
        h1 += h2;
        h1 = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2    = ROTSL64(k2, 33);
        k2 *= c1;
        h2 ^= k2;

        h2 = ROTSL64(h2, 31);
        h2 += h1;
        h2 = h2 * 5 + 0x38495ab5;
    }

    //----------
    // tail

    const int8_t *tail = (const int8_t *)(data + nblocks * 16);

    int64_t k1 = 0;
    int64_t k2 = 0;

    switch (len & 15) {
    case 15:
        k2 ^= ((int64_t)tail[14]) << 48;
    case 14:
        k2 ^= ((int64_t)tail[13]) << 40;
    case 13:
        k2 ^= ((int64_t)tail[12]) << 32;
    case 12:
        k2 ^= ((int64_t)tail[11]) << 24;
    case 11:
        k2 ^= ((int64_t)tail[10]) << 16;
    case 10:
        k2 ^= ((int64_t)tail[9]) << 8;
    case    9:
        k2 ^= ((int64_t)tail[ 8]) << 0;
        k2 *= c2;
        k2    = ROTSL64(k2, 33);
        k2 *= c1;
        h2 ^= k2;
    case 8:
        k1 ^= ((int64_t)tail[7]) << 56;
    case 7:
        k1 ^= ((int64_t)tail[6]) << 48;
    case 6:
        k1 ^= ((int64_t)tail[5]) << 40;
    case 5:
        k1 ^= ((int64_t)tail[4]) << 32;
    case 4:
        k1 ^= ((int64_t)tail[3]) << 24;
    case 3:
        k1 ^= ((int64_t)tail[2]) << 16;
    case 2:
        k1 ^= ((int64_t)tail[1]) << 8;
    case 1:
        k1 ^= ((int64_t)tail[0]) << 0;
        k1 *= c1;
        k1 = ROTSL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;
    h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = sfmix(h1);
    h2 = sfmix(h2);

    h1 += h2;
    h2 += h1;

    ((int64_t *)out)[0] = h1;
    ((int64_t *)out)[1] = h2;
}
