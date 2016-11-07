#ifndef PARAMETER_H
#define PARAMETER_H
#include <iostream>
#include <limits>

class Parameter {
  public:
    // setters
    bool operator<(const Parameter& e) const;
    void setName(std::string name) { mName = name; }
    void setStartingPoint(double sp) { mStartingPoint = sp; }
    void setBoundaryLeft(double bl) { mBoundaryLeft = bl; }
    void setBoundaryRight(double br) { mBoundaryLeft = br; }

    // getters
    std::string getName() const { return mName; }
    double getStartingPoint() const { return mStartingPoint; }
    double getBoundaryLeft() const { return mBoundaryLeft; }
    double getBoundaryRight() const { return mBoundaryRight; }

  private:
    std::string mName;
    double mStartingPoint = 0.;
    double mBoundaryLeft = -std::numeric_limits<double>::max();
    double mBoundaryRight = std::numeric_limits<double>::max();
};

#endif
