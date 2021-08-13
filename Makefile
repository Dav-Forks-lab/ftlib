# Compiler settings
CC = gcc
CFLAGS = -Wall -Os -g
TFLAGS = -lpthread


# Folders
FUNC = src/functions
HEAD = src/head
BUILD = build
BIN = bin
TEST = test


# Lib files
ifeq ($(OS), Windows_NT)
	TESTFILE = $(TEST)/win_test.c
	TESTEXE = $(BIN)/test
	LIBFILE = $(BIN)\libftlib.dll
else
	TESTFILE = $(TEST)/linux_test.c
	TESTEXE = $(BIN)/test
	LIBFILE = $(BIN)/libftlib.so
endif

HEADERS := $(HEAD)/*.h
SRCS := $(wildcard $(FUNC)/*.c $(HEADERS))
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.c=.o)))


all: $(LIBFILE)
.PHONY: test


# Create lib file
$(LIBFILE): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^


# Create object files
$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $^
ifeq ($(OS), Windows_NT)
	move *.o $(BUILD)
else
	mv *.o $(BUILD)
endif


#Create test file
test: $(TESTFILE) $(LIBFILE)
	$(CC) $(CFLAGS) -o $(TESTEXE) $? $(TFLAGS)


# Clear folders
clean:
ifeq ($(OS), Windows_NT)
	del /q /f $(BIN)\*.* $(BUILD)\*.* $(LIB)\*.*
else
	rm $(BIN)/* $(BUILD)/* $(LIB)/*
endif


# Clear bin folder
tclean:
ifeq ($(OS), Windows_NT)
	del /q /f $(BIN)\*.*
else
	rm $(BIN)/* $(BUILD)/*
endif