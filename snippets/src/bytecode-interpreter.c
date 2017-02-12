#include <stdio.h>
#include <stdlib.h>

#define BIT_OC 5
#define BIT_REG 3
#define MASK(a) ((1 << (a)) - 1)
#define NBR_REGS (1 << BIT_REG)
#define PC reg[NBR_REGS - 1]

#define OC_HLT 0
#define OC_MOVI 1
#define OC_ADDI 2
#define OC_CMPI 3
#define OC_STB 4
#define OC_BR 5
#define OC_DUMP 6

#define BR_NEQ 0

static int reg[NBR_REGS];
static int cmp;
static unsigned char mem[1024];

static int instr(int oc, int a) {
	return (oc & MASK(BIT_OC)) | ((a & MASK(BIT_REG)) << BIT_OC);
}

static int offs(void)
{
	int b = mem[PC++];
	int c = b >> BIT_REG;
	b &= MASK(BIT_REG);
	printf("b:%i c:%i reg[b]+c:%i\n", b, c, reg[b] + c);
	return reg[b] + c;
}

static void branch(int br)
{
	switch (br) {
	case BR_NEQ: if (cmp) PC += (signed char)mem[PC]; break;
	default: printf("br:%i\n", br);
	}
}

static int run(void)
{
	while (1) {
		int ir = mem[PC++];
		int oc = ir & MASK(BIT_OC);
		int a = (ir >> BIT_OC) & MASK(BIT_REG);
		switch (oc) {
		case OC_MOVI: reg[a] = (signed char)mem[PC++]; break;
		case OC_ADDI: reg[a] += (signed char)mem[PC++]; break;
		case OC_CMPI: cmp = reg[a] - (signed char)mem[PC++]; break;
		case OC_STB: mem[offs()] = reg[a]; break;
		case OC_BR: branch(a); break;
		case OC_DUMP: printf("reg[%i] = %i\n", a, reg[a]); break;
		case OC_HLT: return a;
		default: printf("oc:%i %i\n", oc, a);
		}
	}
}

int main(int argc, char *argv[])
{
	PC = 0;
	mem[PC++] = instr(OC_MOVI, 0); mem[PC++] = 100;
	mem[PC++] = instr(OC_MOVI, 1); mem[PC++] = 65;
	mem[PC++] = instr(OC_DUMP, 1);
	mem[PC++] = instr(OC_STB, 1); mem[PC++] = 0;
	mem[PC++] = instr(OC_ADDI, 1); mem[PC++] = 1;
	mem[PC++] = instr(OC_ADDI, 0); mem[PC++] = 1;
	mem[PC++] = instr(OC_CMPI, 1); mem[PC++] = 75;
	mem[PC++] = instr(OC_BR, BR_NEQ); mem[PC++] = -10;
	mem[PC++] = instr(-2, 2);
	mem[PC++] = instr(-1, -1);
	mem[PC++] = instr(OC_HLT, 0);
	PC = 0;
	int exit = run();
	printf("fwrite(&mem[100], 1, 10, stdout: ");
	fwrite(&mem[100], 1, 10, stdout);
	printf("\nexit:%i", exit);
	return exit;
}
