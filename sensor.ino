#include <OneWire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

OneWire  ds(10);  
#define AX 2
byte addr[AX][8];
void setup() 
{
  lcd.print("DS1820 Test");
  if (!ds.search(addr[0])) 
  {
    lcd.print("No more addresses.");
    ds.reset_search();
    delay(250);
    return;
  }
  if ( !ds.search(addr[1])) 
  {
    lcd.print("No more addresses.");
    ds.reset_search();
    delay(250);
    return;
  }
}
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
char buf[20];

void loop() 
{
  byte i, sensor;
  byte present = 0;
  byte data[12];

  for (sensor=0;sensor<AX;sensor++)
  {
    if ( OneWire::crc8( addr[sensor], 7) != addr[sensor][7]) 
    {
      
      lcd.print("CRC is not valid");
      return;
    }

    if ( addr[sensor][0] != 0x28) 
    {
      lcd.print("Device not a DS18S20 family");
      return;
    }

    ds.reset();
    ds.select(addr[sensor]);
    ds.write(0x44,1);         
    delay(1000);     
    present = ds.reset();
    ds.select(addr[sensor]);    
    ds.write(0xBE);         
    for ( i = 0; i < 9; i++) 
    {           
      data[i] = ds.read();
    }

    LowByte = data[0];
    HighByte = data[1];
    TReading = (HighByte << 8) + LowByte;
    SignBit = TReading & 0x8000;  
    if (SignBit) 
    {
      TReading = (TReading ^ 0xffff) + 1; 
    }
    Tc_100 = (TReading*100/2);    

    Whole = Tc_100 / 1000;  
    Fract = Tc_100 % 100;
     lcd.print(Whole);
     lcd.print(".")
     lcd.print(Fract);

  }
    
  
}
