.POSIX:

BIN_DIR = /usr/local/bin
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CC     = cc
CFLAGS = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pedantic -O2

NAME    = tide
VERSION = 7.0
LDLIBS = -s -lncurses

BIN = src/$(NAME)
