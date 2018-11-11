#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include "linux/i2c-dev.h"

#include "display.h"

/*
  --- 
 |\|/|
  - -
 |/|\|
  ---  .
*/

#define D(a, \
  b,c,d,e,f, \
  g,h, \
  i,j,k,l,m, \
  n,o) \
  (0x01*a) | \
  (0x02*f) | \
  (0x04*m) | \
  (0x08*n) | \
  (0x10*i) | \
  (0x20*b) | \
  (0x40*g) | \
  (0x80*h),  \
  (0x40*o) | \
  (0x20*l) | \
  (0x10*k) | \
  (0x08*j) | \
  (0x04*e) | \
  (0x02*d) | \
  (0x01*c)


#define xxxxxxxxx 1
#define xxxx 1
#define x 1
#define _________ 0
#define ____ 0
#define _ 0


const unsigned char font[128][2] = {
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },

  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
  { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },

  /*0x20   */ { 0x00, 0x00 },

  /*0x21 ! */ { D(_________,
                  _,_,_,_,x,
                  ____,____,
                  _,_,_,_,_,
                  _________,x)},

  /*0x22 " */ { D(_________,
                  x,_,_,_,x,
                  ____,____,
                  _,_,_,_,_,
                  _________,_)},

  /*0x23 # */ { 0x00, 0x00 },
  /*0x24 $ */ { 0x00, 0x00 },
  /*0x25 % */ { 0x00, 0x00 },
  /*0x26 & */ { 0x00, 0x00 },
  /*0x27 ' */ { 0x00, 0x00 },
  /*0x28 ( */ { D(_________,
                  _,_,_,x,_,
                  ____,____,
                  _,_,_,x,_,
                  _________,_)},

  /*0x29 ) */ { D(_________,
                  _,x,_,_,_,
                  ____,____,
                  _,x,_,_,_,
                  _________,_)},

  /*0x2a * */ { D(_________,
                  _,x,x,x,_,
                  ____,____,
                  _,x,x,x,_,
                  _________,_)},

  /*0x2b + */ { D(_________,
                  _,_,x,_,_,
                  xxxx,xxxx,
                  _,_,x,_,_,
                  _________,_)},

  /*0x2c , */ { 0x00, 0x00 },

  /*0x2d - */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  _,_,_,_,_,
                  _________,_)},

  /*0x2e . */ { 0x00, 0x00 },

  /*0x2f / */ { D(_________,
                  _,_,_,x,_,
                  ____,____,
                  _,x,_,_,_,
                  _________,_)},

  /*0x30  0*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  ____,____,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x31  1*/ { D(_________,
                  _,_,_,x,x,
                  ____,____,
                  _,_,_,_,x,
                  _________,_)},

  /*0x32  2*/ { D(xxxxxxxxx,
                  _,_,_,_,x,
                  xxxx,xxxx,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},

  /*0x33  3*/ { D(xxxxxxxxx,
                  _,_,_,_,x,
                  ____,xxxx,
                  _,_,_,_,x,
                  xxxxxxxxx,_)},


  /*0x34  4*/ { D(_________,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  _,_,_,_,x,
                  _________,_)},

  /*0x35  5*/ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  xxxx,xxxx,
                  _,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x36  6*/ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  xxxx,xxxx,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x37  7*/ { D(xxxxxxxxx,
                  _,_,_,x,_,
                  ____,____,
                  _,x,_,_,_,
                  _________,_)},

  /*0x38  8*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x39  9*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  _,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x3a : */ { D(_________,
                  _,_,_,_,_,
                  ____,xxxx,
                  _,_,_,_,_,
                  _________,x)},
 

  /*0x3b ; */ { 0x00, 0x00 },

  /*0x3c < */ { D(_________,
                  _,_,_,_,_,
                  ____,xxxx,
                  _,_,_,x,_,
                  _________,_)},

  /*0x3d = */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  _,_,_,_,_,
                  xxxxxxxxx,_)},


  /*0x3e > */ { D(_________,
                  _,_,_,_,_,
                  xxxx,____,
                  _,x,_,_,_,
                  _________,_)},

  /*0x3f ? */ { D(xxxxxxxxx,
                  _,_,_,_,x,
                  ____,xxxx,
                  _,_,x,_,_,
                  _________,x)},

  /*0x40 @ */ { 0x00, 0x00 },

  /*0x41  A*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  x,_,_,_,x,
                  _________,_)},

  /*0x42  B*/ {  D(xxxxxxxxx,
                  _,_,x,_,x,
                  ____,xxxx,
                  _,_,x,_,x,
                  xxxxxxxxx,_)},

  /*0x43  C*/ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  ____,____,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},

  /*0x44  D*/ { D(xxxxxxxxx,
                  _,_,x,_,x,
                  ____,____,
                  _,_,x,_,x,
                  xxxxxxxxx,_)},

  /*0x45  E*/ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  xxxx,____,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},

  /*0x46  F*/ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  xxxx,____,
                  x,_,_,_,_,
                  _________,_)},

  /*0x47  G*/ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  ____,xxxx,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x48  H*/ { D(_________,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  x,_,_,_,x,
                  _________,_)},

  /*0x49  I*/ { D(_________,
                  _,_,x,_,_,
                  ____,____,
                  _,_,x,_,_,
                  _________,_)},

  /*0x4a  J*/ { D(xxxxxxxxx,
                  _,_,_,_,x,
                  ____,____,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x4b  K*/ { D(_________,
                  x,_,_,x,_,
                  xxxx,____,
                  x,_,_,x,_,
                  _________,_)},

  /*0x4c  L*/ { D(_________,
                  x,_,_,_,_,
                  ____,____,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},

  /*0x4d  M*/ { D(_________,
                  x,x,_,x,x,
                  ____,____,
                  x,_,_,_,x,
                  _________,_)},

  /*0x4e  N*/ { D(_________,
                  x,x,_,_,x,
                  ____,____,
                  x,_,_,x,x,
                  _________,_)},

  /*0x4f  O*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  ____,____,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x50  P*/ { D(xxxxxxxxx,
                  _,_,x,_,x,
                  ____,xxxx,
                  _,_,x,_,_,
                  _________,_)},

  /*0x51  Q*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  ____,____,
                  x,_,_,x,x,
                  xxxxxxxxx,_)},

  /*0x52  R*/ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  x,_,_,x,_,
                  _________,_)},

  /*0x53  S*/ { D(xxxxxxxxx,
                  _,x,_,_,_,
                  ____,xxxx,
                  _,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x54  T*/ { D(xxxxxxxxx,
                  _,_,x,_,_,
                  ____,____,
                  _,_,x,_,_,
                  _________,_)},

  /*0x55  U*/ { D(_________,
                  x,_,_,_,x,
                  ____,____,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x56  V*/ { D(_________,
                  x,_,_,x,_,
                  ____,____,
                  x,x,_,_,_,
                  _________,_)},

  /*0x57  W*/ { D(_________,
                  x,_,_,_,x,
                  ____,____,
                  x,x,_,x,x,
                  _________,_)},

  /*0x58  X*/ { D(_________,
                  _,x,_,x,_,
                  ____,____,
                  _,x,_,x,_,
                  _________,_)},

  /*0x59  Y*/ { D(_________,
                  _,x,_,x,_,
                  ____,____,
                  _,_,x,_,_,
                  _________,_)},

  /*0x5a  Z*/ { D(xxxxxxxxx,
                  _,_,_,x,_,
                  ____,____,
                  _,x,_,_,_,
                  xxxxxxxxx,_)},

  /*0x5b [ */ { D(xxxxxxxxx,
                  x,_,_,_,_,
                  ____,____,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},

  /*0x5c \ */ { D(_________,
                  _,x,_,_,_,
                  ____,____,
                  _,_,_,x,_,
                  _________,_)},

  /*0x5d ] */ { D(xxxxxxxxx,
                  _,_,_,_,x,
                  ____,____,
                  _,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x5e ^ */ { 0x00, 0x00 },

  /*0x5f _ */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  _,_,_,_,_,
                  xxxxxxxxx,_)},

  /*0x60 ` */ { D(xxxxxxxxx,
                  x,_,_,_,x,
                  xxxx,xxxx,
                  _,_,_,_,_,
                  _________,_)},



  /*0x61 a */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  _,x,_,_,x,
                  xxxxxxxxx,_)},

  /*0x62 b */ { D(_________,
                  x,_,_,_,_,
                  xxxx,____,
                  x,_,_,x,_,
                  xxxxxxxxx,_)},

  /*0x63 c */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},


  /*0x64 d */ { D(_________,
                  _,_,_,_,x,
                  ____,xxxx,
                  _,x,_,_,x,
                  xxxxxxxxx,_)},


  /*0x65 e */ { D(_________,
                  _,_,_,_,_,
                  xxxx,____,
                  x,x,_,_,_,
                  xxxxxxxxx,_)},


  /*0x66 f */ { D(_________,
                  _,_,x,x,_,
                  ____,xxxx,
                  _,_,x,_,_,
                  _________,_)},


  /*0x67 g */ { D(_________,
                  _,_,_,_,_,
                  ____,xxxx,
                  _,_,_,x,x,
                  xxxxxxxxx,_)},


  /*0x68 h */ { D(_________,
                  x,_,_,_,_,
                  xxxx,____,
                  x,_,x,_,_,
                  _________,_)},


  /*0x69 i */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  _,_,_,_,x,
                  _________,_)},


  /*0x6a j */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  _,_,_,_,x,
                  xxxxxxxxx,_)},


  /*0x6b k */ { D(_________,
                  _,_,x,_,_,
                  ____,xxxx,
                  _,_,x,x,_,
                  _________,_)},


  /*0x6c l */ { D(_________,
                  _,_,x,_,_,
                  ____,____,
                  _,_,x,_,_,
                  _________,_)},


  /*0x6d m */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  x,_,x,_,x,
                  _________,_)},


  /*0x6e n */ { D(_________,
                  _,_,_,_,_,
                  xxxx,____,
                  x,_,x,_,_,
                  _________,_)},


  /*0x6f o */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},

  /*0x70 p */ { D(_________,
                  _,_,_,_,_,
                  xxxx,____,
                  x,_,x,_,_,
                  xxxxxxxxx,_)},

  /*0x71 q */ { D(_________,
                  _,_,_,_,_,
                  ____,xxxx,
                  _,_,x,_,x,
                  xxxxxxxxx,x)},

  /*0x72 r */ { D(_________,
                  _,_,_,_,_,
                  xxxx,____,
                  x,_,_,_,_,
                  _________,_)},


  /*0x73 s */ { D(_________,
                  _,_,_,_,_,
                  ____,xxxx,
                  _,_,_,x,_,
                  xxxxxxxxx,_)},


  /*0x74 t */ { D(_________,
                  x,_,_,_,_,
                  xxxx,____,
                  x,_,_,_,_,
                  xxxxxxxxx,_)},


  /*0x75 u */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  x,_,_,_,x,
                  xxxxxxxxx,_)},


  /*0x76 v */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  x,x,_,_,_,
                  _________,_)},


  /*0x77 w */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  x,x,_,x,x,
                  _________,_)},

  /*0x78 x */ { D(_________,
                  _,_,_,_,_,
                  xxxx,xxxx,
                  _,_,x,_,_,
                  xxxxxxxxx,_)},

  /*0x79 y */ { D(_________,
                  _,_,_,_,_,
                  ____,____,
                  _,_,_,x,x,
                  xxxxxxxxx,_)},

  /*0x7a z */ { D(_________,
                  _,_,_,_,_,
                  xxxx,____,
                  _,x,_,_,_,
                  xxxxxxxxx,_)},


  /*0x7b   */ { 0x00, 0x00 },
  /*0x7c   */ { 0x00, 0x00 },
  /*0x7d   */ { 0x00, 0x00 },
  /*0x7e   */ { 0x00, 0x00 },
  /*0x7f   */ { 0x00, 0x00 },

};

