#ifndef PARAMETER_H
#define PARAMETER_H
#include <map>
#include <iostream>
#include<string>


 using namespace std;
class parameter{
    public:
	//bool cmp(Parameter lhs,Parameter lhs){return
	//Parameter operator < (string rhs){
	//return  	
	//}
	bool operator< (parameter  e)  {
	  return getSpoint() < e.getSpoint();
	}
	void setname(string na){name=na;}
	string getname(){return name;}
	void setSpoint(double Sp){Spoint=Sp;}
	double getSpoint(){return Spoint;}
    private:
	string name;
	double Spoint;
};

#endif 
