# Compiler
CC = g++
CFLAGS = -Wall -g


# Folders
SRC = src
FTL = $(SRC)/ftl
FTLF = $(FTL)/ftl_functions
INC = $(SRC)/include
JSONF = $(SRC)/json_functions
OSF = $(SRC)/os_functions
BIN = bin
BUILD = build
LIB = lib

# Files
SRCS := $(wildcard $(FTLF)/*.c $(OSF)/*.c $(JSONF)/*.cpp)
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.cpp=.o)
LIBA = libftlib.a

# Compile
all: dircheck $(LIBA)


%.o: %.c
	$(CC) -c $(CFLAGS) $^
	mv *.o build/

$(LIBA): $(OBJS)
	echo $^
	ar cr $@ build/*.o
	mv $@ lib/

test: $(LIBA)
	$(CC) $(CFLAGS) main/linux_test.cpp -I $(INC) -lftlib -L $(LIB) -o $(BIN)/test

dircheck:
ifeq ("$(wildcard $(BIN))", "")
	@echo -n 'Creating bin/ folder -> '
	@ mkdir $(BIN)
	@echo done
endif
ifeq ("$(wildcard $(BUILD))", "")
	@echo -n 'Creating build/ folder -> '
	@ mkdir $(BUILD)
	@echo done
endif
ifeq ("$(wildcard $(LIB))", "")
	@echo -n 'Creating lib/ folder -> '
	@ mkdir $(LIB)
	@echo done
endif