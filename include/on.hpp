#ifndef ZWAVE_SWITCH_ON_H
#define ZWAVE_SWITCH_ON_H

#include <atomic>

#include "handler.hpp"

namespace zwave_switch
{
    class on
        : public handler
    {
    public:
        on(uint8_t node_id, bool value, const handler::callback& done);

        on(uint8_t node_id, const handler::callback& done);

        void on_notification(const OZW::Notification* notification);

        void on_signal(bool value);

    private:
        uint8_t node_id_ = 0;
        bool value_ = true;
        bool listen_ = false;
        bool found_ = false;
        bool dead_ = false;
        OZW::ValueID value_id_;
        std::atomic_bool queried_;
    };
}

#endif // ZWAVE_SWITCH_ON_H
