#include <iostream>
#include <map>

#include <utility>

int main(void){
      typedef std::pair <std::map <std::string, double>,double> vertex;
	      vertex A[1];
		A[0].first["x"]=10;
		A[0].second=40;
		std::cout<<A[0].first["x"]<<A[0].second;

    return(0);
}
