#include <pic16f887.h>
#pragma config FOSC = INTRC_NOCLKOUT  // Oscilador interno
#pragma config WDTE = OFF             // Watchdog desactivado
#pragma config PWRTE = OFF            // Power-up Timer off
#pragma config BOREN = ON             // Brown-out Reset on
#pragma config LVP = OFF              // Low Voltage Programming off

#define _XTAL_FREQ 4000000

typedef struct {
    unsigned char pin;
    unsigned char estado_ant;
} Boton;


void Boton_Init(Boton *b, unsigned char pin) {
    b->pin = pin;
    b->estado_ant = 1;
}

void main(void) {
    OSCCON = 0x60;
    TRISB = 0x03;     // Revisar pines
    TRISA = 0x00;     // Puerto A como salida
 
    // Crear botones
    Boton btn1, btn2;
    Boton_Init(&btn1, 0);
    Boton_Init(&btn2, 1);

}