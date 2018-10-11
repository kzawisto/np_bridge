
#include<gtest/gtest.h>

#define FIELD_LIST X(int, hello) X(long double, world)
#define STRUCT_NAME TestType
#include<np_bridge/PandasSerializable.h>


TEST(test, pandas_serializable_type_smoke_test) {
	TestType t;
	t.hello = 10;
	t.world = 12;
	TestTypeVector vec;
	vec.push_back(t);
	vec.push_back({.index_ordinal = 1, .hello= 15, .world=10});
}