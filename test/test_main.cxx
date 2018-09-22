
#include<gtest/gtest.h>
#include<Python.h>
#include<numpy/arrayobject.h>
#include<np_bridge/np_bridge.h>
#include<gmock/gmock.h>

TEST(main, create_double_array_view) {
	PyArrayObject * ar =np_bridge::get_numpy_array_1D<double>(10);
	np_bridge::NumpyArrayView<double> view(ar);
	view.at(0) = 10;
	ASSERT_EQ(view.at(0), 10);
}

TEST(main, create_double_2d_array_view) {
	PyArrayObject * ar =np_bridge::get_numpy_array_2D<double>(10, 10);
	np_bridge::NumpyArrayView2D<double> view(ar);
	for(int i = 0; i < 9; i++) {

		view.at(i, i+1) = i;
		auto actual_value = view.at(i,i+1);
		ASSERT_EQ(actual_value, i);
	}
}
TEST(main, create_numpy_obj_arr_2d) {
	np_bridge::NumpyArrayObj2Dim<int> obj(10,10);
	for(int i = 0; i < 9; i++) {
		obj.at(i, i) = i;
		auto actual_value = obj.at(i,i);
		ASSERT_EQ(actual_value, i);
	}
	EXPECT_THROW(
			np_bridge::NumpyArrayView2D<double> view(obj.arr),
			std::logic_error
	);
}
TEST(main, view_throws_on_type_mismatch) {
	np_bridge::NumpyArrayObj2Dim<int> obj(10,10);
	EXPECT_THROW(
			np_bridge::NumpyArrayView2D<double> view(obj.arr),
			std::logic_error
	);
}

int main(int argc, char **argv) {

	Py_Initialize();
	import_array();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
