all:
	g++ -O0 *.cpp -c -g -std=c++11 -march=native -ljpeg -lm
	g++ *.o -o raytracer -lpthread -ljpeg -lm

clean:
	rm -f *.o
