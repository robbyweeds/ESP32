https://randomnerdtutorials.com/micropython-i2c-lcd-esp32-esp8266/#more-161231

Prerequisites
To follow this tutorial you need MicroPython firmware installed in your ESP32 or ESP8266 boards. You also need an IDE to write and upload the code to your board. We suggest using Thonny IDE or uPyCraft IDE:
Thonny IDE:
Installing and getting started with Thonny IDE
Flashing MicroPython Firmware with esptool.py
uPyCraft IDE:
Getting Started with uPyCraft IDE
Install uPyCraft IDE (Windows, Mac OS X, Linux)
Flash/Upload MicroPython Firmware to ESP32 and ESP8266
Learn more about MicroPython: MicroPython Programming with ESP32 and ESP8266

Parts Required
To follow this tutorial you need the following parts:

I2C LCD Display
ESP32 or ESP8266
Breadboard (optional)
Jumper wires (optional)

You can use the preceding links or go directly to MakerAdvisor.com/tools to find all the parts for your projects at the best price!



Introducing the LCD
One of the simplest and cheapest display screens is the liquid crystal display (LCD). LCDs are found in everyday electronic devices like vending machines, calculators, parking meters, and printers, and are ideal for displaying text or small icons.

LCD Display with I2C interface

LCDs are measured according to the number of rows and col­umns of characters that fit on the screen. A 16×2 LCD can display 2 rows of 16 characters each. You’ll find sizes ranging from 8×1 to 40×4.

One of the easiest ways to control an LCD with a microcontroller is using one that comes with an I2C interface. We’ll use a 16×2 LCD that uses I2C communication.

To follow this tutorial, you should get one that also uses I2C. As for the size, you can choose whatever size you want, and it should be compatible.

LCD with I2C Driver
The advantage of using an I2C LCD is that the wiring is really simple. You just need to wire the SDA and SCL pins.

LCD Display with I2C Driver

Additionally, it comes with a built-in potentiometer to adjust the contrast between the background and the characters on the LCD. On a “regular” LCD without I2C support, you need to add a potentiometer to the circuit to adjust the contrast.

Wiring the I2C LCD to the ESP32/ESP8266
Because this LCD module supports I2C communication, wiring is very straightforward. We’ll connect the I2C pins to the ESP32 and ESP8266 default I2C pins. The VCC pin of the LCD needs to be connected to 5V.

LCD Display	ESP32	    ESP8266
SCL	        GPIO 22	    GPIO 5
SDA	        GPIO 21	    GPIO 4
VCC	        VIN (5V)	VIN (5V)
GND	        GND	        GND




Circuit Diagram
You can also use the following diagrams as a reference.

ESP32
https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2024/07/esp32-i2c-lcd-wiring_bb.png?quality=100&strip=all&ssl=1
ESP32 Wiring to I2C LCD


ESP8266 NodeMCU
https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2024/07/esp8266-i2c-lcd-wiring_bb.png?quality=100&strip=all&ssl=1

Finding the LCD I2C Address
Most I2C LCDs will have the I2C address 0x27. However, yours might be different. So it’s important to check the I2C address before continuing. So, consult the sensor datasheet or run an I2C scanner sketch.

LCD Libraries – MicroPython
There are different libraries that make it easy to communicate with the LCD. We’ll use a combination of two different modules developed by dhylands GitHub user.

Follow the next steps to install the two required modules.

Download and Upload the lcd_api.py
Click here to download the lcd_api.py code;
Copy the code to a file on Thonny IDE;
Go to File > Save as… and select MicroPython Device;
Save the file with the name lcd_api.py (don’t change the name).
# Provides an API for talking to HD44780 compatible character LCDs.
# https://github.com/dhylands/python_lcd/tree/master/lcd
import time

