#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BIT_WORD 32
#define BIT_OC 7
#define BIT_REG 5
#define BIT_IMM (BIT_WORD - BIT_OC - BIT_REG - BIT_REG)
#define MASK(a) ((1 << (a)) - 1)
#define NBR_REGS (1 << BIT_REG)
#define PC r[NBR_REGS - 1]

#define OC_HLT 0

static int r[NBR_REGS];
static int m[1024];

static int instr(int oc, int a, int b, int c) {
	return oc |
		((a & MASK(BIT_REG)) << BIT_OC) |
		((b & MASK(BIT_REG)) << (BIT_OC + BIT_REG)) |
		(c << (BIT_OC + BIT_REG + BIT_REG));
}

static void run(void)
{
	while (1) {
		int ir = m[PC];
		int oc = ir & MASK(BIT_OC);
		int a = (ir >> BIT_OC) & MASK(BIT_REG);
		int b = (ir >> (BIT_OC + BIT_REG)) & MASK(BIT_REG);
		int c = ir >> (BIT_OC + BIT_REG + BIT_REG);
		PC++;
		switch (oc) {
		case OC_HLT: exit(c);
		default: printf("%i %i %i %i\n", oc, a, b, c);
		}
	}
}

int main(int argc, char *argv[])
{
	PC = 1 << (BIT_WORD - 1);
	assert(PC - 1 == -(PC + 1));
	PC = 0;
	m[PC++] = instr(1, 0, 0, 0);
	m[PC++] = instr(2, -1, -2, MASK(BIT_IMM - 1));
	m[PC++] = instr(3, 1, 2, MASK(BIT_IMM - 1) + 1);
	m[PC++] = instr(MASK(BIT_OC), 0, 0, -1);
	m[PC++] = instr(OC_HLT, 0, 0, 0);
	PC = 0;
	run();
	return 0;
}
