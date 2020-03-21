#include <stdio.h>

#include <stdbool.h> 
#include <stdlib.h>	
#include <string.h>
#include <ctype.h>

bool isCharacterAscii(const char ch)
{
	return isascii(ch);
}

int32_t text_7bit_encode(const char* txt_in, char** txt_out) {
	if (txt_in == NULL)
		return -3;
	const unsigned int in_length = strlen(txt_in);
	if (in_length > 32767)
		return -2; // length of string is to big(interface restrictions)

	// note: reduced size of string as MSB will be removed, + '\0'
	const int32_t out_length = in_length > 8 ? in_length - (in_length / 8) + 1 : in_length;

	// note: malloc for expected count of symbols regarding 8b/7b encode
	char *tmp_out = malloc(sizeof(char) * out_length);
	for (unsigned int byte_id = 0; byte_id < in_length; ++byte_id) {
		unsigned int enc_word_start_bit_id = byte_id * 7;
		if (isCharacterAscii(txt_in[byte_id]))
		{
			for (int i = 0; i < 7; i++) {
				if ((txt_in[byte_id] >> i) & 1U) {

					if ((enc_word_start_bit_id + i) % 8 != 7) {
						tmp_out[(enc_word_start_bit_id + i) / 8] ^= (-1 ^ tmp_out[byte_id]) & (1 << (enc_word_start_bit_id + i) % 8);
					}
				}
			}
		}
		else
			return -1;
	}

	/// note: mechanism of alloc/dealloc memory has to be defined to avoid misleadings
	*txt_out = tmp_out;

	return out_length;
}

struct ExtraBit {
	bool extra_bit;
	int extra_byte_id;
	int extra_bit_id;
};

void initExtraBit(struct ExtraBit* eb)
{
	eb->extra_bit = false;
	eb->extra_byte_id = -1;
	eb->extra_bit_id = -1;
}

int32_t text_7bit_decode(const char* txt_in, char** txt_out)
{
	if (txt_in == NULL)
		return -3;
	const unsigned int in_length = strlen(txt_in);

	if (in_length > 32767)
		return -2; // length of string is to big(interface restrictions)

	// note: increased size of string as will set to 0
	const int32_t out_length = in_length > 7 ? (in_length / 7) * 8 : in_length;

	// note: malloc for expected count of symbols regarding 8b/7b encode
	char *tmp_out = calloc(out_length, 1);

	/// todo: check if such storage can be removed
	/// extra bits have to be set, but without override
	struct ExtraBit extra_bits[8];
	for (int eb = 0; eb < 8; ++eb) {
		initExtraBit(&extra_bits[eb]);
	}

	for (unsigned int byte_id = 0; byte_id < in_length; ++byte_id) {
		for (int i = 0; i < 8; i++) {

			if ((txt_in[byte_id] >> i) & 1U) {
				tmp_out[(byte_id * 8 + i) / 7] ^= (-1 ^ tmp_out[byte_id]) & (1 << ((byte_id * 8 + i) % 7));

				if ((byte_id * 8 + i) % 8 == 0) {

					for (int ext = 0; ext < 8; ++ext) {
						if (extra_bits[ext].extra_bit == false) {
							extra_bits[ext].extra_bit = true;
							extra_bits[ext].extra_byte_id = ((byte_id * 8 + i) / 7) + 1;
							extra_bits[ext].extra_bit_id = ((byte_id * 8 + i) % 7);
							break;
						}
					}
				}
			}
		}
	}

	for (int ee = 0; ee < 8; ++ee) {
		if (extra_bits[ee].extra_bit == true) {
			tmp_out[extra_bits[ee].extra_byte_id] ^= (-1 ^ tmp_out[extra_bits[ee].extra_byte_id]) & (1 << extra_bits[ee].extra_bit_id);
		}
	}

	*txt_out = tmp_out;
	return out_length;
}