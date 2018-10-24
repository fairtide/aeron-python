#include "_data.hpp"

using namespace std;
using namespace aeron;
namespace py = pybind11;


py::buffer_info atomic_buffer::__buffer__(AtomicBuffer& self)
{
    return py::buffer_info(
            self.buffer(),
            sizeof(uint8_t),
            py::format_descriptor<uint8_t>::format(),
            self.capacity());
}

PYBIND11_MODULE(_data, m)
{
    py::class_<AtomicBuffer>(m, "AtomicBuffer", py::buffer_protocol())
            .def_buffer(&atomic_buffer::__buffer__);

    py::class_<Header>(m, "Header");

}
