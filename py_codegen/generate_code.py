D = 'double'
LI = 'long int'


from field import *

struct_bar_vol = dict(name="Point", fields=[
    Field(LI, 'ordinal'),
    Field(D, '_x'),
    Field(D, '_y')])



output_dir = "./"


from codegen import *

generate_struct_file(output_dir,struct_bar_vol)
generate_struct_file(output_dir,struct_tick_fields)
