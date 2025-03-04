# Makefile
CC = g++
CFLAGS = -Wall -Werror -g -std=c++11 
LD_FLAGS = 

TARGET = raytracer

.PHONY: clean all

all: raytracer

raytracer: raytracer.o primatives.o
	$(CC) $(CFLAGS) $^ -o $@

raytracer.o: raytracer.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

primatives.o: primatives.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

run: raytracer
	./raytracer >> img.ppm

clean:
	rm -rf raytracer *.o img.ppm