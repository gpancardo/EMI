#include <16F887.h> // Incluir definiciones y registros especiales del PIC16F887
#device ADC=8 // Configura la resolución del ADC a 8 bits
#use delay(internal=4MHz) // Establece el uso del oscilador interno a 4MHz para retardos
#fuses INTRC_IO, NOWDT, NOPROTECT, NOMCLR, NOBROWNOUT, NOLVP // Configura los fuses del microcontrolador (reloj interno, sin watchdog, etc.)

// Definición de pines para el LCD
#define LCD_RS PIN_C0 // Define el pin RS del LCD en RC0
#define LCD_RW PIN_C1 // Define el pin RW del LCD en RC1
#define LCD_EN PIN_C2 // Define el pin EN del LCD en RC2

// Definición de botones
#define BTN_PLANTA PIN_B0 // Define el pin del botón de planta en RB0
#define BTN_ANIMAL PIN_B1 // Define el pin del botón de animal en RB1

// Prototipos de funciones
void LCD_Init(); // Inicializa el LCD enviando los comandos necesarios
void LCD_Command(unsigned char cmd);//envia un comando al LCD
void LCD_Data(unsigned char data);// envia un dato al LCD
void LCD_String(const char* str); // permite usar cadenas constantes
void LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
void LCD_SetCursor(unsigned char row, unsigned char col); // Establece la posición del cursor en el LCD

// Variables del juego
char items[10][12] = { // Arreglo de cadenas que representa los nombres de los ítems (plantas y animales)
   "Rosa", "Perro", "Pino", "Gato", "Margarita",
   "Leon", "Helecho", "Pajaro", "Cactus", "Pez"
};

int8 item_types[10] = { // Arreglo que indica si cada ítem es planta (0) o animal (1)
   0, 1, 0, 1, 0,
   1, 0, 1, 0, 1
};

int8 score = 0; // Puntuación actual del jugador
int8 current_item = 0; // Índice del ítem actual que se está mostrando
int8 total_items = 10; // Total de ítems en el juego

void main() { // Función principal del programa
   // Configuración de puertos
   set_tris_d(0x00);  // Puerto D como salida para LCD // Configura el puerto D como salida (datos del LCD)
   set_tris_c(0x00);  // Pines de control como salida // Configura el puerto C como salida (control del LCD)
   set_tris_b(0x03);  // B0 y B1 como entrada para botones // Configura el puerto B (RB0 y RB1) como entradas (botones)

   // Habilitar pull-ups en los botones
   port_b_pullups(TRUE); // Activa las resistencias pull-up internas en PORTB

   // Inicializar LCD
   LCD_Init(); // Inicializa el LCD enviando los comandos necesarios

   // Mensaje de bienvenida
   LCD_String("  Clasifica!  "); // Muestra una cadena en el LCD
   LCD_SetCursor(1, 0); // Establece la posición del cursor en el LCD
   LCD_String("Planta o Animal"); // Muestra una cadena en el LCD
   delay_ms(2000); // Espera el tiempo especificado en milisegundos

   while(TRUE) {
      LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
      LCD_String(items[current_item]); // Muestra una cadena en el LCD

      while(TRUE) {
         if (!input(BTN_PLANTA)) {
            delay_ms(50); // Espera el tiempo especificado en milisegundos
            if (!input(BTN_PLANTA)) {
               if (item_types[current_item] == 0) {
                  score++;
                  LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
                  LCD_String("Correcto!"); // Muestra una cadena en el LCD
               } else {
                  LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
                  LCD_String("Incorrecto!"); // Muestra una cadena en el LCD
               }
               break;
            }
         }

         if (!input(BTN_ANIMAL)) {
            delay_ms(50); // Espera el tiempo especificado en milisegundos
            if (!input(BTN_ANIMAL)) {
               if (item_types[current_item] == 1) {
                  score++;
                  LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
                  LCD_String("Correcto!"); // Muestra una cadena en el LCD
               } else {
                  LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
                  LCD_String("Incorrecto!"); // Muestra una cadena en el LCD
               }
               break;
            }
         }
      }

      LCD_SetCursor(1, 0); // Establece la posición del cursor en el LCD
      LCD_String("Puntos: "); // Muestra una cadena en el LCD
      char score_str[5];
      sprintf(score_str, "%u", score);
      LCD_String(score_str); // Muestra una cadena en el LCD
      delay_ms(1500); // Espera el tiempo especificado en milisegundos

      current_item++;
      if (current_item >= total_items) {
         LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
         LCD_String("Juego terminado!"); // Muestra una cadena en el LCD
         LCD_SetCursor(1, 0); // Establece la posición del cursor en el LCD
         LCD_String("Puntos finales: "); // Muestra una cadena en el LCD
         LCD_String(score_str); // Muestra una cadena en el LCD

         while(TRUE) {
            if (!input(BTN_PLANTA) || !input(BTN_ANIMAL)) {
               delay_ms(50); // Espera el tiempo especificado en milisegundos
               if (!input(BTN_PLANTA) || !input(BTN_ANIMAL)) {
                  score = 0;
                  current_item = 0;
                  LCD_Clear(); // Llama a la función que limpia la pantalla del LCD
                  LCD_String("  Clasifica!  "); // Muestra una cadena en el LCD
                  LCD_SetCursor(1, 0); // Establece la posición del cursor en el LCD
                  LCD_String("Planta o Animal"); // Muestra una cadena en el LCD
                  delay_ms(1000); // Espera el tiempo especificado en milisegundos
                  break;
               }
            }
         }
      }
   }
}

