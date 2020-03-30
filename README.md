` encoder7bit `

# about:
Library provides an implementation for encoding bytes into 7bit data blocks. And another direction: where encoded data can be decoded back. The most significant bit will be not used for the encoding, but at this place, first bit of the next byte will be stored. Compression happens because each byte will be stored as 7bits instead of 8bits, as a result, output data length will be reduced.

## encoding:
Encoding in the defined interface will take an input string and after applying encoding(compression) will return as the second parameter encoded string.
Also, the input string will be checked if inside only [ASCII](https://en.wikipedia.org/wiki/ASCII) symbols, otherwise encode will return fail.

## decoding:
Decoding string into the output string. Where every 7 bits of data will be used for decoded byte and the most significant byte will be set to 0.

## restrictions:
According to rules of encoding where 8bits will be compressed to 7bits, which means that output[(encoded/compressed)](https://en.wikipedia.org/wiki/Binary_code#Coding_systems) string length will be reduced on 1 bit per byte. That means to verify or test compression better to use some rules of provided data. For example, every 8 bytes will be encoded into 7 bytes.(better to have input string divisible on 8). In case of encoding for example 6 bytes encoded data block length will also have 6 bytes but the last 6 bits in 6th byte will be unused.
Another important thing is about byte order which depends on a platform where the "encode7bit" library/executable is running. In case of encoding under one platform and decode under another one, then appears a risk that bits order will be different and unexpected result can happen.
To avoid such issue maybe additional feature has to be added where will be checked endianness depending on the running platform. 

