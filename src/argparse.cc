#include "argparse.hpp"

#include <cstddef>
#include <iostream>

namespace cellular_automaton
{

Argparse::Argument::Argument(const std::string &identifier,
                             const std::string &help, const bool is_optional)
    : identifier(identifier), help(help), optional(is_optional)
{
}

Argparse::Argparse(std::string help, std::vector<Argument> args)
    : help_(help), args_(std::move(args))
{
}

bool Argparse::parse(const int argc, char **argv)
{
    // Iterate arg string.
    for (int i = 0; i < argc; ++i)
    {
        // Check if help was requested.
        if (std::string_view("-help").compare(argv[i]) == 0)
        {
            // Failure print help text.
            print_help_();
            return false;
        }

        // Reference check argument candidates.
        // FIXME: Use map instead of vector.
        for (const auto &arg : args_)
        {
            // Compare arg identifier.
            if (arg.identifier.compare(argv[i]) == 0)
            {
                // Make sure argument given for identifier.
                if (i + 1 >= argc)
                {
                    // Failure print help text.
                    std::cout << "Error: Got " << arg.identifier
                              << " with missing arg.\n\n";
                    print_help_();
                    return false;
                }
                else
                {
                    // Argument parsed. Add to arg map.
                    parsed_.insert({arg.identifier, std::string(argv[i + 1])});
                }
            }
        }
    }

    // Make sure all non-optional args are present.
    for (const auto &arg : args_)
    {
        // Make sure arg is not optional and has no value.
        if (not arg.optional and not get_argument(arg.identifier).has_value())
        {
            // Non optional arg missing, print help text instead.
            std::cout << "Error: Missing argument " << arg.identifier
                      << ".\n\n";
            print_help_();
            return false;
        }
    }

    // Indicate success.
    return true;
}

std::optional<std::string_view>
Argparse::get_argument(const std::string &identifer) const
{

    // Search requested value.
    auto it = parsed_.find(identifer);

    // Return either empty or value if arg found.
    return (it == parsed_.end()) ? std::optional<std::string_view>{}
                                 : std::string_view(it->second);
}

void Argparse::print_help_() const
{
    // Print help header.
    std::cout << "Help:\n\t" << help_ << "\n\nArguments:\n";

    // Help info line.
    std::cout << "\t-help - Display this help menu.\n";

    // Print information about args.
    for (const auto &arg : args_)
        std::cout << "\t" << arg.identifier << " [ "
                  << (arg.optional ? "optional" : "required") << " ] - "
                  << arg.help << "\n";
}

} // namespace cellular_automaton