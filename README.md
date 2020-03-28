` encoder7bit `

## about:
Library provides implementation for encoding bytes into 7bit date. Most significant bit will be not used, but at this place first bit of next byte will be stored.
Compression happens because each byte will be stored as 7bits, as result output data length will be reduced.

## encoding:
Encoding in defined interface will take input string and after applying encoding(compression) will return as second parameter encoded string.
Also input string will be checked if inside only ASCII symbols, otherwise encode will return fail.

## decoding:
Decoding string into output string. Where each 7 bits of data will be used for decoded byte and most significant byte will be set to 0.

## restrictions:
According to rules of encode where 8bits will be compressed to 7bits, which means that output(encoded/compressed) string length will be redused on 1 bit per byte. That means to veryfy or test compression
better to use some rules of provided data. For example: each 8 bytes will be endoded into 7 bytes.(better to have input string divisible on 8). In case of encode for example 6 bytes encoded data block length will also has 6 bytes but last 6 bits in 6th byte will be unused.
Other important thing is about byte order which depends on platfrom where "encode7bit" library/executable is running. In case of encode under one platform and decode under other one, then appears a risk that bits order will be dfferent and unexpected result can happen.
To avoid such issue maybe additional feature has to be added where will be checked endianness depending on running platform. 

