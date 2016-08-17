CXX=g++
CPPOBJ= $(wildcard *.cpp)


default: $(CPPOBJ)
	$(CXX) -c $(CPPOBJ) -I../../../include -I../../../src
%.cpp:
	$(CXX) -c $@ $< -I../../include -I../../src
clean:
	rm *.o
