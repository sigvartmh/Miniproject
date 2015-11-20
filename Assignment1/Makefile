# program executable name
TARGET = run
CC=/home/s/Downloads/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc

# compiler flags
CFLAGS = -g -std=c99 -Wall -D_GNU_SOURCE

# linker flags
LDFLAGS = -L/usr/lib \
	-L/usr/local/lib \
	-pthread \

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# compile
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
