#include <Mw/Milsko.h>

char* MwACPTextToUTF8Text(const char* input) {
#if defined(_WIN32) && defined(MW_HAS_WCHAR)
	int wbytes = 0;
	int len;

	wchar_t* wout;
	char*	 mbout;

	wbytes = MultiByteToWideChar(CP_ACP, 0, input, strlen(input), NULL, 0) * sizeof(wchar_t);
	if(wbytes == 0) {
		return MwStringDuplicate(input);
	}

	wout = malloc(wbytes + sizeof(wchar_t));
	len  = wbytes / sizeof(wchar_t);

	memset(wout, 0, wbytes);

	len = MultiByteToWideChar(CP_ACP, 0, input, strlen(input), wout, len);
	if(len == 0) {
		free(wout);
		return MwStringDuplicate(input);
	}
	wout[len] = 0;

	mbout = MwUTF16TextToUTF8Text(wout);

	free(wout);

	return mbout;
#else
	return MwStringDuplicate(input);
#endif
}

char* MwUTF8TextToACPText(const char* input) {
#if defined(_WIN32) && defined(MW_HAS_WCHAR)
	int mbbytes = (strlen(input) + 1) * 4;
	int len;

	wchar_t* wout;
	char*	 mbout = malloc(mbbytes);

	wout = MwUTF8TextToUTF16Text(input);
	len  = wcslen(wout);

	len = WideCharToMultiByte(CP_ACP, 0, wout, len, mbout, mbbytes, 0, 0);
	if(len == 0) {
		free(mbout);
		free(wout);
		return MwStringDuplicate(input);
	}
	mbout[len] = 0;

	free(wout);

	return mbout;
#else
	return MwStringDuplicate(input);
#endif
}
