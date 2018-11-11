#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "leds.h"

extern unsigned char brenner_tga[];
extern unsigned char ofenoben_tga[];
extern unsigned char ofenunten_tga[];

LEDs leds;

// ---------------------------------------------------------

LEDs::LEDs()
{
    fd = open("/dev/spidev0.0", O_RDWR);

    uint8_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 3000000;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        perror("spi set mode");
        exit(-1);
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
    {
        perror("spi set bits per word");
        exit(-1);
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        perror("spi set speed");
        exit(-1);
    }

    for (auto & b: brennerTemp) b = 0;
}
// ---------------------------------------------------------

LEDs::~LEDs()
{
  if (fd>=0) close(fd);
  fd = -1;
}
// ---------------------------------------------------------

void LEDs::Rotate(Knob, int)
{
}
// ---------------------------------------------------------

void LEDs::Press(Knob)
{
}
// ---------------------------------------------------------

const char * LEDs::GetText()
{
  return nullptr;
}
// ---------------------------------------------------------

void LEDs::Chosen(int i)
{
  switch (i)
  {
  case 1: brennerTemp[0] = 100; break;
  case 2: brennerTemp[0] = 80; break;
  case 3: brennerTemp[0] = 50; break;
  case 4: brennerTemp[0] = 0; break;
  case 5: brennerTemp[2] = 10; break;
  case 6: brennerTemp[2] = 0; break;
  case 7: brennerTemp[3] = 100; break;
  case 8: brennerTemp[3] = 0; break;
  }

  active = nullptr;
}
// ---------------------------------------------------------

const unsigned char gamma8[] = {
    0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,
    3,  3,  3,  3,  4,  4,  4,  5,  5,  5,  6,  6,  7,  7,  8,  8,
    8,  9,  9, 10, 11, 11, 12, 12, 13, 14, 14, 15, 16, 17, 17, 18,
   19, 20, 21, 22, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34,
   35, 37, 38, 39, 41, 42, 43, 45, 46, 48, 49, 51, 52, 54, 55, 57,
   59, 61, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88,
   91, 93, 95, 98,100,101,102,103,104,105,106,107,108,109,110,111,
  112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
  160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
  176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
  192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
  208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
  224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
  240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
  };
// ---------------------------------------------------------


void LEDs::Brenner::Follow(int temp, LED * led, unsigned char * tga, int max)
{
  const int width = tga[12] + tga[13]*256;
  const int height = tga[14] + tga[15]*256;

  const int interval = width/4;

  pos++;
  if (pos==interval) pos = 0;

  switch (state)
  {
  case Aus:
    if (temp>0)
      state = Zuendet;
    if (temp<0)
      state = Anderes;
    pos = 0;
    break;

  case Zuendet:
    if (pos == 0) state = BrenntA;
    break;

  case BrenntA:
  case BrenntB:
  case BrenntC:
  case BrenntD:
    if (pos==0)
    {
      state = static_cast<Brenner::State>(static_cast<int>(BrenntA) + (random()&3));
      offs = random()&15;
    }
    if (temp==0)
    {
      state = Loescht;
      pos = 0;
    }
    break;

  case Loescht:
    if (pos==0) state = Aus;
    break;

  case Anderes:
    if (temp>=0) state = Aus;
    pos = 0;
    break;
  }

  if (temp>0) 
  {
    if (temp<20) factor = 20;
    else factor = temp;
  }

  int posi = pos;

  switch (state)
  {
  case Aus: posi = width-1; break;
  case Zuendet: break;
  case BrenntA: posi = posi + interval; break;
  case BrenntB: posi = posi + 2*interval; break;
  case BrenntC: posi = 2*interval - posi; break;
  case BrenntD: posi = 3*interval - posi; break;
  case Loescht: posi = posi + 3*interval; break;
  case Anderes: return;
  }

  for (int i=0; i<height; i++)
  {
    int y = (i+offs)%height;
    led[i].red = tga[y*width*4 + posi*4 + 18 + 2] * factor / max;
    led[i].green = tga[y*width*4 + posi*4 + 18 + 1] * factor / max;
    led[i].blue = tga[y*width*4 + posi*4 + 18  + 0] * factor / max;
    unsigned char w = tga[y*width*4 + posi*4 + 18 + 3];
    if (w==255) w = 0;
    led[i].white = w * factor / max;
  }
}
// ---------------------------------------------------------

void LEDs::Tick()
{
    if (fd < 0) return;

    for (int i=0; i<4; i++)
      brenner[i].Follow(brennerTemp[i], &led[i*16], brenner_tga, 100);

    brenner[4].Follow(brennerTemp[4], &led[4*16], ofenoben_tga, 250);
    brenner[5].Follow(brennerTemp[5], &led[4*16+8], ofenunten_tga, 250);

    const unsigned char code[4] { 0x88, 0x8E, 0xE8, 0xEE };

    unsigned char buf[ sizeof(led)*4 + 4];

    unsigned char * p = buf;
    unsigned char * s = &led[0].green;

    for (unsigned int i=0; i<sizeof(led); i++)
    {
       unsigned char b = gamma8[*s++];
       *p++ = code[(b >> 6) & 3];
       *p++ = code[(b >> 4) & 3];
       *p++ = code[(b >> 2) & 3];
       *p++ = code[(b >> 0) & 3];
    }

    for (int i=0; i<3; i++)
      *p++ = 0;

    write(fd, &buf, sizeof(buf));
}
// ---------------------------------------------------------
