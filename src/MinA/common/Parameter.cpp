#include "MinA/common/Parameter.h"

Parameter::Parameter(){
	startingPoint=nullptr;
	boundaryLeft=nullptr;
	boundaryRight=nullptr;
	}
bool Parameter::operator< (const Parameter&  e)const  {
	return strcmp (getname().c_str(),e.getname().c_str())<0;
	}



void Parameter::setname(string na){
	name=na;
	}

string Parameter::getname()const{
	return name;
	}

void Parameter::setstartingPoint(double sp){
	startingPoint=std::allocate_shared<double> (alloc,sp);
	}

void Parameter::setboundaryLeft(double bl){
	boundaryLeft=std::allocate_shared<double> (alloc,bl);
	}

void Parameter::setboundaryRight(double br){
	boundaryRight=std::allocate_shared<double> (alloc,br);
	}

double Parameter::getstartingPoint()const{
	return 	*startingPoint;
	}

double Parameter::getboundaryLeft() const{
	return 	*boundaryLeft;
	}

double Parameter::getboundaryRight() const{
	return 	*boundaryRight;
	}
