#include "crow/english.hpp"
#include "crow/format.hpp"
#include <algorithm>
#include <cmath>
#include <map>
#include <utility>

namespace Crow {

    // Constants

    const std::unordered_set<std::string>& stopwords() {
        static const std::unordered_set<std::string> words = {
            "a", "an", "and", "at", "for", "in", "is", "of", "on", "or", "the", "to", "with"
        };
        return words;
    }

    // Case conversion functions

    std::string extended_titlecase(const std::string& str, bool initial) {
        return extended_titlecase(str, stopwords(), initial);
    }

    std::string extended_titlecase(const std::string& str, const std::unordered_set<std::string>& stop,
            bool initial) {

        std::string result;
        auto i = str.begin();
        auto i_end = str.end();
        auto j = i;
        int words = 0;

        while (i != i_end) {

            j = std::find_if(i, i_end, ascii_isalpha);
            result.append(i, j);

            if (j == i_end)
                break;

            i = j;

            do i = std::find_if_not(i + 1, i_end, ascii_isalpha);
                while (*i == '\'');

            std::string word(j, i);
            word = ascii_lowercase(word);
            ++words;

            if ((words == 1 && initial) || stop.contains(word) == 0)
                word[0] = ascii_toupper(word[0]);

            result += word;

        }

        return result;

    }

    std::string sentence_case(const std::string& str) {

        auto result = str;
        bool up = true;

        for (auto& c: result) {
            if (c == '.') {
                up = true;
            } else if (up && ascii_isalpha(c)) {
                c = ascii_toupper(c);
                up = false;
            }
        }

        return result;

    }

    // List formatting functions

    namespace Detail {

        std::string comma_list_helper(const std::vector<std::string>& list, const std::string& conj) {

            std::string s;
            size_t comma1 = 0;
            size_t comma2 = 0;
            size_t n = 0;

            for (auto& x: list) {
                s += x + ", ";
                comma1 = comma2;
                comma2 = s.size() - 2;
                ++n;
            }

            if (n == 0)
                return {};

            s.resize(comma2);

            if (n == 1)
                return s;

            if (! conj.empty()) {
                s.insert(comma1 + 2, std::string(conj) + ' ');
                if (n == 2)
                    s.erase(comma1, 1);
            }

            return s;

        }

    }


    // Number formatting functions

    namespace {

        const std::vector<std::string>& card_ones() {
            static const std::vector<std::string> vec = {
                "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
                "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
            };
            return vec;
        }

        const std::vector<std::string>& card_tens() {
            static const std::vector<std::string> vec = {
                "", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
            };
            return vec;
        }

        const std::vector<std::string>& ord_ones() {
            static const std::vector<std::string> vec = {
                "", "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth",
                "eleventh", "twelfth", "thirteenth", "fourteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "nineteenth"
            };
            return vec;
        }

        const std::vector<std::string>& ord_tens() {
            static const std::vector<std::string> vec = {
                "", "tenth", "twentieth", "thirtieth", "fortieth", "fiftieth", "sixtieth", "seventieth", "eightieth", "ninetieth"
            };
            return vec;
        }

        size_t card_add(std::vector<std::string>& words, size_t n, size_t scale, const std::string& scale_name, const std::string& suffix) {
            if (n < scale)
                return n;
            if (! words.empty())
                words.back() += ',';
            words.push_back(cardinal(n / scale));
            words.push_back(std::string(scale_name));
            n %= scale;
            if (n == 0)
                words.back() += suffix;
            return n;
        }

        std::string number_name(size_t n, const std::vector<std::string>& ones, const std::vector<std::string>& tens, const std::string& suffix) {

            if (n == 0)
                return "zero" + suffix;

            std::string sign;
            std::vector<std::string> words;

            n = card_add(words, n, 1'000'000'000, "billion", suffix);
            n = card_add(words, n, 1'000'000, "million", suffix);
            n = card_add(words, n, 1000, "thousand", suffix);
            n = card_add(words, n, 100, "hundred", suffix);

            if (n > 0 && ! words.empty())
                words.push_back("and");

            std::string extra;

            if (n % 10 == 0) {

                extra += tens[n / 10];

            } else {

                if (n >= 20) {
                    extra += card_tens()[n / 10];
                    extra += '-';
                    n %= 10;
                }

                extra += ones[n];

            }

            if (! extra.empty())
                words.push_back(extra);

            return join(words, " ");

        }

    }

