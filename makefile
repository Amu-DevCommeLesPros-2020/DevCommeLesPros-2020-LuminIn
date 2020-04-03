.DEFAULT_GOAL := all

.PHONY: clean all

clean:
	rm -rf build

build:
	mkdir -p build

build/liblumi.a: lib/bd.c lib/bd.h lib/compagnie.h lib/compagnie.c lib/journal.c lib/journal.h lib/liste.h lib/liste.c lib/types.h | build
	gcc -Wall -Werror -pedantic -g -c lib/bd.c -o build/bd.o
	gcc -Wall -Werror -pedantic -g -c lib/compagnie.c -o build/compagnie.o
	gcc -Wall -Werror -pedantic -g -c lib/liste.c -o build/liste.o
	gcc -Wall -Werror -pedantic -g -c lib/journal.c -o build/journal.o
	ar crs build/liblumi.a build/bd.o build/compagnie.o build/liste.o build/journal.o

build/luminin: | build

build/test: build/liblumi.a test/main.c | build
	gcc -Wall -Werror -pedantic -g -I lib -L build -l lumi test/main.c -o build/test

all: build/luminin build/test
