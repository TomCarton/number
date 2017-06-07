# Makefile

TARGET := number

CC=clang
CC_FLAGS=-Wall -O3

all: number

number: number.c
	$(CC) $(CC_FLAGS) number.c -o $(TARGET)

install: number
	cp number /usr/local/bin
	
.PHONY: clean
clean:
	rm $(TARGET)
