#include <openzwave/Manager.h>

#include "remove.hpp"

namespace zwave_switch
{
    remove::remove(const handler::callback& done)
        : handler(done)
    {
    }

    void remove::on_notification(const OZW::Notification* notification)
    {
        handler::on_notification(notification);

        switch (notification->GetType())
        {
            case OZW::Notification::Type_NodeRemoved:
            {
                std::cout << "Removed node "
                          << static_cast<int32_t>(notification->GetNodeId())
                          << std::endl;
                break;
            }
            case OZW::Notification::Type_AllNodesQueried:
            case OZW::Notification::Type_AllNodesQueriedSomeDead:
            {
                if (!OZW::Manager::Get()->RemoveNode(notification->GetHomeId()))
                {
                    done_(false);
                    return;
                }

                std::cout << "Press a button on your switch to remove it from the network..." << std::endl;
                break;
            }
            default:
            {
            }
        }
    }
}
