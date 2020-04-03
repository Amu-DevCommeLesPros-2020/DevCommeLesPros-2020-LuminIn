.DEFAULT_GOAL := all

.PHONY: clean

clean:
	rm -rf build

build:
	mkdir -p build

liblumi: lib/journal.h lib/journal.c | build
	gcc -Wall -Werror -pedantic -g -c lib/journal.c -o build/journal.o
	ar crs build/liblumi.a build/journal.o

luminin: | build

test: liblumi test/main.c | build
	gcc -Wall -Werror -pedantic -g -I lib -L build -l lumi test/main.c -o build/test

all: luminin test
