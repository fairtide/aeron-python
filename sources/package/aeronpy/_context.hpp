#pragma once

#include "_exclusive_publication.hpp"
#include "_publication.hpp"
#include "_subscription.hpp"

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <list>
#include <memory>
#include <string>


/**
 * @brief Provides a helper for exposing Aeron to python.
 */
class context final
{
public:
    /**
     * @brief Creates a new instance of Aeron interop context.
     * @details
     * This constructor
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
    /**
     *
     * @param channel
     * @param stream_id
     * @return
     */
    exclusive_publication add_exclusive_publication(const std::string& channel, int32_t stream_id);

private:
    aeron::Context aeron_context_;
    std::shared_ptr<aeron::Aeron> aeron_instance_;

};