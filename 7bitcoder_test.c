#include "7bitcoder.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function for dumping into console bits for character into string
// can help in debugging)
void consoleDumpAllBitsPerBytesInString(const char *name, const char *str) {
  const unsigned int in_length = strlen(str);
  printf("dump str:%s len:%d\n", name, in_length);
  for (unsigned int b = 0; b < in_length; ++b) {
    for (unsigned int i = 0; i < 8; ++i) {
      printf("_%d", (str[b] >> i) & 1U);
    }
    printf("\n");
  }
}

bool checkSingleBitEncodeAndDecode() {
  const char *in_str = "a";
  char *out_str = NULL;

  text_7bit_encode(in_str, &out_str);
  // consoleDumpAllBitsPerBytesInString("in_encode", in_str);
  // consoleDumpAllBitsPerBytesInString("out_encode", out_str);

  const char *in_str1 = out_str;
  char *out_str1 = NULL;
  text_7bit_decode(in_str1, &out_str1);
  // consoleDumpAllBitsPerBytesInString("in_decode", in_str1);
  // consoleDumpAllBitsPerBytesInString("out_decode", out_str1);

  // check only single symbol, decoding tried to reserve as byte it defined in
  // rule of compression.
  const bool result = (in_str[0] == out_str1[0]);
  printf("Decode/Encode 1bytes test: %s\n", result ? "passed." : "failed.");

  free(out_str);
  free(out_str1);
  return result;
}

bool checkDecodeEncode_8_to_7_and_decode_back_8bytes() {
  const char *in_str = "abcdefgh";
  char *out_str = NULL;

  text_7bit_encode(in_str, &out_str);

  const char *in_str1 = out_str;
  char *out_str1 = NULL;

  text_7bit_decode(in_str1, &out_str1);

  // check if the same string after decoding for encoded data
  const bool result = strcmp(in_str, out_str1) == 0;
  printf("Decode/Encode 8bytes test: %s\n", result ? "passed." : "failed.");

  free(out_str);
  free(out_str1);
  return result;
}

bool checkDecodeEncode_8_to_7_and_decode_back_16bytes() {
  const char *in_str = "abcdefgh01234567";
  char *out_str = NULL;

  text_7bit_encode(in_str, &out_str);
  // consoleDumpAllBitsPerBytesInString("in_encode", in_str);
  // consoleDumpAllBitsPerBytesInString("out_encode", out_str);

  const char *in_str1 = out_str;
  char *out_str1 = NULL;

  text_7bit_decode(in_str1, &out_str1);
  /// note: for debugging, just uncomment this code
  // consoleDumpAllBitsPerBytesInString("in_decode", in_str1);
  // consoleDumpAllBitsPerBytesInString("out_decode", out_str1);

  const bool result = strcmp(in_str, out_str1) == 0;
  printf("Decode/Encode 16bytes test: %s\n", result ? "passed." : "failed.");

  free(out_str);
  free(out_str1);
  return result;
}

bool checkDecodeEncode_8_to_7_and_decode_back_32bytes() {
  const char *in_str = "abcdefgh01234567abcdefgh01234567";
  char *out_str = NULL;

  text_7bit_encode(in_str, &out_str);

  const char *in_str1 = out_str;
  char *out_str1 = (char *)calloc(10000, sizeof(char));
  text_7bit_decode(in_str1, &out_str1);

  const bool result = strcmp(in_str, out_str1) == 0;
  printf("Decode/Encode 32bytes test: %s\n", result ? "passed." : "failed.");

  free(out_str);
  free(out_str1);
  return result;
}

bool checkEncodeEmptyInputString() {
  const char *in_str = NULL;
  char *out_str = NULL;

  int a = text_7bit_encode(in_str, &out_str);

  bool result = (STRING_EMPTY == (enum error_type)a);
  printf("%s: %s.\n", __FUNCTION__, result ? "OK" : "NOK");
  return result;
}

bool checkDecodeEmptyOutputString() {
  const char *in_str = NULL;
  char *out_str = NULL;

  int a = text_7bit_decode(in_str, &out_str);

  bool result = (STRING_EMPTY == (enum error_type)a);
  printf("%s: %s.\n", __FUNCTION__, result ? "OK" : "NOK");
  return result;
}

enum endianness_type TestByteOrder() {
  short int word = 0x0001;
  char *b = (char *)&word;
  return (b[0] ? LITTLE_ENDIAN_ : BIG_ENDIAN_);
}

int main() {
  enum endianness_type bo = TestByteOrder();
  printf("Tests are running on %s byte order machine.\n",
         bo == LITTLE_ENDIAN_ ? "LITTLE_ENDIAN" : "BIG_ENDIAN.");

  // test for encode some bytes data and decode back and check if decoding was
  // correct. Bytes: 1; 8; 16; 32.  checks can be extended
  if (checkSingleBitEncodeAndDecode() &&
      checkDecodeEncode_8_to_7_and_decode_back_8bytes() &&
      checkEncodeEmptyInputString() && checkDecodeEmptyOutputString() &&
      checkDecodeEncode_8_to_7_and_decode_back_16bytes() &&
      checkDecodeEncode_8_to_7_and_decode_back_32bytes()) {
    printf("Basic tests have been passed.\n");
    return 0;
  } else {
    printf("Basic test failed\n");
  }

  return 1;
}