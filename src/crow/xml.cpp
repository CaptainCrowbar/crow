#include "crow/xml.hpp"
#include "crow/binary.hpp"
#include "crow/format.hpp"
#include "crow/string.hpp"
#include "crow/unicode.hpp"
#include <algorithm>

using namespace Crow;

namespace Crow::Xml {

    namespace {

        constexpr std::string_view default_xmldecl = R"(<?xml version="1.0"?>)";
        constexpr std::string_view xml_whitespace = "\t\r\n ";

        void dump_core(std::string& out, NodePtr node, size_t depth) {

            out += dent(depth);

            if (node) {

                out += to_string(node->type());

                switch (node->type()) {
                    case NodeType::element:
                        out += ": " + std::static_pointer_cast<Element>(node)->name();
                        break;
                    case NodeType::document:
                        break;
                    default:
                        out += ": " + quote(node->outer());
                        break;
                }

                out += "\n";
                auto compound = std::dynamic_pointer_cast<CompoundNode>(node);

                if (compound)
                    for (auto& child: *compound)
                        dump_core(out, child, depth + 1);

            } else {

                out += "null\n";

            }

        }

        size_t find_end(std::string_view xml, size_t start = 0) noexcept {

            if (start >= xml.size() || xml[start] != '<')
                return npos;

            size_t depth = 0;
            size_t i = start;

            while (i < xml.size()) {

                size_t j = xml.find_first_of("\"<>", i);

                if (j == npos) {

                    return npos;

                } else if (xml[j] == '\"') {

                    size_t k = xml.find('\"', j + 1);

                    if (k == npos)
                        return npos;

                    i = k + 1;

                } else if (xml[j] == '<') {

                    i = j + 1;
                    ++depth;

                } else {

                    i = j + 1;

                    if (--depth == 0)
                        return i;

                }

            }

            return npos;

        }

        constexpr bool is_special_char(char c) noexcept {
            if (uint8_t(c) < 0x20)
                return c != '\t' && c != '\n' && c != '\r';
            else
                return c == '&' || c == '<' || c == '>' || c == '\"' || c == '\'' || c == '\x7f';
        }

        constexpr bool is_valid_name_start(char c, bool digits = false) noexcept {
            // This is a cheat, we just accept all non-ASCII characters
            return ascii_isalpha(c)
                || (digits && ascii_isdigit(c))
                || c == ':' || c == '_'
                || uint8_t(c) >= 0x80;
        }

        constexpr bool is_valid_name_continue(char c) noexcept {
            return is_valid_name_start(c) || ascii_isdigit(c) || c == '-' || c == '.';
        }

        constexpr bool is_xml_whitespace(char c) noexcept {
            return xml_whitespace.find(c) != npos;
        }

        void xml_encode_char(char32_t c, std::string& out) {
            switch (c) {
                case U'&':   out += "&amp;"; break;
                case U'<':   out += "&lt;"; break;
                case U'>':   out += "&gt;"; break;
                case U'\"':  out += "&quot;"; break;
                case U'\'':  out += "&apos;"; break;
                default:     out += concat("&#x", hex(c, 1), ';'); break;
            }
        }

        bool xml_decode_char(std::string_view entity, std::string& text, Options opt) {

            using namespace Detail;

            auto key = entity.substr(1, entity.size() - 2);
            bool found;

            if (key[0] == '#') {

                char32_t c;

                if (key[1] == 'x')
                    c = char32_t(to_uint32(std::string(key.substr(2)), 16));
                else
                    c = char32_t(to_uint32(std::string(key.substr(1))));

                encode_char(c, text);
                found = true;

            } else {

                static const auto search = [] (const StringViewMap& table, std::string_view key) {
                    auto it = table.find(key);
                    return it == table.end() ? std::string_view() : it->second;
                };

                auto value = search(xml_entity_table(), key);

                if (value.empty() && has_bit(opt, Options::xentity))
                    value = search(html_entity_table(), key);

                found = ! value.empty();
                text += found ? value : entity;

            }

            return found;

        }

        std::string_view first_line(std::string_view str) noexcept {
            auto end = str.find('\n');
            if (end == npos)
                return str;
            else
                return str.substr(0, end + 1);
        }

        std::string fold_ws(std::string_view str) {

            std::string out;
            size_t i = 0;

            while (i < str.size()) {

                auto j = str.find_first_of(xml_whitespace, i);
                out.append(str, i, j - i);

                if (j == npos)
                    break;

                out += ' ';
                i = str.find_first_not_of(xml_whitespace, j);

            }

            return out;

        }

        void name_check(std::string_view name) {
            if (! is_valid_name(name))
                throw Error("Invalid name", name);
        }

