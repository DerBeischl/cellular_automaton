#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cellular_automaton
{

/**
 * @brief A simple argparser class.
 *
 * Supports setting optional arguments and automatically generates help text.
 */
class Argparse
{
  public:
    /**
     * @brief A helper wrapper around a single argument.
     *
     * It records the identifier, help text and whether the argument is
     * optional. By default the argument is not optional.
     * Note that the constructor is not explicit so you can instantiate
     * this object like so:
     *
     * Argparse::Argument arg {"-a", "Some info...", true};
     *
     */
    struct Argument
    {
        /**
         * @brief Construct a new Argument object.
         *
         * @param identifier The identifier to match against.
         *                   Must not start with "-".
         * @param help The help text displayed in the help menu.
         * @param is_optional Whether the argument is required.
         */
        Argument(const std::string &identifier, const std::string &help,
                 const bool is_optional = false);

        std::string identifier, help;
        bool optional;
    };

    /**
     * @brief Construct a new Argparse object.
     *
     * @param help Module help text for help menu.
     * @param args A list of arguments this parser will match.
     *             Additional args the user might provide are simply ignored.
     */
    explicit Argparse(std::string help, std::vector<Argument> args);

    /**
     * @brief Parses the specified arguments.
     *
     * Simply pass in the arguments to the main function.
     *
     * @param argc Number of arguments.
     * @param argv List of argument strings. ideally the same number
     *             of entries as specified in argc.
     * @return "true" In case the input was parsed successfully.
     * @return "false" In case there was a problem parsing the input.
     */
    bool parse(const int argc, char **argv);

    /**
     * @brief Get the parsed value for a certain identifier.
     *
     * @param identifer The identifier of the argument e.g. "-a".
     * @return An optional that is empty if no value is available for the
     *         specified identifier.
     */
    std::optional<std::string_view>
    get_argument(const std::string &identifer) const;

  private:
    /**
     * @brief Prints help text to stdout.
     *
     */
    void print_help_() const;

    std::string help_;
    std::unordered_map<std::string, std::string> parsed_;
    std::vector<Argument> args_;
};
} // namespace cellular_automaton