#ifndef ZWAVE_SWITCH_CONFIG_H
#define ZWAVE_SWITCH_CONFIG_H

#include "handler.hpp"

namespace zwave_switch
{
    class config
        : public handler
    {
    public:
        config(uint8_t node_id, uint8_t value_index, uint32_t value, const handler::callback& done);

        config(uint8_t node_id, uint8_t value_index, const handler::callback& done);

        void on_notification(const OZW::Notification* notification);

    private:
        uint8_t node_id_ = 0;
        uint8_t value_index_ = 0;
        uint32_t value_ = 0;
        bool set_ = true;
        bool found_ = false;
        OZW::ValueID value_id_;
        bool queried_ = false;
    };
}

#endif // ZWAVE_SWITCH_CONFIG_H
