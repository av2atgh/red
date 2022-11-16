# the compiler: gcc for C program, define as g++ for C++

CC = gcc

# compiler flags:
#  -O2    all supported optimizations that do not involve a space-speed tradeoff
#  -Wall turns on most, but not all, compiler warnings

LIB = "$(PWD)/libs"
CFLAGS  = -O2 -Wall -I$(LIB)

# the build target executable:

TARGET0 = model
TARGET1 = red
TARGET2 = topology
TARGET3 = generators
TARGET4 = processes
TARGET5 = rand

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET0)

$(TARGET1):
	$(CC) $(CFLAGS) -o $(TARGET1).o -c $(LIB)/$(TARGET1).c

$(TARGET2):
	$(CC) $(CFLAGS) -o $(TARGET2).o -c $(LIB)/$(TARGET2).c

$(TARGET3):
	$(CC) $(CFLAGS) -o $(TARGET3).o -c $(LIB)/$(TARGET3).c

$(TARGET4):
	$(CC) $(CFLAGS) -o $(TARGET4).o -c $(LIB)/$(TARGET4).c

$(TARGET5):
	$(CC) $(CFLAGS) -o $(TARGET5).o -c $(LIB)/$(TARGET5).c

$(TARGET0): red.o topology.o generators.o processes.o rand.o
	$(CC) $(CFLAGS) -o $@ $@.c red.o topology.o generators.o processes.o rand.o

clean:
	$(RM) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)
