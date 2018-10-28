#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>


class publication
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
     *
     * @return
     */
    int32_t session_id() const;

    int32_t initial_term_id() const;

    int64_t offer(pybind11::object data);

    void close();

    bool __bool__() const;

private:
    std::shared_ptr<aeron::Publication> aeron_publication_;

};
