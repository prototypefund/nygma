# libriot

`cxx` library for indexing and quering network traffic leveraging SIMD based integer compression
techniques.

## integer compression

this contains SIMD implementations for integer compression forked ( and modified ) from
[streamvbyte](https://github.com/lemire/streamvbyte) and
[simdcomp](https://github.com/lemire/simdcomp).

in addition it provides a customized compression strategy which is a crossover between
[k-gamma](https://www.uni-mannheim.de/media/Einrichtungen/dws/Files_People/Profs/rgemulla/publications/schlegel10compression.pdf),
`streamvbyte` and `bitpack`ing.

### streamvqb - custom compression strategy

`streamvqb` is a configurable compression algorithm storing control data in 2 nibbles forming the
control byte for a 2 blocks of integers. the block size is either 2 x 4 integers or 2 x 8
integers. depending on the variant the nibble or `4bit-tag` stores the maximum length of the encoding
of the corresponding integer block. the bit mask of `0b0010` means the maximum bit length of each
integer in the block is 4bits, 16bits in total.

this configuration is called `v128q4x0`: 2x4 int, 4bit length encoding. no alignment.

using the `streamv128q4x0` encoding we want to compress the following sequence of integers: `< 0x801
0x802 0x803 0x804 | 0x805 0x806 0x807 0x808 >`. the maximum bit length of both blocks is `12`. the
control byte therefore is `0b0110 0b0110`. the encoded size is `1 + (12*4+7)/8 + (12*4+7)/8 => 7bytes`
instead of `32bytes` of the uncompressed sequence. in this case the compression using `binpack` would
be better, `streamvbyte** would compress worse.

### streamvqb - stripping trailing zeros in case of aligned numbers

**TBD***

a different encoding strategy might be to also store a shift ammount before the maximum needed
bit-width gets computed. e.g. 4q31 means `3` bits available for length information. and `1` as
alignment requirement. the number of bits shiftet before encoding to supress trailing zeros.

## indexing

