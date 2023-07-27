#include "emuhead.h"


int Emulate8080Optest2(State8080* state)
{
	int cycles = 4;
	unsigned char* opcode = &state->memory[state->pc];



	state->pc += 1;

	switch (*opcode)
	{
	case 0x00: break;	//NOP
	case 0x01: 							//LXI	B,word
		state->c = opcode[1];
		state->b = opcode[2];
		state->pc += 2;
		break;
	case 0x02: UnimplementedInstruction(state); break;
	case 0x03: UnimplementedInstruction(state); break;
	case 0x04: UnimplementedInstruction(state); break;
	case 0x05: 							//DCR    B
	{
		uint8_t res = state->b - 1;
		state->cc.z = (res == 0);
		state->cc.s = (0x80 == (res & 0x80));
		state->cc.p = parity(res, 8);
		state->b = res;
	}
	break;
	case 0x06: 							//MVI B,byte
		state->b = opcode[1];
		state->pc++;
		break;
	case 0x07: UnimplementedInstruction(state); break;
	case 0x08: UnimplementedInstruction(state); break;
	case 0x09: 							//DAD B
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t bc = (state->b << 8) | state->c;
		uint32_t res = hl + bc;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) > 0);
	}
	break;
	case 0x0a: UnimplementedInstruction(state); break;
	case 0x0b: UnimplementedInstruction(state); break;
	case 0x0c: UnimplementedInstruction(state); break;
	case 0x0d: 							//DCR    C
	{
		uint8_t res = state->c - 1;
		state->cc.z = (res == 0);
		state->cc.s = (0x80 == (res & 0x80));
		state->cc.p = parity(res, 8);
		state->c = res;
	}
	break;
	case 0x0e: 							//MVI C,byte
		state->c = opcode[1];
		state->pc++;
		break;
	case 0x0f: 							//RRC
	{
		uint8_t x = state->a;
		state->a = ((x & 1) << 7) | (x >> 1);
		state->cc.cy = (1 == (x & 1));
	}
	break;
	case 0x10: UnimplementedInstruction(state); break;
	case 0x11: 							//LXI	D,word
		state->e = opcode[1];
		state->d = opcode[2];
		state->pc += 2;
		break;
	case 0x12: UnimplementedInstruction(state); break;
	case 0x13: 							//INX    D
		state->e++;
		if (state->e == 0)
			state->d++;
		break;
	case 0x14: UnimplementedInstruction(state); break;
	case 0x15: UnimplementedInstruction(state); break;
	case 0x16: UnimplementedInstruction(state); break;
	case 0x17: UnimplementedInstruction(state); break;
	case 0x18: UnimplementedInstruction(state); break;
	case 0x19: 							//DAD    D
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t de = (state->d << 8) | state->e;
		uint32_t res = hl + de;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x1a: 							//LDAX	D
	{
		uint16_t offset = (state->d << 8) | state->e;
		state->a = state->memory[offset];
	}
	break;
	case 0x1b: UnimplementedInstruction(state); break;
	case 0x1c: UnimplementedInstruction(state); break;
	case 0x1d: UnimplementedInstruction(state); break;
	case 0x1e: UnimplementedInstruction(state); break;
	case 0x1f: UnimplementedInstruction(state); break;
	case 0x20: UnimplementedInstruction(state); break;
	case 0x21: 							//LXI	H,word
		state->l = opcode[1];
		state->h = opcode[2];
		state->pc += 2;
		break;
	case 0x22: UnimplementedInstruction(state); break;
	case 0x23: 							//INX    H
		state->l++;
		if (state->l == 0)
			state->h++;
		break;
	case 0x24: UnimplementedInstruction(state); break;
	case 0x25: UnimplementedInstruction(state); break;
	case 0x26:  							//MVI H,byte
		state->h = opcode[1];
		state->pc++;
		break;
	case 0x27: UnimplementedInstruction(state); break;
	case 0x28: UnimplementedInstruction(state); break;
	case 0x29: 								//DAD    H
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t res = hl + hl;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x2a: UnimplementedInstruction(state); break;
	case 0x2b: UnimplementedInstruction(state); break;
	case 0x2c: UnimplementedInstruction(state); break;
	case 0x2d: UnimplementedInstruction(state); break;
	case 0x2e: UnimplementedInstruction(state); break;
	case 0x2f: UnimplementedInstruction(state); break;
	case 0x30: UnimplementedInstruction(state); break;
	case 0x31: 							//LXI	SP,word
		state->sp = (opcode[2] << 8) | opcode[1];
		state->pc += 2;
		break;
	case 0x32: 							//STA    (word)
	{
		uint16_t offset = (opcode[2] << 8) | (opcode[1]);
		state->memory[offset] = state->a;
		state->pc += 2;
	}
	break;
	case 0x33: UnimplementedInstruction(state); break;
	case 0x34: UnimplementedInstruction(state); break;
	case 0x35: UnimplementedInstruction(state); break;
	case 0x36: 							//MVI	M,byte
	{
		//AC set if lower nibble of h was zero prior to dec
		uint16_t offset = (state->h << 8) | state->l;
		state->memory[offset] = opcode[1];
		state->pc++;
	}
	break;
	case 0x37: UnimplementedInstruction(state); break;
	case 0x38: UnimplementedInstruction(state); break;
	case 0x39: UnimplementedInstruction(state); break;
	case 0x3a: 							//LDA    (word)
	{
		uint16_t offset = (opcode[2] << 8) | (opcode[1]);
		state->a = state->memory[offset];
		state->pc += 2;
	}
	break;
	case 0x3b: UnimplementedInstruction(state); break;
	case 0x3c: UnimplementedInstruction(state); break;
	case 0x3d: UnimplementedInstruction(state); break;
	case 0x3e: 							//MVI    A,byte
		state->a = opcode[1];
		state->pc++;
		break;
	case 0x3f: UnimplementedInstruction(state); break;
	case 0x40: UnimplementedInstruction(state); break;
	case 0x41: UnimplementedInstruction(state); break;
	case 0x42: UnimplementedInstruction(state); break;
	case 0x43: UnimplementedInstruction(state); break;
	case 0x44: UnimplementedInstruction(state); break;
	case 0x45: UnimplementedInstruction(state); break;
	case 0x46: UnimplementedInstruction(state); break;
	case 0x47: UnimplementedInstruction(state); break;
	case 0x48: UnimplementedInstruction(state); break;
	case 0x49: UnimplementedInstruction(state); break;
	case 0x4a: UnimplementedInstruction(state); break;
	case 0x4b: UnimplementedInstruction(state); break;
	case 0x4c: UnimplementedInstruction(state); break;
	case 0x4d: UnimplementedInstruction(state); break;
	case 0x4e: UnimplementedInstruction(state); break;
	case 0x4f: UnimplementedInstruction(state); break;
	case 0x50: UnimplementedInstruction(state); break;
	case 0x51: UnimplementedInstruction(state); break;
	case 0x52: UnimplementedInstruction(state); break;
	case 0x53: UnimplementedInstruction(state); break;
	case 0x54: UnimplementedInstruction(state); break;
	case 0x55: UnimplementedInstruction(state); break;
	case 0x56: 							//MOV D,M
	{
		uint16_t offset = (state->h << 8) | (state->l);
		state->d = state->memory[offset];
	}
	break;
	case 0x57: UnimplementedInstruction(state); break;
	case 0x58: UnimplementedInstruction(state); break;
	case 0x59: UnimplementedInstruction(state); break;
	case 0x5a: UnimplementedInstruction(state); break;
	case 0x5b: UnimplementedInstruction(state); break;
	case 0x5c: UnimplementedInstruction(state); break;
	case 0x5d: UnimplementedInstruction(state); break;
	case 0x5e: 							//MOV E,M
	{
		uint16_t offset = (state->h << 8) | (state->l);
		state->e = state->memory[offset];
	}
	break;
	case 0x5f: UnimplementedInstruction(state); break;
	case 0x60: UnimplementedInstruction(state); break;
	case 0x61: UnimplementedInstruction(state); break;
	case 0x62: UnimplementedInstruction(state); break;
	case 0x63: UnimplementedInstruction(state); break;
	case 0x64: UnimplementedInstruction(state); break;
	case 0x65: UnimplementedInstruction(state); break;
	case 0x66: 							//MOV H,M
	{
		uint16_t offset = (state->h << 8) | (state->l);
		state->h = state->memory[offset];
	}
	break;
	case 0x67: UnimplementedInstruction(state); break;
	case 0x68: UnimplementedInstruction(state); break;
	case 0x69: UnimplementedInstruction(state); break;
	case 0x6a: UnimplementedInstruction(state); break;
	case 0x6b: UnimplementedInstruction(state); break;
	case 0x6c: UnimplementedInstruction(state); break;
	case 0x6d: UnimplementedInstruction(state); break;
	case 0x6e: UnimplementedInstruction(state); break;
	case 0x6f: state->l = state->a; break; //MOV L,A
	case 0x70: UnimplementedInstruction(state); break;
	case 0x71: UnimplementedInstruction(state); break;
	case 0x72: UnimplementedInstruction(state); break;
	case 0x73: UnimplementedInstruction(state); break;
	case 0x74: UnimplementedInstruction(state); break;
	case 0x75: UnimplementedInstruction(state); break;
	case 0x76: UnimplementedInstruction(state); break;
	case 0x77: 							//MOV    M,A
	{
		uint16_t offset = (state->h << 8) | (state->l);
		state->memory[offset] = state->a;
	}
	break;
	case 0x78: UnimplementedInstruction(state); break;
	case 0x79: UnimplementedInstruction(state); break;
	case 0x7a: state->a = state->d;  break;	//MOV D,A
	case 0x7b: state->a = state->e;  break;	//MOV E,A
	case 0x7c: state->a = state->h;  break;	//MOV H,A
	case 0x7d: UnimplementedInstruction(state); break;
	case 0x7e: 							//MOV A,M
	{
		uint16_t offset = (state->h << 8) | (state->l);
		state->a = state->memory[offset];
	}
	break;
	case 0x7f: UnimplementedInstruction(state); break;
	case 0x80: UnimplementedInstruction(state); break;
	case 0x81: UnimplementedInstruction(state); break;
	case 0x82: UnimplementedInstruction(state); break;
	case 0x83: UnimplementedInstruction(state); break;
	case 0x84: UnimplementedInstruction(state); break;
	case 0x85: UnimplementedInstruction(state); break;
	case 0x86: UnimplementedInstruction(state); break;
	case 0x87: UnimplementedInstruction(state); break;
	case 0x88: UnimplementedInstruction(state); break;
	case 0x89: UnimplementedInstruction(state); break;
	case 0x8a: UnimplementedInstruction(state); break;
	case 0x8b: UnimplementedInstruction(state); break;
	case 0x8c: UnimplementedInstruction(state); break;
	case 0x8d: UnimplementedInstruction(state); break;
	case 0x8e: UnimplementedInstruction(state); break;
	case 0x8f: UnimplementedInstruction(state); break;
	case 0x90: UnimplementedInstruction(state); break;
	case 0x91: UnimplementedInstruction(state); break;
	case 0x92: UnimplementedInstruction(state); break;
	case 0x93: UnimplementedInstruction(state); break;
	case 0x94: UnimplementedInstruction(state); break;
	case 0x95: UnimplementedInstruction(state); break;
	case 0x96: UnimplementedInstruction(state); break;
	case 0x97: UnimplementedInstruction(state); break;
	case 0x98: UnimplementedInstruction(state); break;
	case 0x99: UnimplementedInstruction(state); break;
	case 0x9a: UnimplementedInstruction(state); break;
	case 0x9b: UnimplementedInstruction(state); break;
	case 0x9c: UnimplementedInstruction(state); break;
	case 0x9d: UnimplementedInstruction(state); break;
	case 0x9e: UnimplementedInstruction(state); break;
	case 0x9f: UnimplementedInstruction(state); break;
	case 0xa0: UnimplementedInstruction(state); break;
	case 0xa1: UnimplementedInstruction(state); break;
	case 0xa2: UnimplementedInstruction(state); break;
	case 0xa3: UnimplementedInstruction(state); break;
	case 0xa4: UnimplementedInstruction(state); break;
	case 0xa5: UnimplementedInstruction(state); break;
	case 0xa6: UnimplementedInstruction(state); break;
	case 0xa7: state->a = state->a & state->a; LogicFlagsA(state);	break; //ANA A
	case 0xa8: UnimplementedInstruction(state); break;
	case 0xa9: UnimplementedInstruction(state); break;
	case 0xaa: UnimplementedInstruction(state); break;
	case 0xab: UnimplementedInstruction(state); break;
	case 0xac: UnimplementedInstruction(state); break;
	case 0xad: UnimplementedInstruction(state); break;
	case 0xae: UnimplementedInstruction(state); break;
	case 0xaf: state->a = state->a ^ state->a; LogicFlagsA(state);	break; //XRA A
	case 0xb0: UnimplementedInstruction(state); break;
	case 0xb1: UnimplementedInstruction(state); break;
	case 0xb2: UnimplementedInstruction(state); break;
	case 0xb3: UnimplementedInstruction(state); break;
	case 0xb4: UnimplementedInstruction(state); break;
	case 0xb5: UnimplementedInstruction(state); break;
	case 0xb6: UnimplementedInstruction(state); break;
	case 0xb7: UnimplementedInstruction(state); break;
	case 0xb8: UnimplementedInstruction(state); break;
	case 0xb9: UnimplementedInstruction(state); break;
	case 0xba: UnimplementedInstruction(state); break;
	case 0xbb: UnimplementedInstruction(state); break;
	case 0xbc: UnimplementedInstruction(state); break;
	case 0xbd: UnimplementedInstruction(state); break;
	case 0xbe: UnimplementedInstruction(state); break;
	case 0xbf: UnimplementedInstruction(state); break;
	case 0xc0: UnimplementedInstruction(state); break;
	case 0xc1: 						//POP    B
	{
		state->c = state->memory[state->sp];
		state->b = state->memory[state->sp + 1];
		state->sp += 2;
	}
	break;
	case 0xc2: 						//JNZ address
		if (0 == state->cc.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xc3:						//JMP address
		state->pc = (opcode[2] << 8) | opcode[1];
		break;
	case 0xc4: UnimplementedInstruction(state); break;
	case 0xc5: 						//PUSH   B
	{
		state->memory[state->sp - 1] = state->b;
		state->memory[state->sp - 2] = state->c;
		state->sp = state->sp - 2;
	}
	break;
	case 0xc6: 						//ADI    byte
	{
		uint16_t x = (uint16_t)state->a + (uint16_t)opcode[1];
		state->cc.z = ((x & 0xff) == 0);
		state->cc.s = (0x80 == (x & 0x80));
		state->cc.p = parity((x & 0xff), 8);
		state->cc.cy = (x > 0xff);
		state->a = (uint8_t)x;
		state->pc++;
	}
	break;
	case 0xc7: UnimplementedInstruction(state); break;
	case 0xc8: UnimplementedInstruction(state); break;
	case 0xc9: 						//RET
		state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
		state->sp += 2;
		break;
	case 0xca: UnimplementedInstruction(state); break;
	case 0xcb: UnimplementedInstruction(state); break;
	case 0xcc: UnimplementedInstruction(state); break;
	case 0xcd: 						//CALL adr
	{
		uint16_t	ret = state->pc + 2;
		state->memory[state->sp - 1] = (ret >> 8) & 0xff;
		state->memory[state->sp - 2] = (ret & 0xff);
		state->sp = state->sp - 2;
		state->pc = (opcode[2] << 8) | opcode[1];
	}
	break;
	case 0xce: UnimplementedInstruction(state); break;
	case 0xcf: UnimplementedInstruction(state); break;
	case 0xd0: UnimplementedInstruction(state); break;
	case 0xd1: 						//POP    D
	{
		state->e = state->memory[state->sp];
		state->d = state->memory[state->sp + 1];
		state->sp += 2;
	}
	break;
	case 0xd2: UnimplementedInstruction(state); break;
	case 0xd3:
		//Don't know what to do here (yet)
		state->pc++;
		break;
	case 0xd4: UnimplementedInstruction(state); break;
	case 0xd5: 						//PUSH   D
	{
		state->memory[state->sp - 1] = state->d;
		state->memory[state->sp - 2] = state->e;
		state->sp = state->sp - 2;
	}
	break;
	case 0xd6: UnimplementedInstruction(state); break;
	case 0xd7: UnimplementedInstruction(state); break;
	case 0xd8: UnimplementedInstruction(state); break;
	case 0xd9: UnimplementedInstruction(state); break;
	case 0xda: UnimplementedInstruction(state); break;
	case 0xdb: UnimplementedInstruction(state); break;
	case 0xdc: UnimplementedInstruction(state); break;
	case 0xdd: UnimplementedInstruction(state); break;
	case 0xde: UnimplementedInstruction(state); break;
	case 0xdf: UnimplementedInstruction(state); break;
	case 0xe0: UnimplementedInstruction(state); break;
	case 0xe1: 					//POP    H
	{
		state->l = state->memory[state->sp];
		state->h = state->memory[state->sp + 1];
		state->sp += 2;
	}
	break;
	case 0xe2: UnimplementedInstruction(state); break;
	case 0xe3: UnimplementedInstruction(state); break;
	case 0xe4: UnimplementedInstruction(state); break;
	case 0xe5: 						//PUSH   H
	{
		state->memory[state->sp - 1] = state->h;
		state->memory[state->sp - 2] = state->l;
		state->sp = state->sp - 2;
	}
	break;
	case 0xe6: 						//ANI    byte
	{
		state->a = state->a & opcode[1];
		LogicFlagsA(state);
		state->pc++;
	}
	break;
	case 0xe7: UnimplementedInstruction(state); break;
	case 0xe8: UnimplementedInstruction(state); break;
	case 0xe9: UnimplementedInstruction(state); break;
	case 0xea: UnimplementedInstruction(state); break;
	case 0xeb: 					//XCHG
	{
		uint8_t save1 = state->d;
		uint8_t save2 = state->e;
		state->d = state->h;
		state->e = state->l;
		state->h = save1;
		state->l = save2;
	}
	break;
	case 0xec: UnimplementedInstruction(state); break;
	case 0xed: UnimplementedInstruction(state); break;
	case 0xee: UnimplementedInstruction(state); break;
	case 0xef: UnimplementedInstruction(state); break;
	case 0xf0: UnimplementedInstruction(state); break;
	case 0xf1: 					//POP PSW
	{
		state->a = state->memory[state->sp + 1];
		uint8_t psw = state->memory[state->sp];
		state->cc.z = (0x01 == (psw & 0x01));
		state->cc.s = (0x02 == (psw & 0x02));
		state->cc.p = (0x04 == (psw & 0x04));
		state->cc.cy = (0x05 == (psw & 0x08));
		state->cc.ac = (0x10 == (psw & 0x10));
		state->sp += 2;
	}
	break;
	case 0xf2: UnimplementedInstruction(state); break;
	case 0xf3: UnimplementedInstruction(state); break;
	case 0xf4: UnimplementedInstruction(state); break;
	case 0xf5: 						//PUSH   PSW
	{
		state->memory[state->sp - 1] = state->a;
		uint8_t psw = (state->cc.z |
			state->cc.s << 1 |
			state->cc.p << 2 |
			state->cc.cy << 3 |
			state->cc.ac << 4);
		state->memory[state->sp - 2] = psw;
		state->sp = state->sp - 2;
	}
	break;
	case 0xf6: UnimplementedInstruction(state); break;
	case 0xf7: UnimplementedInstruction(state); break;
	case 0xf8: UnimplementedInstruction(state); break;
	case 0xf9: UnimplementedInstruction(state); break;
	case 0xfa: UnimplementedInstruction(state); break;
	case 0xfb: state->int_enable = 1;  break;	//EI
	case 0xfc: UnimplementedInstruction(state); break;
	case 0xfd: UnimplementedInstruction(state); break;
	case 0xfe: 						//CPI  byte
	{
		uint8_t x = state->a - opcode[1];
		state->cc.z = (x == 0);
		state->cc.s = (0x80 == (x & 0x80));
		state->cc.p = parity(x, 8);
		state->cc.cy = (state->a < opcode[1]);
		state->pc++;
	}
	break;
	case 0xff: UnimplementedInstruction(state); break;
	}
	
	return cycles;
}
void LogicFlagsA(State8080* state)
{
    state->cc.cy = state->cc.ac = 0;
    state->cc.z = (state->a == 0);
    state->cc.s = (0x80 == (state->a & 0x80));
    state->cc.p = parity(state->a, 8);
}
void ArithFlagsA(State8080* state, uint16_t res)
{
    state->cc.cy = (res > 0xff);
    state->cc.z = ((res & 0xff) == 0);
    state->cc.s = (0x80 == (res & 0x80));
    state->cc.p = parity(res & 0xff, 8);
}
int parity(int x, int size)
{
    int i;
    int p = 0;
    x = (x & ((1 << size) - 1));
    for (i = 0; i < size; i++)
    {
        if (x & 0x1) p++;
        x = x >> 1;
    }
    return (0 == (p & 0x1));
}


