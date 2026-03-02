libdir ?= build
src = $(wildcard ./src/*.c)
objects = $(patsubst ./src/%.c,$(libdir)/objects/%.o,$(src))

all: $(libdir)/objects lib

lib: $(objects)
	ar rcs $(libdir)/liblabs.a $^ 

$(libdir)/objects:
	mkdir -p $(libdir)/objects

$(libdir)/objects/%.o: ./src/%.c
	gcc -c $< -o $@ -I./include
