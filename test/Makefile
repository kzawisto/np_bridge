



%.o: %.cxx $(shell find .. -name '*.h' -type f)
	g++ -c $< -o $@ -g -fPIC $(shell python3-config --cflags)	-I../include -O0
.PHONY: all
all: test.bin test_eigen.bin
test.bin: test_main.o
	g++ $^ -o $@  -lgtest -lpthread -g $(shell python3-config --libs --ldflags)

test_eigen.bin: test_eigen.o
	g++ $^ -o $@  -lgtest -lpthread -g $(shell python3-config --libs --ldflags)
	
.PHONY: clean
clean:
	rm *.bin *.o
	



