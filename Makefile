CXX?=g++

all: crypt

crypt: crypt.cpp
	$(CXX) -std=c++11 -O3 -g -o crypt crypt.cpp

clean:
	rm -rf crypt crypt.dSYM
