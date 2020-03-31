` encoder7bit `

# about:
Library provides an implementation for encoding bytes into 7bit data blocks. And another direction: where encoded data can be decoded back. The most significant bit will be not used for the encoding, but at this place, the first bit of the next byte will be stored. Compression happens because each byte will be stored as 7bits instead of 8bits, as a result, output data length will be reduced.

## encoding:
Encoding in the defined interface will take an input string and after applying encoding(compression) will return as the second parameter encoded string.
Also, the input string will be checked if inside only [ASCII](https://en.wikipedia.org/wiki/ASCII) symbols, otherwise encode will return fail.

## decoding:
Decoding string into the allocated output string. Where every 7 bits of data will be used for decoded byte and the most significant byte will be set to 0. Important is that inside of decoding new string will be allocated, which means a user of this function(decode) should take care of freeing returned(decoded) string.

## byte length:
According to rules of encoding where 8bits will be compressed to 7bits, which means that output[(encoded/compressed)](https://en.wikipedia.org/wiki/Binary_code#Coding_systems) string length will be reduced on 1 bit per byte.

For example, every 8 bytes will be encoded into 7 bytes. (better to have input string divisible on 8). In case of encoding, for example, 6 bytes encoded data block length will also have 6 bytes but the last 6 bits in 6th byte will be unused. By this simple logic can be used formulas for the expected decoded or encoded string.

In case of encoding: 
_Output_Length = Input_Length - (Input_Length / 8)_

And in other hand for decoding:
_Output_length = Input_Length + (Input_Length / 7)_

## endianness
Another important thing is about byte order which depends on a platform where the "encode7bit" library/executable is running. In case of encoding under one platform and then decode the same encoded data under another one, appears a risk that bits order will be different and unexpected result can happen. To avoid such issue additional feature has to be added where will be checked endianness depending on the running platform. 

