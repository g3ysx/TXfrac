//#include "WiFi.h"
#include <U8x8lib.h>

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

unsigned long ct; // current time
unsigned long nt; // next time
int ctr = 0;
char buf[3];
char buf2[300];

const int PttPin = 5;

#define FRACTSEC 2
#define NUMSAMP (60*6*FRACTSEC)
int sample[NUMSAMP];
#define NEXTTIME (1000/FRACTSEC)

int sampleTot = 0;
int nextSamp = 0;
int samp;

float fract = 0.0;
int fractI = 0;

void setup()
{
  // Set up display
  u8x8.begin();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_inb46_4x8_r);
  
  nt = millis() + NEXTTIME;
  Serial.begin(115200);

  pinMode(PttPin, INPUT_PULLUP);
  for (int i=0; i<NUMSAMP; i++)
     sample[i]=0;
  ct = millis();
}

void loop()
{
  // How much time does the monitor take
  // sprintf(buf2,"%d msec %d %d\n", millis()-ct, sampleTot, fractI);
  // Serial.write(buf2);

  // How much time does the display write take

  sprintf(buf, "%3d", 1);
  ct = millis();
  u8x8.drawString(0, 0, buf);
  nt = millis();
  sprintf(buf2,"Display time = %d\n",nt-ct);
  Serial.write(buf2);
  delay(1000);

}
