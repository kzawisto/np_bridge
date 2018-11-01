
from template_vector_struct import *
from template_dataframe import *
def generate_struct_file(output_dir, descr):
    print(f"codegen run for {descr}")
    st1 = "#pragma once\n"
    st1 += "#include<np_bridge/pandas_columns.h>"
    st1 += "#include<vector>"
    st1 += "#include<ostream>"

    st1 += template_struct_header(descr)
    st1 += template_vector_struct(descr)
    st1 += template_dataframe(descr)
    name = descr["name"]
    file_path = f"{output_dir}/{name}.hpp"
    with open(file_path, "w") as fp:
        fp.write(st1)

    import distutils.spawn

    exec_clang_format = distutils.spawn.find_executable("clang-format")
    if exec_clang_format:
        import subprocess
        subprocess.call([exec_clang_format, "-i", file_path])
