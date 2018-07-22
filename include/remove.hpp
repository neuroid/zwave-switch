#ifndef ZWAVE_SWITCH_REMOVE_H
#define ZWAVE_SWITCH_REMOVE_H

#include "handler.hpp"

namespace zwave_switch
{
    class remove
        : public handler
    {
    public:
        remove(const handler::callback& done);

        void on_notification(const OZW::Notification* notification);
    };
}

#endif // ZWAVE_SWITCH_REMOVE_H
