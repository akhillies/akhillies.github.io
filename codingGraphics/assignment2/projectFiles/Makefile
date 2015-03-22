CC = g++

RM = /bin/rm -f


all: main

main: ass2.o
	$(CC) -o as2 ass2.o vec3.o matrix.o pixel.o ray.o camera.o shape.o sphere.o triangle.o freeImage/libfreeimage.a

ass2.o: vec3.o matrix.o pixel.o camera.o ray.o camera.o shape.o sphere.o triangle.o ass2.cpp
	$(CC) -c ass2.cpp -o ass2.o

pixel.o: src/pixel.cpp
	$(CC) -c src/pixel.cpp -o pixel.o

camera.o: src/camera.cpp
	$(CC) -c src/camera.cpp -o camera.o

ray.o: src/ray.cpp
	$(CC) -c src/ray.cpp -o ray.o

triangle.o: src/triangle.cpp
	$(CC) -c src/triangle.cpp -o triangle.o

sphere.o: src/sphere.cpp
	$(CC) -c src/sphere.cpp -o sphere.o

shape.o: src/shape.cpp
	$(CC) -c src/shape.cpp -o shape.o

matrix.o: src/matrix.cpp
	$(CC) -c src/matrix.cpp -o matrix.o

vec3.o: src/vec3.cpp
	$(CC) -c src/vec3.cpp -o vec3.o

clean:
	$(RM) *.o as2
 


