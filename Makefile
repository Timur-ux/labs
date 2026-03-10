flags ?=
libdir ?= build
sources = $(wildcard ./src/*.c)
objects = $(patsubst ./src/%.c,$(libdir)/%.o,$(sources)) # что меняем,на что меняем,где меняем

all: $(libdir) $(libdir)/liblabs.a $(libdir)/liblabs.so

$(libdir)/liblabs.a: $(objects)
	ar rcs $@ $^

$(libdir)/liblabs.so: $(objects)
	gcc -shared $^ -o $@ $(flags)

$(libdir)/%.o: ./src/%.c
	gcc -c $< -o $@ -Iinclude -fPIC $(flags)

$(libdir):
	mkdir -p $(libdir)
