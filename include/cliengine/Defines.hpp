#ifndef BEBOP_CLIENGINE_DEFINES_HPP
#define BEBOP_CLIENGINE_DEFINES_HPP

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace bebop::cliengine
{

/**
 * @brief Supported argument types for CLI inputs.
 */
enum class ArgumentType
{
    String,   ///< Represents a string value.
    Integer,  ///< Represents an integer value.
    Float,    ///< Represents a floating-point value.
    Boolean,  ///< Represents a boolean value (true/false).
    None      ///< Represents a flag or argument that takes no value.
};

/**
 * @brief Represents the value of a parsed CLI argument or flag.
 *
 * This is a variant type that can hold:
 * - std::monostate (no value),
 * - std::string,
 * - int,
 * - float,
 * - bool
 */
using ArgumentValue = std::variant<std::monostate, std::string, int, float, bool>;

/**
 * @brief Defines a CLI argument or flag.
 *
 * This structure is used to describe both positional arguments and named flags.
 */
struct ArgumentDef
{
    std::string name;                        ///< Name of the argument or flag (e.g., "filename", "--verbose").
    ArgumentType type = ArgumentType::None;  ///< Type of the value the argument or flag expects.
    bool required = false;                   ///< Whether this argument or flag is required.
};

/**
 * @brief Defines a command and its associated arguments and flags.
 */
struct CommandDef
{
    std::string name;                ///< Name of the command (e.g., "play", "convert").
    std::string description;         ///< Short description of what the command does.
    std::vector<ArgumentDef> args;   ///< Ordered list of positional arguments.
    std::vector<ArgumentDef> flags;  ///< Named flags with optional values.
};

/**
 * @brief Represents parsed CLI input at runtime.
 *
 * This structure is returned by the parser and contains:
 * - the name of the matched command,
 * - ordered positional argument values,
 * - flag values (indexed by name).
 */
struct Input
{
    std::string command;                                   ///< Name of the command that was parsed.
    std::vector<ArgumentValue> args;                       ///< Ordered list of parsed positional argument values.
    std::unordered_map<std::string, ArgumentValue> flags;  ///< Map of flag names to their parsed values.
};

}  // namespace bebop::cliengine

#endif  // BEBOP_CLIENGINE_DEFINES_HPP
