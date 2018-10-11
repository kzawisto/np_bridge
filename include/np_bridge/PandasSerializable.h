
#include<vector>
#include<Python.h>
#include<np_bridge/np_bridge.h>


#define MERGE(a, b) a##b
#define VECTOR_NAME(x) MERGE(x, Vector)

struct STRUCT_NAME {
	int index_ordinal = 0;
	#define X(a,b) a b = 0;
	FIELD_LIST
	#undef X
};


struct  VECTOR_NAME(STRUCT_NAME) {

	#define X(a,b) std::vector<a> b;
	FIELD_LIST
	X(int, index_ordinal)
	#undef X
	void push_back(STRUCT_NAME result) {
		#define X(a,b) b.push_back(result.b);
		FIELD_LIST
		X(int, index_ordinal)
		#undef X(a,b)
	}
	PyObject * get_dic_of_numpy_arrays() {

		#define X(a,b) np_bridge::NpArrayObj<a> _##b(b.size());
		FIELD_LIST
		X(int, index_ordinal)
		#undef X

		for(unsigned i = 0;i < index_ordinal.size(); i++) {
			#define X(a,b) _##b.at(i) = b[i];
			FIELD_LIST
			X(int, index_ordinal)
			#undef X
		}
		PyObject* obj = PyDict_New();
		#define X(a,b) PyDict_SetItemString(obj, #b, (PyObject*)_##b.detach());
		FIELD_LIST
		X(int, index_ordinal)
		#undef X
		return obj;
	}
};

#undef VECTOR_NAME
#undef MERGE
#undef STRUCT_NAME

#undef FIELD_LIST

