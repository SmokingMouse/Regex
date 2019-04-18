#include<stdarg.h>
#include"Bitmap.h"
#include<stdio.h>

bool isIn(int t, ...) {
	va_list args = NULL;
	va_start(args, t);
	char need = va_arg(args, char);
	char arg;
	while (t-- > 0) {
		arg = va_arg(args, char);
		if (arg == need) {
			va_end(args);
			return true;
		}
	}
	va_end(args);
	return false;
}

char normalChar(char c) {
	return c < 0 ? -c - 1 : c;
}

void getCharFromBitset(const Bitmap& b) {
	for (uint8_t i = 0; i < 128; i++) {
		if (b.isSet(i)) printf("%c ", i);
	}
}