class LcdApi:
    """Implements the API for talking with HD44780 compatible character LCDs.
    This class only knows what commands to send to the LCD, and not how to get
    them to the LCD.

    It is expected that a derived class will implement the hal_xxx functions.
    """

    # The following constant names were lifted from the avrlib lcd.h
    # header file, however, I changed the definitions from bit numbers
    # to bit masks.
    #
    # HD44780 LCD controller command set

    LCD_CLR = 0x01              # DB0: clear display
    LCD_HOME = 0x02             # DB1: return to home position

    LCD_ENTRY_MODE = 0x04       # DB2: set entry mode
    LCD_ENTRY_INC = 0x02        # --DB1: increment
    LCD_ENTRY_SHIFT = 0x01      # --DB0: shift

    LCD_ON_CTRL = 0x08          # DB3: turn lcd/cursor on
    LCD_ON_DISPLAY = 0x04       # --DB2: turn display on
    LCD_ON_CURSOR = 0x02        # --DB1: turn cursor on
    LCD_ON_BLINK = 0x01         # --DB0: blinking cursor

    LCD_MOVE = 0x10             # DB4: move cursor/display
    LCD_MOVE_DISP = 0x08        # --DB3: move display (0-> move cursor)
    LCD_MOVE_RIGHT = 0x04       # --DB2: move right (0-> left)

    LCD_FUNCTION = 0x20         # DB5: function set
    LCD_FUNCTION_8BIT = 0x10    # --DB4: set 8BIT mode (0->4BIT mode)
    LCD_FUNCTION_2LINES = 0x08  # --DB3: two lines (0->one line)
    LCD_FUNCTION_10DOTS = 0x04  # --DB2: 5x10 font (0->5x7 font)
    LCD_FUNCTION_RESET = 0x30   # See "Initializing by Instruction" section

    LCD_CGRAM = 0x40            # DB6: set CG RAM address
    LCD_DDRAM = 0x80            # DB7: set DD RAM address

    LCD_RS_CMD = 0
    LCD_RS_DATA = 1

    LCD_RW_WRITE = 0
    LCD_RW_READ = 1

    def __init__(self, num_lines, num_columns):
        self.num_lines = num_lines
        if self.num_lines > 4:
            self.num_lines = 4
        self.num_columns = num_columns
        if self.num_columns > 40:
            self.num_columns = 40
        self.cursor_x = 0
        self.cursor_y = 0
        self.implied_newline = False
        self.backlight = True
        self.display_off()
        self.backlight_on()
        self.clear()
        self.hal_write_command(self.LCD_ENTRY_MODE | self.LCD_ENTRY_INC)
        self.hide_cursor()
        self.display_on()

    def clear(self):
        """Clears the LCD display and moves the cursor to the top left
        corner.
        """
        self.hal_write_command(self.LCD_CLR)
        self.hal_write_command(self.LCD_HOME)
        self.cursor_x = 0
        self.cursor_y = 0

    def show_cursor(self):
        """Causes the cursor to be made visible."""
        self.hal_write_command(self.LCD_ON_CTRL | self.LCD_ON_DISPLAY |
                               self.LCD_ON_CURSOR)

    def hide_cursor(self):
        """Causes the cursor to be hidden."""
        self.hal_write_command(self.LCD_ON_CTRL | self.LCD_ON_DISPLAY)

    def blink_cursor_on(self):
        """Turns on the cursor, and makes it blink."""
        self.hal_write_command(self.LCD_ON_CTRL | self.LCD_ON_DISPLAY |
                               self.LCD_ON_CURSOR | self.LCD_ON_BLINK)

    def blink_cursor_off(self):
        """Turns on the cursor, and makes it no blink (i.e. be solid)."""
        self.hal_write_command(self.LCD_ON_CTRL | self.LCD_ON_DISPLAY |
                               self.LCD_ON_CURSOR)

    def display_on(self):
        """Turns on (i.e. unblanks) the LCD."""
        self.hal_write_command(self.LCD_ON_CTRL | self.LCD_ON_DISPLAY)

    def display_off(self):
        """Turns off (i.e. blanks) the LCD."""
        self.hal_write_command(self.LCD_ON_CTRL)

    def backlight_on(self):
        """Turns the backlight on.

        This isn't really an LCD command, but some modules have backlight
        controls, so this allows the hal to pass through the command.
        """
        self.backlight = True
        self.hal_backlight_on()

    def backlight_off(self):
        """Turns the backlight off.

        This isn't really an LCD command, but some modules have backlight
        controls, so this allows the hal to pass through the command.
        """
        self.backlight = False
        self.hal_backlight_off()

    def move_to(self, cursor_x, cursor_y):
        """Moves the cursor position to the indicated position. The cursor
        position is zero based (i.e. cursor_x == 0 indicates first column).
        """
        self.cursor_x = cursor_x
        self.cursor_y = cursor_y
        addr = cursor_x & 0x3f
        if cursor_y & 1:
            addr += 0x40    # Lines 1 & 3 add 0x40
        if cursor_y & 2:    # Lines 2 & 3 add number of columns
            addr += self.num_columns
        self.hal_write_command(self.LCD_DDRAM | addr)

    def putchar(self, char):
        """Writes the indicated character to the LCD at the current cursor
        position, and advances the cursor by one position.
        """
        if char == '\n':
            if self.implied_newline:
                # self.implied_newline means we advanced due to a wraparound,
                # so if we get a newline right after that we ignore it.
                self.implied_newline = False
            else:
                self.cursor_x = self.num_columns
        else:
            self.hal_write_data(ord(char))
            self.cursor_x += 1
        if self.cursor_x >= self.num_columns:
            self.cursor_x = 0
            self.cursor_y += 1
            self.implied_newline = (char != '\n')
        if self.cursor_y >= self.num_lines:
            self.cursor_y = 0
        self.move_to(self.cursor_x, self.cursor_y)

    def putstr(self, string):
        """Write the indicated string to the LCD at the current cursor
        position and advances the cursor position appropriately.
        """
        for char in string:
            self.putchar(char)

    def custom_char(self, location, charmap):
        """Write a character to one of the 8 CGRAM locations, available
        as chr(0) through chr(7).
        """
        location &= 0x7
        self.hal_write_command(self.LCD_CGRAM | (location << 3))
        self.hal_sleep_us(40)
        for i in range(8):
            self.hal_write_data(charmap[i])
            self.hal_sleep_us(40)
        self.move_to(self.cursor_x, self.cursor_y)

    def hal_backlight_on(self):
        """Allows the hal layer to turn the backlight on.

        If desired, a derived HAL class will implement this function.
        """
        pass

    def hal_backlight_off(self):
        """Allows the hal layer to turn the backlight off.

        If desired, a derived HAL class will implement this function.
        """
        pass

    def hal_write_command(self, cmd):
        """Write a command to the LCD.

        It is expected that a derived HAL class will implement this
        function.
        """
        raise NotImplementedError

    def hal_write_data(self, data):
        """Write data to the LCD.

        It is expected that a derived HAL class will implement this
        function.
        """
        raise NotImplementedError

    # This is a default implementation of hal_sleep_us which is suitable
    # for most micropython implementations. For platforms which don't
    # support `time.sleep_us()` they should provide their own implementation
    # of hal_sleep_us in their hal layer and it will be used instead.
    def hal_sleep_us(self, usecs):
        """Sleep for some time (given in microseconds)."""
        time.sleep_us(usecs)  # NOTE this is not part of Standard Python library, specific hal layers will need to override this

