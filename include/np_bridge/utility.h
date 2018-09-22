#pragma once

#include<sstream>
#include<Python.h>
#include "numpy/arrayobject.h"
#include<set>

namespace np_bridge {

template<typename T>
std::string stringify(T arg) {
	std::stringstream ss;
	ss<<arg;
	return ss.str();
}

template<typename ...Args, typename T>
std::string stringify(T arg, Args ...args) {
	return stringify(arg) + stringify(args...);
}

template<typename T, typename K>
std::ostream & operator<<(std::ostream & os, const std::multiset<T,K> & s){
	os<<"{{";
	if(!s.empty()) {
		os << *s.begin();
	}
	for(auto it = ++s.begin(); it != s.end(); ++it) {
		os <<  "," << *it;
	}
	os<<"}}";
	return os;
}


template<typename T>
PyArrayObject * get_numpy_array_1D(long int dim) {
	  npy_intp dims[] = {dim};
	  void * buffer = malloc(dim *sizeof(T));
	  PyObject *narray = PyArray_SimpleNewFromData(1, dims, NpDType<T>::typevalue, buffer);
	  PyArray_ENABLEFLAGS((PyArrayObject*)narray, NPY_ARRAY_OWNDATA);
	  return (PyArrayObject*)PyArray_Return((PyArrayObject*) narray);

}
template<typename T>
PyArrayObject * get_numpy_array_2D( long int dimX, long int dimY) {
	  npy_intp dims[] = {dimX, dimY};
	  void * buffer = malloc(dimX * dimY * sizeof(T));

	  PyObject *narray = PyArray_SimpleNewFromData(2, dims, NpDType<T>::typevalue, buffer);
	  PyArray_ENABLEFLAGS((PyArrayObject*)narray, NPY_ARRAY_OWNDATA);
	  return (PyArrayObject*)PyArray_Return((PyArrayObject*) narray);
}

inline
PyObject * unicode_pystr(std::string s){
	return (PyObject*)PyUnicode_FromString(s.c_str());
}

template<typename T>
npy_intp * get_strides_2d(long dimX, long dimY,  bool c_order) {
		int type_size = sizeof(T);
		if(c_order) {
			return new npy_intp[2]{type_size,type_size*dimX};
		} else {
			return new npy_intp[2]{type_size * dimY,type_size};
		}
}
}
