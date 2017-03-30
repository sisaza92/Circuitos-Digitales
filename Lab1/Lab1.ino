#include <TimerOne.h>

int PIN_5 = 5;
long DIT = 60000;
long DASH = 4 * DIT;
long LETTER_PAUSE = DASH;
long WORD_SPACE = 5 * DIT;
long SIMBOL = DIT;

boolean inicio = true;
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
  pinMode(PIN_5, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  inputString.reserve(100);

  attachInterrupt(1, apagado, RISING) ;
  attachInterrupt(0, prendido, FALLING) ;

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

  if (stringComplete) {
    char simbolo;
    morse_code = "";
    inputString.trim();
    Serial.println(inputString);
    int longitud = inputString.length();
    for (int i = 0; i <= longitud - 1; i++) {
      simbolo = inputString.charAt(i);
      Serial.println(i);
      Serial.println(simbolo);
      if (simbolo == ' ') {
        morse_code += '+';
        Serial.println(morse_code);
      } else if (morse_code.equals("") || inputString.charAt(i - 1) == ' ' ) {
        morse_code += traductorAMorse(simbolo);
        Serial.println(morse_code);
      } else {
        //morse_code.concat()
        morse_code += '*';
        Serial.println(morse_code);
        String simbolo_traducido = traductorAMorse(simbolo);
        morse_code += simbolo_traducido;
        Serial.println(morse_code);
      }

    }
    Serial.println("su codigo morce es: ");
    Serial.println(morse_code);
    emision(morse_code);
    Serial.println(morse_code_in);
    //Serial.println(traductor(inputString, 0, 1));
    inputString = "";
    stringComplete = false;
  }
}

void prendido() {
  if (inicio) {
    Timer1.restart();
    inicio = false;
  } else {
    time = Timer1.read();
    Timer1.restart();
    Serial.println("low :  ");
    //Serial.println(tipoSenal(time, false));
    morse_code_in = morse_code_in + tipoSenal(time, false);
    //digitalWrite(PIN_5,HIGH);
  }
}
void apagado() {
  if (inicio) {
    Timer1.restart();
  } else {
    //digitalWrite(PIN_5,LOW);
    time = Timer1.read();
    Timer1.restart();
    Serial.println("high :  ");
    //Serial.println(tipoSenal(time, true));
    morse_code_in = morse_code_in + tipoSenal(time, true);
  }

}



String tipoSenal(long tiempo, boolean tipo) {
  String senal;
  if (tipo) {
    if (tiempo < (DIT + 10000)) {
      senal = ".";//dit
    } else if (tiempo < (DASH + 10000)) {
      senal = "-";//dash
    }

  } else {
    if (tiempo < (SIMBOL + 10000)) {
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
      Serial.println(diccionario[i].morse);
      resultado = diccionario[i].morse;
      break;
    }
  }
  return resultado;
}

void emision(String mensaje) {
  char simbolo;
  int longitud = mensaje.length();
  for (int i = 0; i <= longitud; i++) {
    simbolo = mensaje.charAt(i);

    if (simbolo == ' ') {
      digitalWrite(7, LOW);
      delay(SIMBOL / 1000);
    } else if (simbolo == '+') {
      digitalWrite(7, LOW);
      delay(WORD_SPACE / 1000);
    } else if (simbolo == '*') {
      digitalWrite(7, LOW);
      delay(LETTER_PAUSE / 1000);
    } else if (simbolo == '-') {
      digitalWrite(7, HIGH);
      delay(DASH / 1000);
    } else if (simbolo == '.') {
      digitalWrite(7, HIGH);
      delay(DIT / 1000);
    }
  }
  digitalWrite(7, LOW);

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

