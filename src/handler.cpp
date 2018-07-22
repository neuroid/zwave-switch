#include "handler.hpp"

namespace zwave_switch
{
    handler::handler(const handler::callback& done)
        : done_(done)
    {
    }
}
