#include <3rdparty/rapidjson/stringbuffer.h>
#include <3rdparty/rapidjson/prettywriter.h>

#include "log/Log.h"
#include "ControlCommand.h"

ControlCommand::ControlCommand()
    : m_command(Command::START)
{

}

ControlCommand::ControlCommand(Command command)
    : m_command(command)
{

}

bool ControlCommand::parseFromJsonString(const std::string& json)
{
    bool result = false;

    rapidjson::Document document;
    if (!document.Parse(json.c_str()).HasParseError()) {
        result = parseFromJson(document);
    }

    return result;
}

bool ControlCommand::parseFromJson(const rapidjson::Document& document)
{
    bool result = false;

    if (document.HasMember("control_command")) {
        rapidjson::Value::ConstObject controlCommand = document["control_command"].GetObject();
        if (controlCommand.HasMember("command")) {
            m_command = toCommand(controlCommand["command"].GetString());
            result = true;
        }
        else {
            LOG_ERR("Parse Error, JSON does not contain: command");
        }
    } else {
        LOG_ERR("Parse Error, JSON does not contain: control_command");
    }

    return result;
}

rapidjson::Value ControlCommand::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
{
    rapidjson::Value controlCommand(rapidjson::kObjectType);

    controlCommand.AddMember("command", rapidjson::StringRef(toString(m_command)), allocator);

    return controlCommand;
}

void ControlCommand::setCommand(Command command)
{
    m_command = command;
}

ControlCommand::Command ControlCommand::getCommand() const
{
    return m_command;
}

bool ControlCommand::isOneTimeCommand() const {

    return m_command == ControlCommand::UPDATE_CONFIG ||
           m_command == ControlCommand::PUBLISH_CONFIG ||
           m_command == ControlCommand::RESTART ||
           m_command == ControlCommand::SHUTDOWN;
}
