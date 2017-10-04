all: build_and_test

build_and_test: Core
	make test

test:
	make -C src/Tests

Core: ConfigParser
	make -C src/Core

ConfigParser:
	make -C src/ConfigParser

clean:
	find . -name "*.o" -exec rm '{}' ';'
