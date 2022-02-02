#include <stdio.h>
#include <string.h>

typedef struct RC4
{
	unsigned char s[256];
	unsigned char t[256];
	unsigned char key[256];
	unsigned char i, j;
}RC4;

void init(RC4 *rc4);
void ksa(RC4 *rc4);
int prga(RC4 *rc4);
void encrypt(RC4 *rc4, unsigned char *text);
void decrypt(RC4 *rc4, unsigned char *text);
void exchange(unsigned char *a, unsigned char *b);
unsigned int len(unsigned char *string);

int main(int argc, char const *argv[])
{
	RC4 rc4;
	char text[256];
	puts("key");
	scanf("%s", rc4.key);
	init(&rc4);

	unsigned char result[256];
	puts("encrypt or decrypt:");
	char type[7];
	scanf("%s", type);
	if (!strcmp(type, "encrypt"))
		encrypt(&rc4, result);
	else
		decrypt(&rc4, result);
	printf("%s\n", result);
	return 0;
}

void exchange(unsigned char *a, unsigned char *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}
/* 
A new strlen function which can used for both constant and variable strings
The strlen function in string.h is only useful for const char * type parameter
*/
unsigned int len(unsigned char *string)
{
	int count = 0;
	while(string[count])
	{
		count++;
	}
	return count;
}

void init(RC4 *rc4) // initialize the struct
{
	rc4->i = rc4->j = 0;
	memset(rc4->s, 0, sizeof(unsigned char)*256);
	ksa(rc4);
}
void ksa(RC4 *rc4)
{
	int j = 0;
	for (int i = 0; i < 256; ++i)
	{
		rc4->s[i] = i;
		rc4->t[i] = rc4->key[i % len(rc4->key)];
	}
	for (int i = 0; i < 256; ++i)
	{
		j = (j + rc4->s[i] + rc4->t[i]) % 256;
		exchange(&(rc4->s[i]), &(rc4->s[j]));
	}
}
int prga(RC4 *rc4)
{
	rc4->i++;
	rc4->j += rc4->s[rc4->i];
	exchange(&(rc4->s[rc4->i]), &(rc4->s[rc4->j]));
	int t = (rc4->s[rc4->i] + rc4->s[rc4->j]) % 256;
	return rc4->s[t];
}
void encrypt(RC4 *rc4, unsigned char *text)
{
	puts("text");
	scanf("%s", text);
	for (int i = 0; text[i]; ++i)
	{
		text[i] ^= prga(rc4);
	}
}
void decrypt(RC4 *rc4, unsigned char *text)
{
	encrypt(rc4, text);
}
