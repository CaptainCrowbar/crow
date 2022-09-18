#include "crow/options.hpp"
#include "crow/terminal.hpp"
#include <set>

using namespace Crow::Literals;
using namespace std::literals;

namespace Crow {

    namespace {

        std::string trim_name(const std::string& name) {
            return trim(name, std::string(ascii_whitespace) + '-');
        }

    }

    Options::setup_error::setup_error(const std::string& message):
    std::logic_error("Internal error: " + message) {}

    Options::user_error::user_error(const std::string& message):
    std::runtime_error(message) {}

    Options::Options(const std::string& app, const std::string& version,
        const std::string& description, const std::string& extra):
    options_(),
    app_(trim(app)),
    version_(trim(version)),
    description_(trim(description)),
    extra_(trim(extra)),
    colour_(-1),
    allow_help_(false),
    auto_help_(false) {
        if (app.empty())
            throw setup_error("No application name was supplied");
        if (description.empty())
            throw setup_error("No application description was supplied");
        if (! version_.empty())
            version_.insert(0, 1, ' ');
    }

    bool Options::parse(std::vector<std::string> args, std::ostream& out) {

        allow_help_ = true;
        char help_abbrev = option_index('h') == npos ? 'h' : '\0';
        char version_abbrev = option_index('v') == npos ? 'v' : '\0';
        bool want_help = false;
        bool want_version = false;
        add(want_help, "help", help_abbrev, "Show usage information");
        add(want_version, "version", version_abbrev, "Show version information");

        if (auto_help_ && args.empty()) {
            out << format_help();
            return false;
        }

        std::set<std::string> groups_found;
        option_info* current = nullptr;
        size_t arg_index = 0;
        bool escaped = false;
        bool paired = false;

        auto on_match = [this,&current,&groups_found,&paired] (option_info& opt) {
            current = &opt;
            if (opt.found && opt.kind != mode::multiple)
                throw user_error("Repeated option: --" + opt.name);
            if (! opt.group.empty()) {
                if (groups_found.count(opt.group) == 1)
                    throw user_error("Options {0} are mutually exclusive"_fmt(group_list(opt.group)));
                groups_found.insert(opt.group);
            }
            opt.found = true;
            if (opt.kind == mode::boolean && !paired) {
                opt.setter("t");
                current = nullptr;
            }
        };

        while (arg_index < args.size()) {

            const auto& arg = args[arg_index];

            if (escaped || arg[0] != '-') {

                // Argument to an option

                if (current == nullptr) {
                    auto it = std::find_if(options_.begin(), options_.end(), [] (auto& opt) {
                        return opt.is_anon && (opt.kind == mode::multiple || ! opt.found);
                    });
                    if (it == options_.end())
                        throw user_error("Argument not associated with an option: {0:q}"_fmt(arg));
                    on_match(*it);
                }

                if (current->validator && ! current->validator(arg))
                    throw user_error("Argument does not match expected pattern: {0:q}"_fmt(arg));

                try {
                    current->setter(arg);
                }
                catch (const std::invalid_argument& ex) {
                    throw user_error(ex.what());
                }
                if (current->kind != mode::multiple)
                    current = nullptr;
                paired = false;
                ++arg_index;

            } else if (arg == "--") {

                // Remaining arguments can't be options

                escaped = true;
                ++arg_index;

            } else if (arg[0] == '-' && arg[1] == '-') {

                size_t eq_pos = arg.find('=');

                if (eq_pos == npos) {

                    // Long option name

                    size_t opt_index = npos;
                    bool invert = false;
                    if (starts_with(arg, "--no-")) {
                        opt_index = option_index(arg.substr(5));
                        if (opt_index != npos && options_[opt_index].kind == mode::boolean)
                            invert = paired = true;
                    }
                    if (! invert) {
                        opt_index = option_index(arg.substr(2));
                        if (opt_index == npos)
                            throw user_error("Unknown option: {0:q}"_fmt(arg));
                    }
                    on_match(options_[opt_index]);
                    ++arg_index;
                    if (invert)
                        args.insert(args.begin() + arg_index, "f");

                } else if (eq_pos >= 4) {

                    // Long option name and value combined

                    auto key = arg.substr(0, eq_pos);
                    auto value = arg.substr(eq_pos + 1);
                    args.erase(args.begin() + arg_index);
                    args.insert(args.begin() + arg_index, key);
                    args.insert(args.begin() + arg_index + 1, value);
                    paired = true;

                } else {

                    throw user_error("Invalid option: {0:q}"_fmt(arg));

                }

            } else if (arg.size() > 2) {

                // Multiple short options

                std::vector<std::string> new_args;
                for (char c: arg.substr(1))
                    new_args.push_back({'-', c});
                auto it = args.begin() + arg_index;
                args.erase(it);
                args.insert(it, new_args.begin(), new_args.end());

            } else {

                // Short option name

                paired = false;
                size_t opt_index = option_index(arg[1]);
                if (opt_index == npos)
                    throw user_error("Unknown option: {0:q}"_fmt(arg));
                on_match(options_[opt_index]);
                ++arg_index;

            }

        }

        size_t index = option_index("help");
        if (options_[index].found) {
            out << format_help();
            return false;
        }

        index = option_index("version");
        if (options_[index].found) {
            out << app_ << version_ << "\n";
            return false;
        }

        auto it = std::find_if(options_.begin(), options_.end(), [] (auto& opt) { return opt.is_required && ! opt.found; });
        if (it != options_.end())
            throw user_error("Required option not found: --" + it->name);

        return true;

    }

