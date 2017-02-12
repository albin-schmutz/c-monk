#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TVAL { CONS, INTEGER, STRING };

struct VAL {
	enum TVAL tag;
	union {
		struct CONS *cons;
		int integer;
		char *string;
	};
};

struct CONS {
	struct VAL *car;
	struct VAL *cdr;
};

/* VAL constructors */

static struct VAL *vcons(struct CONS *c)
{
	struct VAL *v = malloc(sizeof(struct VAL));
	v->tag = CONS;
	v->cons = c;
	return v;
}

static struct VAL *vinteger(int i)
{
	struct VAL *v = malloc(sizeof(struct VAL));
	v->tag = INTEGER;
	v->integer = i;
	return v;
}

static struct VAL *vstring(char *s)
{
	struct VAL *v = malloc(sizeof(struct VAL));
	v->tag = STRING;
	int len = strlen(s);
	v->string = malloc(len + 1);
	strcpy(v->string, s);
	return v;
}

/* CONS constructor */

static struct CONS *cons(struct VAL *a, struct VAL *b)
{
	struct CONS *c = malloc(sizeof(struct CONS));
	c->car = a;
	c->cdr = b;
	return c;
}

static struct VAL *val_cons(struct VAL *a, struct VAL *b)
{
	return vcons(cons(a, b));
}

/* printers */

static void print_val(struct VAL *v);
static void print_cons(struct CONS *c);

static void print_val(struct VAL *v)
{
	if (v) {
		switch (v->tag) {
		case CONS: print_cons(v->cons); break;
		case INTEGER: printf("%i", v->integer); break;
		case STRING: printf("%s", v->string); break;
		}
	} else {
		printf("NIL");
	}
}

static void print_cons(struct CONS *c)
{
	printf("(");
	print_val(c->car);
	printf(".");
	print_val(c->cdr);
	printf(")");
}

int main(int argc, char *argv[])
{
	printf("cons\n");
	struct CONS *c = cons(vinteger(1), vstring("zwei"));
	print_cons(c);
	printf("\nlist 1 2 3\n");
	struct CONS *list =
		cons(vinteger(1),
		vcons(cons(vstring("zwei"), 
		vcons(cons(vinteger(3), NULL)))));
	print_cons(list);
	printf("\nlist (list 1 2 3) (list 4 5 6)\n");
	struct VAL *list123 =
		val_cons(vinteger(1),
		val_cons(vinteger(2),
		val_cons(vinteger(3), NULL)));
	struct VAL *list456 =
		val_cons(vinteger(4),
		val_cons(vinteger(5),
		val_cons(vinteger(6), NULL)));
	struct CONS *tree =
		cons(list123,
		val_cons(list456, NULL));
	print_cons(tree);
	return 0;
}
