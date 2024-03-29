CFLAGS = -Wall -Wextra -pedantic -Werror -g

.DEFAULT_GOAL := all

.PHONY: clean all

clean:
	rm -rf build

build:
	mkdir -p build

# Bibliothèque de journal.
build/libjournal.a: lib/journal/journal.c lib/journal/journal.h | build
	mkdir -p build/lib
	gcc $(CFLAGS) -c lib/journal/journal.c -o build/lib/journal.o
	ar crs build/libjournal.a build/lib/journal.o

# Bibliothèque de liste.
build/libliste.a: lib/liste/liste.c lib/liste/liste.h | build
	mkdir -p build/lib
	gcc $(CFLAGS) -c lib/liste/liste.c -o build/lib/liste.o
	ar crs build/libliste.a build/lib/liste.o

# Bibliothèque des entités et d'interface de l'application.
build/libluminin.a: lib/luminin/bd.c lib/luminin/bd.h lib/luminin/constantes.h lib/luminin/chercheur.c lib/luminin/chercheur.h lib/luminin/employe.c lib/luminin/employe.h lib/luminin/entreprise.c lib/luminin/entreprise.h lib/luminin/luminin.c lib/luminin/luminin.h lib/luminin/poste.c lib/luminin/poste.h | build
	mkdir -p build/lib
	gcc $(CFLAGS) -I lib -c lib/luminin/bd.c -o build/lib/bd.o
	gcc $(CFLAGS) -I lib -c lib/luminin/chercheur.c -o build/lib/chercheur.o
	gcc $(CFLAGS) -I lib -c lib/luminin/employe.c -o build/lib/employe.o
	gcc $(CFLAGS) -I lib -c lib/luminin/entreprise.c -o build/lib/entreprise.o
	gcc $(CFLAGS) -I lib -c lib/luminin/luminin.c -o build/lib/luminin.o
	gcc $(CFLAGS) -I lib -c lib/luminin/poste.c -o build/lib/poste.o
	ar crs build/libluminin.a build/lib/bd.o build/lib/chercheur.o build/lib/employe.o build/lib/entreprise.o build/lib/luminin.o build/lib/poste.o

# Application.
build/luminin: build/libjournal.a build/libliste.a build/libluminin.a bin/main.c | build
	gcc $(CFLAGS) bin/main.c -o build/luminin -I lib -L build -l luminin -l liste -l journal

# Programme de test.
build/test: build/libjournal.a build/libliste.a build/libluminin.a test/main.c | build
	gcc $(CFLAGS) test/main.c -o build/test -I lib -L build -l luminin -l liste -l journal

all: build/luminin build/test

check: build/test
	cd build; ./test
