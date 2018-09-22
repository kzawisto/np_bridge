
#pragma once
#include<eigen3/Eigen/Dense>
#include<np_bridge/np_bridge.h>

namespace np_bridge {

template<typename T>
using EigenVectorMapUnaligned = Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1>,
Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic, 1>
>;
template<typename T>
using EigenMatrixMapUnaligned = Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>,
Eigen::Unaligned, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>
>;
template<typename T>
EigenVectorMapUnaligned<T> eigenify(NumpyArray1Dim<T> & arr) {
	constexpr int s = sizeof(T);
	return EigenVectorMapUnaligned<T>(
			&arr.at(0),
			arr.size(),
			Eigen::Stride<Eigen::Dynamic, 1>(arr.strides[0]/ s, 1)
	);
}

template<typename T>
EigenMatrixMapUnaligned<T> eigenify2d(NumpyArray2Dim<T> & arr) {

	constexpr int s = sizeof(T);
	return EigenMatrixMapUnaligned<T>(&arr.at(0,0),arr.sizeX(), arr.sizeY(),
			Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>( arr.strides[1] / s, arr.strides[0] /s)
			);
}
}
