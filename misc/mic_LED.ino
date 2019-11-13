/****************************************
Basic Logic for (Mic) -> (LED) hypersax
More sound = more angry light
****************************************/

/*To-do: 
1. read https://forum.arduino.cc/index.php?topic=421081 and send using RF
2. Switch, or gradient, or both? Revisit after soldering
3. Temperature sensor? In setup (ie on reset), take initial reading, then use as comparison during performance
*/
#include <Adafruit_NeoPixel.h>
#define PIN            5
#define NUMPIXELS      1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
void setup() 
{
   Serial.begin(9600);
   pixels.begin();
   
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
   Serial.println(peakToPeak);

   if (0 <= peakToPeak <= 250)
   {
     pixels.setPixelColor(0, pixels.Color(50, 0, 150));
     //Send trigger
   }
   else if (peakToPeak <= 500)
   {
     pixels.setPixelColor(0, pixels.Color(150, 50, 50));
   }
   else
   {
     pixels.setPixelColor(0, pixels.Color(50, 150, 50));
   }
   
   pixels.show();
}
