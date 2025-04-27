//https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/
// 16x2 LCD

//The advantage of using an I2C LCD is that the wiring is really simple. You just need to wire the SDA and SCL pins.


// Additionally, it comes with a built-in potentiometer you can use to adjust the contrast between the background and the characters on the LCD. On a “regular” LCD you need to add a potentiometer to the circuit to adjust the contrast. 

//Wire your LCD to the ESP32 by following the next schematic diagram. We’re using the ESP32 default I2C pins (GPIO 21 and GPIO 22).

//I2C LCD	    ESP32
//GND	        GND
//VCC	        VIN
//SDA	        GPIO 21
//SCL	        GPIO 22


//There are several libraries that work with the I2C LCD. We’re using this library by Marco Schwartz. Follow the next steps to install the library:
//
//Click here to download the LiquidCrystal_I2C library. You should have a .zip folder in your Downloads
//Unzip the .zip folder and you should get LiquidCrystal_I2C-master folder
//Rename your folder from LiquidCrystal_I2C-master to LiquidCrystal_I2C
//Move the LiquidCrystal_I2C folder to your Arduino IDE installation libraries folder
//Finally, re-open your Arduino IDE

//Getting the LCD Address
//Before displaying text on the LCD, you need to find the LCD I2C address. With the LCD properly wired to the ESP32, upload the following I2C Scanner sketch.

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/


/*
#include <Wire.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}
*/




/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, World!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print("Hello, World!");
  delay(1000);
  lcd.clear(); 
}





//SCROLLING TEXT

/*

  Rui Santos
  Complete project details at https://randomnerdtutorials.com  


#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

String messageStatic = "Static message";
String messageToScroll = "This is a scrolling message with more than 16 characters";

// Function to scroll text
// The function acepts the following arguments:
// row: row number where the text will be displayed
// message: message to scroll
// delayTime: delay between each character shifting
// lcdColumns: number of columns of your LCD
void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print static message
  lcd.print(messageStatic);
  // print scrolling message
  scrollText(1, messageToScroll, 250, lcdColumns);
}

*/



