#include <cliengine/Engine.hpp>

void bebop::cliengine::Engine::loadPredefinedCommands() noexcept
{
#ifndef CLIENGINE_PRECOMPILED_HEADER_PATH
    std::cerr << "[cliengine] Warning: CLIENGINE_PRECOMPILED_HEADER_PATH macro not defined. No commands loaded.\n";
    return;

#else
    if (predefined::commandDefs.empty())
    {
        std::cerr << "[cliengine] Warning: Precompiled command definitions are empty.\n";
    }
    for (const auto& commandDef : predefined::commandDefs)
    {
        commandDefs[commandDef.name] = commandDef;
    }
#endif  // CLIENGINE_PRECOMPILED_HEADER_PATH
}

void bebop::cliengine::Engine::registerCallback(const std::string& command, CommandCallback callback) noexcept
{
    if (commandDefs.find(command) == commandDefs.end())
    {
        return;  // silently ignore unknown commands
    }

    callbacks[command] = std::move(callback);
}

bool bebop::cliengine::Engine::execute(int argc, char* argv[]) noexcept
{
    if (argc <= 1)  // No input
    {
        return true;
    }

    std::string commandName = argv[1];

    const auto defIt = commandDefs.find(commandName);
    if (defIt == commandDefs.end())  // Unknown command
    {
        return false;
    }

    const auto cbIt = callbacks.find(commandName);
    if (cbIt == callbacks.end())  // No callback
    {
        return true;
    }

    try
    {
        const auto input = Parser::parse(defIt->second, argc, argv);
        cbIt->second(input);
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Engine] Execution error: " << e.what() << "\n";
        return false;
    }

    return true;
}
