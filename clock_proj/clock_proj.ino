// Include Libraries
#include <Wire.h>
#include <RTClib.h>
#include <U8glib.h>
#include <Servo.h>

// Init constants
const int Ldr_pin = A0;

// Init global variables
Servo myServo;

// construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);


void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();  // startar i2c komplikationen, sdc och sda
  rtc.begin();

  // Init Hardware
  myServo.attach(8);

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // justerar tid och datum
  u8g.setFont(u8g_font_5x8);
}

void loop() {

  oledWrite(getTime(), getTemp());
  //servoWrite(getTemp());
  Serial.print(getTemp());
  Serial.print("   ");
  Serial.println(getTime());
  delay(1000);

  float temperature = getTemp();

  int servoAngle = map(temperature, 0, 50, 0 ,180);
  servoAngle = constrain(servoAngle, 0, 180);
  myServo.write(servoAngle);

  int brightness = getLDRBrightness();
  adjustOledBrightness(brightness);

  Serial.print("Ljusnivå ");
  Serial.println(analogRead(Ldr_pin));
}

int getLDRBrightness(){
  int lightlevel = analogRead(Ldr_pin);
  int brightness = map(lightlevel, 0, 1023, 0, 255);
  return brightness;
}

void adjustOledBrightness (int brightness){
u8g.setContrast(brightness);
}

/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();  // rtc.now gör så att vi får ut en klass som innehåller datum och tid, now används men syns inte här
  String timeString = "the Time is : " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  return "the Time is :" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp() {
  return rtc.getTemperature();
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String timeText, float temperature) {
  u8g.firstPage();

  do {
    u8g.drawStr(0, 20, timeText.c_str());
    String tempText = "Temp: " + String(temperature) + " C";
    u8g.drawStr(0, 40, tempText.c_str());

  } while (u8g.nextPage());
}

