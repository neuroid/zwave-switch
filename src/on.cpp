#include <unistd.h>

#include <openzwave/Manager.h>

#include "on.hpp"

namespace zwave_switch
{
    on::on(uint8_t node_id, bool value, const handler::callback& done)
        : handler(done), node_id_(node_id), value_(value), value_id_((uint32)0, (uint64)0), queried_(false)
    {
    }

    on::on(uint8_t node_id, const handler::callback& done)
        : on(node_id, false, done)
    {
        listen_ = true;
    }

    void on::on_notification(const OZW::Notification* notification)
    {
        switch (notification->GetType())
        {
            case OZW::Notification::Type_NodeAdded:
            {
                if (notification->GetNodeId() == node_id_)
                {
                    found_ = true;
                }

                break;
            }
            case OZW::Notification::Type_ValueAdded:
            {
                if (notification->GetNodeId() == node_id_)
                {
                    auto value_id = notification->GetValueID();

                    if (value_id.GetCommandClassId() == 0x25 &&
                        value_id.GetType() == OZW::ValueID::ValueType_Bool)
                    {
                        value_id_ = value_id;
                    }
                }

                break;
            }
            case OZW::Notification::Type_ValueChanged:
            {
                if (queried_ &&
                    notification->GetNodeId() == node_id_ &&
                    notification->GetValueID() == value_id_)
                {
                    bool value = false;
                    if (!OZW::Manager::Get()->GetValueAsBool(value_id_, &value))
                    {
                        done_(false);
                        return;
                    }

                    std::cout << "Switch "
                              << static_cast<int32_t>(notification->GetNodeId())
                              << " [" << OZW::Manager::Get()->GetNodeProductName(
                                     notification->GetHomeId(), notification->GetNodeId()) << "]"
                              << " is " << (value ? "ON" : "OFF") << std::endl;

                    if (!listen_)
                    {
                        done_(true);
                        return;
                    }
                }

                break;
            }
            case OZW::Notification::Type_AllNodesQueried:
            case OZW::Notification::Type_AllNodesQueriedSomeDead:
            {
                queried_ = true;

                if (!found_)
                {
                    std::cout << "Node not found" << std::endl;
                    done_(false);
                    return;
                }

                if (value_id_.GetHomeId() == 0)
                {
                    std::cout << "Node is not a switch" << std::endl;
                    done_(false);
                    return;
                }

                if (listen_)
                {
                    std::cout << "Listening [pid: " << getpid() << "]..." << std::endl;
                    return;
                }

                if (!OZW::Manager::Get()->SetValue(value_id_, value_))
                {
                    done_(false);
                    return;
                }

                break;
            }
            case OZW::Notification::Type_Notification:
            {
                if (notification->GetNotification() == OZW::Notification::Code_Dead &&
                    notification->GetNodeId() == node_id_)
                {
                    std::cout << "Node has failed" << std::endl;
                    done_(false);
                    return;
                }
            }
            default:
            {
            }
        }
    }

    void on::on_signal(bool value)
    {
        if (listen_ && queried_)
        {
            if (!OZW::Manager::Get()->SetValue(value_id_, value))
            {
                done_(false);
                return;
            }
        }
    }
}
