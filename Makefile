CXX = g++
CFLAGS = -Wall -Wextra -Iinclude
CDEBUG = -g

SRC_DIR = src


# All source files
SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)

#output will be main
TARGET = simplelang 

$(TARGET) : $(SOURCES)
	$(CXX) $(CFLAGS) $(CDEBUG) -o $@ $^

clean:
	rm main

doxygen:
	doxygen Doxyfile

.PHONY: doxygen
