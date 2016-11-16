#ifndef PARAMETER_H
#define PARAMETER_H
#include <iostream>
#include <limits>
#include <stdexcept>

class Parameter {
  public:
    Parameter()
      : mStartingValue{ 0. },
        mLeftBoundary{ std::numeric_limits<double>::lowest() },
        mRightBoundary{ std::numeric_limits<double>::max() }
    {
    }

    Parameter(std::string name, double startingValue = 0.,
              double leftBoundary = std::numeric_limits<double>::lowest(),
              double rightBoundary = std::numeric_limits<double>::max())
      : mName(name), mLeftBoundary(leftBoundary), mRightBoundary(rightBoundary)
    {
        if (!setStartingValue(startingValue))
            throw std::logic_error("Starting value out of boundaries.");
    }

    bool operator<(const Parameter& e) const;
    // setters
    void setName(std::string name) { mName = name; }
    bool setStartingValue(double sValue)
    {
        if (sValue < mLeftBoundary || sValue > mRightBoundary)
            return false;
        else
            mStartingValue = sValue;

        return true;
    }
    void setLeftBoundary(double leftBoundary) { mLeftBoundary = leftBoundary; }
    void setRightBoundary(double rightBoundary) { mRightBoundary = rightBoundary; }

    // getters
    std::string getName() const { return mName; }
    double getStartingValue() const { return mStartingValue; }
    double getLeftBoundary() const { return mLeftBoundary; }
    double getRightBoundary() const { return mRightBoundary; }

  private:
    std::string mName;
    double mStartingValue;
    double mLeftBoundary;
    double mRightBoundary;
};

#endif
