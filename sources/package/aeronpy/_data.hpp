#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <array>
#include <memory>


class atomic_buffer
{
public:
    static pybind11::buffer_info __buffer__(aeron::AtomicBuffer& self);

};

class header
{
public:


};

