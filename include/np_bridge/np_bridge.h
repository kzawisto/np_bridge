
#pragma once
#include<numpy/ndarraytypes.h>
#include<Python.h>
#include<numpy/arrayobject.h>
#include<vector>
#include<unordered_set>
#include<np_bridge/NpDTypes.h>
#include<np_bridge/utility.h>

#include "../np_bridge/utility.h"
namespace np_bridge {
class NumpyArrWrapper {
public:
	PyArrayObject * arr = nullptr;
	long int * dimmensions, *strides;
	char * data;
};
template<class T>
class NumpyArray1Dim : public NumpyArrWrapper{
public:
	NumpyArray1Dim(PyArrayObject * arr) {
		this->arr = arr;
		if(arr) {
			this->data = arr->data;
			this->dimmensions = arr->dimensions;
			this->strides = arr->strides;
		}
	}

	T & at(long i) {
		return *(T*)(data + strides[0]*i);
	}
	T* begin() {
		return this->data;
	}
	T * end() {
		return this->data + this->dimmensions[0] * strides[0];
	}
	const T & c_at(long i) const {
		return *(T*)(data + strides[0]*i);
	}
	long size() const {
		return dimmensions[0];
	}
	std::vector<T> to_vector() const{
		std::vector<T> vec;
		vec.reserve(size());
		for(long i =0;i < size();++i){
			return vec.push_back(c_at(i));
		}
		return vec;
	}
	std::unordered_set<T> to_set() const{
		std::unordered_set<T> vec;
		for(long i =0;i < size();++i){
			return vec.insert(c_at(i));
		}
		return vec;
	}
	T sum() const {
		T result = 0;
		for(long i = 0; i < size();++i){
			result += c_at(i);
		}
		return result;
	}
	T max() const {
		T result = -9999999;
		for(long i = 0; i < size();++i){
			result = std::max(c_at(i), result);
		}
		return result;
	}
	T min() const {
		T result = 9999999;
		for(long i = 0; i < size();++i){
			result = std::min(c_at(i), result);
		}
		return result;
	}
};


template<class T>
class NumpyArray2Dim : public NumpyArrWrapper {
public:
	NumpyArray2Dim(PyArrayObject * arr) {
		this->arr = arr;
		if(arr) {
			this->data = arr->data;
			this->dimmensions = arr->dimensions;
			this->strides = arr->strides;
		}
	}
	T & at(long x, long y) {
		return *(T*)( data + strides[0]*x + strides[1]*y);
	}
	const T & at_c(long x, long y) const {
		return *(T*)( data + strides[0]*x + strides[1]*y);
	}
	long sizeX() const {
		return dimmensions[0];
	}
	long sizeY() const {
		return dimmensions[1];
	}

};
template<typename T>
class NumpyArrayObjImpl :public NumpyArray1Dim<T>{

public:
	NumpyArrayObjImpl(int dim) : NumpyArray1Dim<T>(nullptr){
		this->arr = get_numpy_array_1D<T>(dim);
		if(this->arr) {
			this->data = this->arr->data;
			this->dimmensions = this->arr->dimensions;
			this->strides = this->arr->strides;
		}
	}
	template<typename K>
	NumpyArrayObjImpl(const std::vector<K> & vec) : NumpyArrayObjImpl<T>(vec.size()) {
		for(unsigned int i = 0; i < vec.size(); ++i) {
			this->at(i) = vec.at(i);
		}
	}

	~NumpyArrayObjImpl() {
		Py_XDECREF(this->arr);
	}


};
template<class K>
class NumpyDetachable : public K{
public:
    template<typename ...Args>
    NumpyDetachable(Args... args) : K(args...) {}
	PyArrayObject * detach() {
		K * ptr = static_cast<K*>(this);
		assert(ptr->arr != nullptr);
		PyArrayObject * arr_tmp = ptr->arr;
		ptr->arr = nullptr;
		return arr_tmp;
	}
};

template<typename K>
using NpArrayObj = NumpyDetachable<NumpyArrayObjImpl<K>>;


template<typename T>
class NumpyArrayObj2Dim : public NumpyArray2Dim<T>{//, public NumpyDetachable<NumpyArrayObj2D>{
public:

