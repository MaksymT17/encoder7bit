#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint32_t max_len = 2147483647;

bool isCharacterAscii(const char ch) { return isascii(ch); }

uint32_t encode_get_outId_from_inId(const int32_t inId) {
  return inId - inId / 8;
}

int32_t text_7bit_encode(const char *txt_in, char *txt_out) {
  if (txt_in == NULL || txt_out == NULL)
    return -3;

  const uint32_t in_length = strlen(txt_in);

  if (in_length > max_len) // length of string is to big(interface restrictions)
    return -2;

  // note: reduced size of string as MSB will be removed, + '\0'
  const uint32_t out_length =
      in_length >= 8 ? encode_get_outId_from_inId(in_length * 8) / 8
                     : in_length;

  memset(txt_out, '\0', sizeof(char) * out_length);
  for (unsigned int byte_id = 0; byte_id < in_length; ++byte_id) {
    if (!isCharacterAscii(txt_in[byte_id])) {
      return -1;
    }

    int32_t in_id = byte_id * 8;

    for (unsigned int i = 0; i < 7; i++) {
      if ((txt_in[byte_id] >> i) & 1U) {
        uint32_t out_id = encode_get_outId_from_inId(in_id);
        txt_out[out_id / 8] ^= (-1 ^ txt_out[out_id / 8]) & (1 << out_id % 8);
      }
      ++in_id;
    }
  }
  txt_out[out_length] = '\0';

  return out_length;
}

uint32_t decode_get_outId_from_inId(const int32_t inId) {
  return inId + inId / 7;
}

int32_t text_7bit_decode(const char *txt_in, char *txt_out) {
  if (txt_in == NULL || txt_out == NULL)
    return -3;

  const uint32_t in_length = strlen(txt_in);

  if (in_length > max_len) // length of string is to big(interface restrictions)
    return -2;

  const uint32_t out_length =
      in_length >= 7 ? decode_get_outId_from_inId(in_length * 8) / 8
                     : in_length;

  memset(txt_out, '\0', sizeof(char) * out_length);
  for (unsigned int byte_id = 0; byte_id < in_length; ++byte_id) {

    uint32_t in_id = byte_id * 8;

    for (unsigned int i = 0; i < 8; i++) {
      if ((txt_in[byte_id] >> i) & 1U) {
        uint32_t out_id = decode_get_outId_from_inId(in_id);
        txt_out[out_id / 8] ^= (-1 ^ txt_out[out_id / 8]) & (1 << out_id % 8);
      }
      ++in_id;
    }
  }

  txt_out[out_length] = '\0';

  return out_length;
}
