CC:=gcc
FLAGS:= -std=c11 -fshort-enums  -Wall -Wpedantic -g
INCLUDE:= -I./include
C_SRC:= $(wildcard ./src/*.c)

ENC_SRC:= $(wildcard ./src/encoding*.c)

TARGET:=./bin/prog



all:
	$(CC) $(C_SRC) $(FLAGS) $(INCLUDE) -o $(TARGET) && $(TARGET)