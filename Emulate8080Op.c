#include "emuhead.h"

int Emulate8080Op(State8080 * state){
    //Put pointer at memory at location pc
    unsigned char *opcode = &(state->memory[state->pc]);
	uint16_t temp;
	uint8_t temp_bit7;
	uint8_t temp_bit0;
	uint32_t hl;
	uint32_t de;
	uint32_t bc;
	uint16_t ret;
	uint8_t temph;
	uint8_t templ;
	uint16_t sum;
	uint8_t psw;
    int cycles = 0;

    //modify processor state based on operation
        //NOTE TO SELF: AC won't be used, but check that its algorithm
        //for the DCR operations is correct
	state->pc+=0x0001;
    switch(*opcode){
        //NOP
        case 0x00:  cycles = NOP; break; 
        //LXI B, D16
        case 0x01:  state->b = opcode[2];  
                    state->c = opcode[1]; 
                    state->pc+=2; 
                    cycles = LXI_B; break;
        //STAX B
        case 0x02:  state->memory[regCombine(state->b) | state->c] = state->a; 
                    cycles = STAX_B; break;
        //INX B
        case 0x03:  bc =  (regCombine(state->b) | state->c) + 1;
                    state->b = (uint8_t) ((bc>>8 )& 0x00ff);
                    state->c = (uint8_t) (bc & 0x00ff);
                    cycles = INX_B; break;
        //INR B (Z,S,P,AC)
        case 0x04:  temp = state->b;
                    state->b = state->b + 1;
                    state->cc.s = (state->b >= 0x80 ? 1 : 0);
                    state->cc.z = (state->b == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->b);
                    cycles = INR_R; break;
        //DCR B (Z,S,P,AC)
        case 0x05:  temp = state->b;
                    state->b = state->b - 1;
                    state->cc.s = (state->b >= 0x80 ? 1 : 0);
                    state->cc.z = (state->b == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->b);
                    cycles = DCR_R; break;
        //MVI B, D8
        case 0x06:  state->b = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //RLC (CY)
        case 0x07:  temp_bit7 =  (state->a & 0x80)>>7;
                    state->a = (state->a << 1) | temp_bit7;
                    state->cc.cy = temp_bit7;
                    cycles = RLC; break;
        //NOP
        case 0x08:  cycles = NOP; break;
        //DAD B (CY)
        case 0x09:  hl = (uint32_t) (regCombine(state->h) | state->l);
                    hl += (uint32_t) (regCombine(state->b) | state->c);
                    state->h = (uint8_t) ((hl>>8) & 0x00ff);
                    state->l = (uint8_t) (hl & 0x00ff);
                    state->cc.cy = hl >= 0x00010000 ? 1 : 0;
                    cycles = DAD_B; break;
        //LDAX B
        case 0x0a:  state->a = state->memory[regCombine(state->b) | state->c];
                    cycles = LDAX_B; break;
        //DCX B
        case 0x0b:  bc =  (regCombine(state->b) | state->c) - 1;
                    state->b = (uint8_t) ((bc>>8 )& 0x00ff);
                    state->c = (uint8_t) (bc & 0x00ff);
                    cycles = DCX_B; break;
        //INR C (Z,S,P,AC)
        case 0x0c:  temp = state->c;
                    state->c = state->c + 1;
                    state->cc.s = (state->c >= 0x80 ? 1 : 0);
                    state->cc.z = (state->c == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->c);
                    cycles = INR_R; break;
        //DCR C (Z,S,P,AC)
        case 0x0d:  temp = state->c;
                    state->c = state->c - 1;
                    state->cc.s = (state->c >= 0x80 ? 1 : 0);
                    state->cc.z = (state->c == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->c);
                    cycles = DCR_R; break;
        //MVI C, D8
        case 0x0e:  state->c = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //RRC (CY)
        case 0x0f:  temp_bit0 =(state->a & 0x01);
                    state->a = (state->a >> 1) | (temp_bit0 << 7);
                    state->cc.cy = temp_bit0;
                    cycles = RRC; break;
/*
-----------------------------------------------------------------------------------
*/
        //NOP
        case 0x10:  cycles = NOP; break; 
        //LXI D, D16
        case 0x11:  state->d = opcode[2];  
                    state->e = opcode[1]; 
                    state->pc+=2; 
                    cycles = LXI_D; break;
        //STAX D
        case 0x12:  state->memory[regCombine(state->d) | state->e] = state->a; 
                    cycles = STAX_D; break;
        //INX D
        case 0x13:  de =  (regCombine(state->d) | state->e) + 0x0001;
                    state->d = (uint8_t) ((de>>8 )& 0x00ff);
                    state->e = (uint8_t) (de & 0x00ff);
                    cycles = INX_D; break;
        //INR D (Z,S,P,AC)
        case 0x14:  temp = state->d;
                    state->d = state->d + 1;
                    state->cc.s = (state->d >= 0x80 ? 1 : 0);
                    state->cc.z = (state->d == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->d);
                    cycles = INR_R; break;
        //DCR D (Z,S,P,AC)
        case 0x15:  temp = state->d;
                    state->d = state->d - 1;
                    state->cc.s = (state->d >= 0x80 ? 1 : 0);
                    state->cc.z = (state->d == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->d);
                    cycles = DCR_R; break;
        //MVI D, D8
        case 0x16:  state->d = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //RAL (CY)
        case 0x17:  temp_bit7 =  (state->a & 0x80)>>7;
                    state->a = (state->a << 1) | state->cc.cy;
                    state->cc.cy = temp_bit7;
                    cycles = RAL; break;
        //NOP
        case 0x18:  cycles = NOP; break;
        //DAD D (CY)
        case 0x19:  hl = (uint32_t) (regCombine(state->h) | state->l);
                    hl += (uint32_t) (regCombine(state->d) | state->e);
                    state->h = (uint8_t) ((hl>>8) & 0x00ff);
                    state->l = (uint8_t) (hl & 0x00ff);
                    state->cc.cy = hl >= 0x00010000 ? 1 : 0;
                    cycles = DAD_D; break;
        //LDAX D
        case 0x1a:  state->a = state->memory[regCombine(state->d) | state->e];
                    cycles = LDAX_D; break;
        //DCX D
        case 0x1b:  de =  (regCombine(state->d) | state->e) - 1;
                    state->d = (uint8_t) ((de>>8 )& 0x00ff);
                    state->e = (uint8_t) (de & 0x00ff);
                    cycles = DCX_D; break;
        //INR E (Z,S,P,AC)
        case 0x1c:  temp = state->e;
                    state->e = state->e + 1;
                    state->cc.s = (state->e >= 0x80 ? 1 : 0);
                    state->cc.z = (state->e == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->e);
                    cycles = INR_R; break;
        //DCR E (Z,S,P,AC)
        case 0x1d:  temp = state->e;
                    state->e = state->e - 1;
                    state->cc.s = (state->e >= 0x80 ? 1 : 0);
                    state->cc.z = (state->e == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->e);
                    cycles = DCR_R; break;
        //MVI E, D8
        case 0x1e:  state->e = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //RAR (CY)
        case 0x1f:  temp = state->a;
                    state->a = (state->a >> 1) | (state->cc.cy << 7);
                    state->cc.cy = (uint8_t)(temp & 0x01);
                    cycles = RAR; break;
/*
-----------------------------------------------------------------------------------
*/      
        //NOP
        case 0x20:  cycles = NOP; break;
        //LXI H, D16
        case 0x21:  state->h = opcode[2];  
                    state->l = opcode[1]; 
                    state->pc+=2; 
                    cycles = LXI_H; break;
        //SHLD ADR:
        case 0x22:  temp = regCombine(opcode[2]) | opcode[1];
                    state->memory[temp] = state->l;
                    state->memory[temp+1] = state->h;
                    state->pc+=2;
                    cycles = SHLD; break;
        //INX H
        case 0x23:  hl =  (regCombine(state->h) | state->l) + 1;
                    state->h = (uint8_t) ((hl>>8 )& 0x00ff);
                    state->l = (uint8_t) (hl & 0x00ff);
                    cycles = INX_H; break;
        //INR H (Z,S,P,AC)
        case 0x24:  temp = state->h;
                    state->d = state->h + 1;
                    state->cc.s = (state->h >= 0x80 ? 1 : 0);
                    state->cc.z = (state->h == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->h);
                    cycles = INR_R; break;
        //DCR H (Z,S,P,AC)
        case 0x25:  temp = state->h;
                    state->h = state->h - 1;
                    state->cc.s = (state->h >= 0x80 ? 1 : 0);
                    state->cc.z = (state->h == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->h);
                    cycles = DCR_R; break;
        //MVI H, D8
        case 0x26:  state->h = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //DAA
        case 0x27:  
            if ((state->a & 0xf) > 9 || state->cc.ac == 0x1) {
                state->a += 6;
            }
            if ((state->a & 0xf0) > 0x90 || state->cc.cy == 0x1) {
               temp = (uint16_t)state->a + 0x60;
               state->a = temp & 0xff;
               state->cc.cy = 0x1;
               state->cc.s = (state->a >= 0x80 ? 1 : 0);
               state->cc.z = (state->a == 0x00 ? 1 : 0);
               state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1 : 0;
               state->cc.p = Parity(state->a);
            }
            else {
                temp = state->a;
                state->cc.cy = 0x0;
            }
  
            break;
        //DAD H (CY)
        case 0x29:  hl = (uint32_t) (regCombine(state->h) | state->l);
                    hl += (uint32_t) (regCombine(state->h) | state->l);
                    state->h = (uint8_t) ((hl>>8) & 0x00ff);
                    state->l = (uint8_t) (hl & 0x00ff);
                    state->cc.cy = hl >= 0x00010000 ? 1 : 0;
                    cycles = DAD_H; break;
        //LHLD adr
        case 0x2a:  hl = state->memory[regCombine(opcode[2]) | opcode[1]] | ((state->memory[regCombine(opcode[2]) | opcode[1] + 1]) << 8);
                    state->h = (uint8_t)((hl >> 8) & 0x00ff);
                    state->l = (uint8_t)(hl & 0x00ff);
                    state->pc+=2;
                    cycles = LHLD; break;
        //DCX H
        case 0x2b:  hl =  (regCombine(state->h) | state->l) - 1;
                    state->h = (uint8_t) ((hl>>8 )& 0x00ff);
                    state->l = (uint8_t) (hl & 0x00ff);
                    cycles = DCX_H; break;
        //INR L (Z,S,P,AC)
        case 0x2c:  temp = state->l;
                    state->l = state->l + 1;
                    state->cc.s = (state->l >= 0x80 ? 1 : 0);
                    state->cc.z = (state->l == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->l);
                    cycles = INR_R; break;
        //DCR L (Z,S,P,AC)
        case 0x2d:  temp = state->l;
                    state->l = state->l - 1;
                    state->cc.s = (state->l >= 0x80 ? 1 : 0);
                    state->cc.z = (state->l == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->l);
                    cycles = DCR_R; break;
        //MVI L, D8
        case 0x2e:  state->l = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //CMA
        case 0x2f:  state->a = ~(state->a);
                    cycles = CMA; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //NOP
        case 0x30:  cycles = NOP; break;
        //LXI SP, D16
        case 0x31:  state->sp = regCombine(opcode[2]) | opcode[1];
                    state->pc+=2;
                    cycles = LXI_SP; break;
        //STA adr
        case 0x32:  state->memory[regCombine(opcode[2]) | opcode[1]] = state->a;
                 
                    state->pc+=2;
                  
                    cycles = STA; break;
        //INX SP
        case 0x33:  state->sp +=1;
                    cycles = INX_SP; break;
        //INR M
        case 0x34:  temp = state->memory[regCombine(state->h) | state->l];
                    state->memory[regCombine(state->h) | state->l] += 1;
                    state->cc.s = (state->memory[regCombine(state->h) | state->l] >= 0x80 ? 1 : 0);
                    state->cc.z = (state->memory[regCombine(state->h) | state->l] == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->memory[regCombine(state->h) | state->l]);
                    cycles = INR_M; break;
        //DCR M
        case 0x35:  temp = state->memory[regCombine(state->h) | state->l];
                    state->memory[regCombine(state->h) | state->l] -= 1;
                    state->cc.s = (state->memory[regCombine(state->h) | state->l] >= 0x80 ? 1 : 0);
                    state->cc.z = (state->memory[regCombine(state->h) | state->l] == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->memory[regCombine(state->h) | state->l]);
                    cycles = DCR_M; break;
        //MVI M, D8
        case 0x36:  state->memory[regCombine(state->h) | state->l] = opcode[1];
                    state->pc +=1;
                    cycles = MVI_M; break;
        //STC
        case 0x37:  state->cc.cy = 1;
                    cycles = STC; break;
        //NOP
        case 0x38:  cycles = NOP; break;
        //DAD SP (CY)
        case 0x39:  hl = (uint32_t) (regCombine(state->h) | state->l);
                    hl += (uint32_t) state->sp;
                    state->h = (uint8_t) ((hl>>8) & 0x00ff);
                    state->l = (uint8_t) (hl & 0x00ff);
                    state->cc.cy = hl >= 0x00010000 ? 1 : 0;
                    cycles = DAD_SP; break;  
        //LDA adr
        case 0x3a:  state->a = state->memory[regCombine(opcode[2]) | opcode[1]];
                    state->pc += 2;
                    cycles = LDA; break;
        //DCX SP
        case 0x3b:  state->sp -=1;
                    cycles = DCX_SP; break;
        //INR A (S,Z,AC,P)
        case 0x3c:  temp = state->a;
                    state->a = state->a + 1;
                    state->cc.s = (state->a >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) + (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    cycles = INR_R; break;
        //DCR A (S,Z,AC,P)
        case 0x3d:  temp = state->a;
                    state->a = state->a - 1;
                    state->cc.s = (state->a >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((temp & 0x0f) - (0x01 & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    cycles = DCR_R; break;
        //MVI A, D8
        case 0x3e:  state->a = opcode[1];
                    state->pc +=1;
                    cycles = MVI_R; break;
        //CMC
        case 0x3f:  state->cc.cy = ~(state->cc.cy);
                    cycles = CMC; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //MOV B, B
        case 0x40:  state->b = state->b;
                    cycles = MOV_R1R2; break;
        //MOV B, C
        case 0x41:  state->b = state->c;
                    cycles = MOV_R1R2; break;
        //MOV B, D
        case 0x42:  state->b = state->d;
                    cycles = MOV_R1R2; break;
        //MOV B, E
        case 0x43:  state->b = state->e;
                    cycles = MOV_R1R2; break;
        //MOV B, H
        case 0x44:  state->b = state->h;
                    cycles = MOV_R1R2; break;
        //MOV B, L
        case 0x45:  state->b = state->l;
                    cycles = MOV_R1R2; break;
        //MOV B, M
        case 0x46:  state->b = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV B, A
        case 0x47:  state->b = state->a;
                    cycles = MOV_R1R2; break;
        //MOV C, B
        case 0x48:  state->c = state->b;
                    cycles = MOV_R1R2; break;
        //MOV C, C
        case 0x49:  state->c = state->c;
                    cycles = MOV_R1R2; break;
        //MOV C, D
        case 0x4a:  state->c = state->d;
                    cycles = MOV_R1R2; break;
        //MOV C, E
        case 0x4b:  state->c = state->e;
                    cycles = MOV_R1R2; break;
        //MOV C, H
        case 0x4c:  state->c = state->h;
                    cycles = MOV_R1R2; break;
        //MOV C, L
        case 0x4d:  state->c = state->l;
                    cycles = MOV_R1R2; break;
        //MOV C, M
        case 0x4e:  state->c = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV C, A
        case 0x4f:  state->c = state->a;
                    cycles = MOV_R1R2; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //MOV D, B
        case 0x50:  state->d = state->b;
                    cycles = MOV_R1R2; break;
        //MOV D, C
        case 0x51:  state->d = state->c;
                    cycles = MOV_R1R2; break;
        //MOV D, D
        case 0x52:  state->d = state->d;
                    cycles = MOV_R1R2; break;
        //MOV D, E
        case 0x53:  state->d = state->e;
                    cycles = MOV_R1R2; break;
        //MOV D, H
        case 0x54:  state->d = state->h;
                    cycles = MOV_R1R2; break;
        //MOV D, L
        case 0x55:  state->d = state->l;
                    cycles = MOV_R1R2; break;
        //MOV D, M
        case 0x56:  state->d = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV D, A
        case 0x57:  state->d = state->a;
                    cycles = MOV_R1R2; break;
        //MOV E, B
        case 0x58:  state->e = state->b;
                    cycles = MOV_R1R2; break;
        //MOV E, C
        case 0x59:  state->e = state->c;
                    cycles = MOV_R1R2; break;
        //MOV E, D
        case 0x5a:  state->e = state->d;
                    cycles = MOV_R1R2; break;
        //MOV E, E
        case 0x5b:  state->e = state->e;
                    cycles = MOV_R1R2; break;
        //MOV E, H
        case 0x5c:  state->e = state->h;
                    cycles = MOV_R1R2; break;
        //MOV E, L
        case 0x5d:  state->e = state->l;
                    cycles = MOV_R1R2; break;
        //MOV E, M
        case 0x5e:  state->e = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV E, A
        case 0x5f:  state->e = state->a;
                    cycles = MOV_R1R2; break;
/*
-----------------------------------------------------------------------------------
*/          
        //MOV H, B
        case 0x60:  state->h = state->b;
                    cycles = MOV_R1R2; break;
        //MOV H, C
        case 0x61:  state->h = state->c;
                    cycles = MOV_R1R2; break;
        //MOV H, D
        case 0x62:  state->h = state->d;
                    cycles = MOV_R1R2; break;
        //MOV H, E
        case 0x63:  state->h = state->e;
                    cycles = MOV_R1R2; break;
        //MOV H, H
        case 0x64:  state->h = state->h;
                    cycles = MOV_R1R2; break;
        //MOV H, L
        case 0x65:  state->h = state->l;
                    cycles = MOV_R1R2; break;
        //MOV H, M
        case 0x66:  state->h = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV H, A
        case 0x67:  state->h = state->a;
                    cycles = MOV_R1R2; break;
        //MOV L, B
        case 0x68:  state->l = state->b;
                    cycles = MOV_R1R2; break;
        //MOV L, C
        case 0x69:  state->l = state->c;
                    cycles = MOV_R1R2; break;
        //MOV L, D
        case 0x6a:  state->l = state->d;
                    cycles = MOV_R1R2; break;
        //MOV L, E
        case 0x6b:  state->l = state->e;
                    cycles = MOV_R1R2; break;
        //MOV L, H
        case 0x6c:  state->l = state->h;
                    cycles = MOV_R1R2; break;
        //MOV L, L
        case 0x6d:  state->l = state->l;
                    cycles = MOV_R1R2; break;
        //MOV L, M
        case 0x6e:  state->l = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV L, A
        case 0x6f:  state->l = state->a;
                    cycles = MOV_R1R2; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //MOV M, B
        case 0x70:  state->memory[regCombine(state->h) | state->l]= state->b;
                    cycles = MOV_MR; break;
        //MOV M, C
        case 0x71:  state->memory[regCombine(state->h) | state->l] = state->c;
                    cycles = MOV_MR; break;
        //MOV M, D
        case 0x72:  state->memory[regCombine(state->h) | state->l] = state->d;
                    cycles = MOV_MR; break;
        //MOV M, E
        case 0x73:  state->memory[regCombine(state->h) | state->l] = state->e;
                    cycles = MOV_MR; break;
        //MOV M, H
        case 0x74:  state->memory[regCombine(state->h) | state->l] = state->h;
                    cycles = MOV_MR; break;
        //MOV M, L
        case 0x75:  state->memory[regCombine(state->h) | state->l] = state->l;
                    cycles = MOV_MR; break;
        //HLT
        case 0x76:  exit(0);
					cycles = HLT; break;
        //MOV M, A
        case 0x77:  state->memory[regCombine(state->h) | state->l] = state->a;
                    cycles = MOV_MR; break;
        //MOV A, B
        case 0x78:  state->a = state->b;
                    cycles = MOV_R1R2; break;
        //MOV A, C
        case 0x79:  state->a = state->c;
                    cycles = MOV_R1R2; break;
        //MOV A, D
        case 0x7a:  state->a = state->d;
                    cycles = MOV_R1R2; break;
        //MOV A, E
        case 0x7b:  state->a = state->e;
                    cycles = MOV_R1R2; break;
        //MOV A, H
        case 0x7c:  state->a = state->h;
                    cycles = MOV_R1R2; break;
        //MOV A, L
        case 0x7d:  state->a = state->l;
                    cycles = MOV_R1R2; break;
        //MOV A, M
        case 0x7e:  state->a = state->memory[regCombine(state->h) | state->l];
                    cycles = MOV_RM; break;
        //MOV A, A
        case 0x7f:  state->a = state->a;
                    cycles = MOV_R1R2; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //ADD B (all flags)
        case 0x80:  sum =  (uint16_t) state->a + (uint16_t) state->b;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->b & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADD C (all flags)
        case 0x81:  sum =  (uint16_t) state->a + (uint16_t) state->c;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->c & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADD D (all flags)
        case 0x82:  sum =  (uint16_t) state->a + (uint16_t) state->d;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->d & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADD E (all flags)
        case 0x83:  sum =  (uint16_t) state->a + (uint16_t) state->e;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->e & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADD H (all flags)
        case 0x84:  sum =  (uint16_t) state->a + (uint16_t) state->h;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->h & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADD L (all flags)
        case 0x85:  sum =  (uint16_t) state->a + (uint16_t) state->l;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->l & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADD M (all flags)
        case 0x86:  sum =  (uint16_t) state->a + (uint16_t) state->memory[regCombine(state->h) | state->l];
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_M; break;
        //ADD A (all flags)
        case 0x87:  sum =  (uint16_t) state->a + (uint16_t) state->a;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + (state->a & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADD_R; break;
        //ADC B (all flags)
        case 0x88:  sum =  (uint16_t) state->a + (uint16_t) state->b + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->b + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
        //ADC C (all flags)
        case 0x89:  sum =  (uint16_t) state->a + (uint16_t) state->c + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->c + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
        //ADC D (all flags)
        case 0x8a:  sum =  (uint16_t) state->a + (uint16_t) state->d + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->d + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
        //ADC E (all flags)
        case 0x8b:  sum =  (uint16_t) state->a + (uint16_t) state->e + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->e + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
        //ADC H (all flags)
        case 0x8c:  sum =  (uint16_t) state->a + (uint16_t) state->h + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->h + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
        //ADC L (all flags)
        case 0x8d:  sum =  (uint16_t) state->a + (uint16_t) state->l + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->l + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
        //ADC M (all flags)
        case 0x8e:  sum =  (uint16_t) state->a + (uint16_t) state->memory[regCombine(state->h) | state->l] + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (((state->a + state->cc.cy) & 0x0f) + (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_M; break;
        //ADC A (all flags)
        case 0x8f:  sum =  (uint16_t) state->a + (uint16_t) state->a + state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) + ((state->a + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ADC_R; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //SUB B (all flags)
        case 0x90:  sum =  (uint16_t) state->a - (uint16_t) state->b;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->b & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SUB C (all flags)
        case 0x91:  sum =  (uint16_t) state->a - (uint16_t) state->c;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->c & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SUB D (all flags)
        case 0x92:  sum =  (uint16_t) state->a - (uint16_t) state->d;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->d & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SUB E (all flags)
        case 0x93:  sum =  (uint16_t) state->a - (uint16_t) state->e;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->e & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SUB H (all flags)
        case 0x94:  sum =  (uint16_t) state->a - (uint16_t) state->h;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->h & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SUB L (all flags)
        case 0x95:  sum =  (uint16_t) state->a - (uint16_t) state->l;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->l & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SUB M (all flags)
        case 0x96:  sum =  (uint16_t) state->a - (uint16_t) state->memory[regCombine(state->h) | state->l];
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_M; break;
        //SUB A (all flags)
        case 0x97:  sum =  (uint16_t) state->a - (uint16_t) state->a;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->a & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SUB_R; break;
        //SBB B (all flags)
        case 0x98:  sum =  (uint16_t) state->a - (uint16_t) state->b - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->b + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
        //SBB C (all flags)
        case 0x99:  sum =  (uint16_t) state->a - (uint16_t) state->c - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->c + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
        //SBB D (all flags)
        case 0x9a:  sum =  (uint16_t) state->a - (uint16_t) state->d - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->d + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
        //SBB E (all flags)
        case 0x9b:  sum =  (uint16_t) state->a - (uint16_t) state->e - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->e + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
        //SBB H (all flags)
        case 0x9c:  sum =  (uint16_t) state->a - (uint16_t) state->h - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->h + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
        //SBB L (all flags)
        case 0x9d:  sum =  (uint16_t) state->a - (uint16_t) state->l - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->l + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
        //SBB M (all flags)
        case 0x9e:  sum =  (uint16_t) state->a - (uint16_t) state->memory[regCombine(state->h) | state->l] - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (((state->a - state->cc.cy) & 0x0f) - (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_M; break;
        //SBB A (all flags)
        case 0x9f:  sum =  (uint16_t) state->a - (uint16_t) state->a - state->cc.cy;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - ((state->a + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = SBB_R; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //ANA B (all flags)
        case 0xa0:  sum =  (uint16_t) state->a & (uint16_t) state->b;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->b & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //ANA C (all flags)
        case 0xa1:  sum =  (uint16_t) state->a & (uint16_t) state->c;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->c & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //ANA D (all flags)
        case 0xa2:  sum =  (uint16_t) state->a & (uint16_t) state->d;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->d & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //ANA E (all flags)
        case 0xa3:  sum =  (uint16_t) state->a & (uint16_t) state->e;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->e & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //ANA H (all flags)
        case 0xa4:  sum =  (uint16_t) state->a & (uint16_t) state->h;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->h & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //ANA L (all flags)
        case 0xa5:  sum =  (uint16_t) state->a & (uint16_t) state->l;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->l & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //ANA M (all flags)
        case 0xa6:  sum =  (uint16_t) state->a & (uint16_t) state->memory[regCombine(state->h) | state->l];
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_M; break;
        //ANA A (all flags)
        case 0xa7:  sum =  (uint16_t) state->a & (uint16_t) state->a;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) & (state->a & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ANA_R; break;
        //XRA B (all flags)
        case 0xa8:  sum =  (uint16_t) state->a ^ (uint16_t) state->b;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->b & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
        //XRA C (all flags)
        case 0xa9:  sum =  (uint16_t) state->a ^ (uint16_t) state->c;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->c & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
        //XRA D (all flags)
        case 0xaa:  sum =  (uint16_t) state->a ^ (uint16_t) state->d;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->d & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
        //XRA E (all flags)
        case 0xab:  sum =  (uint16_t) state->a ^ (uint16_t) state->e;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->e & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
        //XRA H (all flags)
        case 0xac:  sum =  (uint16_t) state->a ^ (uint16_t) state->h;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->h & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
        //XRA L (all flags)
        case 0xad:  sum =  (uint16_t) state->a ^ (uint16_t) state->l;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->l & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
        //XRA M (all flags)
        case 0xae:  sum =  (uint16_t) state->a ^ (uint16_t) state->memory[regCombine(state->h) | state->l];
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) ^ (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_M; break;
        //XRA A (all flags)
        case 0xaf:  sum =  (uint16_t) state->a ^ (uint16_t) state->a;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((state->a & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = XRA_R; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //ORA B (all flags)
        case 0xb0:  sum =  (uint16_t) state->a | (uint16_t) state->b;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->b & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //ORA C (all flags)
        case 0xb1:  sum =  (uint16_t) state->a | (uint16_t) state->c;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->c & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //ORA D (all flags)
        case 0xb2:  sum =  (uint16_t) state->a | (uint16_t) state->d;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->d & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //ORA E (all flags)
        case 0xb3:  sum =  (uint16_t) state->a | (uint16_t) state->e;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->e & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //ORA H (all flags)
        case 0xb4:  sum =  (uint16_t) state->a | (uint16_t) state->h;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->h & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //ORA L (all flags)
        case 0xb5:  sum =  (uint16_t) state->a | (uint16_t) state->l;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->l & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //ORA M (all flags)
        case 0xb6:  sum =  (uint16_t) state->a | (uint16_t) state->memory[regCombine(state->h) | state->l];
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_M; break;
        //ORA A (all flags)
        case 0xb7:  sum =  (uint16_t) state->a | (uint16_t) state->a;
                    state->a = (uint8_t) (sum & 0x00ff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) | (state->a & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = ORA_R; break;
        //CMP B (all flags)
        case 0xb8:  sum =  (uint16_t) state->a - (uint16_t) state->b;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->b & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
        //CMP C (all flags)
        case 0xb9:  sum =  (uint16_t) state->a - (uint16_t) state->c;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->c & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
        //CMP D (all flags)
        case 0xba:  sum =  (uint16_t) state->a - (uint16_t) state->d;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->d & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
        //CMP E (all flags)
        case 0xbb:  sum =  (uint16_t) state->a - (uint16_t) state->e;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->e & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
        //CMP H (all flags)
        case 0xbc:  sum =  (uint16_t) state->a - (uint16_t) state->h;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->h & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
        //CMP L (all flags)
        case 0xbd:  sum =  (uint16_t) state->a - (uint16_t) state->l;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->l & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
        //CMP M (all flags)
        case 0xbe:  sum =  (uint16_t) state->a - (uint16_t) state->memory[regCombine(state->h) | state->l];
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = ((state->a & 0x0f) - (state->memory[regCombine(state->h) | state->l] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_M; break;
        //CMP A (all flags)
        case 0xbf:  sum =  (uint16_t) state->a - (uint16_t) state->a;
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = ((uint8_t) sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((state->a & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t) sum);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
                    cycles = CMP_R; break;
/*
-----------------------------------------------------------------------------------
*/ 
        //RNZ
        case 0xc0:  if(state->cc.z != 1) {
                        ret8080(state, opcode);
                    }
                    cycles = RNZ; break;
        //POP B
        case 0xc1:  state->c = state->memory[state->sp];
					state->b = state->memory[state->sp+1];
					state->sp +=2;
					cycles = POP_B; break;
		//JNZ adr
		case 0xc2:  if(state->cc.z != 1) 
						state->pc = regCombine(opcode[2]) | opcode[1];
					else
						state->pc+=2;
					cycles = JNZ; break;
		//JMP adr
		case 0xc3:  state->pc = regCombine(opcode[2]) | opcode[1];
					cycles = JMP; break;
		//CNZ adr
		case 0xc4:  if(state->cc.z != 1){
						call8080(state, opcode);
					}
					else
						state->pc+=2;
					cycles = CNZ; break;
		//PUSH B
		case 0xc5:  state->memory[state->sp-2] = state->c;
					state->memory[state->sp-1] = state->b;
					state->sp -=2;
					cycles = PUSH_B; break;
		//ADI D8 (all flags)
		case 0xc6:  sum =  (uint16_t) state->a + (uint16_t) opcode[1];
					state->a = (uint8_t) (sum & 0xff);
                    state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) + ((opcode[1] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
                    cycles = ADI; break;   
		//RST 0
		case 0xc7:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0000;
					cycles = RST; break;
		//RZ
		case 0xc8:  if(state->cc.z == 1) {
						ret8080(state, opcode);
					}
					cycles = RZ; break;
		//RET
		case 0xc9:  ret8080(state, opcode);
					cycles = RET; break;
		//JZ adr
		case 0xca:  if(state->cc.z == 1){
						state->pc = regCombine(opcode[2]) | opcode[1];
					}
					else
						state->pc+=2;
					cycles = JZ; break;
		//NOP
		case 0xcb:  cycles = NOP; break;
		//CZ adr
		case 0xcc:  if(state->cc.z == 1) {
						call8080(state, opcode);
					}
					else
						state->pc+=2;
					cycles = CZ; break;
		//CALL adr 
		case 0xcd:  call8080(state, opcode);
					cycles = CALL; break;
		//ACI D8 (all flags)
		case 0xce:  sum =  state->a + opcode[1] +state->cc.cy;
					state->a = (uint8_t) (sum & 0xff);
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) + (((opcode[1] + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
					cycles = ACI; break;
		///RST 1
		case 0xcf:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0008;
					cycles = RST; break;
/*
-----------------------------------------------------------------------------------
*/ 		//RNC
		case 0xd0: if(state->cc.cy == 0) {
						ret8080(state, opcode);
					}
					cycles = RNC; break;
		//POP D
		case 0xd1:  state->e = state->memory[state->sp];
					state->d = state->memory[state->sp+1];
					state->sp+=2;
					cycles = POP_D; break;
		//JNC adr
		case 0xd2:  if(state->cc.cy == 0) {
						state->pc = regCombine(opcode[2]) | opcode[1];
					}
					else
						state->pc +=2;
					cycles = JNC; break;
		//OUT D8 //handled specific to program - check main
		case 0xd3:  state->pc += 1;
					cycles = OUT; break;
		//CNC adr
		case 0xd4:  if( state->cc.cy == 0 ){
						call8080(state, opcode);
					}
					else
						state->pc+=2;	
					cycles = CNC; break;
		//PUSH D
		case 0xd5:  state->memory[state->sp-2] = state->e;
					state->memory[state->sp-1] = state->d;
					state->sp-=2;
					cycles = PUSH_D; break;
		//SUI D8 (all flags)
		case 0xd6:  sum =  state->a - opcode[1];
					state->a = (uint8_t) (sum & 0xff);
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((opcode[1] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
					cycles = SUI; break;
		//RST 2
		case 0xd7:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0000;
					cycles = RST; break;
		//RC
		case 0xd8:  if(state->cc.cy == 1){
						ret8080(state,opcode);
					}
					else
						cycles = RC; break;
		//NOP
		case 0xd9:  cycles = NOP; break;
		//JC adr
		case 0xda:  if(state->cc.cy) {
						state->pc = regCombine(opcode[2]) | opcode[1];
					}
					else
						state->pc+=2;
					cycles = JC; break;
		//IN D8 (handled game specific - see main program)
		case 0xdb:  state->pc+=1;
					cycles = IN; break;
		//CC adr
		case 0xdc:  if(state->cc.cy == 1) {
						call8080(state, opcode);
					}
					else
						state->pc+=2;
					cycles = CC; break;
		//NOP
		case 0xdd: cycles = NOP; break;
		//SBI D8
		case 0xde:  sum =  state->a - opcode[1] - state->cc.cy;
					state->a = (uint8_t) (sum & 0xff);
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - (((opcode[1] + state->cc.cy) & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
					cycles = SBI; break;
		//RST 3
		case 0xdf:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0018;
					cycles = RST; break;
/*
-----------------------------------------------------------------------------------
*/ 	
		//RPO
		case 0xe0:  if(state->cc.p == 0){
					ret8080(state,opcode);
					}
					cycles = RPO; break;
		//POP H 
		case 0xe1:  state->l = state->memory[state->sp];
					state->h = state->memory[state->sp+1];
					state->sp += 2;
                    cycles = POP_H; break;
		//JPO adr
		case 0xe2:  if(state->cc.p == 0) {
						state->pc = regCombine(opcode[2]) | opcode[1];
					}
					else
						state->pc +=2;
					cycles = JPO; break;
		//XTHL
		case 0xe3:  templ =state->l;
					temph =state->h;
					state->l = state->memory[state->sp];
					state->h = state->memory[state->sp+1];
					state->memory[state->sp] = templ;
					state->memory[state->sp+1]= temph;
					cycles = XTHL; break;
		//CPO adr
		case 0xe4:  if(state->cc.p == 0){
						call8080(state,opcode);
					}
					else
						state->pc+=2;
					cycles = CPO; break;
		//PUSH H
		case 0xe5:  state->memory[state->sp-2] = state->l;
					state->memory[state->sp-1] = state->h;
					state->sp-=2;
					cycles = PUSH_H; break;
		//ANI D8
		case 0xe6:  sum =  state->a & opcode[1];
					state->a = (uint8_t) (sum & 0xff);
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) & ((opcode[1] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
					cycles = ANI; break;
	    //RST 4
		case 0xe7:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0020;
                    cycles = RST; break;
		//RPE
		case 0xe8:  if(state->cc.p == 1){
						ret8080(state,opcode);
					}
					cycles = RPE; break;
		//PCHL
		case 0xe9:  state->pc = regCombine(state->h) | state->l;
					cycles = PCHL; break;
		//JPE adr
		case 0xea:  if(state->cc.p ==1 ){
						state->pc = regCombine(opcode[2]) | opcode[1];
					}
					else
						state->pc += 2;
					cycles = JPE; break;
		//XCHG
		case 0xeb:  temph =state->h;
					templ =state->l;
					state->h = state->d;
					state->l = state->e;
					state->d = temph;
					state->e = templ;
					cycles = XCHG; break;
		//CPE adr
		case 0xec:  if(state->cc.p == 1) {
						call8080(state,opcode);
					}
					else
						state->pc += 2;
					cycles = CPE; break;
		//NOP
		case 0xed:  cycles = NOP; break;
		//XRI D8
		case 0xee:  sum =  state->a & opcode[1];
					state->a = (uint8_t) state->a;
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) ^ ((opcode[1] & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc += 1;
					cycles = XRI; break;
		//RST 5
		case 0xef:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0028;
					cycles = RST; break;
/*
-----------------------------------------------------------------------------------
*/ 		//RP
		case 0xf0:  if(state->cc.s == 0){
						ret8080(state, opcode);
					}
					cycles = RP; break;
		//POP PSW
		case 0xf1:  state->a = state->memory[state->sp+1];    
					psw =state->memory[state->sp];    
					state->cc.z  = (0x01 == (psw & 0x01));    
					state->cc.s  = (0x02 == (psw & 0x02));    
					state->cc.p  = (0x04 == (psw & 0x04));    
					state->cc.cy = (0x08 == (psw & 0x08));    
					state->cc.ac = (0x10 == (psw & 0x10));    
					state->sp += 2;    
         		    cycles = POP_PSW; break;    
		
		//JP adr
		case 0xf2:  if(state->cc.s == 0) {
						state->pc = regCombine(opcode[2]) | opcode[1];
					}
					else	
						state->pc+=2;
					cycles = JP; break;
		//DI
        case 0xf3:  state->int_enable = 0; 
                    cycles = DI; break;
		//CP adr
		case 0xf4:  if(state->cc.s == 0) {
						call8080(state,opcode);
					}
					else	
						state->pc+=2;
					cycles = CP; break;
	    //PUSH PSW  			
		case 0xf5:  state->memory[state->sp - 1] = state->a;
					psw =(0x00 | state->cc.z | state->cc.s << 1 | state->cc.p << 2 | state->cc.cy << 3 | state->cc.ac << 4 );    
					state->memory[state->sp-2] = psw;    
					state->sp -=2;
					cycles = PUSH_PSW; break; 
		//ORI D8
		case 0xf6:  sum =  (uint16_t) state->a | (uint16_t) opcode[1];
					state->a = (uint8_t) (sum & 0x00ff);
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (state->a == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) | ((opcode[1]  & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity(state->a);
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
					cycles = ORI; break;
		//RST 6
		case 0xf7:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0030;
					cycles = RST; break;
		//RM
		case 0xf8:  if(state->cc.s == 1) {
					ret8080(state,opcode);
					}
					else
					cycles = RM; break;
		//SPHL
		case 0xf9:  state->sp = regCombine(state->h) | state->l;
					cycles = SPHL; break;
		//JM adr
		case 0xfa:  if(state->cc.s == 1){
						state->pc = regCombine(opcode[2]) | opcode[1];
					}	
					else
						state->pc+=2;
					cycles = JM; break;
		//EI
        case 0xfb:  state->int_enable = 1;
                    cycles = EI; break;
		//CM adr
		case 0xfc:  if(state->cc.s == 1 ){
						call8080(state,opcode);
					}
					else
						state->pc+=2;
					cycles = CM; break;
		//NOP
		case 0xfd:  cycles = NOP; break;
		//CPI D8
		case 0xfe:	sum =  (uint16_t) state->a - (uint16_t) opcode[1];
					state->cc.s = ((uint8_t) sum >= 0x80 ? 1 : 0);
                    state->cc.z = (sum == 0x00 ? 1 : 0);
                    state->cc.ac = (state->a & 0x0f) - ((opcode[1]  & 0x0f)) >= 0x10 ? 1: 0;
                    state->cc.p = Parity((uint8_t)(sum & 0xff));
                    state->cc.cy = sum > 0x00ff ? 1 : 0;
					state->pc +=1;
					cycles = CPI; break;
		//RST 7 
		case 0xff:  ret = state->pc+2;
					state->memory[state->sp-1] = (ret>>8) &0xff;
					state->memory[state->sp-2] = (ret & 0xff);
					state->sp-=2;
					state->pc = 0x0038;
					cycles = RST; break;
        default: printf("Error: No matching op"); cycles = 0; break;
    }
	

    return cycles;
}

int call8080(State8080 * state, unsigned char * opcode){
    uint16_t ret = state->pc+2;
    state->memory[state->sp - 1] = (ret>>8) &0xff;
    state->memory[state->sp - 2] = (ret & 0xff);
    state->sp -=2;
    state->pc = regCombine(opcode[2]) | opcode[1];
    return 0;
}

int ret8080(State8080 * state, unsigned char * opcode) {
    state->pc = state->memory[state->sp] | (state->memory[state->sp+1] << 8);
    state->sp+=2;
    return 0;
}

uint8_t Parity(uint8_t byte) {

    uint8_t parity;
    uint8_t working;
    int ones = 0;
    for (int i = 0; i < 8; i++) {
        working = (byte & (0x1 << i));
        if (working != 0x00)
            ones++;

    }
    parity = !(ones % 2);
    return parity;

}

uint16_t regCombine(uint8_t reg) {
    uint16_t returnval = ((uint16_t) reg)<<8;
   
    return returnval;
    
}


void UnimplementedInstruction(State8080* state) {
    //printf("Error: Unimplemented Instruction - code 0x%02x\n", state->memory[state->pc - 1]);
    state->pc--;
    Emulate8080Op(state);
    //exit(1);
}

void interrupt(State8080* state, int inr_num) {
    //push pc
    uint16_t ret = state->pc;
    state->memory[state->sp - 1] = (uint8_t) (ret >> 8) & 0xff;
    state->memory[state->sp - 2] = (uint8_t) (ret & 0xff);
    state->sp -= 2;
    //jump to interrupt code
    state->pc = 8 * inr_num;
    state->int_enable = 0;

    return;
}

