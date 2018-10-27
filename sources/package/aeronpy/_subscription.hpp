#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>


class subscription final
{
public:
    subscription(std::shared_ptr<aeron::Subscription> aeron_subscription);

    const std::string& channel() const;
    int64_t stream_id() const;

    int poll(pybind11::function handler, int fragment_limit);

    int poll_eos();

    bool __bool__() const;

private:
    std::shared_ptr<aeron::Subscription> aeron_subscription_;

};
