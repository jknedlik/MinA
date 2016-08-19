#ifndef PARAMETER_H
#define PARAMETER_H
#include <map>
#include <iostream>
#include<cstring>

#include <memory>



 using namespace std;
class Parameter{
    public:
	
	
	
	Parameter();
	bool operator< (const Parameter&  e) const;
	void setname(string na);
	string getname() const;
	void setstartingPoint(double sp);
	void setboundaryLeft(double bl);
	void setboundaryRight(double br);
	double getstartingPoint()const;
	double getboundaryLeft()const;
	double getboundaryRight()const;
	
    private:
	string name;
	std::shared_ptr<double> startingPoint;
	std::shared_ptr<double> boundaryLeft;
	std::shared_ptr<double> boundaryRight;
	std::allocator<double> alloc;
};

#endif 
