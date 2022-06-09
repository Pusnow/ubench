#include <pybind11/pybind11.h>

#define UBENCH_SYMBOL_MODE
#include "../../ubench.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

#define ADD_DEFS_ALL(func_name, ...)                                           \
    py::class_<ubench::func_name>(m, STRINGIFY(func_name))                     \
        .def(py::init<__VA_ARGS__>())                                          \
        .def("reset", &ubench::func_name::reset)                               \
        .def("add", &ubench::func_name::add)                                   \
        .def("print", &ubench::func_name::print)                               \
        .def("add_print", &ubench::func_name::add_print)                       \
        .def("add_print_reset", &ubench::func_name::add_print_reset)           \
        .def("dump", &ubench::func_name::dump)                                 \
        .def("add_dump", &ubench::func_name::add_dump)                         \
        .def("add_dump_reset", &ubench::func_name::add_dump_reset);

PYBIND11_MODULE(ubench, m) {
    m.doc() = R"pbdoc(
        ubench
        -----------------------
        .. currentmodule:: ubench
        .. autosummary::
           :toctree: _generate
           ubench_get_cycles
    )pbdoc";

    m.def("ubench_get_cycles", &ubench_get_cycles);
    ADD_DEFS_ALL(stat);
    // ADD_DEFS_ALL_DOUBLE(stat);
    ADD_DEFS_ALL(var);
    // ADD_DEFS_ALL_DOUBLE(var);
    ADD_DEFS_ALL(hist);
    ADD_DEFS_ALL(hist_range, const ubench_uint_t, const ubench_uint_t,
                 const ubench_uint_t);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}