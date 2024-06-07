#include <pybind11/cast.h>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>

#include <string>

namespace py = pybind11;

constexpr int add(int x, int y)
{
    return x + y;
}

PYBIND11_MODULE(example, m) 
{
    m.doc() = "pybind11 example plugin";
    m.def("add", 
         &add, 
         "A function that adds two numbers",
         py::arg("i"),
         py::arg("j")
         );
}

struct Pet {
    Pet(const std::string& name)
      : name{ name }
    {}

    void set_name(const std::string& name_) noexcept { name = name_; }
    const std::string get_name() const noexcept { return name; }
 
    std::string name;
};