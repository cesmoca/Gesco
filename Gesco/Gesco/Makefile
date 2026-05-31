# Gesture Recognition's Makefile
# Cesar Mora Castro
# 2013 - University of Minnesota

LIBDIR = ./lib/
LIBNAME = libgesturerecognizer.so
OBJSDIR = ./obj/
INCDIR = ./include/
SRCDIR = ./source/
CXX = g++
CXXFLAGS = -Wall -c -fPIC -O2

# Compilation mode
ifeq ($(mode), release) # Release mode
	CXXFLAGS += -O2
else # Debug mode
	CXXFLAGS += -ggdb
endif

# Libraries
LDFLAGS = `pkg-config --libs sdl`  `pkg-config --libs opencv4` 

# Includes
INCFLAGS = -I$(INCDIR) `pkg-config --cflags sdl`  `pkg-config --cflags opencv4`

# Objects
OBJSCRUDE = HandDetector.o GestureRecognizer.o Recognizer.o Hand.o\
	DebugController.o

OBJS= $(addprefix $(OBJSDIR),$(OBJSCRUDE))

# Rules
all:  dirs $(OBJS) tool

lib: dirs  $(OBJS)
	$(CXX) $(OBJS) -Wall $(INCFLAGS) -shared -o $(LIBDIR)/$(LIBNAME) $(LDFLAGS)

tool: 
	cd tool; $(MAKE)	

install: $(LIBDIR)/$(LIBNAME)
	cp $(LIBDIR)/$(LIBNAME) /usr/lib/
	ldconfig

#Controller objs
$(OBJSDIR)%.o: $(SRCDIR)%.cpp $(INCDIR)%.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS)


#Controller objs
$(OBJSDIR)%.o: $(SRCDIR)Controllers/%.cpp $(INCDIR)Controllers/%.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS)

#Kernel objs
$(OBJSDIR)%.o: $(SRCDIR)Kernel/%.cpp $(INCDIR)Kernel/%.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS)

clean:
	rm -Rf $(OBJSDIR) $(LIBDIR) *~

dirs:
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJSDIR)