        std::string_view read_name(std::string_view& xml, bool digits = false) noexcept {

            if (xml.empty() || ! is_valid_name_start(xml[0], digits))
                return {};

            auto next = std::find_if_not(xml.begin() + 1, xml.end(), is_valid_name_continue) - xml.begin();
            auto name = xml.substr(0, next);
            xml = xml.substr(next, npos);

            return name;

        }

        std::string read_attr_string(std::string_view& xml, Options opt, bool digits) {

            if (xml[0] != '\"' && xml[0] != '\'')
                return std::string(read_name(xml, digits));

            auto end = xml.find(xml[0], 1);

            if (end == npos)
                throw Error("Invalid string", xml);

            auto text = xml.substr(1, end - 1);
            xml = xml.substr(end + 1, npos);

            return decode_text(text, opt);

        }

        void skip_line_break(std::string_view& str) noexcept {
            if (str.empty())
                {}
            else if (str[0] == '\n')
                str = str.substr(1, npos);
            else if (str.starts_with("\r\n"))
                str = str.substr(2, npos);
        }

        class ParseState {

        public:

            ParseState(CompoundNode& out, std::string_view xml, Options opt):
                out_(&out), xml_(xml), opt_(opt) {}

            void parse() { while (read_next()) {} }

        private:

            enum class tag_type {
                empty,
                open,
                close,
            };

            struct tag_info {
                std::string_view text;
                tag_type type = tag_type::open;
                std::string name;
                std::map<std::string, std::string> attrs;
            };

            CompoundNode* out_;
            std::string_view xml_;
            Options opt_;
            std::vector<ElementPtr> context_;

            void append(NodePtr node);
            std::string_view read(size_t len) noexcept;
            bool read_next();
            void read_xmldecl();
            void read_dtd();
            void read_cdata();
            void read_comment();
            void read_entity();
            void read_processing();
            void read_text();
            void read_tag();
            void handle_opening_tag(const tag_info& tag);
            void handle_closing_tag(const tag_info& tag);

        };

            void ParseState::append(NodePtr node) {

                CompoundNode& parent = context_.empty() ? *out_ : *context_.back();

                if (node->type() == NodeType::text
                        && ! parent.empty()
                        && parent.back()->type() == NodeType::text) {
                    auto merged_text = parent.back()->outer() + node->outer();
                    node = Text::make(merged_text, Options::encoded);
                    parent.pop_back();
                }

                parent.push_back(node);

            }

            std::string_view ParseState::read(size_t len) noexcept {
                auto prefix = xml_.substr(0, len);
                if (len < xml_.size())
                    xml_ = xml_.substr(len, npos);
                else
                    xml_ = view_end(xml_);
                return prefix;
            }

            bool ParseState::read_next() {

                if (xml_.empty())
                    return false;

                if (xml_.size() < 3) {

                    read_text();

                } else if (xml_[0] == '&') {

                    read_entity();

                } else if (xml_[0] == '<') {

                    if (xml_[1] == '!') {

                        if (xml_[2] == '-')
                            read_comment();
                        else if (xml_[2] == '[')
                            read_cdata();
                        else if (xml_[2] == 'D')
                            read_dtd();
                        else
                            throw Error({}, xml_);

                    } else if (xml_[1] == '?') {

                        if (ascii_lowercase(xml_.substr(2, 3)) == "xml")
                            read_xmldecl();
                        else
                            read_processing();

                    } else if (xml_[1] == '/' || is_valid_name_start(xml_[1])) {

                        read_tag();

                    } else {

                        throw Error({}, xml_);

                    }

                } else {

                    read_text();

                }

                return true;

            }

            void ParseState::read_xmldecl() {

                size_t end = find_end(xml_);

                if (end == npos)
                    throw Error("Invalid XML declaration", xml_);

                auto xmldecl = read(end);
                append(Xmldecl::make(xmldecl));
                skip_line_break(xml_);

            }

            void ParseState::read_dtd() {

                size_t end = find_end(xml_);

                if (end == npos)
                    throw Error("Invalid document type definition", xml_);

                auto dtd = read(end);
                append(Dtd::make(dtd));
                skip_line_break(xml_);

            }

            void ParseState::read_cdata() {

                auto end = xml_.find("]]>");

                if (end == npos)
                    throw Error("Invalid character data", xml_);

                auto text = read(end + 3);
                append(Cdata::make(text));

            }

            void ParseState::read_comment() {

                auto end = xml_.find("-->");

                if (end < 4 || end == npos)
                    throw Error("Invalid comment", xml_);

                auto text = read(end + 3);

                if (has_bit(opt_, Options::comments))
                    append(Comment::make(text));

            }

            void ParseState::read_entity() {

                auto semi = xml_.find_first_of(";\r\n");

                if (semi == npos || xml_[semi] != ';')
                    throw Error("Invalid entity", xml_);

                auto entity = read(semi + 1);
                std::string text;
                bool is_char = xml_decode_char(entity, text, opt_);

                if (is_char)
                    append(Text::make(text));
                else
                    append(Entity::make(text));

            }

