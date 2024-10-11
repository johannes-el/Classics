.PHONY: clean

compile: main.c
	gcc main.c \
		-o game \
		-I../deps/include \
		-L../deps/lib/linux -lraylib -Werror -Wall -lm

clean:
	rm -f game
