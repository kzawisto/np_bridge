## NP_BRIDGE

### Highlights

Create numpy array wrappers with checked types and dimmensions.

```c++
// allocate new numpy array
PyArrayObject * ar =np_bridge::get_numpy_array_1D<double>(10);
// wrap array passed as argument, throws if type is not float64
np_bridge::NumpyArrayView<double> view(array1);
// and cast view to Eigen wrapper to use Eigen for computing
auto eigen_vector_map = eigenify(view);
```

Access Pandas columns easily

```c++
// pandas dataframe with column "epoch" and type np.int64, of type PyObject* (throws if wrong dtype, not a dataframe etc).
auto eigen_map = get_eigen_col<long int>(dataframe, "epoch");
```


Easily create and export POJO C++ structures to Pandas dataframes with autogenerated boilerplate

```c++
#define FIELD_LIST X(int, age) X(double, timestamp) X(long, id) 
#define STRUCT_NAME Customer
#include <np_bridge/PandasSerializable.h>
//...
//...
// Automatically defined types
CustomerVector container;
Customer john;
john.index_ordinal = 12;
john.age = 40;
john.timestamp = 1.5e-9 + 50;
john.id = 2;
container.push_back(john);
container.push_back({.index_ordinal = 0, .age=12, .timestamp =1.5e-9, .id=0});
container.push_back({.index_ordinal = 1, .age=23, .timestamp =1.5e-9, .id=1});
//..
PyObject * dict = container.get_dic_of_numpy_arrays();
// Dictionary of Numpy array columns, can be casted to Pandas dataframe from python

```

### Usage

Header only, please include np_bridge/all.h.

Please see tests for reference. Work in progress do not use features that are not yet tested.

Tested with Eigen 3.3.2-1, GCC 5.2, C++11



