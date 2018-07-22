#include <openzwave/Manager.h>

#include "remove_failed.hpp"

namespace zwave_switch
{
    remove_failed::remove_failed(const handler::callback& done)
        : handler(done)
    {
    }

    void remove_failed::on_notification(const OZW::Notification* notification)
    {
        switch (notification->GetType())
        {
            case OZW::Notification::Type_NodeAdded:
            {
                node_ids_.push_back(notification->GetNodeId());
                break;
            }
            case OZW::Notification::Type_NodeRemoved:
            {
                std::cout << "Removed node "
                          << static_cast<int32_t>(notification->GetNodeId())
                          << std::endl;
                break;
            }
            case OZW::Notification::Type_AllNodesQueried:
            {
                done_(true);
                return;
            }
            case OZW::Notification::Type_AllNodesQueriedSomeDead:
            {
                for (auto node_id : node_ids_)
                {
                    if (!OZW::Manager::Get()->HasNodeFailed(notification->GetHomeId(), node_id))
                    {
                        done_(false);
                        return;
                    }

                    pending_controller_commands_++;
                }

                break;
            }
            case OZW::Notification::Type_ControllerCommand:
            {
                if (notification->GetEvent() == OpenZWave::Driver::ControllerState_Completed ||
                    notification->GetEvent() == OpenZWave::Driver::ControllerState_NodeOK ||
                    notification->GetEvent() == OpenZWave::Driver::ControllerState_NodeFailed)
                {
                    if (--pending_controller_commands_ == 0)
                    {
                        done_(true);
                        return;
                    }
                }

                break;
            }
            case OZW::Notification::Type_Notification:
            {
                if (pending_controller_commands_ > 0 &&
                    notification->GetNotification() == OZW::Notification::Code_Dead)
                {
                    if (!OZW::Manager::Get()->RemoveFailedNode(notification->GetHomeId(),
                                                               notification->GetNodeId()))
                    {
                        done_(false);
                        return;
                    }

                    pending_controller_commands_++;
                }
            }
            default:
            {
            }
        }
    }
}
