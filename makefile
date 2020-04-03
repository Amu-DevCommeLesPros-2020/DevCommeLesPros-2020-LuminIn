.DEFAULT_GOAL := all

.PHONY: clean

clean:
	rm -rf build

build:
	mkdir -p build

liblumi: lib/bd.c lib/bd.h lib/journal.c lib/journal.h lib/liste.h lib/liste.c | build
	gcc -Wall -Werror -pedantic -g -c lib/bd.c -o build/bd.o
	gcc -Wall -Werror -pedantic -g -c lib/liste.c -o build/liste.o
	gcc -Wall -Werror -pedantic -g -c lib/journal.c -o build/journal.o
	ar crs build/liblumi.a build/bd.o build/liste.o build/journal.o

luminin: | build

test: liblumi test/main.c | build
	gcc -Wall -Werror -pedantic -g -I lib -L build -l lumi test/main.c -o build/test

all: luminin test