    bool Options::parse(int argc, char** argv, std::ostream& out) {
        std::vector<std::string> args(argv + 1, argv + argc);
        return parse(args, out);
    }

    bool Options::found(const std::string& name) const {
        auto i = option_index(name);
        return i != npos && options_[i].found;
    }

    void Options::do_add(setter_type setter, validator_type validator, const std::string& name, char abbrev,
            const std::string& description, const std::string& placeholder, const std::string& default_value,
            mode kind, int flags, const std::string& group) {

        bool anon_complete = false;
        option_info info;

        info.setter = setter;
        info.validator = validator;
        info.name = trim_name(name);
        info.description = trim(description);
        info.placeholder = placeholder;
        info.default_value = default_value;
        info.group = group;
        info.abbrev = abbrev;
        info.kind = kind;
        info.is_anon = (flags & anon) != 0;
        info.is_no_default = (flags & no_default) != 0;
        info.is_required = (flags & required) != 0;

        std::string long_name = "--" + info.name;
        std::string short_name = {'-', info.abbrev};

        if (info.name.empty() || name.find_first_of(ascii_whitespace) != npos
                || std::find_if(name.begin(), name.end(), ascii_iscntrl) != name.end())
            throw setup_error("Invalid long option: " + name);
        if (! allow_help_ && (name == "help" || name == "version"))
            throw setup_error("Invalid long option: " + name);
        if (option_index(info.name) != npos)
            throw setup_error("Duplicate long option: --" + info.name);

        if (info.abbrev != '\0') {
            if (! ascii_isgraph(info.abbrev) || info.abbrev == '-')
                throw setup_error("Invalid short option: -"s + info.abbrev);
            if (option_index(info.abbrev) != npos)
                throw setup_error("Duplicate short option: -"s + info.abbrev);
        }

        if (info.kind == mode::boolean && info.is_anon)
            throw setup_error("Boolean options can't be anonymous: --" + info.name);
        if (info.kind == mode::boolean && info.is_required)
            throw setup_error("Boolean options can't be required: --" + info.name);

        if (info.is_anon) {
            if (anon_complete)
                throw setup_error("All anonymous arguments are already accounted for: --" + info.name);
            anon_complete = info.kind == mode::multiple;
        }

        if (info.is_required && ! group.empty())
            throw setup_error("Required options can't be in a mutual exclusion group: --" + info.name);

        if (info.description.empty())
            throw setup_error("Option description is empty: --" + info.name);

        options_.push_back(info);

    }

    std::string Options::format_help() const {

        auto xterm = colour_ == -1 ? Xterm() : Xterm(bool(colour_));
        auto head_colour = xterm.rgb(5, 5, 1);
        auto body_colour = xterm.rgb(5, 5, 3);
        auto prefix_colour = xterm.rgb(1, 5, 1);
        auto suffix_colour = xterm.rgb(2, 4, 5);

        std::string text = "\n"
            "{3}{4}{0}{1}{6}\n\n"
            "{5}{2}{6}\n\n"
            "{5}Options:{6}\n"_fmt
        (app_, version_, description_,
            xterm.bold(), head_colour, body_colour, xterm.reset());

        std::vector<std::string> left, right;
        std::string block;
        size_t left_width = 0;

        for (auto& info: options_) {

            block.clear();

            if (info.is_anon)
                block += '[';
            block += "--" + info.name;
            if (info.abbrev != '\0')
                block += ", -" + std::string{info.abbrev};
            if (info.is_anon)
                block += ']';

            if (info.kind != mode::boolean) {
                block += " " + info.placeholder;
                if (info.kind == mode::multiple)
                    block += " ...";
            }

            left.push_back(block);
            left_width = std::max(left_width, block.size());
            block = info.description;
            bool show_default = ! info.is_no_default && ! info.default_value.empty();

            if (info.is_required || show_default) {
                if (block.back() == ')') {
                    block.pop_back();
                    block += "; ";
                } else {
                    block += " (";
                }
                if (info.is_required)
                    block += "required";
                else if (show_default)
                    block += "default " + info.default_value;
                block += ")";
            }

            right.push_back(block);

        }

        for (size_t i = 0; i < left.size(); ++i) {
            left[i].resize(left_width, ' ');
            text += "    {2}{0}  {3}= {1}{4}\n"_fmt
                (left[i], right[i], prefix_colour, suffix_colour, xterm.reset());
        }

        text += '\n';
        if (! extra_.empty())
            text += "{1}{0}{2}\n\n"_fmt(extra_, body_colour, xterm.reset());

        return text;

    }

    std::string Options::group_list(const std::string& group) const {
        std::vector<std::string> names;
        for (auto& info: options_)
            if (info.group == group)
                names.push_back("--" + info.name);
        return join(names, ", ");
    }

    size_t Options::option_index(const std::string& name) const {
        auto key = trim_name(name);
        auto it = std::find_if(options_.begin(), options_.end(),
            [&key] (const option_info& opt) { return opt.name == key; });;
        return it == options_.end() ? npos : size_t(it - options_.begin());
    }

    size_t Options::option_index(char abbrev) const {
        if (abbrev == 0)
            return npos;
        auto it = std::find_if(options_.begin(), options_.end(),
            [abbrev] (const option_info& opt) { return opt.abbrev == abbrev; });;
        return it == options_.end() ? npos : size_t(it - options_.begin());
    }

}