            void ParseState::read_processing() {

                auto end = xml_.find("?>");

                if (end == npos)
                    throw Error("Invalid processing instruction", xml_);

                auto text = read(end + 2);
                append(Processing::make(text));

            }

            void ParseState::read_text() {

                if (is_special_char(xml_[0])) {

                    auto ch = read(1);
                    append(Entity::make(encode_text(ch)));

                } else {

                    auto end = std::find_if(xml_.begin(), xml_.end(), is_special_char) - xml_.begin();
                    auto view = read(end);

                    if (has_bit(opt_, Options::foldws)) {

                        auto text = fold_ws(view);

                        if (! text.empty())
                            append(Text::make(text, Options::encoded));

                    } else {

                        append(Text::make(view, Options::encoded));

                    }

                }

            }

            void ParseState::read_tag() {

                size_t end = find_end(xml_);

                if (end == npos)
                    throw Error("Invalid element", xml_);

                tag_info tag;
                tag.text = read(end);
                auto rest = tag.text.substr(1, tag.text.size() - 2);
                view_trim(rest, xml_whitespace);

                if (rest[0] == '/') {
                    tag.type = tag_type::close;
                    rest = rest.substr(1, npos);
                }

                if (rest.back() == '/') {
                    if (tag.type == tag_type::close)
                        throw Error("Invalid element", tag.text);
                    tag.type = tag_type::empty;
                    rest = rest.substr(0, rest.size() - 1);
                }

                view_trim_left(rest, xml_whitespace);
                tag.name = read_name(rest);

                if (tag.name.empty())
                    throw Error("Invalid element", tag.text);

                if (has_bit(opt_, Options::icase))
                    tag.name = ascii_lowercase(tag.name);

                view_trim_left(rest, xml_whitespace);
                std::string key, value;

                for (;;) {

                    view_trim_left(rest, xml_whitespace);

                    if (rest.empty())
                        break;

                    key = read_attr_string(rest, opt_, false);

                    if (key.empty())
                        throw Error("Invalid element", tag.text);

                    if (has_bit(opt_, Options::icase))
                        key = ascii_lowercase(key);

                    if (rest[0] == '=') {
                        rest = rest.substr(1, npos);
                        value = read_attr_string(rest, opt_, true);
                    } else if (has_bit(opt_, Options::keyonly)) {
                        value = key;
                    } else {
                        throw Error("Invalid attribute", tag.text);
                    }

                    if (! tag.attrs.insert({key, value}).second)
                        throw Error("Duplicate attribute", tag.text);

                }

                if (tag.type == tag_type::open && has_bit(opt_, Options::selfclose)
                        && Detail::html_self_closing_tags().contains(ascii_lowercase(tag.name)))
                    tag.type = tag_type::empty;

                if (tag.type == tag_type::close)
                    handle_closing_tag(tag);
                else
                    handle_opening_tag(tag);

            }

            void ParseState::handle_opening_tag(const tag_info& tag) {

                auto elem = Element::make(tag.name);
                append(elem);

                for (auto [key,value]: tag.attrs)
                    elem->set_attr(std::string(key), std::string(value));

                if (tag.type == tag_type::open)
                    context_.push_back(elem);

            }

            void ParseState::handle_closing_tag(const tag_info& tag) {

                if (has_bit(opt_, Options::autoclose)) {

                    auto i = context_.size();

                    while (i != 0 && context_[i - 1]->name() != tag.name)
                        --i;

                    if (i != 0)
                        context_.resize(i - 1);

                } else {

                    if (context_.empty() || context_.back()->name() != tag.name)
                        throw Error("Unmatched closing tag", tag.text);

                    context_.pop_back();

                }

            }

    }

    namespace Detail {

        std::string dump(NodePtr node) {
            std::string out;
            dump_core(out, node, 0);
            return out;
        }

    }

    // Functions

    std::string decode_text(std::string_view str, Options opt) {

        std::string text;
        size_t i = 0;
        size_t len = str.size();

        while (i < len) {

            size_t j = str.find('&', i);
            text.append(str, i, j - i);

            if (j == npos)
                break;

            size_t k = str.find(';', j + 1);

            if (k == npos) {
                text += '&';
                i = j + 1;
            } else {
                xml_decode_char(str.substr(j, k - j + 1), text, opt);
                i = k + 1;
            }

        }

        return text;

    }

    std::string encode_text(std::string_view str) {

        std::string xml;
        auto i = str.begin();
        auto end = str.end();

        while (i != end) {

            auto j = std::find_if(i, end, is_special_char);
            xml.append(i, j);

            if (j == end)
                break;

            xml_encode_char(uint8_t(*j), xml);
            i = j + 1;

        }

        return xml;

    }

