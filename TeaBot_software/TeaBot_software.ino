/* ********************************************************
 *  TeaBot software
 *  by dustinbrun, 2025 
 *  
 *  License: CC BY 4.0 
 *  https://creativecommons.org/licenses/by/4.0/
 * 
 *  ********************************************************
 */

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>       // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>   // https://github.com/adafruit/Adafruit_SSD1306
#include "OneButton.h"          // https://github.com/mathertel/OneButton/
#include <Encoder.h>            // https://github.com/PaulStoffregen/Encoder
#include <EEPROM.h>


// ---------------- Config parameters ----------------------
// Servo position value definition
int POS_UP = 120;       // Up-position
int POS_MIDDLE = 85;    // Middle-position
int POS_DOWN = 40;      // Down-position

int counter = 240;      // Timer init value (seconds)

// ---------------- Pins -----------------------------------
int buttonPin = 4;      // select the input pin for the potentiometer
int encoder_DT_pin = 3;
int encoder_CLK_pin = 2;
int servoPin = 9;
int buzzerPin = 10;
 
// ---------------------------------------------------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int target = 0;
bool time_blink = false;
int dip_enabled = 0;
bool running_a = false;
unsigned long int currentTime, last_time_display_update, last_time_second_counter, last_time_dip;
int dip_interval = 10000; // ms
int display_update_interval = 500; // ms
long oldPosition  = -999;
int tone_frequency = 1000;
bool start_running = false;
bool stop_running = false;

OneButton button(buttonPin, true);
Encoder encoder(encoder_DT_pin, encoder_CLK_pin);
Servo myservo;

// Start timer
void singleClick()
{
  if (running_a == true)
    stop_running = true;
  else
    start_running = true;

  tone(buzzerPin, tone_frequency, 50);
}

void set_dip_interval()
{
  if (dip_enabled == 1)
    dip_interval = 10000;
  else if (dip_enabled == 2)
    dip_interval = 30000;
}
// enable dipping
void doubleClick()
{
  if (dip_enabled == 0)
    dip_enabled = 1;
  else if (dip_enabled == 1)
    dip_enabled = 2;
  else
    dip_enabled = 0;
  
  set_dip_interval();
  tone(buzzerPin, tone_frequency, 50);
}

void longPressStart()
{

}

void setup()
{
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pinMode(LED_BUILTIN, OUTPUT);
  button.attachClick(singleClick);
  button.attachDoubleClick(doubleClick);
  button.attachLongPressStart(longPressStart);

  myservo.attach(servoPin);
  up();

  pinMode(buzzerPin, OUTPUT);

  // ----------- Spalsh Screen  ---------------
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0);
  display.println("TeaBot");
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.println("by dustinbrun, 2025");
  display.display();
  delay(1000);
  display.clearDisplay();
  tone(buzzerPin, tone_frequency, 100);

  // Set init values based on eeprom content
  EEPROM.get(1, target);
  EEPROM.get(100, dip_enabled);
  set_dip_interval();
  counter = target;
  encoder.write(counter / 10 * 4);
}

void dip()
{
  for (int i = POS_DOWN; i < POS_MIDDLE; i++)
  {
    myservo.write(i);
    delay(35);
  }
  for (int i = POS_MIDDLE; i > POS_DOWN; i--)
  {
    myservo.write(i);
    delay(35);
  }
}

void up()
{
  for (int i = POS_DOWN; i < POS_UP; i++)
  {
    myservo.write(i);
    delay (25);
  }
}

void down()
{
  for (int i = POS_UP; i > POS_DOWN; i--)
  {
    myservo.write(i);
    delay (25);
  }
}


void update_display()
{
  display.clearDisplay();

  // Timer
  int minutes = counter / 60;
  int seconds = counter % 60;
  display.setTextSize(3);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (minutes < 10)
    display.print("0");
  display.print(minutes);
  if (time_blink == true and running_a)
  {
    time_blink = false;
    display.print(" ");
  }
  else
  {
    time_blink = true;
    display.print(":");
  }
  if (seconds < 10)
    display.print("0");
  display.print(seconds);

  // Process Bar
  int percent = map(counter, 0, target, 0, SCREEN_WIDTH);
  for (int i = 0; i <= 10; i++)
    display.fillRect(SCREEN_WIDTH / 10 * i, 25, 2, 2, SSD1306_WHITE);
  // x Top left corner x coordinate, y Top left corner y coordinate, w Width in pixels, h Height in pixels
  if (running_a)
    display.fillRect(1, 27, percent, 5, SSD1306_WHITE);

  // Settings
  display.fillRect(90, 0, 2, 22, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(95, 0);
  display.print("Dip s");
  display.setTextSize(2);
  display.setCursor(95,10);
  if (dip_enabled == 0)
    display.print("no");
  else if(dip_enabled == 1)
    display.print("10");
  else if(dip_enabled == 2)
    display.print("30");
    

  display.display();
}


void loop()
{
  // Display update timer
  if (millis() - last_time_display_update > display_update_interval)
  {
    update_display();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    last_time_display_update = millis();
  }

  // Seconds counter
  if (millis() - last_time_second_counter > 1000 && running_a)
  {
    int decrement = (millis() - last_time_second_counter) / 1000;
    if (counter - decrement > 0)
      counter = counter - decrement;
    else
    {
      // Counter expired
      stop_running = true;
      counter = target;
    }
    last_time_second_counter = millis();
  }

  // Dip interval counter
  if (millis() - last_time_dip > dip_interval && running_a && dip_enabled)
  {
    dip();
    last_time_dip = millis();
  }

  if (start_running == true)
  {
    running_a = true;
    start_running = false;
    EEPROM.put(1, target);
    EEPROM.put(100, dip_enabled);
    last_time_second_counter = millis();
    last_time_dip = millis();
    down();
  }
  else if (stop_running == true)
  {
    running_a = false;
    stop_running = false;
    up();

    for (int i = 0; i < 4; i++)
    {
      tone(buzzerPin, tone_frequency, 400);
      delay(800);
    }
  }

  // read rotary encoder
  if (!running_a)
  {
    long newPosition = encoder.read() / 4;

    if (newPosition < 0)
    {
      encoder.write(0);
      newPosition = 0;
    }

    if (newPosition != oldPosition)
    {
      counter = newPosition * 10; // one tick = 4xtick
      target = counter;
      update_display();
      oldPosition = newPosition;
      tone(buzzerPin, tone_frequency, 50);
    }
  }

  button.tick();

}
