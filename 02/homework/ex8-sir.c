#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char haystack[] = "abcabcabcabc";
	char needle[] = "abc";

	int hayLen = strlen(haystack);
	int needleLen = strlen(needle);

	printf("Found at positions: ");
	for (int i = 0; i <= hayLen - needleLen; i++) {
		if (strncmp(&haystack[i], needle, needleLen) == 0) {
			printf("%d ", i);
		}
	}
	printf("\n");

	return 0;
}
