#####################################################################
#
# Copyright(c) 2009  Xiaoyong Guo
#
# 
# Makefile for LeeKweed ConLeeKweed, bmp2c, GNU make is needed
#
#####################################################################


DEBUG = #-ggdb3
OPTIM = -O3
PROF = #-pg
GCOV = #-fprofile-arcs -ftest-coverage

CC = gcc
CPPFLAGS = -I. `sdl-config --cflags`
CFLAGS =  -Wall -Wextra $(OPTIM) $(DEBUG) $(PROF) $(GCOV)
LOADLIBES = `sdl-config --static-libs` $(OPTIM) $(DEBUG) $(PROF) $(GCOV)

all: LeeKweed 

LeeKweed: LeeKweed.o heightmap.o tcimg.o image.o 

ConLeeKweed: ConLeeKweed.c heightmap.c tcimg.c conimage.o
	gcc -o $@  -I. $^

Crayonizer: Crayonizer.c heightmap.c tcimg.c
	gcc -o $@ -I. $(DEBUG) $^

bmp2c: bmp2c.c tcimg.c
	gcc -o $@  -I. $(DEBUG) $^

image.o: image.c
	gcc -c -I. $<

conimage.o: conimage.c
	gcc -c -I. $<

image.c: bmp2c bg.bmp 
	./bmp2c bg.bmp TCBGImage updown > image.c

conimage.c: bmp2c bg.bmp 
	./bmp2c bg.bmp TCBGImage > conimage.c



#res.o: res.rc
#	windres -I. $< $@



clean:
	rm -rf *.o *.exe *.orig image.c conimage.c *.gcno 0*.bmp *.out


