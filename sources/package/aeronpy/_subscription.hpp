#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>
#include <vector>


class subscription final
{
public:
    subscription(std::shared_ptr<aeron::Subscription> aeron_subscription);

    /**
     *
     * @return
     */
    const std::string& channel() const;
    /**
     *
     * @return
     */
    int64_t stream_id() const;
    /**
     * @brief
     * @return
     */
    bool is_connected() const;
    /**
     *
     * @return
     */
    bool is_closed() const;

    /**
     *
     * @return
     */
    std::vector<aeron::Image> images() const;

    /**
     *
     * @param handler
     * @param fragment_limit
     * @return
     */
    int poll(pybind11::function handler, int fragment_limit);
    /**
     *
     * @return
     */
    int poll_eos(pybind11::object handler);

    /**
     *
     * @return
     */
    bool __bool__() const;
    /**
     *
     * @return
     */
    std::string __str__() const;

private:
    bool is_complete_poll_handler(pybind11::function& handler);

    std::shared_ptr<aeron::Subscription> aeron_subscription_;

};