View raw code

Download and Upload the machine_i2c_lcd.py
Click here to download the machine_i2c_lcd.py code;
Copy the code to a file on Thonny IDE;
Go to File > Save as… and select MicroPython Device;
Save the file with the name machine_i2c_lcd.py (don’t change the name).
# Implements a HD44780 character LCD connected via PCF8574 on I2C.
# This was tested with: https://www.wemos.cc/product/d1-mini.html
# https://github.com/dhylands/python_lcd/blob/master/lcd/machine_i2c_lcd.py

from lcd_api import LcdApi
from time import sleep_ms

# The PCF8574 has a jumper selectable address: 0x20 - 0x27
DEFAULT_I2C_ADDR = 0x27

# Defines shifts or masks for the various LCD line attached to the PCF8574

MASK_RS = 0x01
MASK_RW = 0x02
MASK_E = 0x04
SHIFT_BACKLIGHT = 3
SHIFT_DATA = 4


class I2cLcd(LcdApi):
    """Implements a HD44780 character LCD connected via PCF8574 on I2C."""

    def __init__(self, i2c, i2c_addr, num_lines, num_columns):
        self.i2c = i2c
        self.i2c_addr = i2c_addr
        self.i2c.writeto(self.i2c_addr, bytearray([0]))
        sleep_ms(20)   # Allow LCD time to powerup
        # Send reset 3 times
        self.hal_write_init_nibble(self.LCD_FUNCTION_RESET)
        sleep_ms(5)    # need to delay at least 4.1 msec
        self.hal_write_init_nibble(self.LCD_FUNCTION_RESET)
        sleep_ms(1)
        self.hal_write_init_nibble(self.LCD_FUNCTION_RESET)
        sleep_ms(1)
        # Put LCD into 4 bit mode
        self.hal_write_init_nibble(self.LCD_FUNCTION)
        sleep_ms(1)
        LcdApi.__init__(self, num_lines, num_columns)
        cmd = self.LCD_FUNCTION
        if num_lines > 1:
            cmd |= self.LCD_FUNCTION_2LINES
        self.hal_write_command(cmd)

    def hal_write_init_nibble(self, nibble):
        """Writes an initialization nibble to the LCD.

        This particular function is only used during initialization.
        """
        byte = ((nibble >> 4) & 0x0f) << SHIFT_DATA
        self.i2c.writeto(self.i2c_addr, bytearray([byte | MASK_E]))
        self.i2c.writeto(self.i2c_addr, bytearray([byte]))

    def hal_backlight_on(self):
        """Allows the hal layer to turn the backlight on."""
        self.i2c.writeto(self.i2c_addr, bytearray([1 << SHIFT_BACKLIGHT]))

    def hal_backlight_off(self):
        """Allows the hal layer to turn the backlight off."""
        self.i2c.writeto(self.i2c_addr, bytearray([0]))

    def hal_write_command(self, cmd):
        """Writes a command to the LCD.

        Data is latched on the falling edge of E.
        """
        byte = ((self.backlight << SHIFT_BACKLIGHT) | (((cmd >> 4) & 0x0f) << SHIFT_DATA))
        self.i2c.writeto(self.i2c_addr, bytearray([byte | MASK_E]))
        self.i2c.writeto(self.i2c_addr, bytearray([byte]))
        byte = ((self.backlight << SHIFT_BACKLIGHT) | ((cmd & 0x0f) << SHIFT_DATA))
        self.i2c.writeto(self.i2c_addr, bytearray([byte | MASK_E]))
        self.i2c.writeto(self.i2c_addr, bytearray([byte]))
        if cmd <= 3:
            # The home and clear commands require a worst case delay of 4.1 msec
            sleep_ms(5)

    def hal_write_data(self, data):
        """Write data to the LCD."""
        byte = (MASK_RS | (self.backlight << SHIFT_BACKLIGHT) | (((data >> 4) & 0x0f) << SHIFT_DATA))
        self.i2c.writeto(self.i2c_addr, bytearray([byte | MASK_E]))
        self.i2c.writeto(self.i2c_addr, bytearray([byte]))
        byte = (MASK_RS | (self.backlight << SHIFT_BACKLIGHT) | ((data & 0x0f) << SHIFT_DATA))
        self.i2c.writeto(self.i2c_addr, bytearray([byte | MASK_E]))
        self.i2c.writeto(self.i2c_addr, bytearray([byte]))
