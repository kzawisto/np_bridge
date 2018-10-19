
#include<np_bridge/np_bridge.h>
#include<np_bridge/pandas_columns.h>

using namespace np_bridge;
PyObject * cumsum_columns(PyObject * dataframe) {
	auto array_view1 = get_column_values<double>(dataframe, "hello");
	auto array_view2 = get_column_values<double>(dataframe, "foo");
	NpArrayObj<double> obj(array_view1.size());

	for(int i = 0;i < array_view1.size();++i) {
		obj.at(i) = array_view1.at(i) * array_view2.at(i);
	}

	for(int i = 1;i < array_view1.size();++i) {
		obj.at(i) += obj.at(i-1);
	}
	return (PyObject*)obj.detach();
}


#define STRUCT_NAME Example
#define FIELD_LIST X(int, foo) X(int, wuj) X(double, hello)
#include<np_bridge/PandasSerializable.h>


PyObject * produce_example() {
	ExampleVector collection;
	Example ex;

	collection.push_back(
	Example{.foo = 1, .wuj = 3, .hello = 0.1});
	collection.push_back(
	Example{.foo = 2, .wuj = 2, .hello = 1.1});
	return collection.get_dic_of_numpy_arrays();
}
