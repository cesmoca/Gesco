all: calcBackProject.cpp
	g++ calcBackProject.cpp `pkg-config --cflags --libs opencv4` -o calc
clean:
	rm -Rf calc
	rm -Rf *~ *.yml