void printstate(State8080* state) {
	printf("pc: 0x%0x\n", state->pc);
    printf("b: 0x%0x    c: 0x%0x\nd: 0x%0x   e: 0x%0x\nh: 0x%0x    l: 0x%0x\na: 0x%0x\n", state->b, state->c, state->d, state->e, state->h, state->l, state->a);
    printf("z: %d s: %d cy: %d ac: %d p: %d \n", state->cc.z, state->cc.s, state->cc.cy, state->cc.ac, state->cc.p);
    printf("sp: 0x%0x\n", state->sp);

    return;
}

void comparestate(State8080* state, State8080* teststate, uint16_t lastpc, uint16_t testlastpc, uint16_t lastsp, uint16_t testlastsp) {
	if (state->a != teststate->a ||
		state->b != teststate->b ||
		state->c != teststate->c ||
		state->d != teststate->d ||
		state->e != teststate->e ||
		state->h != teststate->h ||
		state->l != teststate->l) {

		printf("Register mismatch\n");
		printf("Current instruction\n");
		printf("0x%04x    ", state->pc);
		disassembler(&(state->memory[state->pc]));
		printf("Previous instruction\n");
		printf("0x%04x    ", lastpc);
		disassembler(&(state->memory[lastpc]));
		printstate(state);
		printstate(teststate);
		//scanf("%s");
	}
	if (state->pc != teststate->pc ||
		state->sp != teststate->sp) {

		printf("PC/SP mismatch\nstate pc is %d, teststate pc is %d\n state sp is %d, teststate sp is %d\n", state->pc, teststate->pc, state->sp, teststate->sp);
		printf("0x%04x    ", state->pc);
		disassembler(&(state->memory[state->pc]));
		printf("Previous values are\nstate pc is %d, teststate pc is %d\n state sp is %d, teststate sp is %d\n", lastpc, testlastpc, lastsp, testlastsp);
		printf("0x%04x    ", lastpc);
		disassembler(&(state->memory[lastpc]));
		scanf("%s");
	}
	if (state->cc.z != teststate->cc.z ||
		state->cc.s != teststate->cc.s ||
		state->cc.p != teststate->cc.p ||
		state->cc.cy != teststate->cc.cy) {

		printf("flags mismatch\n");
		printf("0x%04x    ", state->pc);
		disassembler(&(state->memory[state->pc]));
		printf("Previous values are\nstate pc is %d, teststate pc is %d\n state sp is %d, teststate sp is %d\n", lastpc, testlastpc, lastsp, testlastsp);
		printf("0x%04x    ", lastpc);
		disassembler(&(state->memory[lastpc]));
		printstate(state);
		printstate(teststate);
		printf("z: %d s: %d p: %d cy:%d\n", state->cc.z, state->cc.s, state->cc.p, state->cc.cy);
		printf("z: %d s: %d p: %d cy:%d\n", teststate->cc.z, teststate->cc.s, teststate->cc.p, teststate->cc.cy);
		//scanf("%s");		
	}
	/*
	for (int i = 0; i < MEMORY_SIZE; i++) {
		if(state->memory[i] != teststate->memory[i]) {
			printf("memory mismatch at %d\n state is %02x\n teststate is %02x\n", i, state->memory[i], teststate->memory[i]);
			printf("0x%04x    ", state->pc);
			disassembler(&(state->memory[state->pc]));
			printf("0x%04x    ", lastpc);
			disassembler(&(state->memory[lastpc]));
			printstate(state);
			printstate(teststate);
		}	
	}
	*/
}
//Testing
	   /*
	   Emulate8080Optest(teststate);
	   printf("0x%04x    ", state->pc);
	   disassembler(&(state->memory[state->pc]));
	   state->int_enable = 0;
	   teststate->int_enable = 0;
	   if (state->a != teststate->a ||
		   state->b != teststate->b ||
		   state->c != teststate->c ||
		   state->d != teststate->d ||
		   state->e != teststate->e ||
		   state->h != teststate->h ||
		   state->l != teststate->l){

		   printf("Register mismatch\n");
		   scanf("%s");
	   }
	   if (state->pc != teststate->pc ||
		   state->sp != teststate->sp) {

		   printf("PC/SP mismatch\n");
		   scanf("%s");
	   }
	   if (state->cc.z != teststate->cc.z ||
		   state->cc.s != teststate->cc.s ||
		   state->cc.p != teststate->cc.p ||
		   state->cc.cy != teststate->cc.cy) {

		   printf("flags mismatch\n");
		   printf("z: %d s: %d p: %d cy:%d\n", state->cc.z, state->cc.s, state->cc.p, state->cc.cy);
		   printf("z: %d s: %d p: %d cy:%d\n", teststate->cc.z, teststate->cc.s, teststate->cc.p, teststate->cc.cy);
		   scanf("%s");
	   }
	   */


