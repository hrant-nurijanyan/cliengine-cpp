#ifndef BEBOP_CLIENGINE_ENGINE_HPP
#define BEBOP_CLIENGINE_ENGINE_HPP

#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include "Defines.hpp"
#include "Parser.hpp"

// Precompiled header include
#ifdef CLIENGINE_PRECOMPILED_HEADER_PATH
#include CLIENGINE_PRECOMPILED_HEADER_PATH
#endif  // CLIENGINE_PRECOMPILED_HEADER_PATH

namespace bebop::cliengine
{

/**
 * @brief Type alias for command handler callbacks.
 *
 * Each callback receives the fully parsed CLI input, including:
 * - the name of the invoked command,
 * - the values of positional arguments (in order),
 * - the values of named flags.
 */
using CommandCallback = std::function<void(const Input& input)>;

/**
 * @brief Central class of the CLI engine responsible for handling commands and dispatching execution.
 *
 * This class supports:
 * - Loading precompiled command definitions,
 * - Registering user-defined callbacks for commands,
 * - Parsing and executing CLI input.
 *
 * The engine expects that the user links a precompiled command header, defined by:
 * `CLIENGINE_PRECOMPILED_HEADER_PATH`, defaulting to `"PrecompiledCommands.hpp"`.
 *
 * Example usage:
 * @code
 * bebop::cliengine::Engine engine;
 * engine.loadPredefinedCommands();
 * engine.registerCallback("play", [](const Input& input) {
 *     // Handle "play" command logic here
 * });
 * return engine.execute(argc, argv);
 * @endcode
 */
struct Engine
{
    /**
     * @brief Registers a callback for a specific command.
     *
     * The callback will be invoked during execution when the corresponding command is matched.
     * Each command can only have one associated callback.
     *
     * @param command The name of the command to register the callback for.
     * @param callback The function to invoke when the command is executed.
     */
    void registerCallback(const std::string& command, CommandCallback callback) noexcept;

    /**
     * @brief Parses and executes a CLI command based on program input.
     *
     * This method tokenizes, validates, and parses the command-line arguments.
     * If the command matches one of the definitions and has a registered callback,
     * it is invoked with the parsed input.
     *
     * @param argc Argument count from main.
     * @param argv Argument values from main.
     * @return true if a valid command was matched and executed, false otherwise.
     */
    bool execute(int argc, char* argv[]) noexcept;

   private:
    std::unordered_map<std::string, CommandCallback> callbacks;  // Storage of command callbacks
};

}  // namespace bebop::cliengine

#endif  // BEBOP_CLIENGINE_ENGINE_HPP
