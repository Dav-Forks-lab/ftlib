# Compiler settings
CC = gcc
CFLAGS = -Wall -Os -g
TFLAG = -lpthread


# Folders
FUNC = src/functions
HEAD = src/head
BUILD = build
BIN = bin
LIB = lib
TEST = test


# Lib files
ifeq ($(OS), Windows_NT)
	TESTFILE = $(TEST)/win_test.c
	TESTEXE = $(BIN)/test.exe
	LIBFILE = $(LIB)/libftlib.dll
else
	TESTFILE = $(TEST)/linux_test.c
	TESTEXE = $(BIN)/test
	LIBFILE = $(LIB)/libftlib.so
endif

HEADERS := $(HEAD)/*.h
SRCS := $(wildcard $(FUNC)/*.c $(HEADERS))
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.c=.o)))

all: $(LIBFILE)

# Create lib file
$(LIBFILE): $(OBJS)
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $^

$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $^
ifeq ($(OS), Windows_NT)
	move *.o $(BUILD)
else
	mv *.o $(BUILD)
endif


#Create test files
test: $(TESTFILE) $(OBJS)
	$(CC) $(CFLAGS) -o $(TESTEXE) $? -L$(LIB) -lftlib $(TFLAG)


# Clear folders
clean:
ifeq ($(OS), Windows_NT)
	del /q /f $(BIN)\*.* $(BUILD)\*.* $(LIB)\*.*
else
	rm $(BIN)/* $(BUILD)/* $(LIB)/*
endif