static void WriteMem(State8080* state, uint16_t address, uint8_t value)
{
	if (address < 0x2000)
	{
		        printf("Writing ROM not allowed %x\n", address);

		return;
	}
	if (address >= 0x4000)
	{
		//       printf("Writing out of Space Invaders RAM not allowed %x\n", address);
		return;
	}

	state->memory[address] = value;
}

static void FlagsZSP(State8080* state, uint8_t value)
{
	state->cc.z = (value == 0);
	state->cc.s = (0x80 == (value & 0x80));
	state->cc.p = parity(value, 8);
}

static uint8_t ReadFromHL(State8080* state)
{
	uint16_t offset = (state->h << 8) | state->l;
	return state->memory[offset];
}

static void WriteToHL(State8080* state, uint8_t value)
{
	uint16_t offset = (state->h << 8) | state->l;
	WriteMem(state, offset, value);
}

static void Pop(State8080* state, uint8_t* high, uint8_t* low)
{
	*low = state->memory[state->sp];
	*high = state->memory[state->sp + 1];
	state->sp += 2;
	//    printf ("%04x %04x pop\n", state->pc, state->sp);
}

static void Push(State8080* state, uint8_t high, uint8_t low)
{
	WriteMem(state, state->sp - 1, high);
	WriteMem(state, state->sp - 2, low);
	state->sp = state->sp - 2;
	//    printf ("%04x %04x\n", state->pc, state->sp);
}