    bool is_valid_name(std::string_view name) noexcept {
        return ! name.empty()
            && is_valid_name_start(name[0])
            && std::find_if_not(name.begin() + 1, name.end(), is_valid_name_continue) == name.end();
    }

    // Exceptions

    Error::Error(const std::string& details, std::string_view text):
    std::runtime_error([=] {
        std::string msg = "XML error";
        if (! details.empty())
            msg += ": " + details;
        if (! text.empty())
            msg += ": " + quote(first_line(text));
        return msg;
    }()),
    error_text_(text) {}

    // SimpleNode class

    void SimpleNode::init(std::string_view str, std::string_view left, std::string_view right) {

        bool alpha = ascii_isalpha(left.back());
        bool lower = ascii_islower(left.back());

        if (lower ? ascii_lowercase(str.substr(0, left.size())) != left
                : ! str.starts_with(left)) {
            xml_ += left;
            if (alpha && ! str.empty() && is_xml_whitespace(str[0]))
                xml_ += ' ';
        }

        xml_ += str;

        if (! str.ends_with(right))
            xml_ += right;

    }

    // Xmldecl class

    Xmldecl::Xmldecl(hidden, std::string_view str) {
        if (str.empty())
            xml_ = default_xmldecl;
        else
            init(str, "<?xml", "?>");
    }

    // Dtd class

    Dtd::Dtd(hidden, std::string_view str) {
        init(str, "<!DOCTYPE", ">");
    }

    // Cdata class

    Cdata::Cdata(hidden, std::string_view str) {
        init(str, "<![CDATA[", "]]>");
    }

    // Comment class

    Comment::Comment(hidden, std::string_view str) {
        init(str, "<!--", "-->");
    }

    // Entity class

    Entity::Entity(hidden, std::string_view str) {
        init(str, "&", ";");
    }

    // Processing class

    Processing::Processing(hidden, std::string_view str) {
        init(str, "<?", "?>");
    }

    // Text class

    Text::Text(hidden, std::string_view str, Options opt) {
        if (has_bit(opt, Options::encoded))
            xml_ = str;
        else
            xml_ = encode_text(str);
    }

    // CompoundNode class

    std::string CompoundNode::inner_core(Options opt) const {

        std::string xml;

        for (auto& c: children_) {

            if (! c)
                continue;

            auto is_prologue = bool(std::dynamic_pointer_cast<PrologueNode>(c));

            if (is_prologue && ! xml.empty() && xml.back() != '\n')
                xml += '\n';

            xml += c->outer(opt);

            if (is_prologue)
                xml += '\n';

        }

        return xml;

    }

    // AttributeMap class

    std::string AttributeMap::attr(const std::string& key) const {
        name_check(key);
        auto it = attrs_.find(key);
        if (it == attrs_.end())
            return {};
        else
            return it->second;
    }

    void AttributeMap::set_attr(const std::string& key, const std::string& value) {
        name_check(key);
        attrs_[key] = value;
    }

    // Element class

    Element::Element(hidden, const std::string& name):
    name_(name) {
        name_check(name);
    }

    std::string Element::outer_core(Options opt) const {

        bool key_only = has_bit(opt, Options::keyonly);
        auto xml = concat('<', name_);

        for (auto& [key,value]: attr_range()) {
            xml += concat(' ', encode_text(key));
            if (! key_only || key != value)
                xml += concat("=\"", encode_text(value), '\"');
        }

        if (! empty())
            xml += concat('>', inner(opt), "</", name_, '>');
        else if (has_bit(opt, Options::selfclose)
                && Detail::html_self_closing_tags().contains(ascii_lowercase(name_)))
            xml += ">";
        else
            xml += " />";

        return xml;

    }

    // Document class

    Document::Document(hidden, std::string_view xml, Options opt) {
        ParseState state(*this, xml, opt);
        state.parse();
        init_xmldecl(opt);
    }

    XmldeclPtr Document::xmldecl() const noexcept {
        if (! empty() && front()->type() == NodeType::xmldecl)
            return std::static_pointer_cast<Xmldecl>(front());
        else
            return nullptr;
    }

    DtdPtr Document::dtd() const noexcept {
        auto it = std::find_if_not(begin(), end(),
            [] (const NodePtr& node) {
                auto t = node->type();
                return t == NodeType::xmldecl
                    || t == NodeType::comment
                    || t == NodeType::processing
                    || t == NodeType::text;
            });
        if (it != end() && (**it).type() == NodeType::dtd)
            return std::static_pointer_cast<Dtd>(*it);
        else
            return nullptr;
    }

    void Document::init_xmldecl(Options opt) {
        if (! has_bit(opt, Options::noxmldecl)
                && (empty() || front()->type() != NodeType::xmldecl))
            insert(begin(), Xmldecl::make());
    }

}
