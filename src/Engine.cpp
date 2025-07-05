#include <cliengine/Engine.hpp>

#ifdef CLIENGINE_PRECOMPILED_HEADER_PATH

void bebop::cliengine::Engine::registerCallback(const std::string& command, CommandCallback callback) noexcept
{
    if (precompiled::commandDefs.find(command) == precompiled::commandDefs.end())
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

    const auto defIt = precompiled::commandDefs.find(commandName);
    if (defIt == precompiled::commandDefs.end())  // Unknown command
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

#endif CLIENGINE_PRECOMPILED_HEADER_PATH