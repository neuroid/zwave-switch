#include <openzwave/Manager.h>

#include "add.hpp"

namespace zwave_switch
{
    add::add(const handler::callback& done)
        : handler(done)
    {
    }

    void add::on_notification(const OZW::Notification* notification)
    {
        switch (notification->GetType())
        {
            case OZW::Notification::Type_NodeQueriesComplete:
            {
                std::cout << "Added node "
                          << static_cast<int32_t>(notification->GetNodeId())
                          << " [" << OZW::Manager::Get()->GetNodeProductName(
                                 notification->GetHomeId(), notification->GetNodeId()) << "]"
                          << std::endl;
                break;
            }
            case OZW::Notification::Type_AllNodesQueried:
            case OZW::Notification::Type_AllNodesQueriedSomeDead:
            {
                if (!OZW::Manager::Get()->AddNode(notification->GetHomeId()))
                {
                    done_(false);
                    return;
                }

                std::cout << "Press a button on your switch to add it to the network..." << std::endl;
                break;
            }
            default:
            {
            }
        }
    }
}
