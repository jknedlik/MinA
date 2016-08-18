CXX=g++
CPPOBJ= $(wildcard *.cpp)


default: $(CPPOBJ)
	$(CXX) -c -std=c++11 $(CPPOBJ) -I../../../include -I../../../src
%.cpp:
	$(CXX) -c $@ $< -I../../include -I../../src
clean:
	rm *.o
