#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definir el pin donde está conectado el termistor
const int termistorPin = A0;
const int tempPot = A1;
const int ledPin = 13;
int tempObjetivo = 30;

// Valor de la resistencia a 25 grados Celsius del termistor (en ohms)
const float termistorNominal = 100000;

// Valor de la temperatura nominal del termistor (en grados Celsius)
const float temperaturaNominal = 25.0;

// Valor de la resistencia a 25 grados Celsius del termistor (en ohms)
const float betaCoefficiente = 3950;

// Valor de la resistencia del resistor serie conectado al termistor (en ohms)
const float resistorSerie = 10000;

// Inicializar con la dirección I2C 0x3C (para la pantalla 128x64 SSD1306)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(9600);  // Iniciar comunicación serial
  pinMode(ledPin, OUTPUT);

  // Iniciar la comunicación I2C
  Wire.begin();

  // Iniciar la pantalla OLED SSD1306 con la dirección I2C 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Limpiar la pantalla
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // Ajustar el tamaño del texto (valores válidos: 1, 2, 3)
  display.setTextSize(2);  // Tamaño 2: texto más grande
  
  display.setCursor(0, 0);
  display.display();  // Mostrar cambios en la pantalla OLED
}

void loop() {
  // Leer el valor del pin analógico A0
  int lectura = analogRead(termistorPin);

  // Convertir la lectura a resistencia
  float voltaje = lectura * 5.0 / 1023.0;
  float resistencia = resistorSerie / (5.0 / voltaje - 1.0);

  // Calcular temperatura en grados Celsius usando la fórmula de Steinhart-Hart
  float temperatura;
  temperatura = 1.0 / (log(resistencia / termistorNominal) / betaCoefficiente + 1.0 / (temperaturaNominal + 273.15)) - 273.15;

  //Analogic READ
  int tempControl = analogRead(tempPot);
  float tempObjetivo = map(tempControl , 0, 1023 , 0 , 300);

  Serial.print("Temperatura Objetivo : ");
  Serial.print (tempObjetivo);
  Serial.print(" // ");
  // Mostrar temperatura por el puerto serial
  Serial.print("Temperatura Actual : ");
  Serial.print(temperatura);
  Serial.println(" C");

  // Limpiar la pantalla antes de mostrar la nueva temperatura
  display.clearDisplay();

  // Mostrar temperatura en la pantalla OLED
  display.setCursor(0, 0);
  display.print("Filament ");
  display.drawLine(0,15,128,15, WHITE);
  display.setCursor(0, 20);  // Mover el cursor a la siguiente línea
  display.print("t :");
  display.setCursor(45 , 20);
  display.print(temperatura);
  display.setCursor(0 , 37);
  display.print("s :");
  display.setCursor(45 , 37);
  display.print(tempObjetivo);
  display.display();

  // Encender el LED si la temperatura supera los 28 grados Celsius
  if (temperatura < tempObjetivo ) {
    digitalWrite(ledPin, HIGH);
    //display.setCursor(0,37);
    //display.print("WARNING!");
    //display.display();
    
    Serial.println("Rele : ON");
    
  } else {
    digitalWrite(ledPin, LOW);
    //display.setCursor(0,35);
    //display.print("   ");
    //display.display();
    
    Serial.println("Rele : OFF");
    
  }

  // Esperar un poco antes de tomar otra lectura
  delay(1000);
}