enum HT16K33 {
    BLINK_CMD       = 0x80,
    BLINK_DISPLAYON = 0x01,
    BLINK_OFF       = 0x00,
    SYSTEM_SETUP    = 0x20,
    OSCILLATOR      = 0x01,
    CMD_BRIGHTNESS  = 0xE0
};

Display::Display()
{
    fd = open("/dev/i2c-1", O_RDWR);

    if(fd < 0) 
    {
        perror("open i2c");
        exit(-1);
    }

    Init();
}

Display::~Display()
{
    if (fd>=0) close(fd);
    fd = -1;
}

void Display::SetBits(unsigned char * buf)
{
    memcpy( bits, buf, 16 );
}

void Display::Init()
{
    unsigned char buf[1];
    if(ioctl(fd, I2C_SLAVE, 0x71) < 0) return;

    buf[0] = SYSTEM_SETUP | OSCILLATOR;
    if(write(fd, &buf, 1) != 1) return;

    buf[0] = BLINK_CMD | BLINK_DISPLAYON;
    if(write(fd, &buf, 1) != 1) return;

    if(ioctl(fd, I2C_SLAVE, 0x70) < 0) return;

    buf[0] = SYSTEM_SETUP | OSCILLATOR;
    if(write(fd, &buf, 1) != 1) return;

    buf[0] = BLINK_CMD | BLINK_DISPLAYON;
    if(write(fd, &buf, 1) != 1) return;

}

