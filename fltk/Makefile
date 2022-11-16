CXX=clang++

CXXFLAGS+=$(shell fltk-config --cxxflags --use-images)
LDFLAGS+=$(shell fltk-config --ldflags --use-images)

all: build

out:
	mkdir -p out/

build: src/main.cxx
	${CXX} ${CXXFLAGS} ${LDFLAGS} src/main.cxx -o out/main 

run: build
	./out/main

clean:
	rm -rf out/

define BEST_PRACTICE
lines
...
CXX      = $(shell fltk-config --cxx)
DEBUG    = -g
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I.
LDFLAGS  = $(shell fltk-config --use-gl --use-images --ldflags )
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK     = $(CXX)
 
TARGET = cube
OBJS = CubeMain.o CubeView.o CubeViewUI.o
SRCS = CubeMain.cxx CubeView.cxx CubeViewUI.cxx
 
.SUFFIXES: .o .cxx
%.o: %.cxx
        $(CXX) $(CXXFLAGS) $(DEBUG) -c $<
 
all: $(TARGET)
        $(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)
 
$(TARGET): $(OBJS)
CubeMain.o: CubeMain.cxx CubeViewUI.h
CubeView.o: CubeView.cxx CubeView.h CubeViewUI.h
CubeViewUI.o: CubeViewUI.cxx CubeView.h
 
clean: $(TARGET) $(OBJS)
        rm -f *.o 2> /dev/null
        rm -f $(TARGET) 2> /dev/null
endef