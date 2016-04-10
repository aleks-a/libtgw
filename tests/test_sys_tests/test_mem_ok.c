#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *tmp = malloc(128);
	snprintf(tmp, 128, "Hello mem\n");
	puts(tmp);
	free(tmp);
	
	return 0;
}
