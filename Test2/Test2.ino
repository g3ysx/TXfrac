//#include "WiFi.h"
#include <U8x8lib.h>

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

unsigned long ct; // current time
unsigned long nt; // next time
int ctr = 0;
char buf[3];
char buf2[30];

void setup()
{
  // Set up display
  u8x8.begin();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_inb46_4x8_r);
  
  nt = millis() + 1000;
  Serial.begin(115200);
}

void loop()
{
  ct = millis();
  if (ct < nt) return;

  nt = nt + 1000;

  ctr = ctr + 10;
  if (ctr>100) ctr = 0;

// u8x8.clearDisplay();
  sprintf(buf, "%3d", ctr);
  u8x8.drawString(0, 0, buf);

  // diag - show how much time this took
  sprintf(buf2,"%d msec\n",millis()-ct);
  Serial.write(buf2);
}
