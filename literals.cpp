#include "literals.h"

/**
 * @brief "_b" suffix for 8-bit binary numbers
 * illegal strings (too long or non-binary) will cause an undefined behavior
 * @param str the number to convert
 * @return the byte as an unsigned char
 **/
unsigned int operator"" _b (const char *str) {
	unsigned int ret = 0;
	while (*str != '\0') {
		ret <<= 1;
		if (*str == '1') {
			ret++;
		}
		str++;
	}
	return ret;
}