#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>
#include <time.h>

#ifndef EMUHEAD_H_   /* Include guard */
#define EMUHEAD_H_

#define SCREEN_WIDTH 256 //224
#define SCREEN_HEIGHT 224 //256
#define MEMORY_SIZE 0x10000
#define VRAM 0x2400
#define VRAM_L 7168
#define FALSE 0
#define TRUE 1
#define CYCLES_PER_FRAME 33330

typedef struct ConditionCodes {
    uint8_t    z:1;    
    uint8_t    s:1;    
    uint8_t    p:1;    
    uint8_t    cy:1;    
    uint8_t    ac:1;    
    uint8_t    pad:3;
} ConditionCodes;

typedef struct State8080 {    
    uint8_t    a;    
    uint8_t    b;    
    uint8_t    c;    
    uint8_t    d;    
    uint8_t    e;    
    uint8_t    h;    
    uint8_t    l;    
    uint16_t    sp;    
    uint16_t    pc;    
    uint8_t     *memory;    
    struct      ConditionCodes      cc;    
    uint8_t     int_enable;
   } State8080;    

//Space invaders shift registers
typedef struct Shift {
    uint8_t     H;
    uint8_t     L;
    uint8_t     offset;
} Shift;
//Space invaders ports
typedef struct Ports {
    uint8_t    read0;
    uint8_t    read1;
    uint8_t    read2;
    uint8_t    read3;
    uint8_t    write2;
    uint8_t    write3;
    uint8_t    write4;
    uint8_t    write5;
    uint8_t    write6;
} Ports;

const char* disassembler(uint8_t * opcode);
void UnimplementedInstruction(State8080* state);
int Emulate8080Op(State8080 *state);


uint16_t regCombine(uint8_t reg);
int call8080(State8080 * state, unsigned char * opcode);
int ret8080(State8080 * state, unsigned char * opcode);
uint8_t Parity(uint8_t byte);

void MachineIN(State8080 * state, Shift * shiftreg, Ports * ports, uint8_t port);
void MachineOUT(State8080 * state, Shift * shiftreg, Ports * ports, uint8_t port);
void interrupt(State8080* state, int inr_num);
int displayInit(SDL_Window** windowptr, SDL_Surface** screenSurfaceptr);
int displayUpdate(State8080* state, SDL_Window** windowptr, SDL_Surface** screenSurfaceptr, SDL_Surface** SIDisplayptr);
int displayClose(SDL_Window** windowptr, SDL_Surface** screenSurfaceptr);
void SIKeyboardHandler(Ports* ports, SDL_Event* eptr); 
uint8_t bytereverse(uint8_t byte);

void printstate(State8080* state);

//Clock cycle constants
#define MOV_R1R2    5
#define MOV_MR      7
#define MOV_RM      7
#define HLT         7
#define MVI_R       7
#define MVI_M       10
#define INR_R       5
#define DCR_R       5
#define INR_M       10
#define DCR_M       10
#define ADD_R       4
#define ADC_R       4
#define SUB_R       4
#define SBB_R       4

#define ANA_R       4
#define XRA_R       4
#define ORA_R       4
#define CMP_R       4
#define ADD_M       7
#define ADC_M       7
#define SUB_M       7
#define SBB_M       7

#define ANA_M       7
#define XRA_M       7
#define ORA_M       7
#define CMP_M       7
#define ADI         7
#define ACI         7

#define SUI         7
#define SBI         7

#define ANI         7
#define XRI         7

#define ORI         7
#define CPI         7
#define RLC         4
#define RRC         4
#define RAL         4
#define RAR         4

#define JMP         10
#define JC          10
#define JNC         10
#define JZ          10
#define JNZ         10
#define JP          10
#define JM          10
#define JPE         10
#define JPO         10
#define CALL        17
#define CC          11
#define CNC         11
#define CZ          11
#define CNZ         11
#define CP          11
#define CM          11
#define CPE         11
#define CPO         11
#define RET         10
#define RC          5
#define RNC         5

#define RZ          5
#define RNZ         5
#define RP          5
#define RM          5
#define RPE         5
#define RPO         5
#define RST         11
#define IN          10
#define OUT         10
#define LXI_B       10

#define LXI_D       10

#define LXI_H       10

#define LXI_SP      10
#define PUSH_B      11

#define PUSH_D      11

#define PUSH_H      11

#define PUSH_PSW    11

#define POP_B       10

#define POP_D       10

#define POP_H       10

#define POP_PSW     10

#define STA         13
#define LDA         13
#define XCHG        4

#define XTHL        18
#define SPHL        5
#define PCHL        5
#define DAD_B       10
#define DAD_D       10
#define DAD_H       10
#define DAD_SP      10
#define STAX_B      7
#define STAX_D      7
#define LDAX_B      7
#define LDAX_D      7
#define INX_B       5
#define INX_D       5
#define INX_H       5
#define INX_SP      5
#define DCX_B       5
#define DCX_D       5
#define DCX_H       5
#define DCX_SP      5
#define CMA         4
#define STC         4
#define CMC         4
#define DAA         4
#define SHLD        16
#define LHLD        16
#define EI          4
#define DI          4
#define NOP         4






#endif EMUHEAD_H_

