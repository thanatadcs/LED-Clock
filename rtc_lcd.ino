#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.hpp>

LiquidCrystal_I2C lcd(0x27,20,4);
DS1302 rtc(2, 3, 4);

int start = 0;
int end = 0;

void setup()
{
  rtc.halt(false);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());

  IrReceiver.begin(11, true);

  start = millis();
}

void loop()
{
  end = millis();
  if (end - start >= 1000)
  {
    lcd.setCursor(0, 0);
    lcd.print(rtc.getTimeStr());
    start = millis();
  }

  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
    Time s = rtc.getTime();
    if (IrReceiver.decodedIRData.command == 0x15) { // Plus button
      rtc.setTime(s.hour, s.min + 1, s.sec);
    } else if (IrReceiver.decodedIRData.command == 0x7) { // Minus button
      rtc.setTime(s.hour, s.min - 1, s.sec);
    } else if (IrReceiver.decodedIRData.command == 0x40) { // Next button
      rtc.setTime(s.hour + 1, s.min, s.sec);
    } else if (IrReceiver.decodedIRData.command == 0x44) { // Prev button
      rtc.setTime(s.hour - 1, s.min, s.sec);
    }
    lcd.setCursor(0, 0);
    lcd.print(rtc.getTimeStr());
  }
}
