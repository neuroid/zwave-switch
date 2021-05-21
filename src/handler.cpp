#include <openzwave/Manager.h>

#include "handler.hpp"

namespace zwave_switch
{
    handler::handler(const handler::callback& done)
        : done_(done)
    {
    }

    void handler::on_notification(const OZW::Notification* notification)
    {
        switch (notification->GetType())
        {
            case OZW::Notification::Type_DriverReady:
            {
                std::cout << "Controller ready ["
                          << notification->GetHomeId()
                          << "]" << std::endl;
                break;
            }
            case OZW::Notification::Type_NodeNew:
            {
                std::cout << "Found a new node" << std::endl;
                break;
            }
            case OZW::Notification::Type_NodeAdded:
            {
                std::cout << "Adding node "
                          << static_cast<int32_t>(notification->GetNodeId())
                          << "..." << std::endl;
                break;
            }
            case OZW::Notification::Type_EssentialNodeQueriesComplete:
            {
                std::cout << "Completed essential queries on node "
                          << static_cast<int32_t>(notification->GetNodeId())
                          << "..." << std::endl;
                break;
            }
            case OZW::Notification::Type_NodeQueriesComplete:
            {
                std::cout << "Added node "
                          << static_cast<int32_t>(notification->GetNodeId())
                          << " [" << OZW::Manager::Get()->GetNodeProductName(
                                 notification->GetHomeId(), notification->GetNodeId()) << "]"
                          << std::endl;
                break;
            }
            default:
            {
            }
        }
    }
}
