#pragma once

#include <Aeron.h>

#include <string>


/**
 * @brief Helper for interop with Aeron Image type.
 */
class image
{
public:
    /**
     *
     * @param self
     * @return
     */
    static int32_t session_id(aeron::Image& self);
    /**
     *
     * @param self
     * @return
     */
    static bool is_end_of_stream(aeron::Image& self);
    /**
     *
     * @param self
     * @return
     */
    static bool is_closed(aeron::Image& self);
    /**
     *
     */
    static void close(aeron::Image& self);

    /**
     *
     * @return
     */
    static std::string __str__(aeron::Image& self);

};
