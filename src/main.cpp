#include <Arduino.h>

#include <i2s.h>
#include <i2s_reg.h>

#define write_sample(data) while (i2s_write_sample_nb(data)==0)

void outputDMA(void)
{
   i2s_begin();
   i2s_set_rate(300000);  // set speed to 300000*32 bits/sec

   uint8_t state=0;
   for (int i=0; i<(300000);i++) 
   {
    state = (i)%2;
    if (state){
      write_sample(0xffffffff); // 16 bits of data
      write_sample(0xffffffff);
      write_sample(0xffffffff);
    }
    else
      write_sample(0x00000000);
      
    if ((i % 1000) == 0) yield(); // without this get WDT resets
   }
   i2s_end();
}

void setup()
{
    Serial.begin(115200);
    
    pinMode(3, OUTPUT); // Override default Serial initiation
    digitalWrite(3,0); // Set pin low
}

void loop()
{
    static unsigned long last = millis();
    if (millis() - last > 3000) {
      outputDMA();  // called every 3s
      last = millis();
    }
}