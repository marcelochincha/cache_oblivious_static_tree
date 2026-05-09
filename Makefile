
CXX ?= g++
SRC_DIR := src
BUILD_DIR := build
TARGET := cache_oblivious_bst

SOURCES := $(SRC_DIR)/main.cpp
OBJECTS := $(BUILD_DIR)/main.o

CXXFLAGS ?= -O3 -DNDEBUG -std=c++17 -Wall -Wextra -Wshadow -Wpedantic
LDFLAGS ?=

ifeq ($(OS),Windows_NT)
EXE := .exe
RM := del /Q
RMDIR := rmdir /S /Q
else
EXE :=
RM := rm -f
RMDIR := rm -rf
endif

.PHONY: all clean run

all: $(TARGET)$(EXE)

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/flat_bst.hpp $(SRC_DIR)/veb_bst.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET)$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

run: $(TARGET)$(EXE)
	./$(TARGET)$(EXE)

clean:
	-$(RM) $(TARGET)$(EXE)
	-$(RMDIR) $(BUILD_DIR)