	using  NumpyArray2Dim<T>::arr;
	NumpyArrayObj2Dim(long dimX, long dimY) : NumpyArray2Dim<T>(nullptr){
		arr = get_numpy_array_2D<T>(dimX, dimY);
		if(arr) {
			this->data = arr->data;
			this->dimmensions = arr->dimensions;
			this->strides = arr->strides;
		}
	}
	~NumpyArrayObj2Dim(){
		Py_XDECREF(arr);
	}
	PyArrayObject * detach() {
		assert(arr != nullptr);
		PyArrayObject * arr_tmp = arr;
		arr = nullptr;
		return arr_tmp;
	}
};

typedef NumpyArray2Dim<double> NumpyArray2DimD;
class NumpyArrayCBuffer2D : public NumpyArray2DimD {
	char * buffer;
	bool is_c_order=true;
public:
	NumpyArrayCBuffer2D(long dimX, long dimY, bool is_c_order=true) : NumpyArray2DimD(nullptr),
	is_c_order(is_c_order){
		buffer = new char[dimX * dimY * sizeof(double)];
		this->data = buffer;
		this->dimmensions = new npy_intp[2]{dimX,dimY};
     	this->strides = get_strides_2d<double>(dimX, dimY, is_c_order);
	}
	NumpyArrayCBuffer2D(NumpyArrayCBuffer2D && buf) : NumpyArray2DimD(nullptr){
		buffer = buf.buffer;
		buf.buffer = nullptr;
		data = buf.data;
		buf.data = nullptr;
		strides = buf.strides;
		buf.strides = nullptr;
		dimmensions = buf.dimmensions;
		buf.dimmensions = dimmensions;
	}
	~NumpyArrayCBuffer2D() {
		if(buffer)
			delete[] buffer;
		if(strides)
			delete[] strides;
		if(dimmensions)
			delete[] dimmensions;
	}
};
class NumpyArrayCBufferView2D : public NumpyArray2DimD {

	char * buffer;
public:
	NumpyArrayCBufferView2D(char * buf, long dimX, long dimY, bool is_c_order=true) : NumpyArray2DimD(nullptr),
			buffer(buf){
			this->data = buffer;
			this->dimmensions = new npy_intp[2]{dimX,dimY};
			this->strides = get_strides_2d<double>(dimX, dimY, is_c_order);

	}
	~NumpyArrayCBufferView2D(){
		delete[] strides;
		delete[] dimmensions;
	}
};
class NumpyArrayCBuffer1D : public NumpyArray1Dim<double> {

public:
	NumpyArrayCBuffer1D(int dim) : NumpyArray1Dim(nullptr){
			data = new char[dim * sizeof(double)];
			dimmensions = new long[1]{dim};
			strides = new long[1]{sizeof(double)};
	}
	~NumpyArrayCBuffer1D() {
			delete[] data;
			delete[] dimmensions;
			delete[] strides;
	}

};
class NumpyArrayCBufferView1D : public NumpyArray1Dim<double> {
public:

	NumpyArrayCBufferView1D(char * data, int dim) : NumpyArray1Dim(nullptr){
			this->data = data;
			dimmensions = new long[1]{dim};
			strides = new long[1]{sizeof(double)};
	}
	~NumpyArrayCBufferView1D() {
			delete[] dimmensions;
			delete[] strides;
	}
};

template<typename T>
class NumpyArrayView : public NumpyArray1Dim<T>{
public:
	NumpyArrayView(PyArrayObject * arr): NumpyArray1Dim<T>(arr){
		if(arr->descr->type != NpDType<T>::typechar) {
			throw std::logic_error(

					stringify("Got typechar ",arr->descr->type, " but expected ",  (char)NpDType<T>::typechar));
		}
		if(arr->nd != 1) {
			throw std::logic_error(stringify("Not one dimmension but ", arr->nd," instead."));
		}
	}
};

template<typename T>
class NumpyArrayView2D : public NumpyArray2Dim<double>{
public:
	NumpyArrayView2D(PyArrayObject * arr): NumpyArray2Dim<double>(arr){
		if(arr->descr->type != NpDType<T>::typechar) {
			throw std::logic_error(

					stringify("Got typechar ",arr->descr->type, " but expected ",  (char)NpDType<T>::typechar));
		}
		if(arr->nd != 2) {
			throw std::logic_error(stringify("Not one dimmension but ", arr->nd," instead."));
		}
	}
};

}
