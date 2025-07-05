#ifndef BEBOP_CLIENGINE_PARSER_HPP
#define BEBOP_CLIENGINE_PARSER_HPP

#include <algorithm>
#include <optional>
#include <stdexcept>

#include "Defines.hpp"

namespace bebop::cliengine
{

/**
 * @brief Provides functionality for parsing command-line input
 *        into structured Input representations based on predefined command definitions.
 */
struct Parser
{
    /**
     * @brief Parses the CLI input using the provided command definition.
     *
     * This function interprets `argc` and `argv`, validates and extracts positional arguments
     * and flags according to the given `CommandDef`, and produces a fully populated `Input` structure.
     *
     * @param def The command definition used to validate the input.
     * @param argc The number of arguments passed to the program.
     * @param argv The array of argument strings passed to the program.
     * @return A populated Input object representing the parsed command input.
     *
     * @throws std::invalid_argument If any required arguments are missing or cannot be parsed.
     *
     * @see bebop::cliengine::Input
     * @see bebop::cliengine::CommandDef
     */
    static Input parse(const CommandDef& def, int argc, char* argv[]);

   private:
    /**
     * @brief Extracts raw command-line tokens from argc/argv.
     *
     * This function skips the first argument (the program name) and converts the rest
     * into a vector of strings for easier processing.
     *
     * @param argc The number of arguments passed to the program.
     * @param argv The array of argument strings.
     * @return A vector containing CLI tokens from argv[1] to argv[argc - 1].
     */
    static std::vector<std::string> extractTokens(int argc, char* argv[]) noexcept;

    /**
     * @brief Represents raw unsorted CLI tokens split into flags and positional arguments.
     *
     * This structure is used internally by the parser to separate raw user input
     * into named flags with optional values and unnamed positional arguments.
     */
    struct RawTokenSplit
    {
        std::unordered_map<std::string, std::string> flags;
        std::vector<std::string> args;
    };

    /**
     * @brief Splits CLI tokens into raw flags and positional arguments.
     *
     * Parses a flat list of CLI tokens (such as those extracted from argv) and separates them into:
     * - Named flags with optional values (e.g., --volume 10)
     * - Unnamed positional arguments (e.g., filename, count, etc.)
     *
     * Flags are expected to start with '--' and may optionally be followed by a value.
     * If a flag is the last token or is followed by another flag, it is treated as a presence-only flag.
     *
     * @param tokens A list of command-line tokens excluding the program name.
     * @return A RawTokenSplit structure containing:
     *         - A map of flag names to their associated raw string values (empty string if none),
     *         - A list of positional argument strings.
     *
     * @note This function does not perform type validation or match arguments to the schema.
     *
     * @see RawTokenSplit
     */
    static RawTokenSplit splitRawTokens(const std::vector<std::string>& tokens) noexcept;

    /**
     * @brief Parses a raw string into a typed ArgumentValue.
     *
     * Converts a raw user input string into a typed variant based on the specified ArgumentType.
     * Handles conversion for string, integer, float, boolean, or presence-only (None) types.
     *
     * @param raw The raw input string from CLI.
     * @param type The expected type of the argument.
     * @return The parsed value stored in an ArgumentValue variant.
     *
     * @throws std::invalid_argument If parsing fails or input is invalid for the given type.
     *
     * @see bebop::cliengine::ArgumentType
     * @see bebop::cliengine::ArgumentValue
     */
    static ArgumentValue parseValue(const std::string& raw, ArgumentType type);
};

}  // namespace bebop::cliengine

#endif  // BEBOP_CLIENGINE_PARSER_HPP
