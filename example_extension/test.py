
import example_bridge.ext as E
import pandas as pd
import numpy as np

df = pd.DataFrame(dict(
    hello=[1,2,3.,4.],
    foo=[2,2,2,2.],
        ))
assert (E.cumsum_col(df) == np.array([ 2.,  6., 12., 20.])).all()

obj = E.example()
assert {'index_ordinal', 'hello', 'foo', 'wuj'} == obj.keys()

