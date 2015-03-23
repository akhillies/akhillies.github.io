CC = g++ -std=c++11 -stdlib=libc++

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif

RM = /bin/rm -f


all: main

main: ass4.o
	$(CC) $(CFLAGS) -o as4 ass4.o joint.o system.o $(LDFLAGS)

ass4.o: ass4.cpp joint.o system.o
	$(CC) $(CFLAGS) -c ass4.cpp -o ass4.o

system.o: src/system.cpp
	$(CC) -c src/system.cpp -o system.o

joint.o: src/joint.cpp
	$(CC) -c src/joint.cpp -o joint.o

clean:
	$(RM) *.o as4
 


