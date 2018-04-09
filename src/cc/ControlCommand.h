#ifndef __CONTROL_COMMAND_H__
#define __CONTROL_COMMAND_H__

#include <string>
#include "rapidjson/document.h"

static const char* command_str[6] = {
        "START",
        "STOP",
        "UPDATE_CONFIG",
        "PUBLISH_CONFIG",
        "RESTART",
        "SHUTDOWN"
};

class ControlCommand
{
public:
    enum Command {
        START,
        STOP,
        UPDATE_CONFIG,
        PUBLISH_CONFIG,
        RESTART,
        SHUTDOWN
    };

public:
    ControlCommand();
    explicit ControlCommand(Command command);

    static inline const char *toString (Command command)
    {
        return command_str[static_cast<int>(command)];
    }

    static inline Command toCommand (const char *command)
    {
        const int n = sizeof(command_str) / sizeof(command_str[0]);
        for (int i = 0; i < n; ++i)
        {
            if (strcmp(command_str[i], command) == 0)
                return (Command) i;
        }
        return Command::START;
    }

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
    bool parseFromJsonString(const std::string& json);
    bool parseFromJson(const rapidjson::Document& document);

    Command getCommand() const;
    void setCommand(Command command);

    bool isOneTimeCommand() const;

private:
    Command m_command;
};

#endif /* __CONTROL_COMMAND_H__ */
