.DEFAULT_GOAL := all

.PHONY: clean all

clean:
	rm -rf build

build:
	mkdir -p build

build/libjournal.a: lib/journal/journal.c lib/journal/journal.h | build
	mkdir -p build/journal
	gcc -Wall -Werror -pedantic -g -c lib/journal/journal.c -o build/journal/journal.o
	ar crs build/libjournal.a build/journal/journal.o

build/libliste.a: lib/liste/liste.c lib/liste/liste.h | build
	mkdir -p build/liste
	gcc -Wall -Werror -pedantic -g -c lib/liste/liste.c -o build/liste/liste.o
	ar crs build/libliste.a build/liste/liste.o

build/libluminin.a: lib/luminin/bd.c lib/luminin/bd.h lib/luminin/chercheur.c lib/luminin/chercheur.h lib/luminin/employe.c lib/luminin/employe.h lib/luminin/entreprise.c lib/luminin/entreprise.h lib/luminin/poste.c lib/luminin/poste.h lib/luminin/types.h | build
	mkdir -p build/luminin
	gcc -Wall -Werror -pedantic -g -I lib -c lib/luminin/bd.c -o build/luminin/bd.o
	gcc -Wall -Werror -pedantic -g -I lib -c lib/luminin/chercheur.c -o build/luminin/chercheur.o
	gcc -Wall -Werror -pedantic -g -I lib -c lib/luminin/employe.c -o build/luminin/employe.o
	gcc -Wall -Werror -pedantic -g -I lib -c lib/luminin/entreprise.c -o build/luminin/entreprise.o
	gcc -Wall -Werror -pedantic -g -I lib -c lib/luminin/poste.c -o build/luminin/poste.o
	ar crs build/libluminin.a build/luminin/bd.o build/luminin/chercheur.o build/luminin/employe.o build/luminin/entreprise.o build/luminin/poste.o

build/luminin: | build

build/test: build/libjournal.a build/libliste.a build/libluminin.a test/main.c | build
	gcc -Wall -Werror -pedantic -g -I lib -L build -l luminin -l liste -l journal test/main.c -o build/test

all: build/luminin build/test
