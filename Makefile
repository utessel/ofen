ARCH?=arm-linux-gnueabi-
CXXFLAGS=-std=c++11 -Wall -ggdb -MMD
CXX=$(ARCH)g++
CC=$(ARCH)g++
LD=$(ARCH)gcc

all: ofen
#	ssh ofen killall ofen || true
#	scp ./ofen ofen:/tmp
#	ssh ofen /tmp/ofen
	
ofen: main.o input.o display.o ofen.o clock.o menu.o wecker.o timer.o states.o countdown.o kochen.o leds.o brenner.o ofenoben.o ofenunten.o

.PHONY: clean
clean:
	-rm -f *.o *.d ofen

%.c: %.tga
	echo "unsigned char $(basename $<)_tga[] = {" >$@
	convert $< -compress None -alpha activate tga:- | xxd -i - >>$@
	echo "};" >>$@

-include *.d
