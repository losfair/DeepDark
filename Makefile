all: build_and_test

build_and_test: Core
	make test

test:
	make -C src/Tests

Core: ConfigParser Protocol
	make -C src/Core

ConfigParser:
	make -C src/ConfigParser

Protocol:
	make -C src/Protocol

clean:
	find . -name "*.o" -exec rm '{}' ';'
