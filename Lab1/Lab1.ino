#include <TimerOne.h>

//microsegundos
long DIT = 60000;
long DASH = 3 * DIT;
long LETTER_PAUSE = DASH;
long WORD_SPACE = 5 * DIT;
long SIMBOL = DIT;

boolean inicio = true;
boolean flanco_subida = true;
boolean lectura = false;
boolean bandera = false;
long time = 0;
String morse_code = "";
String morse_code_in = "";

String inputString = "";
boolean stringComplete = false;

struct par {
  char letra;
  String morse;
};

par diccionario[36] = {
  {'a', ". -"},
  {'b', "- . . ."},
  {'c', "- . - ."},
  {'d', "- . ."},
  {'e', "."},
  {'f', ". . - ."},
  {'g', "- - ."},
  {'h', ". . . ."},
  {'i', ". ."},
  {'j', ". - - -"},
  {'k', "- . -"},
  {'l', ". - . ."},
  {'m', "- -"},
  {'n', "- ."},
  {'o', "- - -"},
  {'p', ". - - ."},
  {'q', "- - . -"},
  {'r', ". - ."},
  {'s', ". . ."},
  {'t', "-"},
  {'u', ". . -"},
  {'v', ". . . -"},
  {'w', ". - -"},
  {'x', "- . . -"},
  {'y', "- . - -"},
  {'z', "- - . ."},
  {'1', ". - - - -"},
  {'2', ". . - - -"},
  {'3', ". . . - -"},
  {'4', ". . . . -"},
  {'5', ". . . . ."},
  {'6', "- . . . ."},
  {'7', "- - . . ."},
  {'8', "- - - . ."},
  {'9', "- - - - ."},
  {'0', "- - - - -"}
};
int x = 36;

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize();
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  inputString.reserve(100);

  // attachInterrupt(1, flanco_subida, RISING) ;
  //attachInterrupt(0, flanco_bajada, FALLING) ;
  attachInterrupt(1, control, CHANGE) ;
  attachInterrupt(0, funcion_lectura, CHANGE) ;

  //  digitalWrite(7,HIGH);
  //  delay(DIT/1000);
  //  digitalWrite(7,LOW);
  //  delay(SIMBOL/1000);
  //  digitalWrite(7,HIGH);
  //  delay(DIT/1000);
  //  digitalWrite(7,LOW);
  //  delay(SIMBOL/1000);
  //  digitalWrite(7,HIGH);
  //  delay(DIT/1000);
  //  digitalWrite(7,LOW);
  //  delay(SIMBOL/1000);
  //  digitalWrite(7,HIGH);
  //  delay(DIT/1000);
  //  digitalWrite(7,LOW);
  //  delay(LETTER_PAUSE/1000);
  //  digitalWrite(7,HIGH);
  //  delay(DASH/1000);
  //  digitalWrite(7,LOW);
  //  delay(SIMBOL/1000);
  //  digitalWrite(7,HIGH);
  //  delay(DASH/1000);
  //  digitalWrite(7,LOW);
  //  delay(SIMBOL/1000);
  //  digitalWrite(7,HIGH);
  //  delay(DASH/1000);
  //  digitalWrite(7,LOW);
  //  delay(SIMBOL/1000);
  //  digitalWrite(7,HIGH);
  //  delay(LETTER_PAUSE/1000);
  //digitalWrite(7,LOW);



}

