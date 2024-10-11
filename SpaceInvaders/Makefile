m := main
compile: main.c
	gcc $(m).c -o $(m) -lm -lraylib
run:
	./$(m)

.PHONY: clean
clean:
	rm -f main
