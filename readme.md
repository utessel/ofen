Ofen
----
(Ofen is german for oven)

The real oven is a toy oven made of wood.

Hardware
--------
I added:
- four Neopixel rings (16 RGBW LEDs) as gas burner on the top
- two Neopixel (8 RGBW LEDs) strips inside
- eight 14-segment displays to show texts
- two incremental rotary encoders
- a 3W I2S based audio board
- a Raspberry PI Zero W to connect everything

Connections
-----------
- The Neopixels are controlled via SPI
- The segment displays are controlles via I2C
- Audio is connected vie I2S
- the rotary encoders are connected by GPIO
  (and converted to Linux input devices)

Code
----
This repo contains the C++ program that contains the menu
(user interface), controls the LEDs (based on TGA Image files) 
the Display, etc.

Note:
The code for controlling the NeoPixels via SPI needs a fix SPI frequency.
The raspberry derives the frequency from the core frequency and that can
change: then the timing will not work anymore. 
To avoid this, add 
core_freq=250
to the config.txt


