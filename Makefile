CXXFLAGS=-std=c++11 -Wall -ggdb -MMD
CXX=arm-linux-gnueabi-g++
CC=arm-linux-gnueabi-g++
LD=arm-linux-gnueabi-gcc

all: ofen
#	ssh ofen killall ofen || true
#	scp ./ofen ofen:/tmp
#	ssh ofen /tmp/ofen
	
ofen: main.o input.o display.o ofen.o clock.o menu.o wecker.o timer.o states.o countdown.o kochen.o leds.o brenner.o ofenoben.o ofenunten.o

.PHONY: clean
clean:
	-rm -f *.o *.d ofen

brenner.c: brenner.tga
	echo "unsigned char brenner_tga[] = {" >brenner.c
	convert brenner.tga - | xxd -i - >>brenner.c
	echo "};" >>brenner.c

ofenoben.c: ofenoben.tga
	echo "unsigned char ofenoben_tga[] = {" >ofenoben.c
	convert ofenoben.tga - | xxd -i - >>ofenoben.c
	echo "};" >>ofenoben.c

ofenunten.c: ofenunten.tga
	echo "unsigned char ofenunten_tga[] = {" >ofenunten.c
	convert ofenunten.tga - | xxd -i - >>ofenunten.c
	echo "};" >>ofenunten.c

-include *.d