View raw code

With the modules loaded to the boards, now you can use the library functionalities in your code to write text to the LCD.

Displaying Static Text – Code
Displaying static text on the LCD is very simple. All you have to do is select where you want the characters to be displayed on the screen and then send the message to the display. If you don’t specify where you want to display the text, it will be written in the first available space.

This example displays the “Hello, World!” message in the first row, and then on the second row. It is compatible with the ESP32 and ESP8266, just make sure you uncomment the pin definition for the ESP8266.

# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-i2c-lcd-esp32-esp8266/

from machine import Pin, SoftI2C
from machine_i2c_lcd import I2cLcd
from time import sleep

# Define the LCD I2C address and dimensions
I2C_ADDR = 0x27
I2C_NUM_ROWS = 2
I2C_NUM_COLS = 16

# Initialize I2C and LCD objects
i2c = SoftI2C(sda=Pin(21), scl=Pin(22), freq=400000)

# for ESP8266, uncomment the following line
#i2c = SoftI2C(sda=Pin(4), scl=Pin(5), freq=400000)

lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)

lcd.putstr("It's working :)")
sleep(4)

try:
    while True:
        # Clear the LCD
        lcd.clear()
        # Display two different messages on different lines
        # By default, it will start at (0,0) if the display is empty
        lcd.putstr("Hello World!")
        sleep(2)
        lcd.clear()
        # Starting at the second line (0, 1)
        lcd.move_to(0, 1)
        lcd.putstr("Hello World!")
        sleep(2)

except KeyboardInterrupt:
    # Turn off the display
    print("Keyboard interrupt")
    lcd.backlight_off()
    lcd.display_off()

