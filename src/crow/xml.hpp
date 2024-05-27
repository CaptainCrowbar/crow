#pragma once

#include "crow/enum.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <compare>
#include <concepts>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// https://www.w3.org/TR/xml/

namespace Crow::Xml {

    // Forward declarations

    class AttributeMap;
    class Error;
    class NodeIterator;
    class PathIterator;

    class Node;
        class SimpleNode;
            class PrologueNode;
                class Xmldecl;
                class Dtd;
            class InlineNode;
                class Cdata;
                class Comment;
                class Entity;
                class Processing;
                class Text;
        class CompoundNode;
            class Element;
            class Document;

    using NodePtr          = std::shared_ptr<Node>;
    using SimpleNodePtr    = std::shared_ptr<SimpleNode>;
    using PrologueNodePtr  = std::shared_ptr<PrologueNode>;
    using XmldeclPtr       = std::shared_ptr<Xmldecl>;
    using DtdPtr           = std::shared_ptr<Dtd>;
    using InlineNodePtr    = std::shared_ptr<InlineNode>;
    using CdataPtr         = std::shared_ptr<Cdata>;
    using CommentPtr       = std::shared_ptr<Comment>;
    using EntityPtr        = std::shared_ptr<Entity>;
    using ProcessingPtr    = std::shared_ptr<Processing>;
    using TextPtr          = std::shared_ptr<Text>;
    using CompoundNodePtr  = std::shared_ptr<CompoundNode>;
    using ElementPtr       = std::shared_ptr<Element>;
    using DocumentPtr      = std::shared_ptr<Document>;

    namespace Detail {

        using NodeList = std::vector<NodePtr>;
        using NodeIterator = NodeList::const_iterator;
        using StringViewSet = std::unordered_set<std::string_view>;
        using StringViewMap = std::unordered_map<std::string_view, std::string_view>;

        std::string dump(NodePtr node);
        const StringViewSet& html_self_closing_tags();
        const StringViewMap& xml_entity_table();
        const StringViewMap& html_entity_table();

    }

    // Supporting types

    class Error:
    public std::runtime_error {
    public:
        explicit Error(const std::string& details, std::string_view text = {});
        std::string text() const { return error_text_; }
    private:
        std::string error_text_;
    };

    CROW_ENUM_BITMASK(Options, int,
        none       = 0,
        autoclose  = 1 << 0,  // Implicit closing
        comments   = 1 << 1,  // Keep comments
        encoded    = 1 << 2,  // Text is already encoded
        foldws     = 1 << 3,  // Collapse whitespace
        icase      = 1 << 4,  // Case insensitive names
        keyonly    = 1 << 5,  // Attribute values are optional
        noxmldecl  = 1 << 6,  // No default XML declaration
        selfclose  = 1 << 7,  // HTML self-closing elements
        xentity    = 1 << 8,  // HTML character entities
        xml        = none,
        html       = autoclose | foldws | icase | keyonly | noxmldecl | selfclose | xentity,
    )

    CROW_ENUM_SCOPED(NodeType, int,
        null,
        xmldecl,
        dtd,
        cdata,
        comment,
        entity,
        processing,
        text,
        element,
        document
    )

    // Functions

    std::string decode_text(std::string_view str, Options opt = {});
    std::string encode_text(std::string_view str);
    bool is_valid_name(std::string_view name) noexcept;

    // XML node base class

    class Node {

    public:

        class search_iterator:
        public ForwardIterator<search_iterator, const NodePtr> {
        public:
            search_iterator() = default;
            search_iterator(const CompoundNode& root, NodeType type, const std::string& element, Options opt);
            const NodePtr& operator*() const noexcept { return *path_.back().current; }
            search_iterator& operator++();
            bool operator==(const search_iterator& si) const noexcept;
        private:
            struct level {
                Detail::NodeIterator current;
                const CompoundNode* parent = nullptr;
            };
            std::vector<level> path_;
            NodeType type_ = NodeType::null;
            std::string element_;
            std::function<bool(const std::string&, const std::string&)> equal_ = std::equal_to<std::string>();
            bool accept(const Node& node) const noexcept;
            void next();
        };

        using search_range = Irange<search_iterator>;

        virtual ~Node() noexcept = default;

        virtual NodePtr clone() const = 0;
        virtual NodeType type() const noexcept = 0;

        std::string inner(Options opt = Options::xml) const { return inner_core(opt); }
        std::string outer(Options opt = Options::xml) const { return outer_core(opt); }

        search_range search(Options opt = Options::none) const;
        search_range search(NodeType type, Options opt = Options::none) const;
        search_range search(const std::string& element, Options opt = Options::none) const;

    protected:

        struct hidden {
            explicit hidden() = default;
        };

        virtual std::string inner_core(Options opt) const = 0;
        virtual std::string outer_core(Options opt) const = 0;
        virtual search_range search_core(NodeType type, const std::string& element, Options opt) const;

    };

    inline NodeType node_type(const NodePtr& node) noexcept {
        return node ? node->type() : NodeType::null;
    }

    namespace Detail {

