#include <condition_variable>
#include <csignal>
#include <iostream>

#include <argh.h>
#include <openzwave/Manager.h>
#include <openzwave/Notification.h>
#include <openzwave/Options.h>

#include "add.hpp"
#include "config.hpp"
#include "on.hpp"
#include "remove.hpp"
#include "remove_failed.hpp"

namespace OZW = OpenZWave;

std::condition_variable shutdown;
std::mutex shutdown_mutex;
zwave_switch::on* listen = nullptr;

int usage(const argh::parser &args)
{
    std::cout << "Usage: " << args[0] << " CONTROLLER add [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER on NODE_ID [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER off NODE_ID [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER listen NODE_ID [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER config get NODE_ID INDEX [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER config set NODE_ID INDEX VALUE [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER remove [OPTION]..." << std::endl
              << "       " << args[0] << " CONTROLLER remove failed [OPTION]..." << std::endl
              << std::endl
              << "Options:" << std::endl
              << "  -c, --ozw-config-path PATH    Path to OpenZWave device database" << std::endl
              << "  -u, --ozw-user-path PATH      Path to OpenZWave user data" << std::endl;

    return 2;
}

uint8_t as_uint8_t(argh::string_stream stream)
{
    int32_t value = 0;
    if ((stream >> value) && value > 0 && value <= 255)
        return static_cast<uint8_t>(value);
    return 0;
}

void on_notification(const OZW::Notification* notification, void* context)
{
    static_cast<zwave_switch::handler*>(context)->on_notification(notification);
}

void on_signal(int signal)
{
    switch (signal)
    {
        case SIGUSR1:
        case SIGUSR2:
        {
            if (listen)
                listen->on_signal(signal == SIGUSR1);
            break;
        }
        default:
        {
            shutdown.notify_one();
        }
    }
}

int main(int argc, char* argv[])
{
    argh::parser args(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

    if (args[{"-h", "--help"}])
    {
        usage(args);
        return 0;
    }

    if (!args(2))
    {
        return usage(args);
    }

    zwave_switch::handler* handler = nullptr;
    int exit_code = 0;
    auto done = [&exit_code](bool success) {
        if (!success)
            exit_code = 1;
        shutdown.notify_one();
    };

    if (args[2] == "add")
    {
        handler = new zwave_switch::add(done);
    }
    else if (args[2] == "listen" || args[2] == "off" || args[2] == "on")
    {
        auto node_id = as_uint8_t(args(3));

        if (node_id == 0)
        {
            std::cout << "Invalid node id" << std::endl;
            return 1;
        }

        if (args[2] == "listen")
            handler = listen = new zwave_switch::on(node_id, done);
        else
            handler = new zwave_switch::on(node_id, args[2] == "on", done);
    }
    else if (args[2] == "config")
    {
        auto node_id = as_uint8_t(args(4));
        auto value_index = as_uint8_t(args(5));

        if (node_id == 0)
        {
            std::cout << "Invalid node id" << std::endl;
            return 1;
        }

        if (value_index == 0)
        {
            std::cout << "Invalid index" << std::endl;
            return 1;
        }

        if (args[3] == "get")
        {
            handler = new zwave_switch::config(node_id, value_index, done);
        }
        else if (args[3] == "set")
        {
            int32_t value = 0;
            if (!(args(6) >> value))
            {
                std::cout << "Invalid value" << std::endl;
                return 1;
            }

            handler = new zwave_switch::config(node_id, value_index, value, done);
        }
        else
        {
            return usage(args);
        }
    }
    else if (args[2] == "remove")
    {
        if (args[3] == "failed")
            handler = new zwave_switch::remove_failed(done);
        else
            handler = new zwave_switch::remove(done);
    }
    else
    {
        return usage(args);
    }

    auto options = OZW::Options::Create(args({"-c", "--ozw-config-path"}, "/etc/openzwave").str(),
                                        args({"-u", "--ozw-user-path"}).str(),
                                        "");
    options->AddOptionBool("ConsoleOutput", false);
    options->Lock();

    auto manager = OZW::Manager::Create();
    manager->AddWatcher(on_notification, handler);
    manager->AddDriver(args[1]);

    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);
    std::signal(SIGUSR1, on_signal);
    std::signal(SIGUSR2, on_signal);

    std::unique_lock<std::mutex> lock(shutdown_mutex);
    shutdown.wait(lock);

    OZW::Manager::Destroy();
    OZW::Options::Destroy();

    return exit_code;
}