void LCD_Init() { // Inicializa el LCD enviando los comandos necesarios
   delay_ms(20); // Espera el tiempo especificado en milisegundos
   LCD_Command(0x38);  // LCD 8-bit, 2 líneas // Configura el LCD en modo 8 bits, 2 líneas
   LCD_Command(0x0C);  // Display ON, cursor OFF // Enciende el display y oculta el cursor
   LCD_Command(0x06);  // Mover cursor a la derecha // Configura el LCD para auto-incrementar el cursor
   LCD_Command(0x01);  // Limpiar pantalla // Limpia la pantalla del LCD
   delay_ms(2); // Espera el tiempo especificado en milisegundos
}

void LCD_Command(unsigned char cmd) {
   output_low(LCD_RS); // Pone RS en bajo para indicar un comando
   output_low(LCD_RW); // Pone RW en bajo para indicar escritura
   output_d(cmd);              // Enviar comando por PORTD
   output_high(LCD_EN); // Activa el pin EN para habilitar el envío de datos/comando
   delay_us(1); // Espera el tiempo especificado en microsegundos
   output_low(LCD_EN); // Desactiva el pin EN después del pulso
   delay_ms(2); // Espera el tiempo especificado en milisegundos
}

void LCD_Data(unsigned char data) {
   output_high(LCD_RS); // Pone RS en alto para indicar un dato
   output_low(LCD_RW); // Pone RW en bajo para indicar escritura
   output_d(data);             // Enviar datos por PORTD
   output_high(LCD_EN); // Activa el pin EN para habilitar el envío de datos/comando
   delay_us(1); // Espera el tiempo especificado en microsegundos
   output_low(LCD_EN); // Desactiva el pin EN después del pulso
   delay_us(100); // Espera el tiempo especificado en microsegundos
}

void LCD_String(const char *str) //permite usar cadenas constantes
   while(*str) {
      LCD_Data(*str++);
   }
}

void LCD_Clear() { // Llama a la función que limpia la pantalla del LCD
   LCD_Command(0x01); // Limpia la pantalla del LCD
   delay_ms(2); // Espera el tiempo especificado en milisegundos
}

void LCD_SetCursor(unsigned char row, unsigned char col) { // Establece la posición del cursor en el LCD
   unsigned char address = (row == 0) ? (0x80 + col) : (0xC0 + col);
   LCD_Command(address);
}

