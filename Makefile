CPP = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -Werror -O3

TARGET = main

SOURCES = main.cpp trie.cpp

build: $(TARGET)

$(TARGET):
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(SOURCES)

run: build
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
