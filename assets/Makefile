# Makefile to convert all .bmp files to 16-bit .img using grit-gba

GRIT := grit-gba
GRITFLAGS := -gB16 -gb -gTFF00FF -ftB -p! -m! -fh!

# Find all .bmp files
BMP_FILES := $(wildcard *.bmp)
IMG_FILES := $(BMP_FILES:.bmp=.img)

# Default rule
all: $(IMG_FILES)

# Rule to convert each BMP → IMG
%.img: %.bmp
	$(GRIT) $< $(GRITFLAGS) -o $*

# Clean generated files
clean:
	rm -f *.img *.img.bin

.PHONY: all clean
