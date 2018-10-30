#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>


/**
 * @brief
 */
class publication final
{
public:
    /**
     * @brief
     * @param aeron_publication
     */
    publication(std::shared_ptr<aeron::Publication> aeron_publication);

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
     * @brief
     * @return
     */
    int32_t session_id() const;
    /**
     * @brief
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

    void close();

    /**
     * @brief Checks state of the publisher.
     * @return True if publisher is connected and is ready to offer data.
     */
    bool __bool__() const;
    /**
     * @brief Provides debug description of publication.
     * @return Debug description of publication.
     */
    std::string __str__() const;

private:
    std::shared_ptr<aeron::Publication> aeron_publication_;

};