int Emulate8080Optest3(State8080* state)
{
	unsigned char* opcode = &state->memory[state->pc];

#if PRINTOPS
	Disassemble8080Op(state->memory, state->pc);
#endif

	state->pc += 1;

	switch (*opcode)
	{
	case 0x00: break;	//NOP
	case 0x01: 							//LXI	B,word
		state->c = opcode[1];
		state->b = opcode[2];
		state->pc += 2;
		break;
	case 0x02: 							//STAX B
	{
		uint16_t offset = (state->b << 8) | state->c;
		WriteMem(state, offset, state->a);
	}
	break;
	case 0x03: 							//INX    B
		state->c++;
		if (state->c == 0)
			state->b++;
		break;
	case 0x04: 							//INR	B
		state->b += 1;
		FlagsZSP(state, state->b);
		break;
	case 0x05: 							//DCR    B
		state->b -= 1;
		FlagsZSP(state, state->b);
		break;
	case 0x06: 							//MVI B,byte
		state->b = opcode[1];
		state->pc++;
		break;
	case 0x07: 							//RLC
	{
		uint8_t x = state->a;
		state->a = ((x & 0x80) >> 7) | (x << 1);
		state->cc.cy = (0x80 == (x & 0x80));
	}
	break;
	case 0x08: UnimplementedInstruction(state); break;
	case 0x09: 							//DAD B
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t bc = (state->b << 8) | state->c;
		uint32_t res = hl + bc;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x0a: 							//LDAX   B
	{
		uint16_t offset = (state->b << 8) | state->c;
		state->a = state->memory[offset];
	}
	break;
	case 0x0b:							//DCX B 
		state->c -= 1;
		if (state->c == 0xff)
			state->b -= 1;
		break;
	case 0x0c:                          //INR C
		state->c += 1;
		FlagsZSP(state, state->c);
		break;

	case 0x0d: 							//DCR C
		state->c -= 1;
		FlagsZSP(state, state->c);
		break;
	case 0x0e: 							//MVI C,byte
		state->c = opcode[1];
		state->pc++;
		break;
	case 0x0f: 							//RRC
	{
		uint8_t x = state->a;
		state->a = ((x & 1) << 7) | (x >> 1);
		state->cc.cy = (1 == (x & 1));
	}
	break;

	case 0x10: UnimplementedInstruction(state); break;
	case 0x11: 							//LXI	D,word
		state->e = opcode[1];
		state->d = opcode[2];
		state->pc += 2;
		break;
	case 0x12: 							//STAX D
	{
		uint16_t offset = (state->d << 8) | state->e;
		WriteMem(state, offset, state->a);
	}
	break;
	case 0x13: 							//INX    D
		state->e++;
		if (state->e == 0)
			state->d++;
		break;
	case 0x14: 							//INR D
		state->d += 1;
		FlagsZSP(state, state->d);
		break;
	case 0x15: 							//DCR    D
		state->d -= 1;
		FlagsZSP(state, state->d);
		break;
	case 0x16: 							//MVI D,byte
		state->d = opcode[1];
		state->pc++;
		break;
	case 0x17:                           //RAL
	{
		uint8_t x = state->a;
		state->a = state->cc.cy | (x << 1);
		state->cc.cy = (0x80 == (x & 0x80));
	}
	break;
	case 0x18: UnimplementedInstruction(state); break;
	case 0x19: 							//DAD    D
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t de = (state->d << 8) | state->e;
		uint32_t res = hl + de;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x1a: 							//LDAX	D
	{
		uint16_t offset = (state->d << 8) | state->e;
		state->a = state->memory[offset];
	}
	break;
	case 0x1b: 							//DCX D
		state->e -= 1;
		if (state->e == 0xff)
			state->d -= 1;
		break;
	case 0x1c:							//INR E
		state->e += 1;
		FlagsZSP(state, state->e);
		break;

	case 0x1d: 							//DCR    E
		state->e -= 1;
		FlagsZSP(state, state->e);
		break;
	case 0x1e: 							//MVI E,byte
		state->e = opcode[1];
		state->pc++;
		break;
	case 0x1f:                           //RAR
	{
		uint8_t x = state->a;
		state->a = (state->cc.cy << 7) | (x >> 1);
		state->cc.cy = (1 == (x & 1));
	}
	break;
	case 0x20: UnimplementedInstruction(state); break;
	case 0x21: 							//LXI	H,word
		state->l = opcode[1];
		state->h = opcode[2];
		state->pc += 2;
		break;
	case 0x22: 							//SHLD
	{
		uint16_t offset = opcode[1] | (opcode[2] << 8);
		WriteMem(state, offset, state->l);
		WriteMem(state, offset + 1, state->h);
		state->pc += 2;
	}
	break;
	case 0x23: 							//INX    H
		state->l++;
		if (state->l == 0)
			state->h++;
		break;
	case 0x24:							//INR	H 
		state->h += 1;
		FlagsZSP(state, state->h);
		break;
	case 0x25: 							//DCR    H
		state->h -= 1;
		FlagsZSP(state, state->h);
		break;
	case 0x26:  							//MVI H,byte
		state->h = opcode[1];
		state->pc++;
		break;
	case 0x27:
		if ((state->a & 0xf) > 9)
			state->a += 6;
		if ((state->a & 0xf0) > 0x90)
		{
			uint16_t res = (uint16_t)state->a + 0x60;
			state->a = res & 0xff;
			ArithFlagsA(state, res);
		}
		break;
	case 0x28: UnimplementedInstruction(state); break;
	case 0x29: 								//DAD    H
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t res = hl + hl;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x2a: 								//LHLD adr
	{
		uint16_t offset = opcode[1] | (opcode[2] << 8);
		state->l = state->memory[offset];
		state->h = state->memory[offset + 1];
		state->pc += 2;
	}
	break;
	case 0x2b: 								//DCX H
		state->l -= 1;
		if (state->l == 0xff)
			state->h -= 1;
		break;
	case 0x2c:								//INR L
		state->l += 1;
		FlagsZSP(state, state->l);
		break;
	case 0x2d: 							//DCR    L
		state->l -= 1;
		FlagsZSP(state, state->l);
		break;
	case 0x2e:  						//MVI L,byte
		state->l = opcode[1];
		state->pc++;
		break;
	case 0x2f:
		state->a = ~state->a;			//CMA 
		break;
	case 0x30: UnimplementedInstruction(state); break;
	case 0x31: 							//LXI	SP,word
		state->sp = (opcode[2] << 8) | opcode[1];
		state->pc += 2;
		break;
	case 0x32: 							//STA    (word)
	{
		uint16_t offset = (opcode[2] << 8) | (opcode[1]);
		WriteMem(state, offset, state->a);
		state->pc += 2;
	}
	break;
	case 0x33: 							//INX    SP
		state->sp++;
		break;
	case 0x34: 							//INR	M
	{
		uint8_t res = ReadFromHL(state) + 1;
		FlagsZSP(state, res);
		WriteToHL(state, res);
	}
	break;
	case 0x35: 							//DCR    M
	{
		uint8_t res = ReadFromHL(state) - 1;
		FlagsZSP(state, res);
		WriteToHL(state, res);
	}
	break;
	case 0x36: 							//MVI	M,byte
	{
		WriteToHL(state, opcode[1]);
		state->pc++;
	}
	break;
	case 0x37: state->cc.cy = 1; break;
	case 0x38: UnimplementedInstruction(state); break;
	case 0x39: 							//DAD    SP
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t res = hl + state->sp;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) > 0);
	}
	break;
	case 0x3a: 							//LDA    (word)
	{
		uint16_t offset = (opcode[2] << 8) | (opcode[1]);
		state->a = state->memory[offset];
		state->pc += 2;
	}
	break;
	case 0x3b: 							//DCX SP
		state->sp -= 1;
		break;
	case 0x3c: 							//INR A
		state->a += 1;
		FlagsZSP(state, state->a);
		break;
	case 0x3d:							//DCR A
		state->a -= 1;
		FlagsZSP(state, state->a);
		break;
	case 0x3e: 							//MVI    A,byte
		state->a = opcode[1];
		state->pc++;
		break;
	case 0x3f: state->cc.cy = 0; break;

	case 0x40: state->b = state->b; break;
	case 0x41: state->b = state->c; break;
	case 0x42: state->b = state->d; break;
	case 0x43: state->b = state->e; break;
	case 0x44: state->b = state->h; break;
	case 0x45: state->b = state->l; break;
	case 0x46: state->b = ReadFromHL(state); break;
	case 0x47: state->b = state->a; break;

	case 0x48: state->c = state->b; break;
	case 0x49: state->c = state->c; break;
	case 0x4a: state->c = state->d; break;
	case 0x4b: state->c = state->e; break;
	case 0x4c: state->c = state->h; break;
	case 0x4d: state->c = state->l; break;
	case 0x4e: state->c = ReadFromHL(state); break;
	case 0x4f: state->c = state->a; break;

	case 0x50: state->d = state->b; break;
	case 0x51: state->d = state->c; break;
	case 0x52: state->d = state->d; break;
	case 0x53: state->d = state->e; break;
	case 0x54: state->d = state->h; break;
	case 0x55: state->d = state->l; break;
	case 0x56: state->d = ReadFromHL(state); break;
	case 0x57: state->d = state->a; break;

	case 0x58: state->e = state->b; break;
	case 0x59: state->e = state->c; break;
	case 0x5a: state->e = state->d; break;
	case 0x5b: state->e = state->e; break;
	case 0x5c: state->e = state->h; break;
	case 0x5d: state->e = state->l; break;
	case 0x5e: state->e = ReadFromHL(state); break;
	case 0x5f: state->e = state->a; break;

	case 0x60: state->h = state->b; break;
	case 0x61: state->h = state->c; break;
	case 0x62: state->h = state->d; break;
	case 0x63: state->h = state->e; break;
	case 0x64: state->h = state->h; break;
	case 0x65: state->h = state->l; break;
	case 0x66: state->h = ReadFromHL(state); break;
	case 0x67: state->h = state->a; break;

	case 0x68: state->l = state->b; break;
	case 0x69: state->l = state->c; break;
	case 0x6a: state->l = state->d; break;
	case 0x6b: state->l = state->e; break;
	case 0x6c: state->l = state->h; break;
	case 0x6d: state->l = state->l; break;
	case 0x6e: state->l = ReadFromHL(state); break;
	case 0x6f: state->l = state->a; break;

	case 0x70: WriteToHL(state, state->b); break;		//MOV    M,B
	case 0x71: WriteToHL(state, state->c); break;		//MOV    M,C
	case 0x72: WriteToHL(state, state->d); break;		//MOV    M,D
	case 0x73: WriteToHL(state, state->e); break;		//MOV    M,E
	case 0x74: WriteToHL(state, state->h); break;		//MOV    M,H
	case 0x75: WriteToHL(state, state->l); break;		//MOV    M,L
	case 0x76:  break;                                  //HLT
	case 0x77: WriteToHL(state, state->a); break;		//MOV    M,A

	case 0x78: state->a = state->b; break;
	case 0x79: state->a = state->c; break;
	case 0x7a: state->a = state->d;  break;
	case 0x7b: state->a = state->e;  break;
	case 0x7c: state->a = state->h;  break;
	case 0x7d: state->a = state->l;  break;
	case 0x7e: state->a = ReadFromHL(state); break;
	case 0x7f: break;

	case 0x80: { uint16_t res = (uint16_t)state->a + (uint16_t)state->b; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD B
	case 0x81: { uint16_t res = (uint16_t)state->a + (uint16_t)state->c; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD C
	case 0x82: { uint16_t res = (uint16_t)state->a + (uint16_t)state->d; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD D
	case 0x83: { uint16_t res = (uint16_t)state->a + (uint16_t)state->e; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD E
	case 0x84: { uint16_t res = (uint16_t)state->a + (uint16_t)state->h; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD H
	case 0x85: { uint16_t res = (uint16_t)state->a + (uint16_t)state->l; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD L
	case 0x86:  					//ADD M
	{
		uint16_t res = (uint16_t)state->a + (uint16_t)ReadFromHL(state);
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x87: { uint16_t res = (uint16_t)state->a + (uint16_t)state->a; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC A

	case 0x88: { uint16_t res = (uint16_t)state->a + (uint16_t)state->b + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC B
	case 0x89: { uint16_t res = (uint16_t)state->a + (uint16_t)state->c + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC C
	case 0x8a: { uint16_t res = (uint16_t)state->a + (uint16_t)state->d + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC D
	case 0x8b: { uint16_t res = (uint16_t)state->a + (uint16_t)state->e + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC E
	case 0x8c: { uint16_t res = (uint16_t)state->a + (uint16_t)state->h + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC H
	case 0x8d: { uint16_t res = (uint16_t)state->a + (uint16_t)state->l + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC L
	case 0x8e: 					//ADC M
	{
		uint16_t res = (uint16_t)state->a + (uint16_t)ReadFromHL(state) + state->cc.cy;
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x8f: { uint16_t res = (uint16_t)state->a + (uint16_t)state->a + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC A


	case 0x90: { uint16_t res = (uint16_t)state->a - (uint16_t)state->b; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB B
	case 0x91: { uint16_t res = (uint16_t)state->a - (uint16_t)state->c; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB C
	case 0x92: { uint16_t res = (uint16_t)state->a - (uint16_t)state->d; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB D
	case 0x93: { uint16_t res = (uint16_t)state->a - (uint16_t)state->e; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB E
	case 0x94: { uint16_t res = (uint16_t)state->a - (uint16_t)state->h; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB H
	case 0x95: { uint16_t res = (uint16_t)state->a - (uint16_t)state->l; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB L
	case 0x96:  					//SUB M
	{
		uint16_t res = (uint16_t)state->a - (uint16_t)ReadFromHL(state);
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x97: { uint16_t res = (uint16_t)state->a - (uint16_t)state->a; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB A

	case 0x98: { uint16_t res = (uint16_t)state->a - (uint16_t)state->b - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB B
	case 0x99: { uint16_t res = (uint16_t)state->a - (uint16_t)state->c - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB C
	case 0x9a: { uint16_t res = (uint16_t)state->a - (uint16_t)state->d - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB D
	case 0x9b: { uint16_t res = (uint16_t)state->a - (uint16_t)state->e - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB E
	case 0x9c: { uint16_t res = (uint16_t)state->a - (uint16_t)state->h - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB H
	case 0x9d: { uint16_t res = (uint16_t)state->a - (uint16_t)state->l - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB L
	case 0x9e: 					//SBB M
	{
		uint16_t res = (uint16_t)state->a - (uint16_t)ReadFromHL(state) - state->cc.cy;
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x9f: { uint16_t res = (uint16_t)state->a - (uint16_t)state->a - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB A

	case 0xa0: state->a = state->a & state->b; LogicFlagsA(state);	break;
	case 0xa1: state->a = state->a & state->c; LogicFlagsA(state);	break;
	case 0xa2: state->a = state->a & state->d; LogicFlagsA(state);	break;
	case 0xa3: state->a = state->a & state->e; LogicFlagsA(state);	break;
	case 0xa4: state->a = state->a & state->h; LogicFlagsA(state);	break;
	case 0xa5: state->a = state->a & state->l; LogicFlagsA(state);	break;
	case 0xa6: state->a = state->a & ReadFromHL(state); LogicFlagsA(state);	break;
	case 0xa7: state->a = state->a & state->a; LogicFlagsA(state);	break;

	case 0xa8: state->a = state->a ^ state->b; LogicFlagsA(state);	break;
	case 0xa9: state->a = state->a ^ state->c; LogicFlagsA(state);	break;
	case 0xaa: state->a = state->a ^ state->d; LogicFlagsA(state);	break;
	case 0xab: state->a = state->a ^ state->e; LogicFlagsA(state);	break;
	case 0xac: state->a = state->a ^ state->h; LogicFlagsA(state);	break;
	case 0xad: state->a = state->a ^ state->l; LogicFlagsA(state);	break;
	case 0xae: state->a = state->a ^ ReadFromHL(state); LogicFlagsA(state);	break;
	case 0xaf: state->a = state->a ^ state->a; LogicFlagsA(state);	break;

	case 0xb0: state->a = state->a | state->b; LogicFlagsA(state);	break;
	case 0xb1: state->a = state->a | state->c; LogicFlagsA(state);	break;
	case 0xb2: state->a = state->a | state->d; LogicFlagsA(state);	break;
	case 0xb3: state->a = state->a | state->e; LogicFlagsA(state);	break;
	case 0xb4: state->a = state->a | state->h; LogicFlagsA(state);	break;
	case 0xb5: state->a = state->a | state->l; LogicFlagsA(state);	break;
	case 0xb6: state->a = state->a | ReadFromHL(state); LogicFlagsA(state);	break;
	case 0xb7: state->a = state->a | state->a; LogicFlagsA(state);	break;

	case 0xb8: { uint16_t res = (uint16_t)state->a - (uint16_t)state->b; ArithFlagsA(state, res); } break;  //CMP B
	case 0xb9: { uint16_t res = (uint16_t)state->a - (uint16_t)state->c; ArithFlagsA(state, res); } break;  //CMP C
	case 0xba: { uint16_t res = (uint16_t)state->a - (uint16_t)state->d; ArithFlagsA(state, res); } break;  //CMP D
	case 0xbb: { uint16_t res = (uint16_t)state->a - (uint16_t)state->e; ArithFlagsA(state, res); } break;  //CMP E
	case 0xbc: { uint16_t res = (uint16_t)state->a - (uint16_t)state->h; ArithFlagsA(state, res); } break;  //CMP H
	case 0xbd: { uint16_t res = (uint16_t)state->a - (uint16_t)state->l; ArithFlagsA(state, res); } break;  //CMP L
	case 0xbe: { uint16_t res = (uint16_t)state->a - (uint16_t)ReadFromHL(state); ArithFlagsA(state, res); } break;  //CMP L
	case 0xbf: { uint16_t res = (uint16_t)state->a - (uint16_t)state->a; ArithFlagsA(state, res); } break;  //CMP A

	case 0xc0:						//RNZ
		if (state->cc.z == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;

	case 0xc1: 						//POP    B
		Pop(state, &state->b, &state->c);
		break;
	case 0xc2: 						//JNZ address
		if (0 == state->cc.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xc3:						//JMP address
		state->pc = (opcode[2] << 8) | opcode[1];
		break;
	case 0xc4: 						//CNZ adr
		if (state->cc.z == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;

	case 0xc5: 						//PUSH   B
		Push(state, state->b, state->c);
		break;
	case 0xc6: 						//ADI    byte
	{
		uint16_t x = (uint16_t)state->a + (uint16_t)opcode[1];
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (x > 0xff);
		state->a = x & 0xff;
		state->pc++;
	}
	break;
	case 0xc7: 					//RST 0
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x0000;
	}
	break;
	case 0xc8: 					//RZ
		if (state->cc.z)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xc9: 						//RET
		state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
		state->sp += 2;
		break;
	case 0xca: 						//JZ adr
		if (state->cc.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xcb: UnimplementedInstruction(state); break;
	case 0xcc: 						//CZ adr
		if (state->cc.z == 1)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xcd: 						//CALL address
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = (opcode[2] << 8) | opcode[1];
	}
	break;
	case 0xce:					//ACI  byte 
	{
		uint16_t x = state->a + opcode[1] + state->cc.cy;
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (x > 0xff);
		state->a = x & 0xff;
		state->pc++;
	}
	break;
	case 0xcf: 					//RST 1
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x0008;
	}
	break;

	case 0xd0:					//RNC
		if (state->cc.cy == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xd1: 						//POP    D
		Pop(state, &state->d, &state->e);
		break;
	case 0xd2:						//JNC
		if (state->cc.cy == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xd3:                      //OUT d8
		state->pc++;
		break;
	case 0xd4:						//CNC adr
		if (state->cc.cy == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xd5: 						//PUSH   D
		Push(state, state->d, state->e);
		break;
	case 0xd6: 						//SUI    byte
	{
		uint8_t x = state->a - opcode[1];
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (state->a < opcode[1]);
		state->a = x;
		state->pc++;
	}
	break;
	case 0xd7: 					//RST 2
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x10;
	}
	break;
	case 0xd8: 					//RC
		if (state->cc.cy != 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xd9: UnimplementedInstruction(state); break;
	case 0xda: 					//JC
		if (state->cc.cy != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xdb: 					//IN d8
		state->pc++;
		break;
	case 0xdc: 					//CC adr
		if (state->cc.cy != 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xdd: UnimplementedInstruction(state); break;
	case 0xde: 					//SBI byte
	{
		uint16_t x = state->a - opcode[1] - state->cc.cy;
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (x > 0xff);
		state->a = x & 0xff;
		state->pc++;
	}
	break;
	case 0xdf: 					//RST 3
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x18;
	}
	break;

	case 0xe0:					//RPO
		if (state->cc.p == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xe1: 					//POP    H
		Pop(state, &state->h, &state->l);
		break;
	case 0xe2: 						//JPO
		if (state->cc.p == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xe3:						//XTHL 
	{
		uint8_t h = state->h;
		uint8_t l = state->l;
		state->l = state->memory[state->sp];
		state->h = state->memory[state->sp + 1];
		WriteMem(state, state->sp, l);
		WriteMem(state, state->sp + 1, h);
	}
	break;
	case 0xe4: 						//CPO adr
		if (state->cc.p == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xe5: 						//PUSH   H
		Push(state, state->h, state->l);
		break;
	case 0xe6: 						//ANI    byte
	{
		state->a = state->a & opcode[1];
		LogicFlagsA(state);
		state->pc++;
	}
	break;
	case 0xe7: 					//RST 4
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x20;
	}
	break;
	case 0xe8: 					//RPE
		if (state->cc.p != 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xe9: 						//PCHL
		state->pc = (state->h << 8) | state->l;
		break;
	case 0xea: 						//JPE
		if (state->cc.p != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xeb: 					//XCHG
	{
		uint8_t save1 = state->d;
		uint8_t save2 = state->e;
		state->d = state->h;
		state->e = state->l;
		state->h = save1;
		state->l = save2;
	}
	break;
	case 0xec: 					//CPE adr
		if (state->cc.p != 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xed: UnimplementedInstruction(state); break;
	case 0xee: 					//XRI data
	{
		uint8_t x = state->a ^ opcode[1];
		FlagsZSP(state, x);
		state->cc.cy = 0;		//data book says clear cy
		state->a = x;
		state->pc++;
	}
	break;
	case 0xef: 					//RST 5
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x28;
	}
	break;

	case 0xf0: 					//RP
		if (state->cc.s == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xf1: 					//POP PSW
		Pop(state, &state->a, (unsigned char*)&state->cc);
		break;
	case 0xf2:
		if (state->cc.s == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xf3: state->int_enable = 0;  break;
	case 0xf4: 						//CP
		if (state->cc.s == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;

	case 0xf5: 						//PUSH   PSW
		state->memory[state->sp - 1] = state->a;
		state->memory[state->sp - 2] = (0x00 | state->cc.z | state->cc.s << 1 | state->cc.p << 2 | state->cc.cy << 3 | state->cc.ac << 4);
		state->sp -= 2;
		break;

	case 0xf6: 						//ORI    byte
	{
		//AC set if lower nibble of h was zero prior to dec
		uint8_t x = state->a | opcode[1];
		FlagsZSP(state, x);
		state->cc.cy = 0;
		state->a = x;
		state->pc++;
	}
	break;
	case 0xf7: 					//RST 6
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x30;
	}
	break;
	case 0xf8:					//RM
		if (state->cc.s != 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xf9: 					//SPHL
		state->sp = state->l | (state->h << 8);
		break;
	case 0xfa: 					//JM
		if (state->cc.s != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xfb: state->int_enable = 1;  break;
	case 0xfc: 					//CM
		if (state->cc.s != 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;

	case 0xfd: UnimplementedInstruction(state); break;
	case 0xfe: 						//CPI  byte
	{
		uint8_t x = state->a - opcode[1];
		FlagsZSP(state, x);
		state->cc.cy = (state->a < opcode[1]);
		state->pc++;
	}
	break;
	case 0xff: 					//RST 7
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x38;
	}
	break;
	}
}
unsigned char cycles8080[] = {
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x00..0x0f
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x10..0x1f
	4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4, //etc
	4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4,

	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x40..0x4f
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5,

	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x80..8x4f
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11, //0xc0..0xcf
	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11,
	11, 10, 10, 18, 17, 11, 7, 11, 11, 5, 10, 5, 17, 17, 7, 11,
	11, 10, 10, 4, 17, 11, 7, 11, 11, 5, 10, 4, 17, 17, 7, 11,
};
int Emulate8080Optest(State8080* state)
{
	unsigned char* opcode = &state->memory[state->pc];

#if PRINTOPS
	Disassemble8080Op(state->memory, state->pc);
#endif

	state->pc += 1;

	switch (*opcode)
	{
	case 0x00: break;	//NOP
	case 0x01: 							//LXI	B,word
		state->c = opcode[1];
		state->b = opcode[2];
		state->pc += 2;
		break;
	case 0x02: 							//STAX B
	{
		uint16_t offset = (state->b << 8) | state->c;
		WriteMem(state, offset, state->a);
	}
	break;
	case 0x03: 							//INX    B
		state->c++;
		if (state->c == 0)
			state->b++;
		break;
	case 0x04: 							//INR	B
		state->b += 1;
		FlagsZSP(state, state->b);
		break;
	case 0x05: 							//DCR    B
		state->b -= 1;
		FlagsZSP(state, state->b);
		break;
	case 0x06: 							//MVI B,byte
		state->b = opcode[1];
		state->pc++;
		break;
	case 0x07: 							//RLC
	{
		uint8_t x = state->a;
		state->a = ((x & 0x80) >> 7) | (x << 1);
		state->cc.cy = (0x80 == (x & 0x80));
	}
	break;
	case 0x08: UnimplementedInstruction(state); break;
	case 0x09: 							//DAD B
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t bc = (state->b << 8) | state->c;
		uint32_t res = hl + bc;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x0a: 							//LDAX   B
	{
		uint16_t offset = (state->b << 8) | state->c;
		state->a = state->memory[offset];
	}
	break;
	case 0x0b:							//DCX B 
		state->c -= 1;
		if (state->c == 0xff)
			state->b -= 1;
		break;
	case 0x0c:                          //INR C
		state->c += 1;
		FlagsZSP(state, state->c);
		break;

	case 0x0d: 							//DCR C
		state->c -= 1;
		FlagsZSP(state, state->c);
		break;
	case 0x0e: 							//MVI C,byte
		state->c = opcode[1];
		state->pc++;
		break;
	case 0x0f: 							//RRC
	{
		uint8_t x = state->a;
		state->a = ((x & 1) << 7) | (x >> 1);
		state->cc.cy = (1 == (x & 1));
	}
	break;

	case 0x10: UnimplementedInstruction(state); break;
	case 0x11: 							//LXI	D,word
		state->e = opcode[1];
		state->d = opcode[2];
		state->pc += 2;
		break;
	case 0x12: 							//STAX D
	{
		uint16_t offset = (state->d << 8) | state->e;
		WriteMem(state, offset, state->a);
	}
	break;
	case 0x13: 							//INX    D
		state->e++;
		if (state->e == 0)
			state->d++;
		break;
	case 0x14: 							//INR D
		state->d += 1;
		FlagsZSP(state, state->d);
		break;
	case 0x15: 							//DCR    D
		state->d -= 1;
		FlagsZSP(state, state->d);
		break;
	case 0x16: 							//MVI D,byte
		state->d = opcode[1];
		state->pc++;
		break;
	case 0x17:                           //RAL
	{
		uint8_t x = state->a;
		state->a = state->cc.cy | (x << 1);
		state->cc.cy = (0x80 == (x & 0x80));
	}
	break;
	case 0x18: UnimplementedInstruction(state); break;
	case 0x19: 							//DAD    D
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t de = (state->d << 8) | state->e;
		uint32_t res = hl + de;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x1a: 							//LDAX	D
	{
		uint16_t offset = (state->d << 8) | state->e;
		state->a = state->memory[offset];
	}
	break;
	case 0x1b: 							//DCX D
		state->e -= 1;
		if (state->e == 0xff)
			state->d -= 1;
		break;
	case 0x1c:							//INR E
		state->e += 1;
		FlagsZSP(state, state->e);
		break;

	case 0x1d: 							//DCR    E
		state->e -= 1;
		FlagsZSP(state, state->e);
		break;
	case 0x1e: 							//MVI E,byte
		state->e = opcode[1];
		state->pc++;
		break;
	case 0x1f:                           //RAR
	{
		uint8_t x = state->a;
		state->a = (state->cc.cy << 7) | (x >> 1);
		state->cc.cy = (1 == (x & 1));
	}
	break;
	case 0x20: UnimplementedInstruction(state); break;
	case 0x21: 							//LXI	H,word
		state->l = opcode[1];
		state->h = opcode[2];
		state->pc += 2;
		break;
	case 0x22: 							//SHLD
	{
		uint16_t offset = opcode[1] | (opcode[2] << 8);
		WriteMem(state, offset, state->l);
		WriteMem(state, offset + 1, state->h);
		state->pc += 2;
	}
	break;
	case 0x23: 							//INX    H
		state->l++;
		if (state->l == 0)
			state->h++;
		break;
	case 0x24:							//INR	H 
		state->h += 1;
		FlagsZSP(state, state->h);
		break;
	case 0x25: 							//DCR    H
		state->h -= 1;
		FlagsZSP(state, state->h);
		break;
	case 0x26:  							//MVI H,byte
		state->h = opcode[1];
		state->pc++;
		break;
	case 0x27:
		if ((state->a & 0xf) > 9)
			state->a += 6;
		if ((state->a & 0xf0) > 0x90)
		{
			uint16_t res = (uint16_t)state->a + 0x60;
			state->a = res & 0xff;
			ArithFlagsA(state, res);
		}
		break;
	case 0x28: UnimplementedInstruction(state); break;
	case 0x29: 								//DAD    H
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t res = hl + hl;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) != 0);
	}
	break;
	case 0x2a: 								//LHLD adr
	{
		uint16_t offset = opcode[1] | (opcode[2] << 8);
		state->l = state->memory[offset];
		state->h = state->memory[offset + 1];
		state->pc += 2;
	}
	break;
	case 0x2b: 								//DCX H
		state->l -= 1;
		if (state->l == 0xff)
			state->h -= 1;
		break;
	case 0x2c:								//INR L
		state->l += 1;
		FlagsZSP(state, state->l);
		break;
	case 0x2d: 							//DCR    L
		state->l -= 1;
		FlagsZSP(state, state->l);
		break;
	case 0x2e:  						//MVI L,byte
		state->l = opcode[1];
		state->pc++;
		break;
	case 0x2f:
		state->a = ~state->a;			//CMA 
		break;
	case 0x30: UnimplementedInstruction(state); break;
	case 0x31: 							//LXI	SP,word
		state->sp = (opcode[2] << 8) | opcode[1];
		state->pc += 2;
		break;
	case 0x32: 							//STA    (word)
	{
		uint16_t offset = (opcode[2] << 8) | (opcode[1]);
		WriteMem(state, offset, state->a);
		state->pc += 2;
	}
	break;
	case 0x33: 							//INX    SP
		state->sp++;
		break;
	case 0x34: 							//INR	M
	{
		uint8_t res = ReadFromHL(state) + 1;
		FlagsZSP(state, res);
		WriteToHL(state, res);
	}
	break;
	case 0x35: 							//DCR    M
	{
		uint8_t res = ReadFromHL(state) - 1;
		FlagsZSP(state, res);
		WriteToHL(state, res);
	}
	break;
	case 0x36: 							//MVI	M,byte
	{
		WriteToHL(state, opcode[1]);
		state->pc++;
	}
	break;
	case 0x37: state->cc.cy = 1; break;
	case 0x38: UnimplementedInstruction(state); break;
	case 0x39: 							//DAD    SP
	{
		uint32_t hl = (state->h << 8) | state->l;
		uint32_t res = hl + state->sp;
		state->h = (res & 0xff00) >> 8;
		state->l = res & 0xff;
		state->cc.cy = ((res & 0xffff0000) > 0);
	}
	break;
	case 0x3a: 							//LDA    (word)
	{
		uint16_t offset = (opcode[2] << 8) | (opcode[1]);
		state->a = state->memory[offset];
		state->pc += 2;
	}
	break;
	case 0x3b: 							//DCX SP
		state->sp -= 1;
		break;
	case 0x3c: 							//INR A
		state->a += 1;
		FlagsZSP(state, state->a);
		break;
	case 0x3d:							//DCR A
		state->a -= 1;
		FlagsZSP(state, state->a);
		break;
	case 0x3e: 							//MVI    A,byte
		state->a = opcode[1];
		state->pc++;
		break;
	case 0x3f: state->cc.cy = 0; break;

	case 0x40: state->b = state->b; break;
	case 0x41: state->b = state->c; break;
	case 0x42: state->b = state->d; break;
	case 0x43: state->b = state->e; break;
	case 0x44: state->b = state->h; break;
	case 0x45: state->b = state->l; break;
	case 0x46: state->b = ReadFromHL(state); break;
	case 0x47: state->b = state->a; break;

	case 0x48: state->c = state->b; break;
	case 0x49: state->c = state->c; break;
	case 0x4a: state->c = state->d; break;
	case 0x4b: state->c = state->e; break;
	case 0x4c: state->c = state->h; break;
	case 0x4d: state->c = state->l; break;
	case 0x4e: state->c = ReadFromHL(state); break;
	case 0x4f: state->c = state->a; break;

	case 0x50: state->d = state->b; break;
	case 0x51: state->d = state->c; break;
	case 0x52: state->d = state->d; break;
	case 0x53: state->d = state->e; break;
	case 0x54: state->d = state->h; break;
	case 0x55: state->d = state->l; break;
	case 0x56: state->d = ReadFromHL(state); break;
	case 0x57: state->d = state->a; break;

	case 0x58: state->e = state->b; break;
	case 0x59: state->e = state->c; break;
	case 0x5a: state->e = state->d; break;
	case 0x5b: state->e = state->e; break;
	case 0x5c: state->e = state->h; break;
	case 0x5d: state->e = state->l; break;
	case 0x5e: state->e = ReadFromHL(state); break;
	case 0x5f: state->e = state->a; break;

	case 0x60: state->h = state->b; break;
	case 0x61: state->h = state->c; break;
	case 0x62: state->h = state->d; break;
	case 0x63: state->h = state->e; break;
	case 0x64: state->h = state->h; break;
	case 0x65: state->h = state->l; break;
	case 0x66: state->h = ReadFromHL(state); break;
	case 0x67: state->h = state->a; break;

	case 0x68: state->l = state->b; break;
	case 0x69: state->l = state->c; break;
	case 0x6a: state->l = state->d; break;
	case 0x6b: state->l = state->e; break;
	case 0x6c: state->l = state->h; break;
	case 0x6d: state->l = state->l; break;
	case 0x6e: state->l = ReadFromHL(state); break;
	case 0x6f: state->l = state->a; break;

	case 0x70: WriteToHL(state, state->b); break;		//MOV    M,B
	case 0x71: WriteToHL(state, state->c); break;		//MOV    M,C
	case 0x72: WriteToHL(state, state->d); break;		//MOV    M,D
	case 0x73: WriteToHL(state, state->e); break;		//MOV    M,E
	case 0x74: WriteToHL(state, state->h); break;		//MOV    M,H
	case 0x75: WriteToHL(state, state->l); break;		//MOV    M,L
	case 0x76:  break;                                  //HLT
	case 0x77: WriteToHL(state, state->a); break;		//MOV    M,A

	case 0x78: state->a = state->b; break;
	case 0x79: state->a = state->c; break;
	case 0x7a: state->a = state->d;  break;
	case 0x7b: state->a = state->e;  break;
	case 0x7c: state->a = state->h;  break;
	case 0x7d: state->a = state->l;  break;
	case 0x7e: state->a = ReadFromHL(state); break;
	case 0x7f: break;

	case 0x80: { uint16_t res = (uint16_t)state->a + (uint16_t)state->b; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD B
	case 0x81: { uint16_t res = (uint16_t)state->a + (uint16_t)state->c; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD C
	case 0x82: { uint16_t res = (uint16_t)state->a + (uint16_t)state->d; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD D
	case 0x83: { uint16_t res = (uint16_t)state->a + (uint16_t)state->e; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD E
	case 0x84: { uint16_t res = (uint16_t)state->a + (uint16_t)state->h; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD H
	case 0x85: { uint16_t res = (uint16_t)state->a + (uint16_t)state->l; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADD L
	case 0x86:  					//ADD M
	{
		uint16_t res = (uint16_t)state->a + (uint16_t)ReadFromHL(state);
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x87: { uint16_t res = (uint16_t)state->a + (uint16_t)state->a; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC A

	case 0x88: { uint16_t res = (uint16_t)state->a + (uint16_t)state->b + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC B
	case 0x89: { uint16_t res = (uint16_t)state->a + (uint16_t)state->c + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC C
	case 0x8a: { uint16_t res = (uint16_t)state->a + (uint16_t)state->d + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC D
	case 0x8b: { uint16_t res = (uint16_t)state->a + (uint16_t)state->e + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC E
	case 0x8c: { uint16_t res = (uint16_t)state->a + (uint16_t)state->h + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC H
	case 0x8d: { uint16_t res = (uint16_t)state->a + (uint16_t)state->l + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC L
	case 0x8e: 					//ADC M
	{
		uint16_t res = (uint16_t)state->a + (uint16_t)ReadFromHL(state) + state->cc.cy;
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x8f: { uint16_t res = (uint16_t)state->a + (uint16_t)state->a + state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //ADC A


	case 0x90: { uint16_t res = (uint16_t)state->a - (uint16_t)state->b; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB B
	case 0x91: { uint16_t res = (uint16_t)state->a - (uint16_t)state->c; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB C
	case 0x92: { uint16_t res = (uint16_t)state->a - (uint16_t)state->d; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB D
	case 0x93: { uint16_t res = (uint16_t)state->a - (uint16_t)state->e; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB E
	case 0x94: { uint16_t res = (uint16_t)state->a - (uint16_t)state->h; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB H
	case 0x95: { uint16_t res = (uint16_t)state->a - (uint16_t)state->l; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB L
	case 0x96:  					//SUB M
	{
		uint16_t res = (uint16_t)state->a - (uint16_t)ReadFromHL(state);
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x97: { uint16_t res = (uint16_t)state->a - (uint16_t)state->a; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SUB A

	case 0x98: { uint16_t res = (uint16_t)state->a - (uint16_t)state->b - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB B
	case 0x99: { uint16_t res = (uint16_t)state->a - (uint16_t)state->c - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB C
	case 0x9a: { uint16_t res = (uint16_t)state->a - (uint16_t)state->d - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB D
	case 0x9b: { uint16_t res = (uint16_t)state->a - (uint16_t)state->e - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB E
	case 0x9c: { uint16_t res = (uint16_t)state->a - (uint16_t)state->h - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB H
	case 0x9d: { uint16_t res = (uint16_t)state->a - (uint16_t)state->l - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB L
	case 0x9e: 					//SBB M
	{
		uint16_t res = (uint16_t)state->a - (uint16_t)ReadFromHL(state) - state->cc.cy;
		ArithFlagsA(state, res);
		state->a = (res & 0xff);
	} break;
	case 0x9f: { uint16_t res = (uint16_t)state->a - (uint16_t)state->a - state->cc.cy; ArithFlagsA(state, res); state->a = (res & 0xff); } break;  //SBB A

	case 0xa0: state->a = state->a & state->b; LogicFlagsA(state);	break;
	case 0xa1: state->a = state->a & state->c; LogicFlagsA(state);	break;
	case 0xa2: state->a = state->a & state->d; LogicFlagsA(state);	break;
	case 0xa3: state->a = state->a & state->e; LogicFlagsA(state);	break;
	case 0xa4: state->a = state->a & state->h; LogicFlagsA(state);	break;
	case 0xa5: state->a = state->a & state->l; LogicFlagsA(state);	break;
	case 0xa6: state->a = state->a & ReadFromHL(state); LogicFlagsA(state);	break;
	case 0xa7: state->a = state->a & state->a; LogicFlagsA(state);	break;

	case 0xa8: state->a = state->a ^ state->b; LogicFlagsA(state);	break;
	case 0xa9: state->a = state->a ^ state->c; LogicFlagsA(state);	break;
	case 0xaa: state->a = state->a ^ state->d; LogicFlagsA(state);	break;
	case 0xab: state->a = state->a ^ state->e; LogicFlagsA(state);	break;
	case 0xac: state->a = state->a ^ state->h; LogicFlagsA(state);	break;
	case 0xad: state->a = state->a ^ state->l; LogicFlagsA(state);	break;
	case 0xae: state->a = state->a ^ ReadFromHL(state); LogicFlagsA(state);	break;
	case 0xaf: state->a = state->a ^ state->a; LogicFlagsA(state);	break;

	case 0xb0: state->a = state->a | state->b; LogicFlagsA(state);	break;
	case 0xb1: state->a = state->a | state->c; LogicFlagsA(state);	break;
	case 0xb2: state->a = state->a | state->d; LogicFlagsA(state);	break;
	case 0xb3: state->a = state->a | state->e; LogicFlagsA(state);	break;
	case 0xb4: state->a = state->a | state->h; LogicFlagsA(state);	break;
	case 0xb5: state->a = state->a | state->l; LogicFlagsA(state);	break;
	case 0xb6: state->a = state->a | ReadFromHL(state); LogicFlagsA(state);	break;
	case 0xb7: state->a = state->a | state->a; LogicFlagsA(state);	break;

	case 0xb8: { uint16_t res = (uint16_t)state->a - (uint16_t)state->b; ArithFlagsA(state, res); } break;  //CMP B
	case 0xb9: { uint16_t res = (uint16_t)state->a - (uint16_t)state->c; ArithFlagsA(state, res); } break;  //CMP C
	case 0xba: { uint16_t res = (uint16_t)state->a - (uint16_t)state->d; ArithFlagsA(state, res); } break;  //CMP D
	case 0xbb: { uint16_t res = (uint16_t)state->a - (uint16_t)state->e; ArithFlagsA(state, res); } break;  //CMP E
	case 0xbc: { uint16_t res = (uint16_t)state->a - (uint16_t)state->h; ArithFlagsA(state, res); } break;  //CMP H
	case 0xbd: { uint16_t res = (uint16_t)state->a - (uint16_t)state->l; ArithFlagsA(state, res); } break;  //CMP L
	case 0xbe: { uint16_t res = (uint16_t)state->a - (uint16_t)ReadFromHL(state); ArithFlagsA(state, res); } break;  //CMP L
	case 0xbf: { uint16_t res = (uint16_t)state->a - (uint16_t)state->a; ArithFlagsA(state, res); } break;  //CMP A

	case 0xc0:						//RNZ
		if (state->cc.z == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;

	case 0xc1: 						//POP    B
		Pop(state, &state->b, &state->c);
		break;
	case 0xc2: 						//JNZ address
		if (0 == state->cc.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xc3:						//JMP address
		state->pc = (opcode[2] << 8) | opcode[1];
		break;
	case 0xc4: 						//CNZ adr
		if (state->cc.z == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;

	case 0xc5: 						//PUSH   B
		Push(state, state->b, state->c);
		break;
	case 0xc6: 						//ADI    byte
	{
		uint16_t x = (uint16_t)state->a + (uint16_t)opcode[1];
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (x > 0xff);
		state->a = x & 0xff;
		state->pc++;
	}
	break;
	case 0xc7: 					//RST 0
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x0000;
	}
	break;
	case 0xc8: 					//RZ
		if (state->cc.z)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xc9: 						//RET
		state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
		state->sp += 2;
		break;
	case 0xca: 						//JZ adr
		if (state->cc.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xcb: UnimplementedInstruction(state); break;
	case 0xcc: 						//CZ adr
		if (state->cc.z == 1)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xcd: 						//CALL address
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = (opcode[2] << 8) | opcode[1];
	}
	break;
	case 0xce:					//ACI  byte 
	{
		uint16_t x = state->a + opcode[1] + state->cc.cy;
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (x > 0xff);
		state->a = x & 0xff;
		state->pc++;
	}
	break;
	case 0xcf: 					//RST 1
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x0008;
	}
	break;

	case 0xd0:					//RNC
		if (state->cc.cy == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xd1: 						//POP    D
		Pop(state, &state->d, &state->e);
		break;
	case 0xd2:						//JNC
		if (state->cc.cy == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xd3:                      //OUT d8
		state->pc++;
		break;
	case 0xd4:						//CNC adr
		if (state->cc.cy == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xd5: 						//PUSH   D
		Push(state, state->d, state->e);
		break;
	case 0xd6: 						//SUI    byte
	{
		uint8_t x = state->a - opcode[1];
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (state->a < opcode[1]);
		state->a = x;
		state->pc++;
	}
	break;
	case 0xd7: 					//RST 2
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x10;
	}
	break;
	case 0xd8: 					//RC
		if (state->cc.cy != 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xd9: UnimplementedInstruction(state); break;
	case 0xda: 					//JC
		if (state->cc.cy != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xdb: 					//IN d8
		state->pc++;
		break;
	case 0xdc: 					//CC adr
		if (state->cc.cy != 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xdd: UnimplementedInstruction(state); break;
	case 0xde: 					//SBI byte
	{
		uint16_t x = state->a - opcode[1] - state->cc.cy;
		FlagsZSP(state, x & 0xff);
		state->cc.cy = (x > 0xff);
		state->a = x & 0xff;
		state->pc++;
	}
	break;
	case 0xdf: 					//RST 3
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x18;
	}
	break;

	case 0xe0:					//RPO
		if (state->cc.p == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xe1: 					//POP    H
		Pop(state, &state->h, &state->l);
		break;
	case 0xe2: 						//JPO
		if (state->cc.p == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xe3:						//XTHL 
	{
		uint8_t h = state->h;
		uint8_t l = state->l;
		state->l = state->memory[state->sp];
		state->h = state->memory[state->sp + 1];
		WriteMem(state, state->sp, l);
		WriteMem(state, state->sp + 1, h);
	}
	break;
	case 0xe4: 						//CPO adr
		if (state->cc.p == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xe5: 						//PUSH   H
		Push(state, state->h, state->l);
		break;
	case 0xe6: 						//ANI    byte
	{
		state->a = state->a & opcode[1];
		LogicFlagsA(state);
		state->pc++;
	}
	break;
	case 0xe7: 					//RST 4
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x20;
	}
	break;
	case 0xe8: 					//RPE
		if (state->cc.p != 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xe9: 						//PCHL
		state->pc = (state->h << 8) | state->l;
		break;
	case 0xea: 						//JPE
		if (state->cc.p != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xeb: 					//XCHG
	{
		uint8_t save1 = state->d;
		uint8_t save2 = state->e;
		state->d = state->h;
		state->e = state->l;
		state->h = save1;
		state->l = save2;
	}
	break;
	case 0xec: 					//CPE adr
		if (state->cc.p != 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;
	case 0xed: UnimplementedInstruction(state); break;
	case 0xee: 					//XRI data
	{
		uint8_t x = state->a ^ opcode[1];
		FlagsZSP(state, x);
		state->cc.cy = 0;		//data book says clear cy
		state->a = x;
		state->pc++;
	}
	break;
	case 0xef: 					//RST 5
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x28;
	}
	break;

	case 0xf0: 					//RP
		if (state->cc.s == 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xf1: 					//POP PSW
		Pop(state, &state->a, (unsigned char*)&state->cc);
		break;
	case 0xf2:
		if (state->cc.s == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xf3: state->int_enable = 0;  break;
	case 0xf4: 						//CP
		if (state->cc.s == 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;

	case 0xf5: 						//PUSH   PSW
		Push(state, state->a, *(unsigned char*)&state->cc);
		break;

	case 0xf6: 						//ORI    byte
	{
		//AC set if lower nibble of h was zero prior to dec
		uint8_t x = state->a | opcode[1];
		FlagsZSP(state, x);
		state->cc.cy = 0;
		state->a = x;
		state->pc++;
	}
	break;
	case 0xf7: 					//RST 6
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x30;
	}
	break;
	case 0xf8:					//RM
		if (state->cc.s != 0)
		{
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xf9: 					//SPHL
		state->sp = state->l | (state->h << 8);
		break;
	case 0xfa: 					//JM
		if (state->cc.s != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xfb: state->int_enable = 1;  break;
	case 0xfc: 					//CM
		if (state->cc.s != 0)
		{
			uint16_t	ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
		break;

	case 0xfd: UnimplementedInstruction(state); break;
	case 0xfe: 						//CPI  byte
	{
		uint8_t x = state->a - opcode[1];
		FlagsZSP(state, x);
		state->cc.cy = (state->a < opcode[1]);
		state->pc++;
	}
	break;
	case 0xff: 					//RST 7
	{
		uint16_t	ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x38;
	}
	break;
	}

#if PRINTOPS
	printf("\t");
	printf("%c", state->cc.z ? 'z' : '.');
	printf("%c", state->cc.s ? 's' : '.');
	printf("%c", state->cc.p ? 'p' : '.');
	printf("%c", state->cc.cy ? 'c' : '.');
	printf("%c  ", state->cc.ac ? 'a' : '.');
	printf("A $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n", state->a, state->b, state->c,
		state->d, state->e, state->h, state->l, state->sp);
#endif
	return cycles8080[*opcode];
}
void GenerateInterrupt(State8080* state, int interrupt_num)
{
	//perform "PUSH PC"
	Push(state, (state->pc & 0xFF00) >> 8, (state->pc & 0xff));

	//Set the PC to the low memory vector
	state->pc = 8 * interrupt_num;

	//"DI"
	state->int_enable = 0;
}