.DEFAULT_GOAL := all

.PHONY: clean

clean:
	rm -rf build

build:
	mkdir -p build

luminin: | build

test: | build

all: luminin test