View raw code

How the Code Works
Let’s take a quick look at how the code works to see how to interact with the LCD.

Importing Libraries
We start by including all the required modules to communicate with the LCD. We’ll use software I2C.

from machine import Pin, SoftI2C
from machine_i2c_lcd import I2cLcd
from time import sleep
LCD Properties
On the following lines, define your LCD properties. Our LCD display is 2×16 (2 rows and 16 columns) LCD and the I2C address is 0x27. If your display has different dimensions or if it has a different address, modify the following lines.

# Define the LCD I2C address and dimensions
I2C_ADDR = 0x27
I2C_NUM_ROWS = 2
I2C_NUM_COLS = 16
I2C Communication
Then, we initialize I2C on GPIOs 21 and 22 (for the ESP32), the pins we’re using to connect the LCD.

i2c = SoftI2C(sda=Pin(21), scl=Pin(22), freq=400000)
If you’re using an ESP8266, make sure to comment the previous line and uncomment the following to use the ESP8266 default I2C pins (GPIOs 4 and 5).

# for ESP8266, uncomment the following line
#i2c = SoftI2C(sda=Pin(4), scl=Pin(5), freq=400000)

After creating an I2C instance, we can initialize an I2C communication with the LCD using the following line.

lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)
We create a new object called lcd using the I2cLcd() method and pass as arguments, the i2c instance, the address, and the number of rows and columns. We can now use lcd to refer to the LCD screen.

Writing Text
Now, that we have a communication established we can start writing text. Writing simple text is as easy as using the putstr() method on the lcd object and passing as an argument the text we want to display. For example:

lcd.putstr("It's working :)")

This will display the “It’s working 🙂” message. Because we didn’t specify a place, and the LCD screen is still empty, it will display the message at (0, 0) first row, first column.

Then, we have a while loop that continuously displays “Hello World!” on the LCD, alternating between the top and bottom rows, creating a simple and repeating display pattern.

while True:
    # Clear the LCD
    lcd.clear()
    # Display two different messages on different lines
    # By default, it will start at (0,0) if the display is empty
    lcd.putstr("Hello World!")
    sleep(2)
    lcd.clear()
    # Starting at the second line (0, 1)
    lcd.move_to(0, 1)
    lcd.putstr("Hello World!")
    sleep(2)

Clearing the Screen
Before writing new information, we clear the contents of the screen using the clear() method.

lcd.clear()
Then, we display the string “Hello World!” on the LCD. By default, it starts at the top‑left corner of the display (position 0, 0).

lcd.putstr("Hello World!")
This message will be on the screen for two seconds.

sleep(2)

After that, we’ll clear the screen again.

lcd.clear()
Moving the Cursor
After that, we’ll use the move_to() method that accepts as arguments the row and column number, allowing us to choose where to start displaying text. In this case, we’re setting the cursor position to the beginning of the second line (row 1, column 0) of the LCD. This allows us to display the second “Hello World!” message on the second line.

Turning Off the Backlight
We also add a snippet to turn off the LCD light in case the program is stopped by a keyboard interrupt. You can use the methods backlight_off() and display_off().

except KeyboardInterrupt:
    # Turn off the display
    print("Keyboard interrupt")
    lcd.backlight_off()
    lcd.display_off()

This simple example shows the most useful methods to interact with the LCD. We encourage you to take a look at the machine_i2c_lcd.py and lcd_api.py modules and check and test other methods you think might be useful.

Testing the Code
Run the previous code on your ESP32 or ESP8266 boards. You’ll first get a message showing that it is working.

ESP32/ESP8266 LCD Display Testing with Micropython

After, it will display the Hello, World! message in the first row for two seconds, and then in the second row for another two seconds. Then, the loop repeats until you stop the program.

ESP32 LCD Display Static Text
ESP32 LCD Display Static Text
Note: if you want a code to run automatically when the ESP boots (for example, without being connected to your computer), you need to save the file to the board with the name main.py.

When you name a file main.py, the ESP32 or ESP8266 will run that file automatically on boot. If you call it a different name, it will still be saved on the board filesystem, but it will not run automatically on boot.


Troubleshooting
If the LCD has very little contrast and you can hardly see the characters, rotate the potentiometer at the back to adjust the contrast between the characters and the backlight.

