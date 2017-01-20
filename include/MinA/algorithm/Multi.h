#ifndef MULTIT_H
#define MULTIT_H
#include "MinA/common/algorithm.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
namespace MinA {

template typename<Function> class Multi
  : public MinA::Algorithm<int,
                           TArray<size_t, std::tuple_size<typename Function::parametertype>::value>,
                           Function> public : Multi() {

    reset(){};
    Result<typename Function::parametertype> run()
    {
        TupleVector<typename Function::parametertype>::type tv;
    for_each3(&tv,this->f.bounds,this->mMetaParameters,[](auto t,auto b,auto mp){
            auto step = (b.right - b.left) / (double)(mp + 1);
            for (int i = 0; i < mp; i++) {
                tv.push_back(b.left + i * step);
            }
            std::vector<typename decltype(Function::parametertype)> vec;
            constructtuple(tv, [&vec](auto x) { vec.push_back(x); });

    }
    }
}
}
#endif
