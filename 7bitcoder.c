#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h>	
#include <string.h>
#include <ctype.h>
#include <stdint.h>

const uint32_t max_len = 2147483647;

bool isCharacterAscii(const char ch)
{
	return isascii(ch);
}

int32_t encode_get_outId_from_inId(const int32_t inId) {
	return inId - inId / 8;
}

int32_t text_7bit_encode(const char* txt_in, char** txt_out) {
	if (txt_in == NULL)
		return -3;
	const uint32_t in_length = strlen(txt_in);
	if (in_length > max_len) // length of string is to big(interface restrictions)
		return -2;

	// note: reduced size of string as MSB will be removed, + '\0'
	const uint32_t out_length = in_length >= 8 ? (uint32_t)encode_get_outId_from_inId(in_length * 8) / 8 : in_length;

	// note: alloc for expected count of symbols regarding 8b/7b encode
	// mechanism of allocation can be changed, or more strictly specified to avoid memleaks
	char *tmp_out = (char*)calloc(out_length, sizeof(char));

	for (unsigned int byte_id = 0; byte_id < in_length; ++byte_id) {
		if (!isCharacterAscii(txt_in[byte_id])) {
			return -1;
		}
		for (unsigned int i = 0; i < 7; i++) {
			if ((txt_in[byte_id] >> i) & 1U) {
				int32_t in_id = byte_id * 8 + i;
				int32_t out_id = encode_get_outId_from_inId(in_id);
				tmp_out[out_id / 8] ^= (-1 ^ tmp_out[out_id / 8]) & (1 << out_id % 8);
			}
		}
	}
	tmp_out[out_length] = '\0';
	*txt_out = tmp_out;

	return out_length;
}


int32_t decode_get_outId_from_inId(const int32_t inId) {
	return inId + inId / 7;
}

int32_t text_7bit_decode(const char* txt_in, char** txt_out)
{
	if (txt_in == NULL)
		return -3;
	const uint32_t in_length = strlen(txt_in);

	if (in_length > max_len) // length of string is to big(interface restrictions)
		return -2;

	//todo: add check if user provides unexpected count of bytes, which can not be properly decoded, according to rules
	const int32_t out_length = in_length >= 7 ? (uint32_t)decode_get_outId_from_inId(in_length * 8) / 7 : in_length;

	char *tmp_out = (char*)calloc(out_length, sizeof(char));

	for (unsigned int byte_id = 0; byte_id < in_length; ++byte_id) {
		for (unsigned int i = 0; i < 8; i++) {
			if ((txt_in[byte_id] >> i) & 1U) {
				int32_t in_id = byte_id * 8 + i;
				int32_t out_id = decode_get_outId_from_inId(in_id);
				tmp_out[out_id / 8] ^= (-1 ^ tmp_out[out_id / 8]) & (1 << out_id % 8);
			}
		}
	}
	tmp_out[out_length] = '\0';

	*txt_out = tmp_out;
	return out_length;

}
