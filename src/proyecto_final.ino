#include <LiquidCrystal_I2C.h>
#include <Servo.h>
# define PRENDIDO 2
# define BTN1 3
# define VELOCIDAD 250
int leds[] = {13,12,11,10,8}; //pines
int contLed = 0; //indice de leds
int tiempoVelocidad = VELOCIDAD;
int millisBtn1 = 0;
int millisServo = 0;
int estadoAnteriorBtn1 = LOW;
int direccion = 1; //toma 2 posible valores 1 y -1 para indicar el sentido de las luces
unsigned long tiempoAnterior = 0;
int intentosJuego1 = 2;
int ganasteJuego1 = false;
bool limpiar = false; //indica si debe limpiar la pantalla
char estado = 'A';

LiquidCrystal_I2C display(0x27, 16, 2); //instancio lcd

void setup(){
  Serial.begin(9600);
  
  //Leds
  for(int i = 0; i < 5; i ++){
      pinMode(leds[i], OUTPUT);
  }
  //Botones
  pinMode(PRENDIDO, INPUT); // prende y apaga el juego
  pinMode(BTN1, INPUT); // Boton para el juego 1
  
  todosApagados();
  
  //lcd
  display.init(); 
  display.backlight(); // Enciende la luz de fondo

}

void todosApagados(){
	for(int i = 0; i < 5; i ++){
      digitalWrite(leds[i], LOW);
  }
}

void controladorLeds(int contLed) {
  todosApagados();
  switch (contLed) {
    case 0:
      digitalWrite(leds[0], HIGH);
      break;
    case 1:
      digitalWrite(leds[1], HIGH);
      break;
    case 2:
      digitalWrite(leds[2], HIGH);
      break;
    case 3:
      digitalWrite(leds[3], HIGH);
      break;
    case 4:
      digitalWrite(leds[4], HIGH);
      break;
    default:
      todosApagados();
      break;
  }
}


void printLCDInit(){
  display.setCursor(0,0);
  display.print("Equipo 2");
}

void limpiarPantalla(bool &limpiar){
	if(limpiar){
		display.clear();
		limpiar = false;
		Serial.println("lIMPIAR!");
		Serial.println(limpiar);
	}
}


void loop(){

  //printLCDInit();
  Serial.println(estado);
  switch(estado){
	case 'A':
		if(digitalRead(PRENDIDO) == HIGH){
			estado = 'B';
            intentosJuego1 = 2;
			tiempoVelocidad = VELOCIDAD;
		}
		break;
		
	case 'B':
		limpiarPantalla(limpiar);
		
	  if(digitalRead(PRENDIDO) == LOW && intentosJuego1 >= -1){
		  estado = 'A';
	  }
	  if(digitalRead(PRENDIDO) == HIGH && intentosJuego1 >= -1){
		limpiar = true;
		intentosJuego1 = 2;
		display.setCursor(0,0);
		display.print("Juego 1");
		
		while(intentosJuego1 >= 0 && digitalRead(PRENDIDO) == HIGH){
			display.setCursor(0,1);
			display.print("N# intentos");
			display.setCursor(12,1);
			display.print(intentosJuego1 + 1);
			unsigned long tiempoActual = millis();
			
			if(tiempoActual - tiempoAnterior > tiempoVelocidad){
				contLed += direccion;
				tiempoAnterior = tiempoActual;
			}
			if(contLed >= sizeof(leds) / sizeof(leds[0])){
				direccion = -1;
			}
			if(contLed <= 0){
				direccion = 1;
			}
			
			int estadoBtn1 = digitalRead(BTN1);
			if(estadoBtn1 == HIGH && estadoAnteriorBtn1 ==LOW ){
				if(tiempoActual - millisBtn1 > 10){
					digitalWrite(leds[contLed], HIGH);
					if(leds[contLed] == 11){
						Serial.println("Ganaste!");
						display.clear();
						display.setCursor(3,1);
						display.print("!!YOU WIN!!");
						intentosJuego1 = -2;
					}else{
						Serial.println("fallaste!");
						intentosJuego1 --;
						tiempoVelocidad -= 50;
						display.setCursor(12,1);
						display.print(intentosJuego1 + 1);
					}
					delay(3000);
				}
			}else{
				estadoAnteriorBtn1 = estadoBtn1;
				millisBtn1 = tiempoActual;
			}
			controladorLeds(contLed);
		  }
		}
	  
		if(intentosJuego1 == -2){
			display.setCursor(0,1);
			display.print("Pasar a JUEGO 2");
			display.setCursor(9,0);
			display.print("GANASTE");
			estado = 'C';
			delay(3000);
		}
		if(intentosJuego1 == -1){
			display.setCursor(0,1);
			display.print("T.T");
			display.setCursor(0,0);
			display.print("PERDISTE");
			estado = 'X';
			delay(3000);
		}
		break;
	
	case 'C':
		// Juego 2
		break;
	
	case 'S':
		//servo
		break;
	case 'F':
		//final
		break;
	default:
		//errores o estados no previstos
		break;
	}
}