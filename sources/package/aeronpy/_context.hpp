#pragma once

#include "_publication.hpp"
#include "_subscription.hpp"

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <list>
#include <memory>
#include <string>


/**
 * @brief Provides a helper for exposing Aeron context to python.
 */
class context final
{
public:
    /**
     * @brief
     */
    explicit context(pybind11::kwargs args);

    /**
     * @brief
     * @param channel
     * @param stream_id
     * @return
     */
    subscription add_subscription(const std::string& channel, int32_t stream_id);
    /**
     * @brief
     * @param channel
     * @param stream_id
     * @return
     */
    publication add_publication(const std::string& channel, int32_t stream_id);

private:
    aeron::Context aeron_context_;
    std::shared_ptr<aeron::Aeron> aeron_instance_;

};