void loop() {
    // put your main code here, to run repeatedly:
//if(bandera){
    if (stringComplete) {
      char simbolo;
      morse_code = "";
      inputString.trim();
      Serial.println(inputString);
      int longitud = inputString.length();
      for (int i = 0; i <= longitud - 1; i++) {
        simbolo = inputString.charAt(i);
        //Serial.println(i);
        //Serial.println(simbolo);
        if (simbolo == ' ') {
          morse_code += '+';
          //Serial.println(morse_code);
        } else if (morse_code.equals("") || inputString.charAt(i - 1) == ' ' ) {
          morse_code += traductorAMorse(simbolo);
          //Serial.println(morse_code);
        } else {
          //morse_code.concat()
          morse_code += '*';
          //Serial.println(morse_code);
          String simbolo_traducido = traductorAMorse(simbolo);
          morse_code += simbolo_traducido;
          //Serial.println(morse_code);
        }

      }
      Serial.println("su codigo morce es: ");
      Serial.println(morse_code_in);
      morse_code += "+";
      //morse_code += morse_code_in;
      Serial.println(morse_code);
      digitalWrite(8,HIGH);
      emision(morse_code);
      digitalWrite(8,LOW);
      //Serial.println(traductor(inputString, 0, 1));
      inputString = "";
      stringComplete = false;
    }
    bandera = false;
//}
}

void control() {
  //Serial.print("lect");
  //Serial.println(lectura);
  if (!lectura) {
    lectura = true;
    Serial.println("entré");
  }
  else {
    bandera = true;
  }
}

void funcion_lectura() {
  //Serial.print("flanco");
  //Serial.println(flanco_subida);
  if (lectura) {
    if (!flanco_subida) {
      if (inicio) {
        Timer1.restart();
      } else {
        //digitalWrite(PIN_5,LOW);
        time = Timer1.read();
        Timer1.restart();
        Serial.print("fb :  ");
        Serial.print(time);
        Serial.println(tipoSenal(time, true));
        morse_code_in  += tipoSenal(time, true);
      }
      flanco_subida = true;
    } else {
      if (inicio) {
        Timer1.restart();
        inicio = false;
      } else {
        time = Timer1.read();
        Timer1.restart();
        Serial.print("fs :  ");
        Serial.print(tipoSenal(time, false));
        Serial.println(time);
        morse_code_in += tipoSenal(time, false);
        //digitalWrite(PIN_5,HIGH);
      }
      flanco_subida = false;
    }
    
    
  }
}



String tipoSenal(long tiempo, boolean tipo) {
  String senal;
  if (tipo) {
    if (tiempo < (DIT - 10000)) {
      senal = "";
    } else if (tiempo < (DIT + 10000)) {
      senal = ".";//dit
    } else if (tiempo < (DASH + 10000)) {
      senal = "-";//dash
    }

  } else {
    if (tiempo < (DIT - 10000)) {
      senal = "";
    } else if (tiempo < (SIMBOL + 10000)) {
      senal = " ";//simbol
    } else if (tiempo < (LETTER_PAUSE + 10000)) {
      senal = "*";//letter_pause
    } else if (tiempo < (WORD_SPACE + 10000)) {
      senal = "+";//word_space
    }
  }
  return senal;
}




String traductorAMorse(char letra) {
  String resultado;
  for (int i = 0; i < x - 1; i++)
  {

    if (letra == diccionario[i].letra) {
      //Serial.println(diccionario[i].morse);
      resultado = diccionario[i].morse;
      break;
    }
  }
  return resultado;
}

void emision(String mensaje) {
  Serial.println(mensaje);
  char simbolo;
  int longitud = mensaje.length();
  
  for (int i = 0; i <= longitud; i++) {
    simbolo = mensaje.charAt(i);
    //Serial.println(i);
    Serial.println(simbolo);
    if (simbolo == ' ') {
      digitalWrite(7, LOW);
      //delay(1000);
      delay(SIMBOL / 1000);
    } else if (simbolo == '+') {
      digitalWrite(7, LOW);
      //delay(1000);
      delay(WORD_SPACE / 1000);
    } else if (simbolo == '*') {
      digitalWrite(7, LOW);
      //delay(1000);
      delay(LETTER_PAUSE / 1000);
    } else if (simbolo == '-') {
      digitalWrite(7, HIGH);
      //delay(1000);
      delay(DASH / 1000);
    } else if (simbolo == '.') {
      digitalWrite(7, HIGH);
      //delay(1000);
      delay(DIT / 1000);
    }
  }
  digitalWrite(7, LOW);
  delay(DIT / 1000);
  Serial.println(DIT/1000);

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

