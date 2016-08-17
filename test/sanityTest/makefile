CXX=g++

default:cpo
		
	$(CXX) -c -I../../include/ main.cpp Simplex.cpp -std=c++11
	$(CXX) -o main OptimizationAlgorithm.o main.o Minimizer.o Simplex.o


cpo:
	make -C ../../src/MinA/common/ 
	cp ../../src/MinA/common/*.o .
clean:
	rm *.o main
