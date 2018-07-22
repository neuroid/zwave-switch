#ifndef ZWAVE_SWITCH_REMOVE_FAILED_H
#define ZWAVE_SWITCH_REMOVE_FAILED_H

#include <vector>

#include "handler.hpp"

namespace zwave_switch
{
    class remove_failed
        : public handler
    {
    public:
        remove_failed(const handler::callback& done);

        void on_notification(const OZW::Notification* notification);

    private:
        std::vector<uint8_t> node_ids_;
        size_t pending_controller_commands_ = 0;
    };
}

#endif // ZWAVE_SWITCH_REMOVE_FAILED_H