Additionally, make sure you’re powering the LCD using 5V. Powering with 3V3 is not enough for most of these modules (unless otherwise specified in the datasheet).

Displaying Scrolling Text
Scrolling text on the LCD is useful to display messages longer than your LCD width (in our case, longer than 16 characters).

The library we’re using doesn’t come with a specific function to scroll the text, but you can build your own function by shifting the text to the left one position at a time. See the example below.

# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-i2c-lcd-esp32-esp8266/

from machine import Pin, SoftI2C
from machine_i2c_lcd import I2cLcd
from time import sleep

# Define the LCD I2C address and dimensions
I2C_ADDR = 0x27
I2C_NUM_ROWS = 2
I2C_NUM_COLS = 16

# Initialize I2C and LCD objects
i2c = SoftI2C(sda=Pin(21), scl=Pin(22), freq=400000)

# for ESP8266, uncomment the following line
#i2c = SoftI2C(sda=Pin(4), scl=Pin(5), freq=400000)

lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)

def scroll_message(message, delay=0.3):
    # Add spaces to the beginning of the message to make it appear from the right
    message = " " * I2C_NUM_COLS + message + " "
    # Scroll through the message
    for i in range(len(message) - I2C_NUM_COLS + 1):
        lcd.move_to(0, 0)
        lcd.putstr(message[i:i + I2C_NUM_COLS])
        sleep(delay)

try:
    lcd.clear()
    lcd.putstr("Testing scroll!")
    sleep(2)

    # Define the message to be scrolled
    message_scroll = "This is a scrolling message with more than 16 characters"

    while True:
        # Scroll the message on the LCD
        lcd.clear()
        scroll_message(message_scroll)

except KeyboardInterrupt:
    # Turn off the display when the code is interrupted by the user
    print("Keyboard interrupt")
    lcd.backlight_off()
    lcd.display_off()

View raw code

How the Code Works
In this example, we create a simple function called scroll_message() that accepts as arguments the message to be displayed and the delay between each shift. By default, we set the delay to 0.3 seconds, but you can adjust depending on how fast you want the text to scroll.

def scroll_message(message, delay=0.3):
    # Add spaces to the beginning of the message to make it appear from the right
    message = " " * I2C_NUM_COLS + message + " "
    # Scroll through the message
    for i in range(len(message) - I2C_NUM_COLS + 1):
        lcd.move_to(0, 0)
        lcd.putstr(message[i:i + I2C_NUM_COLS])
        sleep(delay)

On that function, we start by adding a padding to our message that consists of as many blank spaces as the number of columns (I2C_NUM_COLS) and a final space at the end. We do this to create a space from where to start scrolling.

message = " " * I2C_NUM_COLS + message + " "
Then, we have a for loop that will iterate through the characters of our message. We set the cursor position to the top-left corner of the LCD (column 0, row 0) to ensure that each iteration starts from the beginning of the LCD.

lcd.move_to(0, 0)
Then, we display a portion of the message on the LCD, starting from the current iteration index i and covering the width of the LCD (I2C_NUM_COLS characters).

lcd.putstr(message[i:i + I2C_NUM_COLS])

The following expression message[i:i + I2C_NUM_COLS] gets just a portion of the message starting at the character with index i and ending at character i + IC2_NUM_COLS. As we increase the value of i, we select a new portion of the message, making the scrolling effect.

Finally, the delay at the end will determine the scrolling speed.

sleep(delay)
To scroll the text, call the scroll_message() function and pass as arguments the message you want to scroll and the delay time.

scroll_message(message_scroll, 0.4)
Testing the Code
Run the previous code on your ESP32 or ESP8266 board. You should get a scrolling message. You can adjust the scrolling speed in the delay variable in the scroll_message() function.


ESP32 with LCD Scrolling Text MicroPython
ESP32 with LCD Scrolling Text MicroPython
Displaying Custom Characters
In a 16×2 LCD, there are 32 blocks where you can display characters. Each block is made out of 5×8 tiny pixels. You can display custom characters by defining the state of each tiny pixel. For that, you can create a byte array variable to hold the state of each pixel.

Creating Custom Characters
Most LCDs allow you to upload about 8 custom characters to the memory of the LCD that you can use later. Then, you can access the characters by their index (0 to 7).


To create your custom character, you can go here and design your custom character. Then, generate the byte array variable for your character. For example, a heart:

