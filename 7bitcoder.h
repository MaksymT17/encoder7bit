#include <sys/types.h>

/**
* @brief byte order. Make impact on on decode/encode calculations. WHere bytes
* are being shifted.
* https://en.wikipedia.org/wiki/Endianness
*/

// in case of running encode/decode on different platforms,
// user should be aware that different endianness type make impact on
// decoded/encoded data
enum endianness_type {
  LITTLE_ENDIAN_ = 0, /** LITTLE_ENDIAN byte order*/
  BIG_ENDIAN_ = 1     /** BIG_ENDIAN byte order*/
};

enum error_type {
  STRING_EMPTY = -3, /** String empty provided. Encode/Decode */
  STRING_TOO_BIG = -2, /** String length is out of allowed restriction. Encode/Decode */
  NON_ASCII_CHARACTER = -1 /**< Non ASCII character provided in string,
                              Encode[for Decode have to be added check] */
};

/**
  * @brief Encode function.
  *
  * Encoding 8 bits string into 7 bits. Lossy data compression type.
  * Most significant bit is being removed as result of compression.
  * https://en.wikipedia.org/wiki/Data_compression
  @param txt_in input string.
  @param txt_output encoded string, where compression applied. Due encoding will
   be allocated string in the heap, user should be aware and and perform a clean
  up of this string.
  @return count of bytes returned
*/
int32_t text_7bit_encode(const char *txt_in, char **txt_out);

/**
  * @brief Decode function.
  *
  * Decoding each sequence of 7 bits into byte, where MSB will be set to 0.
  @param txt_in input string.
  @param txt_output decoded string. Due decoding will be allocated string in the
   heap, user should be aware and and perform a clean up of this string.
  @return count of bytes returned
*/
int32_t text_7bit_decode(const char *txt_in, char **txt_out);