void Display::SetBrightness(int brightness)
{
    unsigned char buf[1];
    buf[0] = CMD_BRIGHTNESS| brightness;

    if(ioctl(fd, I2C_SLAVE, 0x71) < 0) return;
    if(write(fd, &buf, 1) != 1) return;

    if(ioctl(fd, I2C_SLAVE, 0x70) < 0) return;
    if(write(fd, &buf, 1) != 1) return;
}

void Display::Upload()
{
    unsigned char buf[17];

    buf[0] = 0;
    for (int i=0; i<8; i++) 
    {
      buf[i+1] = bits[i];
      buf[i+9] = 0x00;
    }

    if(ioctl(fd, I2C_SLAVE, 0x71) < 0) return;
    if (write(fd, &buf, 17) != 17) return;

    for (int i=0; i<8; i++) 
      buf[i+1] = bits[i+8];

    if(ioctl(fd, I2C_SLAVE, 0x70) < 0) return;
    if (write(fd, &buf, 17) != 17) return;
}


void Display::SetChar(int index, char c)
{
    int code = c;
    if (code<0) code=0;
    if (code>127) code=0;

    bits[index*2] = font[code][0];
    bits[index*2+1] = font[code][1];
}

void Display::ShowText(const char * string)
{
    int index = 0;
    const char * p = string;

    if (p) for (; index<8; index++,p++)
    {
       if (*p==0) break;
       SetChar(index, *p);
    }
    for (; index<8; index++)
       SetChar(index, ' ');

    Upload();
}

