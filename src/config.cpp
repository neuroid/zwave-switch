#include <openzwave/Manager.h>

#include "config.hpp"

namespace zwave_switch
{
    config::config(uint8_t node_id, uint8_t value_index, uint32_t value, const handler::callback& done)
        : handler(done), node_id_(node_id), value_index_(value_index), value_(value), value_id_((uint32)0, (uint64)0)
    {
    }

    config::config(uint8_t node_id, uint8_t value_index, const handler::callback& done)
        : config(node_id, value_index, 0, done)
    {
        set_ = false;
    }

    void config::on_notification(const OZW::Notification* notification)
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

                    if (value_id.GetCommandClassId() == 0x70 &&
                        value_id.GetIndex() == value_index_)
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
                    std::string value;
                    if (!OZW::Manager::Get()->GetValueAsString(value_id_, &value))
                    {
                        done_(false);
                        return;
                    }

                    std::cout << value << std::endl;
                    done_(true);
                    return;
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
                    std::cout << "Value not found" << std::endl;
                    done_(false);
                    return;
                }

                if (set_)
                {
                    auto manager = OZW::Manager::Get();
                    uint32_t min = manager->GetValueMin(value_id_);
                    uint32_t max = manager->GetValueMax(value_id_);

                    if (value_ < min || value_ > max)
                    {
                        std::cout << "Value must be between "
                                  << min << " and " << max << std::endl;
                        done_(false);
                        return;
                    }

                    if (!manager->SetConfigParam(value_id_.GetHomeId(),
                                                 node_id_,
                                                 value_index_,
                                                 value_))
                    {
                        done_(false);
                    }

                    return;
                }

                std::string value;
                if (!OZW::Manager::Get()->GetValueAsString(value_id_, &value))
                {
                    done_(false);
                    return;
                }

                std::cout << value << std::endl;
                done_(true);
                return;
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
}
