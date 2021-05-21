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
        handler::on_notification(notification);

        switch (notification->GetType())
        {
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
