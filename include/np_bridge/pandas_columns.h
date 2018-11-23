#pragma once

#include <iostream>
#include <np_bridge/np_bridge.h>
#include <np_bridge/eigenify.h>
#include <np_bridge/utility.h>

namespace np_bridge {
template <typename T>
NumpyArrayView<T> get_column_values(PyObject *pandas_dataframe,
                                    std::string key) {

  PyObject *obj =
      PyObject_CallMethod(pandas_dataframe, "__getitem__", "s", key.c_str());
  if (obj == nullptr) {
    throw std::logic_error("Failed to find key" + key);
  }
  obj = PyObject_CallMethod(obj, "get_values", "");
  if (obj == nullptr) {
    throw std::logic_error("Call to get_values failed for key " + key);
  }
  return NumpyArrayView<T>((PyArrayObject *)obj);
}
template <typename T>
EigenVectorMapUnaligned<T> get_eigen_col(PyObject *pandas_dataframe,
                                         std::string key) {
  auto view = get_column_values<T>(pandas_dataframe, key);
  return eigenify(view);
}

inline
PyObject * call_obj(std::string module, std::string obj, PyObject * arg_tuple) {
		PyObject * module_obj = PyImport_ImportModule(module.c_str());
		PyObject * series = PyObject_GetAttrString(module_obj, obj.c_str());
		return PyObject_CallObject(series, arg_tuple);
}

inline
PyObject * to_pandas_dataframe(PyObject * in) {
		PyObject * args = Py_BuildValue("(O)", in);
		auto result = call_obj("pandas", "DataFrame", args);
		Py_DECREF(args);
		return result;
}
inline 
PyObject * to_pandas_series(PyObject * in) {
		PyObject * args = Py_BuildValue("(O)", in);
		auto result = call_obj("pandas", "Series", args);
		Py_DECREF(args);
		return result;
}

} // namespace np_bridge