    std::string cardinal(size_t n, size_t threshold) {
        if (n < threshold)
            return number_name(n, card_ones(), card_tens(), {});
        else
            return std::to_string(n);
    }

    std::string ordinal(size_t n, size_t threshold) {

        if (n < threshold)
            return number_name(n, ord_ones(), ord_tens(), "th");

        std::string suffix = "th";

        if ((n % 100) / 10 != 1) {
            switch (n % 10) {
                case 1:   suffix = "st"; break;
                case 2:   suffix = "nd"; break;
                case 3:   suffix = "rd"; break;
                default:  break;
            }
        }

        return std::to_string(n) + suffix;

    }

    namespace {

        double round_to_digits(double x, int prec) noexcept {
            double threshold = std::pow(10.0, double(prec));
            if (x < threshold)
                return x;
            double scale = 1;
            while (x >= threshold) {
                x *= 0.1;
                scale *= 10;
            }
            return std::lround(x) * scale;
        }

    }

    std::string format_count(double x, int prec) {

        if (x < 0)
            return "-" + format_count(- x, prec);

        prec = std::max(prec, 1);

        if (x < 1e4) {

            x = round_to_digits(x, prec);
            return std::to_string(std::llround(x));

        } else if (x < 1e12) {

            auto mode = "d" + std::to_string(prec);
            double scale;
            std::string suffix;

            if (x < 1e6) {
                scale = 1e-3;
                suffix = "thousand";
            } else if (x < 1e9) {
                scale = 1e-6;
                suffix = "million";
            } else {
                scale = 1e-9;
                suffix = "billion";
            }

            x *= scale;
            x = round_to_digits(x, prec);

            return format_floating_point(x, mode) + " " + suffix;

        } else {

            auto mode = "e" + std::to_string(prec);
            auto str = format_floating_point(x, mode);
            auto [sig,exp] = partition(str, "e");

            std::string result(sig);
            result += "x10^";
            result += exp;

            return result;

        }

    }

    std::string number_of(size_t n, const std::string& name, const std::string& plural_name, size_t threshold) {

        std::string result;

        if (n >= 1)
            result = cardinal(n, threshold);
        else if (name.empty())
            result = "none";
        else
            result = "no";

        if (name.empty())
            return result;

        result += ' ';

        if (n == 1)
            result += name;
        else if (plural_name.empty())
            result += plural(name);
        else
            result += plural_name;

        return result;

    }

    // Pluralization functions

