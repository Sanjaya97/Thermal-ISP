# C source code
CSRC	= io_png/io_png.c \

# C++ source code
CXXSRC	= nuc/nuc.cpp \
	nuc/borders.cpp \
	bpc/bpc.cpp \
	nlm/nlm.cpp \
	denoise/denoise.cpp \
	denoise/DCTdenoising.cpp \
	denoise/DCT2D16x16.cpp \
	denoise/DCT2D.cpp \
	bfd/bfd.cpp \
	eeh/eeh.cpp \
	bcc/bcc.cpp \
		thermal_isp.cpp
# all source code
SRC	= $(CSRC) $(CXXSRC)

# C objects
COBJ	= $(CSRC:.c=.o)
# C++ objects
CXXOBJ	= $(CXXSRC:.cpp=.o)
# all objects
OBJ	= $(COBJ) $(CXXOBJ)
# binary target
BIN	= thermal_isp

default	: $(BIN)

# C optimization flags
COPT	= -O3 -ftree-vectorize -funroll-loops

# C++ optimization flags
CXXOPT	= $(COPT)

# C compilation flags
CFLAGS	= $(COPT) -Wall -Wextra \
	-Wno-write-strings -ansi -fpermissive
# C++ compilation flags
CXXFLAGS	= $(CXXOPT) -Wall -Wextra \
	-Wno-write-strings -Wno-deprecated -ansi 
# link flags
LDFLAGS	= -L/home/sahan/Desktop/Cyber/Codes/srcmire_2/libpng/lib -lpng -lm



# use openMP with `make OMP=1`
ifdef OMP
CFLAGS	+= -fopenmp
CXXFLAGS	+= -fopenmp
LDFLAGS += -lgomp
else
CFLAGS	+= -Wno-unknown-pragmas
CXXFLAGS  += -Wno-unknown-pragmas
endif


# use DEBUG
ifdef DEBUG
CFLAGS	+= -g
CXXFLAGS	+= -g
LDFLAGS += -g
endif
# build the local png library
.PHONY	: libpng
libpng	:
	$(MAKE) -C libpng libpng

# partial compilation of C source code
%.o: %.c %.h
	$(CC) -c -o $@  $< $(CFLAGS)
# partial compilation of C++ source code
%.o: %.cpp %.h
	$(CXX) -c -o $@  $< $(CXXFLAGS)

# link all the opject code
$(BIN): $(OBJ) $(LIBDEPS)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# housekeeping
.PHONY	: clean distclean
clean	:
	$(RM) $(OBJ)
	$(MAKE) -C ./libpng $@
distclean	: clean
	$(RM) $(BIN)
	$(MAKE) -C ./libpng $@
