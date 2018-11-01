import mc


def template_dic_of_arrays(fields):
    def indent(str1):
        return "    " + str1

    def declare_array_obj(field):
        return f"np_bridge::NpArrayObj <{field.type}> _{field.name}({field.name}.size());"

    def set_array_obj(field):
        b = field.name
        return f"_{b}.at(i) = {b}[i];"

    def set_py_dict_elem(field):
        b = field.name
        return 'PyDict_SetItemString(obj,"' + b + f'", (PyObject*)_{b}.detach());'

    fields = mc.List(fields)
    array_obj_declare = '\n'.join(fields.map(declare_array_obj).map(indent))
    array_obj_set = '\n'.join(fields.map(set_array_obj).map(indent))
    py_dic_elem_set = '\n'.join(fields.map(set_py_dict_elem).map(indent))
    return f'''PyObject * get_dic_of_numpy_arrays()
    {{
        {array_obj_declare}

        for (unsigned i = 0;i < ordinal.size();
    i++) {{
        {array_obj_set}

    }}
    PyObject * obj = PyDict_New();
    {py_dic_elem_set}
    return obj;
    }}'''


def template_accumulate(descr):
    name, fields = descr["name"], mc.List(descr["fields"])

    def fields_push_op(field):
        b = field.name
        return f"      {b}.push_back(result.{b});\n"

    return f"""
void accumulate(const {name} & result, long ordinal_override = -1) {{
		if(ordinal_override == -1) {{
			result.ordinal = last_ord;
			last_ord++;
		}}
{"".join(fields.map(fields_push_op))}
}}
    """


def template_vector_struct(descr):
    name, fields = descr["name"], mc.List(descr["fields"])

    def wipe_buffer_op(field):
        b = field.name
        return f"      {b}.clear();\n"

    def vector_declare_op(field):
        b = field.name
        a = field.type
        return f"std::vector<{a}> {b};";

    return f''' 
struct  {name}Vector {{
	{"".join(fields.map(vector_declare_op))}
	int last_ord = 0;
	void wipe_buffers() {{
	    {"".join(fields.map(wipe_buffer_op))}
	}}
	{template_accumulate(descr)}
	{template_dic_of_arrays(fields)}
}};
    '''


def template_struct_header(description: dict):
    name = description["name"]
    fields = mc.List(description["fields"]).map(lambda x: x.declaration())
    fields = "\n".join(fields)
    template = f"\nstruct {name}" + "{\n" + fields;
    template += template_output_operator(description)
    return template + "\n};\n"


def template_output_operator(description):
    name = description["name"]
    fields = mc.List(description["fields"])

    def print_ostream(self):
        return " << " + f'"{self.name}:"' + f" << element.{self.name} "

    str1 = f"\nfriend std::ostream  & operator<<(std::ostream & op, const {name} & element) " + "{\n"
    str1 += '    op << "' + name + '{";\n'
    if not fields.is_empty():
        str1 += "    op" + print_ostream(fields[0]) + ";\n"
        for f in fields[1:]:
            str1 += '    op << "," ' + print_ostream(f) + ";\n"
    str1 += '    op << "}";\n'
    str1 += '    return op;\n' + \
            "}\n"
    return str1
