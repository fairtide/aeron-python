#include "_publication.hpp"

#include <Aeron.h>
#include <pybind11/pybind11.h>

using namespace std;
using namespace aeron;
namespace py = pybind11;


publication::publication(shared_ptr<Publication> aeron_publication)
    :
        aeron_publication_(aeron_publication)
{

}

const string& publication::channel() const
{
    return aeron_publication_->channel();
}

int64_t publication::stream_id() const
{
    return aeron_publication_->streamId();
}

int64_t publication::offer(py::object data)
{
    if (py::isinstance<py::buffer>(data))
    {
        py::buffer buffer = data;
        py::buffer_info info = buffer.request(false);

        AtomicBuffer aeron_buffer(reinterpret_cast<uint8_t*>(info.ptr), info.size * info.itemsize);
        return aeron_publication_->offer(aeron_buffer);
    }
    else if (py::isinstance<py::str>(data))
    {
        auto characters = data.cast<std::string>();

        AtomicBuffer aeron_buffer(
                const_cast<uint8_t*>(
                        reinterpret_cast<const uint8_t*>(characters.data())), characters.length());
        return aeron_publication_->offer(aeron_buffer);
    }

    throw py::type_error("unsupported data type!");
}

void publication::close()
{
    aeron_publication_->close();
}

bool publication::__bool__() const
{
    return aeron_publication_ && aeron_publication_->isConnected();
}

PYBIND11_MODULE(_publication, m)
{
    py::class_<publication>(m, "Publication")
            .def_property_readonly("channel", &publication::channel)
            .def_property_readonly("stream_id", &publication::stream_id)
            .def("offer", &publication::offer,
                    py::arg("data"))
            .def("close", &publication::close)
            .def("__bool__", &publication::__bool__);

}

