# Test app for Gesture Recognizer - Makefile
# Cesar Mora Castro
# 2013 - University of Minnesota

OUTPUTDIR = ./bin/
OBJSDIR = ./obj/
SRCDIR = ./src/
CXX = g++
CXXFLAGS = -Wall -c -O2
SDL_LIBS = $(shell pkg-config --libs sdl3)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)
SDL_CFLAGS = $(shell pkg-config --cflags sdl3)
OPENCV_CFLAGS = $(shell pkg-config --cflags opencv4)
CONSOLE_LDFLAGS = -mconsole

# Compilation mode
ifeq ($(mode), release) # Release mode
	CXXFLAGS += -O2
else # Debug mode
	CXXFLAGS += -ggdb
endif

# Libraries
# Temporarily disabled while isolating startup/runtime behavior.
# Re-enable these incrementally as main.cpp starts using SDL/OpenCV/Gesco again.
# LDFLAGS = $(CONSOLE_LDFLAGS) $(SDL_LIBS) $(OPENCV_LIBS)
LDFLAGS = $(CONSOLE_LDFLAGS) $(SDL_LIBS) $(OPENCV_LIBS)

# Includes
# Temporarily disabled while main.cpp is reduced to standard-library code.
# INCFLAGS = $(SDL_CFLAGS) $(OPENCV_CFLAGS) -IGesco/include
INCFLAGS = $(SDL_CFLAGS) $(OPENCV_CFLAGS) -IGesco/include

# Objects
# Full application objects, temporarily disabled while isolating startup.
# OBJSCRUDE = EndController.o InputEventController.o Logger.o VideoFactory.o\
# 			VideoSource.o World.o main.o
# OBJSLIB = $(wildcard Gesco/obj/*.o)
# Current incremental app objects.
# Add objects here as main.cpp starts using their functionality again.
OBJSCRUDE = EndController.o InputEventController.o VideoFactory.o VideoSource.o\
			Logger.o World.o main.o
OBJSLIB = Gesco/obj/Recognizer.o Gesco/obj/HandDetector.o\
		  Gesco/obj/GestureRecognizer.o Gesco/obj/DebugController.o

OBJS= $(addprefix $(OBJSDIR),$(OBJSCRUDE))

all: credentials dirs $(OUTPUTDIR)/test

# Make the lib Gesco
lib:
	cd Gesco; $(MAKE) lib

$(OUTPUTDIR)/test: $(OBJS) $(OBJSLIB)
	$(CXX) $(OBJS) $(OBJSLIB) -Wall $(INCFLAGS) -o $(OUTPUTDIR)/test $(LDFLAGS)

Gesco/obj/Recognizer.o:
	mkdir -p Gesco/obj
	cd Gesco; $(MAKE) obj/Recognizer.o

Gesco/obj/HandDetector.o:
	mkdir -p Gesco/obj
	cd Gesco; $(MAKE) obj/HandDetector.o

Gesco/obj/GestureRecognizer.o:
	mkdir -p Gesco/obj
	cd Gesco; $(MAKE) obj/GestureRecognizer.o

Gesco/obj/DebugController.o:
	mkdir -p Gesco/obj
	cd Gesco; $(MAKE) obj/DebugController.o


$(OBJSDIR)main.o: $(SRCDIR)main.cpp Makefile
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS)


$(OBJSDIR)%.o: $(SRCDIR)%.cpp $(SRCDIR)%.h Makefile
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS)

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
