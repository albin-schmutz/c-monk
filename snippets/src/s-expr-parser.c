#include <stdio.h>
#include <stdlib.h>

typedef int (*reader)(void);

static char *str;

static int is_lowercase(char c)
{
	return c >= 'a' && c <= 'z';
}

static int is_uppercase(char c)
{
	return c >= 'A' && c <= 'Z';
}

static int is_letter(char c)
{
	return is_lowercase(c) || is_uppercase(c);
}

static int is_digit(char c)
{
	return c >= '0' && c <= '9';
}

static int str_eof(char c)
{
	return c == 0 ? -1 : c;
}

static int str_reader(void)
{
	return str_eof(*str++);
}

static int error(char *s)
{
	fprintf(stderr, s);
	return -1;
}

static int symbol(reader r, char c)
{
	char buf[32]; 
	int buf_ix = 0;
	while (is_letter(c) || is_digit(c)) {
		if (buf_ix >= 31) return error("symbol too long");
		buf[buf_ix++] = c;
		c = r();
	}
	buf[buf_ix] = '\0';
	printf("symbol:%s\n", buf);
	return c;
}

static int integer(reader r, char c)
{
	int n = 0;
	while (c >= '0' && c <= '9') {
		n = n * 10 + c - '0';
		c = r();
	}
	printf("integer:%i\n", n);
	return c;
}

static void parse(reader r)
{
	int c = r();
	while (c != -1) {
		switch (c) {
		case ' ': case '\n': case '\r': c = r(); break;
		case '(': printf("start list\n"); c = r(); break;
		case ')': printf("end list\n"); c = r(); break;
		case 'a'...'z': case 'A'...'Z': c = symbol(r, c); break;
		case '0'...'9': c = integer(r, c); break;
		default: c = error("unexpected character");
		}
	}
}

static void parse_str(char *s)
{
	str = s;
	parse(str_reader);
}

int main(int argc, char *argv[])
{
	parse_str("(1 as s123456789012345678901234567890 45)");
	return 0;
}
