#include "emuhead.h"

void MachineIN(State8080 * state, Shift * shiftreg, Ports * ports, uint8_t port){      
    uint16_t v;
    switch(port){    
        case 0x00:
            state->a = ports->read0;
            break;
        case 0x01: 
            state->a = ports->read1;
            break;
        case 0x03:   
            v = (shiftreg->H <<8) | shiftreg->L;    
            ports->read3 = ((v >> (8 - shiftreg->offset)) & 0xff);
            state->a = ports->read3;
            break;    
        }    
    return;  
}    

void MachineOUT(State8080 * state, Shift * shiftreg, Ports * ports, uint8_t port){    
    switch(port){    
        case 0x02:
            ports->write2 = state->a & 0x07;    
            shiftreg->offset = ports->write2;   
            break;    
        case 0x04:
            ports->write4 = state->a;    
            shiftreg->L = shiftreg->H;
            shiftreg->H = ports->write4;  
            break;    
}    

}    

int displayInit(SDL_Window** windowptr, SDL_Surface** screenSurfaceptr) {
    //SDL initialization

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
    else {
        *windowptr = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (*windowptr == NULL)
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        else {
            *screenSurfaceptr = SDL_GetWindowSurface(*windowptr);
        }
    }
   
    return 1;
}

int displayUpdate(State8080* state, SDL_Window** windowptr, SDL_Surface** screenSurfaceptr, SDL_Surface** SIDisplayptr) {
    uint8_t buffer[VRAM_L];
    for (int i = 0; i < VRAM_L; i++) {
        buffer[i] = bytereverse( *((state->memory) + VRAM + i) );

    }

    memcpy((*SIDisplayptr)->pixels, buffer, VRAM_L);
 
    SDL_BlitSurface(*SIDisplayptr, NULL, *screenSurfaceptr, NULL);
    SDL_UpdateWindowSurface(*windowptr);
    return 1;
}

uint8_t bytereverse(uint8_t byte) {
    uint8_t reversedbyte = 0x00;

    for (int i = 0; i < 8; i++) {
        reversedbyte |= (((byte & (0x01 << i))) != 0x00 ? 0x01 : 0x00) << (7 - i);
    }

    return reversedbyte;
}

int displayClose(SDL_Window ** windowptr, SDL_Surface ** screenSurfaceptr) {
    SDL_FreeSurface(*screenSurfaceptr);
    SDL_DestroyWindow(*windowptr);
    *windowptr = NULL;
    *screenSurfaceptr = NULL;
    SDL_Quit();
    return 1;
}

void SIKeyboardHandler(Ports* ports, SDL_Event* eptr) {
    if (eptr->type == SDL_KEYDOWN) {
        switch (eptr->key.keysym.sym) {
        case SDLK_LEFT:
            ports->read1 |= 0x20;
            printf("left down\n");
            break;

        case SDLK_RIGHT:
            ports->read1 |= 0x40;
            printf("right down\n");
            break;

        case SDLK_SPACE:
            ports->read1 |= 0x10;
            printf("space down\n");
            break;

        case SDLK_e:
            ports->read1 &= 0xfe;
            printf("E down\n");
            break;

        case SDLK_s:
            ports->read1 &= 0xFB;
            printf("S down\n");
            break;

        default: break;

        }
    }
    else if (eptr->type == SDL_KEYUP) {
        switch (eptr->key.keysym.sym) {
        case SDLK_LEFT:
            ports->read1 &= 0xdf;
            printf("left up\n");
            break;

        case SDLK_RIGHT:
            ports->read1 &= 0xbf;
            printf("right up\n");
            break;

        case SDLK_SPACE:
            ports->read1 &= 0xef;
            printf("space up\n");
            break;

        case SDLK_e:
            ports->read1 |= 0x01;
            printf("E up\n");
            break;

        case SDLK_s:
    
            ports->read1 |= 0x04;
            printf("S up\n");
            break;

        default: break;
        }
    }
    return;
} 