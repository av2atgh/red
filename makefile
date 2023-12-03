# the compiler: gcc for C program, define as g++ for C++

CC = gcc

# compiler flags:
#  -O2    all supported optimizations that do not involve a space-speed tradeoff
#  -Wall turns on most, but not all, compiler warnings

TSU = "/Users/avazquez/nl/mono/libs/models/tsunami/containers/tsunami"
SRC = "/Users/avazquez/av2atg/red/libs"

CFLAGS  = -O2 -Wall -I$(SRC)

# the build target executable:

TARGET1 = red
TARGET2 = rand
TARGET3 = topology
TARGET4 = generators
TARGET5 = io
DUPSPLIT = dupsplit
REAL = real
DUPSPLIT_SITE_PERCOLATION = dupsplit_site_percolation
DUPSPLIT_GENERATION = dupsplit_generation

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(DUPSPLIT) $(REAL) $(DUPSPLIT_SITE_PERCOLATION) $(DUPSPLIT_GENERATION)

$(TARGET1):
	$(CC) $(CFLAGS) -o $(TARGET1).o -c $(SRC)/$(TARGET1).c

$(TARGET2):
	$(CC) $(CFLAGS) -o $(TARGET2).o -c $(SRC)/$(TARGET2).c

$(TARGET3): red.o rand.o
	$(CC) $(CFLAGS) -o $(TARGET3).o -c $(SRC)/$(TARGET3).c

$(TARGET4): red.o rand.o
	$(CC) $(CFLAGS) -o $(TARGET4).o -c $(SRC)/$(TARGET4).c

$(TARGET5): red.o
	$(CC) $(CFLAGS) -o $(TARGET5).o -c $(SRC)/$(TARGET5).c

$(DUPSPLIT): red.o rand.o io.o generators.o topology.o
	$(CC) $(CFLAGS) -o $@ $@.c red.o rand.o io.o generators.o topology.o

$(REAL): red.o rand.o io.o topology.o
	$(CC) $(CFLAGS) -o $@ $@.c red.o rand.o io.o topology.o

$(DUPSPLIT_SITE_PERCOLATION): red.o rand.o generators.o topology.o
	$(CC) $(CFLAGS) -o $@ $@.c red.o rand.o generators.o topology.o

$(DUPSPLIT_GENERATION): red.o rand.o generators.o topology.o
	$(CC) $(CFLAGS) -o $@ $@.c red.o rand.o generators.o topology.o

clean:
	$(RM) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(DUPSPLIT) $(REAL) $(DUPSPLIT_SITE_PERCOLATION) $(DUPSPLIT_GENERATION)