        template <typename T, std::derived_from<Node> Intermediate, NodeType NT>
        class DerivedNode:
        public Intermediate {

        public:

            NodePtr clone() const override {
                return std::make_shared<T>(static_cast<const T&>(*this));
            }

            NodeType type() const noexcept override {
                return NT;
            }

            template <typename... Args>
            requires std::constructible_from<T, Node::hidden, Args&&...>
            static std::shared_ptr<T> create(Args&&... args) {
                return std::make_shared<T>(Node::hidden(), std::forward<Args>(args)...);
            }

        };

    }

    // Simple node base class

    class SimpleNode:
    public Node {

    protected:

        std::string xml_;

        std::string inner_core(Options /*opt*/) const override { return {}; }
        std::string outer_core(Options /*opt*/) const override { return xml_; }

        void init(std::string_view str, std::string_view left, std::string_view right);

    };

    // Prologue node classes

    class PrologueNode:
    public SimpleNode {};

    class Xmldecl:
    public Detail::DerivedNode<Xmldecl, PrologueNode, NodeType::xmldecl> {
    public:
        explicit Xmldecl(hidden, std::string_view str = {});
    };

    class Dtd:
    public Detail::DerivedNode<Dtd, PrologueNode, NodeType::dtd> {
    public:
        explicit Dtd(hidden, std::string_view str);
    };

    // Inline node classes

    class InlineNode:
    public SimpleNode {};

    class Cdata:
    public Detail::DerivedNode<Cdata, InlineNode, NodeType::cdata> {
    public:
        explicit Cdata(hidden, std::string_view str);
    };

    class Comment:
    public Detail::DerivedNode<Comment, InlineNode, NodeType::comment> {
    public:
        explicit Comment(hidden, std::string_view str);
    };

    class Entity:
    public Detail::DerivedNode<Entity, InlineNode, NodeType::entity> {
    public:
        explicit Entity(hidden, std::string_view str);
    };

    class Processing:
    public Detail::DerivedNode<Processing, InlineNode, NodeType::processing> {
    public:
        explicit Processing(hidden, std::string_view str);
    };

    class Text:
    public Detail::DerivedNode<Text, InlineNode, NodeType::text> {
    public:
        explicit Text(hidden, std::string_view str, Options opt = Options::xml);
        std::string plain() const { return decode_text(xml_); }
    };

    // Compound node classes

    class CompoundNode:
    public Node {

    public:

        using iterator = Detail::NodeIterator;

        iterator begin() const noexcept { return iterator(children_.begin()); }
        iterator end() const noexcept { return iterator(children_.end()); }
        NodePtr child(size_t i) const noexcept { return i < children_.size() ? children_[i] : NodePtr(); }
        size_t children() const noexcept { return children_.size(); }
        void clear() noexcept { children_.clear(); }
        bool empty() const noexcept { return children_.empty(); }
        void erase(iterator i) noexcept { children_.erase(i); }
        const NodePtr& front() const noexcept { return *begin(); }
        const NodePtr& back() const noexcept { return end()[-1]; }
        iterator insert(iterator i, NodePtr node) { return children_.insert(i, node); }
        void pop_back() noexcept { erase(std::prev(end())); }
        void push_back(NodePtr node) { insert(end(), node); }
        CompoundNode& operator+=(NodePtr node) { push_back(node); return *this; }

    protected:

        std::string inner_core(Options opt) const override;
        search_range search_core(NodeType type, const std::string& element, Options opt) const override;

    private:

        Detail::NodeList children_;

        void check_insert(NodePtr node) const;

    };

    class AttributeMap {

    private:

        using attr_map = std::map<std::string, std::string>;

    public:

        using attr_iterator = attr_map::const_iterator;

        std::string attr(const std::string& key) const;
        Irange<attr_iterator> attr_range() const { return {attrs_.begin(), attrs_.end()}; }
        void clear_attrs() noexcept { attrs_.clear(); }
        void erase_attr(const std::string& key) noexcept { attrs_.erase(key); }
        bool has_attr(const std::string& key) const noexcept { return attrs_.contains(key); }
        size_t num_attrs() const noexcept { return attrs_.size(); }
        void set_attr(const std::string& key) { set_attr(key, key); }
        void set_attr(const std::string& key, const std::string& value);

    private:

        attr_map attrs_;

    };

    class Element:
    public Detail::DerivedNode<Element, CompoundNode, NodeType::element>,
    public AttributeMap {

    public:

        explicit Element(hidden, const std::string& name);

        std::string name() const { return name_; }

    protected:

        std::string outer_core(Options opt) const override;

    private:

        std::string name_;

    };

    // Document construction         Xmldecl in doc?  noxmldecl flag?  Document xmldecl
    // Construct from options alone  n/a              no               Default xmldecl
    // ''                            ''               yes              None
    // Construct from XML source     no               no               Default xmldecl
    // ''                            ''               yes              None
    // ''                            yes              ignored          Xmldecl from source

    class Document:
    public Detail::DerivedNode<Document, CompoundNode, NodeType::document> {

    public:

        explicit Document(hidden, Options opt = {}) { init_xmldecl(opt); }
        explicit Document(hidden, std::string_view xml, Options opt = {});

        XmldeclPtr xmldecl() const noexcept;
        DtdPtr dtd() const noexcept;

    protected:

        std::string outer_core(Options opt) const override { return inner(opt); }

    private:

        void init_xmldecl(Options opt);

    };

}
