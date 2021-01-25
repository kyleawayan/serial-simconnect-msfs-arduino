#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
const byte DATA_MAX_SIZE = 32;
char data[DATA_MAX_SIZE]; // an array to store the received data

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  display.print("text");
  display.display();
}

void setMainBlueDisplay(char text[32])
{
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  display.print(text);
  display.display();
}

void receiveData()
{
  static char endMarker = '\n';   // message separator
  char receivedChar;              // read char from serial port
  int ndx = 0;                    // current index of data buffer
  memset(data, 32, sizeof(data)); // clean data buffer

  // read while we have data available and we are
  // still receiving the same message.
  while (Serial.available() > 0)
  {
    receivedChar = Serial.read();
    if (receivedChar == endMarker)
    {
      data[ndx] = '\0'; // end current message
      setMainBlueDisplay(data);
      return;
    } // looks like a valid message char, so append it and
    // increment our index
    data[ndx] = receivedChar;
    ndx++; // if the message is larger than our max size then
    // stop receiving and clear the data buffer. this will
    // most likely cause the next part of the message
    // to be truncated as well, but hopefully when you
    // parse the message, you'll be able to tell that it's
    // not a valid message.
    if (ndx >= DATA_MAX_SIZE)
    {
      break;
    }
  } // no more available bytes to read from serial and we
  // did not receive the separato. it's an incomplete message!
  Serial.println("error: incomplete message");
  Serial.println(data);
  memset(data, 32, sizeof(data));
}

void loop()
{
  if (Serial.available() > 0)
  {
    receiveData();
  }
  delay(100);
}
