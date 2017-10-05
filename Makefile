all: build_and_test

build_and_test: Core
	make test

test:
	make -C src/Tests

Core: ConfigParser Protocol
	make -C src/Core

Protocol:
	make -C src/Protocol

ConfigParser:
	make -C src/ConfigParser

clean:
	find . -name "*.o" -exec rm '{}' ';'
