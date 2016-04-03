#include <stdio.h>

extern const char * get_every(void);
extern const char * get_one(void);

int main(int argc, char **argv)
{
	const char *s1, *s2, *s3;
	
	(void)argc;
	(void)argv;
	
	s1 = "Hello";
	s2 = get_every();
	s3 = get_one();
	
	printf("%s %s %s\n", s1, s2, s3);
	
	return 0;
}
