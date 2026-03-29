.PHONY: build
build: src/main.c src/configparser.c
	clang src/main.c src/configparser.c -o bin/termark

.PHONY: clean
clean:
	rm bin/termark
