// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
// https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/#more-85752


const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(500);
}