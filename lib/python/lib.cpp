#include <pybind11/pybind11.h>

#define UBENCH_SYMBOL_MODE
#include "../../ubench.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

#define ADD_DEFS_ALL(func_name)                                                \
    py::class_<ubench_##func_name##_t>(m, STRINGIFY(func_name) "_t")           \
        .def(py::init<>());                                                    \
    m.def(STRINGIFY(func_name) "_init", &ubench_##func_name##_init);           \
    m.def(STRINGIFY(func_name) "_reset", &ubench_##func_name##_reset);         \
    m.def(STRINGIFY(func_name) "_add", &ubench_##func_name##_add);             \
    m.def(STRINGIFY(func_name) "_print", &ubench_##func_name##_print);         \
    m.def(STRINGIFY(func_name) "_add_print", &ubench_##func_name##_add_print); \
    m.def(STRINGIFY(func_name) "_add_print_reset",                             \
          &ubench_##func_name##_add_print_reset);                              \
    m.def(STRINGIFY(func_name) "_dump", &ubench_##func_name##_dump);           \
    m.def(STRINGIFY(func_name) "_add_dump", &ubench_##func_name##_add_dump);   \
    m.def(STRINGIFY(func_name) "_add_dump_reset",                              \
          &ubench_##func_name##_add_dump_reset);

#define ADD_DEFS_ALL_DOUBLE(func_name)                                         \
    m.def(STRINGIFY(func_name) "_print_double",                                \
          &ubench_##func_name##_print_double);

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
    ADD_DEFS_ALL_DOUBLE(stat);
    ADD_DEFS_ALL(var);
    ADD_DEFS_ALL_DOUBLE(var);
    ADD_DEFS_ALL(hist);
    ADD_DEFS_ALL(hist_range);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}