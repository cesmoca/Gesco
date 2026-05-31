# Test app for Gesture Recognizer - Makefile
# Cesar Mora Castro
# 2013 - University of Minnesota

OUTPUTDIR = ./bin/
OBJSDIR = ./obj/
SRCDIR = ./src/
CXX = g++
CXXFLAGS = -Wall -c -O2
OBJSLIB = $(wildcard Gesco/obj/*.o)

# Compilation mode
ifeq ($(mode), release) # Release mode
	CXXFLAGS += -O2
else # Debug mode
	CXXFLAGS += -ggdb
endif

# Libraries
LDFLAGS = `pkg-config --libs sdl`  `pkg-config --libs opencv4`

# Includes
INCFLAGS = `pkg-config --cflags sdl` `pkg-config --cflags opencv4` -IGesco/include

# Objects
OBJSCRUDE = EndController.o InputEventController.o Logger.o VideoFactory.o\
			VideoSource.o World.o main.o

OBJS= $(addprefix $(OBJSDIR),$(OBJSCRUDE))

# Rules
all: credentials dirs libs $(OUTPUTDIR)/test

# Make the lib Gesco
	cd Gesco; $(MAKE) lib

$(OUTPUTDIR)/test: $(OBJS)
	$(CXX) $(OBJS) $(OBJSLIB) -Wall $(INCFLAGS) -o $(OUTPUTDIR)/test $(LDFLAGS)


$(OBJSDIR)main.o: $(SRCDIR)main.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS)


$(OBJSDIR)%.o: $(SRCDIR)%.cpp $(SRCDIR)%.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS)

install:
	cd Gesco; $(MAKE) install

clean:
	rm -Rf $(OBJSDIR) $(OUTPUTDIR) *~
	rm -Rf log
	cd Gesco; $(MAKE) clean


# Build gesture recognizer
libs:
	cd Gesco; $(MAKE) $(mode)

credentials:
	@echo
	@echo '--------------------------------------'
	@echo ' Gesture Recognition Makefile'
	@echo ' Cesar Mora Castro - 2013 University of Minnesota'
	@echo ' Compiling $(mode) mode'
	@echo '--------------------------------------'
	@echo

dirs:
	mkdir -p $(OUTPUTDIR)
	mkdir -p $(OBJSDIR)
