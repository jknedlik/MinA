main.o: In function `Myfunction::Myfunction()':
main.cpp:(.text._ZN10MyfunctionC2Ev[_ZN10MyfunctionC5Ev]+0x4b): undefined reference to `Parameter::Parameter()'
main.cpp:(.text._ZN10MyfunctionC2Ev[_ZN10MyfunctionC5Ev]+0x57): undefined reference to `Parameter::Parameter()'
main.cpp:(.text._ZN10MyfunctionC2Ev[_ZN10MyfunctionC5Ev]+0x9a): undefined reference to `Parameter::setname(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)'
main.cpp:(.text._ZN10MyfunctionC2Ev[_ZN10MyfunctionC5Ev]+0xf8): undefined reference to `Parameter::setname(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)'
main.o: In function `std::less<Parameter>::operator()(Parameter const&, Parameter const&) const':
main.cpp:(.text._ZNKSt4lessI9ParameterEclERKS0_S3_[_ZNKSt4lessI9ParameterEclERKS0_S3_]+0x23): undefined reference to `Parameter::operator<(Parameter const&) const'
Simplex.o: In function `Simplex::algorithm(Functiontobeoptimized*)':
Simplex.cpp:(.text+0xa1a): undefined reference to `Parameter::getname[abi:cxx11]() const'
Simplex.cpp:(.text+0xa7c): undefined reference to `Parameter::getname[abi:cxx11]() const'
Simplex.cpp:(.text+0xb58): undefined reference to `Parameter::getname[abi:cxx11]() const'
Simplex.cpp:(.text+0xc29): undefined reference to `Parameter::getname[abi:cxx11]() const'
Simplex.cpp:(.text+0xc69): undefined reference to `Parameter::getname[abi:cxx11]() const'
Simplex.o:Simplex.cpp:(.text+0xcfd): more undefined references to `Parameter::getname[abi:cxx11]() const' follow
collect2: error: ld returned 1 exit status
make: *** [default] Error 1
