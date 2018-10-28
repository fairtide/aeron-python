#include "_exclusive_publication.hpp"

using namespace std;
using namespace aeron;
namespace py = pybind11;


exclusive_publication::exclusive_publication(shared_ptr<ExclusivePublication> aeron_exclusive_publication)
    :
        aeron_exclusive_publication_(aeron_exclusive_publication)
{

}

const string& exclusive_publication::channel() const
{
    return aeron_exclusive_publication_->channel();
}

int64_t exclusive_publication::stream_id() const
{
    return aeron_exclusive_publication_->streamId();
}

int32_t exclusive_publication::session_id() const
{
    return aeron_exclusive_publication_->sessionId();
}

int32_t exclusive_publication::initial_term_id() const
{
    return aeron_exclusive_publication_->initialTermId();
}

int64_t exclusive_publication::offer(py::object data)
{
    if (py::isinstance<py::buffer>(data))
    {
        py::buffer buffer = data;
        py::buffer_info info = buffer.request(false);

        AtomicBuffer aeron_buffer(reinterpret_cast<uint8_t*>(info.ptr), info.size * info.itemsize);
        return aeron_exclusive_publication_->offer(aeron_buffer);
    }
    else if (py::isinstance<py::str>(data))
    {
        auto characters = data.cast<std::string>();

        AtomicBuffer aeron_buffer(
                const_cast<uint8_t*>(
                        reinterpret_cast<const uint8_t*>(characters.data())), characters.length());
        return aeron_exclusive_publication_->offer(aeron_buffer);
    }

    throw py::type_error("unsupported data type!");
}

void exclusive_publication::close()
{
    aeron_exclusive_publication_->close();
}

bool exclusive_publication::__bool__() const
{
    return aeron_exclusive_publication_ && aeron_exclusive_publication_->isConnected();
}

PYBIND11_MODULE(_exclusive_publication, m)
{
    py::class_<exclusive_publication>(m, "ExclusivePublication")
            .def_property_readonly("channel", &exclusive_publication::channel)
            .def_property_readonly("stream_id", &exclusive_publication::stream_id)
            .def("offer", &exclusive_publication::offer,
                 py::arg("data"))
            .def("close", &exclusive_publication::close)
            .def("__bool__", &exclusive_publication::__bool__);

}

