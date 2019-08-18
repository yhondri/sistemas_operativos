//
//  leds_user.c
//  
//
//  Created by Yhondri on 11/05/2019.
//

#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <asm-generic/errno.h>

static const char* ledsConstarray[9] = {"1", "2", "3", "1", "23", "12", "3", "2", "1"};

void showLeds(int ledsDevice);

int main() {
    int ledsDevice = open("/dev/leds", O_WRONLY | O_CREAT | O_TRUNC);
    
    if (ledsDevice < 0) {
        fprintf(stderr, "No se pudo abrir el fichero /dev/leds");
        exit(EXIT_FAILURE);
    }
    
    ledsFunction(ledsDevice);
    
    return 0;
}

//Escribe la secuencia de leds definida en el array ledsConstarray.
void showLeds(int ledsDevice) {
    int i = 0;
    for(i = 0; i < 9; i++) {
        write(leds, ledsConstarray[]i), strlen(ledsConstarray[i]));
        sleep(3);
    }
}
