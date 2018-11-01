
import mc

def template_dataframe(descr):

    name, fields = descr["name"], mc.List(descr["fields"])
    def declare_view_op(field):
       return f"np_bridge::NumpyArrayView<{field.type}> {field.name};"
    decl_view_code = fields.map(declare_view_op).mk_string("\n" ,"" ,"")

    def def_init_op(field):
        b = field.name
        a = field.type
        return f'{b}(np_bridge::get_column_values<{a}>(dataframe, "{b}"))'

    def set_elem_op(field):
        b = field.name
        return f"res.{b} = {b}.at(i);"

    define_initialization = fields.map(def_init_op).mk_string("," ,"" ,"")
    if define_initialization != "":
        define_initialization = " : " + define_initialization


    set_elem = fields.map(set_elem_op).mk_string("\n" ,"" ,"")
    return f'''
    struct  {name}DataFrame {{

	long size = 0;
    {decl_view_code}
    {name}DataFrame (PyObject* dataframe){define_initialization} {{
        { "size = " + fields[0].name + ".size();" if len(fields) != 0 else "" }
    }}
	{name} at(long i) {{
		{name} res;
		{set_elem}
		return res;
	}}
}};
    '''
