#ifndef ZWAVE_SWITCH_HANDLER_H
#define ZWAVE_SWITCH_HANDLER_H

#include <functional>

#include <openzwave/Notification.h>

namespace OZW = OpenZWave;

namespace zwave_switch
{
    class handler
    {
    public:
        typedef std::function<void(bool)> callback;

        virtual void on_notification(const OZW::Notification* notification) = 0;

    protected:
        handler(const callback& done);

        callback done_;
    };
}

#endif // ZWAVE_SWITCH_HANDLER_H
