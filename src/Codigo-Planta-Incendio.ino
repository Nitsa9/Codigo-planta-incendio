 //CODIGO PLANTA DE DETECCION INCENDIO 2024/06
 //V-2


//Librerias necesarias 
#include <Arduino.h>

//Sensor DHT11
#include <DHT.h>
#include <Adafruit_Sensor.h>

//I2C LCD:
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
  
// Configuracion sensor DHT11
#define DHTPIN 7     // Pin digital conectado al sensor DHT11
#define DHTTYPE DHT11   // Definicion tipo de sensor 
DHT dht(DHTPIN, DHTTYPE); //// Inicializar sensor DHT para funcionamiento normal a 16mh Arduino.   

// Configuración de pines
const int flamesensorPin = 8;   // Pin digital donde está conectado el sensor KY-026
const int gassensorpin = A1; // Pin analogico donde esta conectado el sensor de gas MQ-2
const int flamebuzzerPin = 5;   // Pin digital donde está conectado el buzzer (flama)
const int flameledPin = 3;     // Pin digital donde está conectado el LED (flama)
const int gasbuzzerPin = 5;   // Pin digital donde está conectado el buzzer (gas)
const int gasledPin = 4;     // Pin digital donde está conectado el LED (gas)
const int relayPin = 9; // Pin de control del relé
const int ledcontrol = 2; //Pin para LED indicador de buen estado
int h;  //Guarda el valor de humedad
int t; //Guarda el valor de temperatura

//Pines de la pantalla 
//  SDA --> A4 pines analogicos de la pantalla 
//  SCL --> A5 pines analogicos de la pantalla 

void setup() {
  // Configuración pines de los sensores, buzzer, LED, e inizializacion de los mismos.
  pinMode(flamesensorPin, INPUT);
  pinMode(gassensorpin,INPUT);
  pinMode(flamebuzzerPin, OUTPUT);
  pinMode(flameledPin, OUTPUT);
  pinMode(ledcontrol,OUTPUT);
  pinMode(gasbuzzerPin, OUTPUT);
  pinMode(gasledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
      Serial.begin(9600);
    Serial.println("Temperature and Humidity Sensor Test");
    dht.begin();
    lcd.init(); 
    lcd.backlight(); 
}

void loop() {
  digitalWrite(ledcontrol, HIGH);
    //Lectura y guarado de valores de temperatura y humedad
    h = dht.readHumidity();
    t = dht.readTemperature();
    
    // Mostrar valores de temperatura y humedad en la pantalla LCD

    lcd.setCursor(0, 0);
    lcd.println(" Now Temperature ");
    
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(t);
    lcd.print("C");

    lcd.setCursor(6, 1);
    lcd.println("UNAL ");
     
    lcd.setCursor(11, 1);
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
    
  delay(100); 

  // Acciones deteccion de llama 

  int flameDetected = digitalRead(flamesensorPin);

  if (flameDetected == HIGH) {

    digitalWrite(ledcontrol, LOW);
    digitalWrite(flameledPin, HIGH);
    tone(flamebuzzerPin, 2000);
    delay(3000);
    noTone(flamebuzzerPin);
    digitalWrite(flameledPin, LOW);
    digitalWrite(relayPin, HIGH);
    delay(5000); //Tiempo de apertura de la electrovalvula
    digitalWrite(relayPin, LOW);
  }

  //Acciones deteccion de gas

  int gasValue = analogRead(gassensorpin);

  if (gasValue > 250) {  // Ajustar este umbral según sea necesario
  
    digitalWrite(ledcontrol, LOW);
    tone(gasbuzzerPin, 2000);
    digitalWrite(gasledPin, HIGH);
    delay(3000);  
    noTone(gasbuzzerPin);
    digitalWrite(gasledPin, LOW);
  }

  delay(1000);  
  // Puedes continuar con otras acciones o lógica en el bucle principal si es necesario
}
