#include <Servo.h>

Servo servoHorizontal;
int servoHorizontalValue = 0;
int servoHorizontalLimitHigh = 160;
int servoHorizontalLimitLow = 20;

Servo servoVertical;
int servoVerticalValue = 0;
int servoVerticalLimitHigh = 160;
int servoVerticalLimitLow = 20;

int ldrTopLeft = 2;
int ldrTopRight = 1;
int ldrBottomLeft = 3;
int ldrBottomRight = 0;

void setup()
{
  Serial.begin(9600);
  servoHorizontal.attach(7);
  servoHorizontal.write(80);
  servoVertical.attach(8);
  servoVertical.write(80);
  delay(500);
}

void loop()
{
  servoHorizontalValue = servoHorizontal.read();
  servoVerticalValue = servoVertical.read();

  int topl = analogRead(ldrTopLeft);
  int topr = analogRead(ldrTopRight);
  int botl = analogRead(ldrBottomLeft);
  int botr = analogRead(ldrBottomRight);

  int averageTop = (topl + topr) / 10;
  int averageBottom = (botl + botr) / 10;
  int averageLeft = (topl + botl) / 10;
  int averageRight = (topr + botr) / 10;

  Serial.println(averageTop);
  Serial.println(averageBottom);
  Serial.println(averageLeft);
  Serial.println(averageRight);

  if (averageTop < averageBottom)
  {
    servoVertical.write(servoVerticalValue + 1);
    if (servoVerticalValue > servoHorizontalLimitHigh)
    {
      servoVerticalValue = servoVerticalLimitHigh;
    }
    delay(10);
  }
  else if (averageBottom < averageTop)
  {
    servoVertical.write(servoVerticalValue - 1);
    if (servoVerticalValue < servoVerticalLimitLow)
    {
      servoVerticalValue = servoVerticalLimitLow;
    }
    delay(10);
  }
  else
  {
    servoVertical.write(servoVerticalValue);
  }

  if (averageLeft > averageRight)
  {
    servoHorizontal.write(servoHorizontalValue + 1);
    if (servoHorizontalValue > servoHorizontalLimitHigh)
    {
      servoHorizontalValue = servoHorizontalLimitHigh;
    }
    delay(10);
  }
  else if (averageRight > averageLeft)
  {
    servoHorizontal.write(servoHorizontalValue - 1);
    if (servoHorizontalValue < servoHorizontalLimitLow)
    {
      servoHorizontalValue = servoHorizontalLimitLow;
    }
    delay(10);
  }
  else
  {
    servoHorizontal.write(servoHorizontalValue);
  }
  delay(50);
}
