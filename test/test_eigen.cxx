
#include<gtest/gtest.h>
#include<Python.h>
#include<numpy/arrayobject.h>
#include<np_bridge/np_bridge.h>
#include<np_bridge/eigenify.h>
#include<gmock/gmock.h>

using namespace np_bridge;
TEST(main, create_eigen_map_1d) {
	PyArrayObject * ar =np_bridge::get_numpy_array_1D<double>(10);
	np_bridge::NumpyArrayView<double> view(ar);
	for(int i =0; i < 10; ++i) {
		view.at(i) = i;
	}
	auto map = eigenify(view);
	std::cout<<map;
	for(int i =0; i < 10; ++i) {
		ASSERT_EQ(map(i), i);
	}
}
TEST(main, create_eigen_map_2d) {
	np_bridge::NumpyArrayObj2Dim<int> obj(10,20);
	for(int i =0; i < 10; ++i) {
		for(int j =0; j < 20; ++j) {
			obj.at(i, j) = i+ j * 30;
		}
	}
	auto map = eigenify2d(obj);
	std::cout<<map;
	for(int i =0; i < 10; ++i) {


		for(int j =0; j < 20; ++j) {
			int actual = map(i,j);
			ASSERT_EQ(actual, i + j * 30);
		}
	}
}
int main(int argc, char **argv) {

	Py_Initialize();
	import_array();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