    std::string plural(const std::string& noun) {

        // Partly based on Damian Conway's work
        // http://www.csse.monash.edu.au/~damian/papers/HTML/Plurals.html

        // Keep inflections in descending order by length (order within a length is unimportant)

        static const std::vector<std::pair<std::string, std::string>> inflections = {

            { "goose",  "geese" },
            { "louse",  "lice" },
            { "mouse",  "mice" },
            { "tooth",  "teeth" },
            { "drum",   "drums" },
            { "foot",   "feet" },
            { "zoon",   "zoa" },
            { "cis",    "ces" },
            { "man",    "men" },
            { "och",    "ochs" },
            { "sis",    "ses" },
            { "xis",    "xes" },
            { "ao",     "aos" },
            { "ay",     "ays" },
            { "ch",     "ches" },
            { "eo",     "eos" },
            { "ex",     "ices" },
            { "ey",     "eys" },
            { "fe",     "ves" },
            { "io",     "ios" },
            { "iy",     "iys" },
            { "oo",     "oos" },
            { "oy",     "oys" },
            { "sh",     "shes" },
            { "ss",     "sses" },
            { "um",     "a" },
            { "uo",     "uos" },
            { "us",     "i" },
            { "uy",     "uys" },
            { "a",      "ae" },
            { "f",      "ves" },
            { "o",      "oes" },
            { "s",      "ses" },
            { "x",      "xes" },
            { "y",      "ies" },

        };

        static const std::map<std::string, std::string> irregular = {

            { "afreet",         "afreeti" },
            { "afrit",          "afriti" },
            { "albino",         "albinos" },
            { "alto",           "altos" },
            { "aphelion",       "aphelia" },
            { "apparatus",      "apparatuses" },
            { "apsis",          "apsides" },
            { "archipelago",    "archipelagos" },
            { "armadillo",      "armadillos" },
            { "asyndeton",      "asyndeta" },
            { "basso",          "bassos" },
            { "canto",          "cantos" },
            { "cantus",         "cantuses" },
            { "cherub",         "cherubim" },
            { "child",          "children" },
            { "chrysanthemum",  "chrysanthemums" },
            { "coitus",         "coituses" },
            { "comma",          "commas" },
            { "commando",       "commandos" },
            { "contralto",      "contraltos" },
            { "crescendo",      "crescendos" },
            { "criterion",      "criteria" },
            { "ditto",          "dittos" },
            { "djinni",         "djinn" },
            { "dynamo",         "dynamos" },
            { "efreet",         "efreeti" },
            { "embryo",         "embryos" },
            { "ephemeris",      "ephemerides" },
            { "fiasco",         "fiascos" },
            { "generalissimo",  "generalissimos" },
            { "ghetto",         "ghettos" },
            { "goy",            "goyim" },
            { "guano",          "guanos" },
            { "hiatus",         "hiatuses" },
            { "hyena",          "hyenas" },
            { "hyperbaton",     "hyperbata" },
            { "impetus",        "impetuses" },
            { "inferno",        "infernos" },
            { "jumbo",          "jumbos" },
            { "lingo",          "lingos" },
            { "lumbago",        "lumbagos" },
            { "magneto",        "magnetos" },
            { "manifesto",      "manifestos" },
            { "mantis",         "mantids" },
            { "medico",         "medicos" },
            { "mongoose",       "mongooses" },
            { "mythos",         "mythoi" },
            { "nexus",          "nexuses" },
            { "noumenon",       "noumena" },
            { "octavo",         "octavos" },
            { "organon",        "organa" },
            { "ox",             "oxen" },
            { "perihelion",     "perihelia" },
            { "phenomenon",     "phenomena" },
            { "photo",          "photos" },
            { "plexus",         "plexuses" },
            { "pro",            "pros" },
            { "prolegomenon",   "prolegomena" },
            { "prospectus",     "prospectuses" },
            { "quarto",         "quartos" },
            { "rhino",          "rhinos" },
            { "seraph",         "seraphim" },
            { "sinus",          "sinuses" },
            { "soliloquy",      "soliloquies" },
            { "solo",           "solos" },
            { "soprano",        "sopranos" },
            { "sphex",          "sphexes" },
            { "status",         "statuses" },
            { "stylo",          "stylos" },
            { "tempo",          "tempos" },
            { "trilby",         "trilbys" },

        };

        // Keep uninflected words in alphabetical order

        static const std::vector<std::string> uninflected = {

            "bison", "bream", "breeches", "britches", "carp", "chassis",
            "clippers", "cod", "contretemps", "corps", "debris", "diabetes",
            "djinn", "eland", "elk", "fish", "flounder", "gallows",
            "graffiti", "headquarters", "herpes", "highjinks", "hijinks",
            "homework", "innings", "jackanapes", "mackerel", "measles",
            "mews", "mumps", "news", "pincers", "pliers", "proceedings",
            "rabies", "salmon", "scissors", "series", "shears", "species",
            "swine", "trout", "tuna", "whiting", "wildebeest",

        };

        if (noun.empty())
            return {};

        std::string s_noun(noun);
        std::string l_noun = ascii_lowercase(s_noun);
        std::string p_noun;

        if (std::binary_search(uninflected.begin(), uninflected.end(), l_noun)) {

            p_noun = l_noun;

        } else {

            auto it = irregular.find(l_noun);

            if (it == irregular.end()) {

                for (auto& [s,p]: inflections) {
                    if (l_noun.ends_with(s)) {
                        p_noun = l_noun.substr(0, l_noun.size() - s.size()) + p;
                        break;
                    }
                }

                if (p_noun.empty())
                    p_noun = l_noun + 's';

            } else {

                p_noun = it->second;

            }

        }

        auto end_initial = std::find_if(s_noun.begin(), s_noun.end(), ascii_islower);

        switch (std::count_if(s_noun.begin(), end_initial, ascii_isupper)) {
            case 0:   break;
            case 1:   p_noun = ascii_titlecase(p_noun); break;
            default:  p_noun = ascii_uppercase(p_noun); break;
        }

        return p_noun;

    }

    // Test generators

    const std::vector<std::string>& LoremIpsum::classic() {
        static const std::vector<std::string> text = {
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ",
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. ",
            "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. ",
            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. ",
        };
        return text;
    }

}
