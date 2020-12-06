.POSIX:

BIN_DIR = /usr/local/bin
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CC     = cc
CFLAGS = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pedantic -O2
BIN    = src/main

LDLIBS = -s -lncurses -lpthread
NAME    = tide
VERSION = 10.0
