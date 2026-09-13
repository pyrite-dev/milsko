#include <Mw/Milsko.h>

/* this code was taken from my old code... :) */

#define CAST_I32(x) ((int)(x))

static int utf8_count(unsigned char c) {
	if(c < 0x80) {
		return 1;
	}
	if(0xc2 <= c && c < 0xe0) {
		return 2;
	}
	if(0xe0 <= c && c < 0xf0) {
		return 3;
	}
	if(0xf0 <= c && c < 0xf8) {
		return 4;
	}
	return 0;
}

static int utf8_later(unsigned char c) {
	return 0x80 <= c && c < 0xc0;
}

int MwUTF8ToUTF32(const char* input, int* output) {
	const unsigned char* inbuf = (const unsigned char*)input;
	int		     b	   = utf8_count(inbuf[0]);
	if(b == 0) return 0;

	if(b == 1) *output = inbuf[0];
	if(b == 2) {
		if(!utf8_later(inbuf[1])) return 0;
		if((inbuf[0] & 0x1e) == 0) return 0;

		*output = CAST_I32(inbuf[0] & 0x1f) << 6;
		*output |= CAST_I32(inbuf[1] & 0x3f);
	}
	if(b == 3) {
		if(!utf8_later(inbuf[1]) || !utf8_later(inbuf[2])) return 0;
		if((inbuf[0] & 0x0f) == 0 && (inbuf[1] & 0x20) == 0) return 0;

		*output = CAST_I32(inbuf[0] & 0x0f) << 12;
		*output |= CAST_I32(inbuf[1] & 0x3f) << 6;
		*output |= CAST_I32(inbuf[2] & 0x3f);
	}
	if(b == 4) {
		if(!utf8_later(inbuf[1]) || !utf8_later(inbuf[2]) || !utf8_later(inbuf[3])) return 0;
		if((inbuf[0] & 0x07) == 0 && (inbuf[1] & 0x30) == 0) return 0;

		*output = CAST_I32(inbuf[0] & 0x07) << 18;
		*output |= CAST_I32(inbuf[1] & 0x3f) << 12;
		*output |= CAST_I32(inbuf[2] & 0x3f) << 6;
		*output |= CAST_I32(inbuf[3] & 0x3f);
	}

	return b;
}

int MwUTF8ToUTF16(const char* input, wchar_t* output) {
	int o;
	int n = MwUTF8ToUTF32(input, &o);

	if(n == 0 || n >= 0x10ffff) return 0;

	if(o < 0x10000) {
		output[0] = o;
		output[1] = 0;
	} else {
		output[0] = (o - 0x10000) / 0x400 + 0xd800;
		output[1] = (o - 0x10000) % 0x400 + 0xdc00;
		output[2] = 0;
	}

	return n;
}

wchar_t* MwUTF8TextToUTF16Text(const char* input) {
	wchar_t* o = malloc((MwUTF8Length(input) * 2 + 1) * sizeof(*o));
	int	 n = 0;
	wchar_t	 buffer[3];

	o[0] = 0;

	while(input[0] != 0) {
		int new = MwUTF8ToUTF16(input, buffer);

		if(new == 0) {
			free(o);
			return NULL;
		}

		memcpy(o + n, buffer, wcslen(buffer) * sizeof(*o));

		input += new;
		n += wcslen(buffer);
	}

	o[n] = 0;

	return o;
}

int MwUTF8Length(const char* input) {
	int out;
	int len = 0;

	while(input[0] != 0) {
		int new;

		input += (new = MwUTF8ToUTF32(input, &out));
		len++;

		if(new == 0) return -1;
	}

	return len;
}

int MwUTF8Copy(const char* src, int srcskip, char* dst, int dstskip, int len) {
	int i;
	int out;
	int total = 0;
	for(i = 0; i < srcskip; i++) src += MwUTF8ToUTF32(src, &out);
	for(i = 0; i < dstskip; i++) dst += MwUTF8ToUTF32(dst, &out);
	for(i = 0; i < len; i++) {
		int len;
		if(src[0] == 0) break;

		len = MwUTF8ToUTF32(src, &out);

		memcpy(dst, src, len);

		src += len;
		dst += len;
		total += len;
	}
	dst[0] = 0;

	return total;
}

static int is_u16_high(wchar_t ch) {
	return 0xd800 <= ch && ch < 0xdc00;
}

static int is_u16_low(wchar_t ch) {
	return 0xdc00 <= ch && ch < 0xe000;
}

int MwUTF16Count(const wchar_t* input) {
	if(is_u16_high(input[0])) {
		if(is_u16_low(input[1])) {
			return 2;
		} else if(input[1] == 0) {
			return 2;
		} else {
			return 0;
		}
	} else if(is_u16_low(input[0])) {
		if(input[1] == 0) {
			return 2;
		} else {
			return 0;
		}
	} else {
		return 1;
	}
}

int MwUTF16ToUTF8(const wchar_t* input, char* output) {
	if(is_u16_high(input[0])) {
		if(is_u16_low(input[1])) {
			return MwUTF32ToUTF8(0x10000 + (CAST_I32(input[0]) - 0xd800) * 0x400 + (CAST_I32(input[1]) - 0xdc00), output);
		} else if(input[1] == 0) {
			return MwUTF32ToUTF8(input[0], output);
		} else {
			return 0;
		}
	} else if(is_u16_low(input[0])) {
		if(input[1] == 0) {
			return MwUTF32ToUTF8(input[0], output);
		} else {
			return 0;
		}
	} else {
		return MwUTF32ToUTF8(input[0], output);
	}
}

char* MwUTF16TextToUTF8Text(const wchar_t* input) {
	char* o = malloc(4 * wcslen(input) + 1);
	int   n = 0;
	char  buffer[4];

	o[0] = 0;

	while(input[0]) {
		int new = MwUTF16ToUTF8(input, buffer);

		if(new == 0) {
			free(o);
			return NULL;
		}

		memcpy(o + n, buffer, new);

		input += MwUTF16Count(input);
		n += new;
	}

	o[n] = 0;

	return o;
}

int MwUTF32ToUTF8(int input, char* output) {
	if(input < 128) {
		output[0] = input;
		return 1;
	} else if(input < 2048) {
		output[0] = 0xc0 | (input >> 6);
		output[1] = 0x80 | (input & 0x3f);
		return 2;
	} else if(input < 65536) {
		output[0] = 0xe0 | (input >> 12);
		output[1] = 0x80 | ((input >> 6) & 0x3f);
		output[2] = 0x80 | (input & 0x3f);
		return 3;
	} else {
		output[0] = 0xf0 | (input >> 18);
		output[1] = 0x80 | ((input >> 12) & 0x3f);
		output[2] = 0x80 | ((input >> 6) & 0x3f);
		output[3] = 0x80 | (input & 0x3f);
		return 4;
	}
}