Creating icons byte array for LCD
The byte array is highlighted in yellow. Copy the byte array to a single line. For example, in the case of the heart character:


0x00, 0x0A, 0x1F, 0x0E, 0x04, 0x00, 0x00
Save your byte array because you’ll use it later in the code.

Displaying Custom Characters – Code
The following code displays three custom characters, a thermometer, an umbrella, and a heart. You can display any character you want by using its byte array.

# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-i2c-lcd-esp32-esp8266/

from machine import Pin, SoftI2C
from machine_i2c_lcd import I2cLcd
from time import sleep

# Define the LCD I2C address and dimensions
I2C_ADDR = 0x27
I2C_NUM_ROWS = 2
I2C_NUM_COLS = 16

# Initialize I2C and LCD objects
i2c = SoftI2C(sda=Pin(21), scl=Pin(22), freq=400000)

# for ESP8266, uncomment the following line
#i2c = SoftI2C(sda=Pin(4), scl=Pin(5), freq=400000)

lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)

# Create custom characters here: https://maxpromer.github.io/LCD-Character-Creator/
thermometer = bytearray([0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x1B, 0x1F, 0x0E])
lcd.custom_char(0, thermometer)

umbrella = bytearray([0x00, 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x014, 0x0C])
lcd.custom_char(1, umbrella)

heart = bytearray([0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00])
lcd.custom_char(2, heart)

try:
    lcd.putstr("Characters")
    lcd.move_to(0, 1)
    # Display thermometer
    lcd.putchar(chr(0))
    # Display umbrella
    lcd.move_to(2, 1)
    lcd.putchar(chr(1))
    # Display heart
    lcd.move_to(4, 1)
    lcd.putchar(chr(2))
 
except KeyboardInterrupt:
    # Turn off the display when the code is interrupted by the user
    print("Keyboard interrupt")
    lcd.backlight_off()
    lcd.display_off()


View raw code

How the Code Works
First, you need to save your byte array in a variable as follows:

thermometer = bytearray([0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x1B, 0x1F, 0x0E])
Change the byte array for any other characters you may want to display. In our case, we also display an umbrella and a heart.

Then, to save your character on the memory of your LCD, you need to use the custom_char() function on the lcd object and pass as argument the index where you want to save the character and the byte array for that character.

lcd.custom_char (0, thermometer)
We do this previous process for all the custom characters we want to display, but each one is saved on a different custom_char() index.

thermometer = bytearray([0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x1B, 0x1F, 0x0E])
lcd.custom_char(0, thermometer)

umbrella = bytearray([0x00, 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x014, 0x0C])
lcd.custom_char(1, umbrella)

heart = bytearray([0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00])
lcd.custom_char(2, heart)
Finally, to display your custom character you just need to use the putchar() function as follows:

lcd.putchar(chr(0))


In our case, chr(0) corresponds to the thermometer, the character that we saved on index 0. We proceed similarly for all the other characters. In our case, we display a static message on the first row, and then we display our custom characters on the second row.

try:
    lcd.putstr(“Characters”)
    lcd.move_to(0, 1)
    # Display thermometer
    lcd.putchar(chr(0))
    # Display umbrella
    lcd.move_to(2, 1)
    lcd.putchar(chr(1))
    # Display heart
    lcd.move_to(4, 1)
    lcd.putchar(chr(2))
As you can see, displaying custom characters is quite simple.



Testing the Code
Run or upload the code to your ESP32 or ESP8266 board. The LCD will display your custom characters. Feel free to modify the code and display your own custom characters. You can also use several adjacent blocks to create a bigger icon on the display.

ESP32 with LCD Display Custom Characters MicroPython


Wrapping Up
In this tutorial, you learned how to use the I2C LCD (Liquid Crystal Display) with the ESP32 and ESP8266 programmed with MicroPython firmware. We covered how to display static text, scrolling text, and custom characters.

We hope you find this guide useful. We have similar tutorials for the OLED display:

MicroPython: OLED Display with ESP32 and ESP8266
MicroPython: SSD1306 OLED Display Scroll Functions and Draw Shapes (ESP32/ESP8266)
If you would like to learn more about programming the ESP32 and ESP8266 using MicroPython firmware, check out our resources:

Free MicroPython Tutorials and Guides with the ESP32 and ESP8266
MicroPython Programming with ESP32/ESP8266 (eBook)