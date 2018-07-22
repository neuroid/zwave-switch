#ifndef ZWAVE_SWITCH_ADD_H
#define ZWAVE_SWITCH_ADD_H

#include "handler.hpp"

namespace zwave_switch
{
    class add
        : public handler
    {
    public:
        add(const handler::callback& done);

        void on_notification(const OZW::Notification* notification);
    };
}

#endif // ZWAVE_SWITCH_ADD_H
