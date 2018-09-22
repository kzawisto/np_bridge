
#pragma once
#include<numpy/arrayobject.h>
namespace np_bridge {


template<typename T>
struct NpDType {


};

template<> struct NpDType<int> {
	static constexpr NPY_TYPECHAR typechar = NPY_INTLTR;
	static constexpr NPY_TYPES typevalue = NPY_INT;
};

template<> struct NpDType<double> {
	static constexpr NPY_TYPECHAR typechar = NPY_DOUBLELTR;
	static constexpr NPY_TYPES typevalue = NPY_DOUBLE;
};

template<> struct NpDType<float> {
	static constexpr NPY_TYPECHAR typechar = NPY_FLOATLTR;
	static constexpr NPY_TYPES typevalue = NPY_FLOAT;
};
template<> struct NpDType<long> {
	static constexpr NPY_TYPECHAR typechar = NPY_LONGLTR;
	static constexpr NPY_TYPES typevalue = NPY_LONG;
};

template<> struct NpDType<long long> {
	static constexpr NPY_TYPECHAR typechar = NPY_LONGLONGLTR;
	static constexpr NPY_TYPES typevalue = NPY_LONGLONG;
};

template<> struct NpDType<long double> {
	static constexpr NPY_TYPECHAR typechar = NPY_LONGDOUBLELTR;
	static constexpr NPY_TYPES typevalue = NPY_LONGDOUBLE;
};

template<> struct NpDType<short> {
	static constexpr NPY_TYPECHAR typechar = NPY_SHORTLTR;
	static constexpr NPY_TYPES typevalue = NPY_SHORT;
};
}


