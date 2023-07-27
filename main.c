/*
Space invaders emulator made using an online tutorial. Sources: emulator101.com and Lazy Foo' Productions' SDL tutorial

*/
#include "emuhead.h"

int main(int argc, char* args[]) {

    /*
    
    
    Initialization
    
    
    
    */

    FILE *filepointer = NULL; //pointer to space invaders rom
    filepointer = fopen("invaders", "r"); //assign pointer

    printf("Program starting...\n");
    //Emulator vars
    State8080 currentState; //initialize processor data
    State8080 * state = &currentState; //ptr to processor data
    state->a = 0; state->b = 0; state->c = 0; state->d = 0; state->e = 0; state->h = 0; state->l = 0; state->pc = 0;
    state->cc.z = 0; state->cc.s = 0; state->cc.p = 0; state->cc.cy = 0; state->cc.ac = 0;
    state->int_enable = 0;
    int clock = 0;
    int cycles = 0;
    int sleeptime = 0;
    int starttime = 0;
    Ports currentPorts; //intialize ports
    currentPorts = (Ports){
        .read0 = 0x0F,
        .read1 = 0x09,
        .read2 = 0x0F,
    };
    Ports * ports = &currentPorts; //ptr to ports
    Shift currentShift; //initialize shift reg
    Shift * shift = &currentShift; //ptr to shift reg
    int filelen; //will contain length of space invaders file
    //Display vars
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Window** windowptr = &window;
    SDL_Surface** screenSurfaceptr = &screenSurface;
    SDL_Surface* SIDisplay = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, 
                SCREEN_WIDTH, SCREEN_HEIGHT, 1, SDL_PIXELFORMAT_INDEX1MSB);
    SDL_Color monochrome[2] = { {0, 0, 0, 255}, {255, 255, 255, 255} };
    SDL_SetPaletteColors(SIDisplay->format->palette, monochrome, 0, 2);
    SDL_Surface** SIDisplayptr = &SIDisplay;

    //Input variabls
    SDL_Event e;
    int quit = FALSE;
    
    
    /*


    File I/O



    */
    //Getting file size
    fseek(filepointer, 0 ,SEEK_END);    
    filelen = ftell(filepointer);
    fseek(filepointer, 0 ,SEEK_SET);
    char * buffer = (char*) malloc(MEMORY_SIZE);
    for (int i = 0; i < MEMORY_SIZE; i++)
        buffer[i] = 0x00;


    //Read the code into a buffer
    if (fread(buffer, filelen, 1, filepointer) < 0)
        exit(1);


    //2. Get pointer to start of buffer

    state->memory = buffer;
    state->pc=0x0000;
    unsigned char *opcode = &state->memory[state->pc];

    //TESTING
    int instructioncount = 0;
    State8080 currenttestState;
    State8080* teststate = &currenttestState;
    teststate->int_enable = 0;
    char* testbuffer = (char*)malloc(MEMORY_SIZE);
    memcpy(testbuffer, buffer, MEMORY_SIZE);
    teststate->memory = testbuffer;
    teststate->pc = 0x0000;
    teststate->a = 0; teststate->b = 0; teststate->c = 0; teststate->d = 0; teststate->e = 0; teststate->h = 0; teststate->l = 0; teststate->pc = 0;
    teststate->cc.z = 0; teststate->cc.s = 0; teststate->cc.p = 0; teststate->cc.cy = 0; teststate->cc.ac = 0;  
    Ports testcurrentPorts; //intialize ports
    testcurrentPorts = (Ports){
        .read0 = 0x0F,
        .read1 = 0x09,
        .read2 = 0x0F,
    };
    Ports* testports = &currentPorts; //ptr to ports
    Shift testcurrentShift; //initialize shift reg
    Shift* testshift = &currentShift; //ptr to shift reg
    uint16_t testlastsp = 0;
    uint16_t testlastpc = 0;
    uint16_t lastsp = 0;
    uint16_t lastpc = 0;

    //Display 
    displayInit(windowptr, screenSurfaceptr);

    /*
    

    Main loop


    */

    while (!quit/* condition for disassembly: state->pc < filelen */) {
        //Keyboard
        if (SDL_PollEvent(&e) > 0) {
            switch (e.type) {

            case SDL_QUIT: quit = TRUE; SDL_Delay(1000); break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                SIKeyboardHandler(ports, &e);
                break;


            default: break;
            }

        }
        //Interrupts
        if (clock - starttime >= CYCLES_PER_FRAME) {
            if (state->int_enable) {
                interrupt(state, 2);
                GenerateInterrupt(teststate, 2);
                displayUpdate(state, windowptr, screenSurfaceptr, SIDisplayptr);
                //printf("Interrupt generated at %d instructions or at %d cycles after %d cycles\n", instructioncount, clock, clock-starttime);
            }
            starttime = clock;

        }
     //   if(state->memory[0x21ff] !=0xcd ) printf("21ff value is %x\n", state->memory[0x21ff]);
        //get the next instruction
        opcode = &state->memory[state->pc];
        if (state->sp < -0x22ff) {
            printf("Stack too low\n");
        }
        if (state->pc == 0x0100)         {
            printf("reached x at %d instructions\n", instructioncount);
        }
        //machine specific handling for IN    
        if (*opcode == 0xdb) {
            MachineIN(state, shift, ports, opcode[1]);
            MachineIN(teststate, testshift, testports, opcode[1]);

        }
        //machine specific handling for OUT
        else if (*opcode == 0xd3) {
            MachineOUT(state, shift, ports, opcode[1]);
            MachineOUT(teststate, testshift, testports, opcode[1]);

        }

       // printf("0x%04x    ", state->pc);
       // disassembler(&(state->memory[state->pc]));
       // comparestate(state, teststate, lastpc, testlastpc, lastsp, testlastsp);
        lastsp = state->sp;
        lastpc = state->pc;
        testlastsp = teststate->sp;
        testlastpc = teststate->pc;

        cycles = Emulate8080Op(state);

        Emulate8080Optest(teststate);
        instructioncount++;
       /* if (instructioncount % 1 == 0 && instructioncount >= 2315494 - 100) {
            printf("%d\n", instructioncount);
            printf("0x%04x    ", state->pc);
            disassembler(&(state->memory[state->pc]));
            printstate(state);
            if(instructioncount % 100 == 0) 
                printf("\n");
        }
     */
        //printf("0x%04x    ", state->pc);
        //disassembler(&(state->memory[state->pc]));
        
       comparestate(state, teststate, lastpc, testlastpc, lastsp, testlastsp);

        //Timing
        clock += cycles;
        if (clock - sleeptime >= 2500) {
            Sleep(1);
            sleeptime = clock;
        }
       
  

      
       
     }
    /*
    
    
    Exit sequence
    
    
    */

    displayClose(windowptr, screenSurfaceptr);
    fclose(filepointer);
 
    filepointer = NULL;
    return 0;
}