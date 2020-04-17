
objects = denoise.so demo.so

denoise.so : denoise.c
	g++ -o denoise.so -std=c++11 -shared -fPIC denoise.c

demo.so : demo.c
	g++ -o demo.so -std=c++11 -shared -fPIC demo.c
