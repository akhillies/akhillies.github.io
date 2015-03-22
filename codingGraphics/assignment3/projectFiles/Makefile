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

main: ass3.o
	$(CC) $(CFLAGS) -o as3 ass3.o vec3.o curve.o patch.o triangle.o $(LDFLAGS)

ass3.o: ass3.cpp vec3.o curve.o patch.o triangle.o
	$(CC) $(CFLAGS) -c ass3.cpp -o ass3.o

triangle.o: src/triangle.cpp
	$(CC) -c src/triangle.cpp -o triangle.o

patch.o: src/patch.cpp
	$(CC) -c src/patch.cpp -o patch.o

curve.o: src/curve.cpp
	$(CC) -c src/curve.cpp -o curve.o

vec3.o: src/vec3.cpp
	$(CC) -c src/vec3.cpp -o vec3.o

clean:
	$(RM) *.o as3
 


