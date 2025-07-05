#include <cliengine/Parser.hpp>

bebop::cliengine::Input bebop::cliengine::Parser::parse(const CommandDef& def, int argc, char* argv[])
{
    // Extracting tokens first
    const auto& tokens = extractTokens(argc, argv);

    // Split raw tokens
    const auto& raw = splitRawTokens(tokens);

    // Parse positionals
    std::vector<ArgumentValue> args;
    for (std::size_t i = 0; i < def.args.size(); ++i)
    {
        const auto& argDef = def.args[i];
        if (i >= raw.args.size())  // Ran out of arguments
        {
            if (argDef.required)  // If the argument was required but missing, throw
            {
                throw std::invalid_argument("Missing required positional argument: " + argDef.name);
            }

            args.emplace_back(std::monostate{});  // Emplace a monostate for optional positionals
            continue;
        }

        try  // Try to parse the value of the positional
        {
            args.emplace_back(parseValue(raw.args[i], argDef.type));
        }
        catch (const std::exception& e)
        {
            throw std::invalid_argument("Failed to parse positional argument: " + argDef.name + "\n" + e.what());
        }
    }

    // Parse flags
    std::unordered_map<std::string, ArgumentValue> flags;
    for (const auto& flagDef : def.flags)
    {
        if (raw.flags.find(flagDef.name) == raw.flags.end())  // Flag not found in raw flags
        {
            if (flagDef.required)  // If was required, throw
            {
                throw std::invalid_argument("Missing required flag: " + flagDef.name);
            }

            flags[flagDef.name] = std::monostate{};  // Optional flag, monostate value
            continue;
        }
        try
        {
            flags[flagDef.name] = parseValue(raw.flags.at(flagDef.name), flagDef.type);
        }
        catch (const std::invalid_argument& e)
        {
            throw std::invalid_argument("Failed to parse flag: " + flagDef.name + "\n" + e.what());
        }
    }

    return {def.name, args, flags};
}

std::vector<std::string> bebop::cliengine::Parser::extractTokens(int argc, char* argv[]) noexcept
{
    return {argv + 1, argv + argc};  // Skipping the program name
}

bebop::cliengine::Parser::RawTokenSplit bebop::cliengine::Parser::splitRawTokens(const std::vector<std::string>& tokens) noexcept
{
    // Helper function to detect whether a token is a flag or not
    auto isFlag = [](const std::string& token) { return token.size() > 2 && token[0] == '-' && token[1] == '-'; };

    std::unordered_map<std::string, std::string> flagRawValues;
    std::vector<std::string> positionalRawValues;

    for (std::size_t i = 0; i < tokens.size(); ++i)
    {
        const auto& token = tokens[i];
        if (!isFlag(token))  // Treat as a positional argument
        {
            positionalRawValues.emplace_back(token);
            continue;
        }

        flagRawValues[token] = "";   // Store as non value flag at first
        if (i + 1 == tokens.size())  // If this was the last token, break
            break;

        const auto& nextToken = tokens[i + 1];
        if (isFlag(nextToken))  // If the next token is a flag, continue
            continue;

        flagRawValues[token] = nextToken;  // Its the value, store it
        i += 1;                            // Skip the value token
    }

    return {flagRawValues, positionalRawValues};
}

bebop::cliengine::ArgumentValue bebop::cliengine::Parser::parseValue(const std::string& raw, ArgumentType type)
{
    if (type == ArgumentType::None && raw.empty())  // Absence of value returns a monostate
    {
        return std::monostate{};
    }

    if (type == ArgumentType::String && !raw.empty())  // String values are returned as is
    {
        return raw;
    }

    if (type == ArgumentType::Boolean)
    {
        if (raw == "true" || raw == "True")
            return true;
        if (raw == "false" || raw == "False")
            return false;
    }

    try
    {
        auto floatVal = std::stof(raw);
        if (type == ArgumentType::Float)
            return floatVal;
        if (type == ArgumentType::Integer)
            return static_cast<int>(floatVal);
    }
    catch (...)
    {
        throw std::invalid_argument("Invalid argument value for a type");  // Throw when casting falis
    }

    throw std::invalid_argument("Invalid argument value for a type");  // Throw if return did not work
}