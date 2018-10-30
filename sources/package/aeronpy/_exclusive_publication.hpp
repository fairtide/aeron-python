#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <memory>


/**
 * @brief
 */
class exclusive_publication final
{
public:
    /**
     * @brief
     * @param aeron_exclusive_publication
     */
    exclusive_publication(std::shared_ptr<aeron::ExclusivePublication> aeron_exclusive_publication);

    /**
     * @brief
     * @return
     */
    const std::string& channel() const;
    /**
     * @brief
     * @return
     */
    int64_t stream_id() const;
    /**
     *
     * @return
     */
    int32_t session_id() const;
    /**
     *
     * @return
     */
    int32_t initial_term_id() const;
    /**
     * @brief
     * @return
     */
    bool is_connected() const;
    /**
     * @brief
     * @return
     */
    bool is_closed() const;
    /**
     * @brief
     * @return
     */
    bool is_original() const;

    /**
     * Offers a data block to open stream this publisher is for.
     * @param data Data block to be offered to the stream.
     * @return Number of bytes sent or BACK_PRESSURED or NOT_CONNECTED.
     */
    int64_t offer(pybind11::object data);

    /**
     * @brief
     */
    void close();

    /**
     * @brief
     * @return
     */
    bool __bool__() const;
    /**
     * @brief
     * @return
     */
    std::string __str__() const;

private:
    std::shared_ptr<aeron::ExclusivePublication> aeron_exclusive_publication_;

};
