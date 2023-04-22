# XML

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/xml.hpp"
namespace Crow::Xml;
```

## Contents

* TOC
{:toc}

## Introduction

The facilities in the `Crow::Xml` namespace allow generation and parsing of
[https://www.w3.org/TR/xml/](XML). This is a fairly minimal implementation
that does not attempt to support every feature in the XML standard.

* Document type definitions are not explicitly supported. DTDs are recognised
  in parsing, but treated as a single opaque string with no internal
  structure, that can only be read and written as a block.
* XML namespaces are ignored, beyond simply allowing the colon as a valid
  character in element names.
* The restriction of an XML document to a single root node (apart from the
  prologue) is not enforced.
* XML declarations and DTDs are expected only in the standard prologue
  position, but users are not prevented from inserting them anywhere in a
  document.

## Supporting types

```c++
class Error: public std::runtime_error {
    std::string text() const;
};
```

Exception used to report XML construction errors. The `text()` function
returns the part of the input text that appears to be invalid.

```c++
enum class Options: int {
    xml = 0,
    autoclose,  // Implicit closing
    comments,   // Keep comments
    encoded,    // Text is already encoded
    foldws,     // Collapse whitespace
    icase,      // Case insensitive names
    keyonly,    // Attribute values are optional
    noxmldecl,  // No default XML declaration
    selfclose,  // HTML self-closing elements
    xentity,    // HTML character entities
    html = autoclose | foldws | icase | keyonly
        | noxmldecl | selfclose | xentity,
};
```

Bitmask flags controlling the behaviour of XML parsing and generation. Most of
these are only relevant to parsing. The default behaviour, with all options
switched off, follows the syntax of the XML standard, except as noted above.
The `html` option, which switches on all flags except `comments` and
`encoded`, parses markup as HTML5.

Except where the list below states otherwise, options should be assumed to
only affect XML parsing, and ignored in any other context.

* `Options::autoclose` -- Allow implicit closing of elements, when the closing
  tag for an outer element is encountered before that for an inner element.
  Also, ignore spurious closing tags for which a matching opening tag has not
  been seen.
* `Options::comments` -- Keep comments when parsing. In the absence of this
  option, the parsed XML tree will not contain any comment node. This does
  not prevent user from inserting them explicitly. This option is not
  included in `html`.
* `Options::encoded` -- The input text is assumed to be already encoded. This
  option only affects the `Text::create()` function; it has no effect on
  parsing. This option is not included in `html`.
* `Options::foldws` -- Inside text elements, collapse any sequence of
  whitespace characters into a single space.
* `Options::icase` -- Element and attribute names are folded to lower case
  during parsing.
* `Options::keyonly` -- Attribute values are optional. An attribute can
  consist of the key alone, in which case the value is treated as being the
  same as the key. Besides parsing, this also affects the output of the
  `Node::outer()` function.
* `Options::noxmldecl` -- Do not insert the standard default XML declaration
  if one was not found in the document. Besides parsing, this also affects
  the `Document::create()` function(refer to the `Document` class below for
  details).
* `Options::selfclose` -- Recognise the standard list of HTML self-closing
  elements, always treating them as standalone elements. Besides parsing,
  this also affects the output of the `Node::outer()` function.
* `Options::xentity` -- Recognise the standard list of extended HTML character
  entities. In the absence of this option, only the five standard XML
  character entities are recognised(`&amp; &lt; &gt; &quot; &apos;`). Besides
  parsing, this also affects the `decode_text()` function.

```c++
enum class NodeType: int {
    null,
    xmldecl,
    dtd,
    cdata,
    comment,
    entity,
    processing,
    text,
    element,
    document,
};
```

Values returned by the `Node::type()` function, matching the type of the
node.

```c++
class AttributeMap {
    class attr_iterator;
        // Bidirectional const iterator
        // Dereferences to a pair of strings
    std::string attr(const std::string& key) const;
    Irange<attr_iterator> attr_range() const;
    void clear_attrs() noexcept;
    void erase_attr(const std::string& key) noexcept;
    bool has_attr(const std::string& key) const noexcept;
    size_t num_attrs() const noexcept;
    void set_attr(const std::string& key);
    void set_attr(const std::string& key, const std::string& value);
};
```

A key-value map used for element attributes.

## Functions

```c++
std::string decode_text(std::string_view str, Options opt = {});
std::string encode_text(std::string_view str);
```

Encode or decode text using character entities. The `decode_text()` function
respects the `Options::xentity` flag. The `encode_text()` function will only
generate the standard XML character entities, along with code point entities
for non-whitespace control characters.

```c++
bool is_valid_name(std::string_view name) noexcept;
```

True if this is a valid XML element, attribute, or entity name.

```c++
NodeType node_type(const NodePtr& node) noexcept;
```

Returns `node->type()` if the node is valid, or `NodeType::null` if the
pointer is null.

## XML node classes

```c++
class Node;
    class SimpleNode: public Node;
        class PrologueNode: public SimpleNode;
            class Xmldecl: public PrologueNode;
            class Dtd: public PrologueNode;
        class InlineNode: public SimpleNode;
            class Cdata: public InlineNode;
            class Comment: public InlineNode;
            class Entity: public InlineNode;
            class Processing: public InlineNode;
            class Text: public InlineNode;
    class CompoundNode: public Node;
        class Element: public CompoundNode;
        class Document: public CompoundNode;
```

XML node classes, described in detail below.

```c++
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
```

Defined for convenience.

### Node base class

```c++
class Node {
    virtual ~Node() noexcept;
    virtual NodePtr clone() const = 0;
    virtual NodeType type() const noexcept = 0;
    std::string inner(Options opt = Options::xml) const;
    std::string outer(Options opt = Options::xml) const;
};
```

Common abstract base class for all XML node classes. Derived classes do not
have public constructors; they must be created on the heap using each concrete
class's static `create()` function.

The `clone()` function returns a shallow copy of the current node. The
`type()` function is a quick way of checking the node's type.

The `inner()` and `outer()` functions return the node's inner and outer XML.
The `outer()` function returns the complete XML for the node. The `inner()`
function returns the XML text inside the node; it will return an empty string
for all node types except `Element`. The only options that affect these
functions are `keyonly` and `selfclose()`.

### Simple node base class

```c++
class SimpleNode: public Node;
```

Abstract base class for nodes that do not contain any other nodes.

## Prologue node classes

```c++
class PrologueNode: public SimpleNode;
```

Abstract base class for node types that normally appear only in a document's
prologue (`Xmldecl` and `Dtd`).

```c++
class Xmldecl: public PrologueNode {
    static std::shared_ptr<Xmldecl> create(std::string_view str = {});
};
```

An XML declaration of the form `"<?xml...?>"` (case insensitive). The
bracketing `"<?xml"` and `"?>"` will be inserted if they are not supplied. If
`Xmldecl::create()` is called with no arguments, or with an empty string, the
standard default XML declaration will be used.

```c++
class Dtd: public PrologueNode {
    static std::shared_ptr<Dtd> create(std::string_view str);
};
```

A document type declaration of the form `"<!DOCTYPE...>"`. The bracketing
`"<!DOCTYPE"` and `">"` will be inserted if they are not supplied.

## Inline node classes

```c++
class InlineNode: public SimpleNode;
```

Abstract base class for simple inline nodes.

```c++
class Cdata: public InlineNode {
    static std::shared_ptr<Cdata> create(std::string_view str);
};
```

A character data block of the form `"<[CDATA[...]]>"`. The bracketing
`"<[[CDATA"` and `"]]>"` will be inserted if they are not supplied.

```c++
class Comment: public InlineNode {
    static std::shared_ptr<Comment> create(std::string_view str);
};
```

A comment of the form `"<!--...-->"`. The bracketing `"<!--"` and `"-->"` will
be inserted if they are not supplied.

```c++
class Entity: public InlineNode {
    static std::shared_ptr<Entity> create(std::string_view str);
};
```

An inline entity of the form `"&...;>"`. The bracketing `"&"` and `";"` will
be inserted if they are not supplied.

```c++
class Processing: public InlineNode {
    static std::shared_ptr<Processing> create(std::string_view str);
};
```

A processing instruction of the form `"<?...?>"`. The bracketing `"<?"` and
`"?>"` will be inserted if they are not supplied.

```c++
class Text: public InlineNode {
    std::string plain() const;
    static std::shared_ptr<Text> create(std::string_view str,
        Options opt = Options::xml);
};
```

A plain text block. By default, the input text is assumed to be plain text
that needs to be encoded. It will be assumed to be already encoded if
`Options::encoded` is passed. No other option affects this function.

The `plain()` function returns the node's contents as decoded plain text.

## Compound node classes

```c++
class CompoundNode: public Node {
    class iterator;
        // Random access const iterator
        // Dereferences to a non-null NodePtr
    iterator begin() const noexcept;
    iterator end() const noexcept;
    NodePtr child(size_t i) const noexcept;
    size_t children() const noexcept;
    void clear() noexcept;
    bool empty() const noexcept;
    void erase(iterator i) noexcept;
    const NodePtr& front() const noexcept;
    const NodePtr& back() const noexcept;
    iterator insert(iterator i, NodePtr node);
    void pop_back() noexcept;
    void push_back(NodePtr node);
    CompoundNode& operator+=(NodePtr node);
};
```

Abstract base class for nodes that contain other nodes(`Element` and
`Document`). Most of these functions have their normal meaning for
containers. The `child()` function will return a child node by index, or a
null pointer if the index is out of bounds. The `+=` operator is equivalent
to `push_back()`. Inserting a null `NodePtr` will have no effect.

```c++
class Element: public CompoundNode, public AttributeMap {
    std::string name() const { return name_; }
    static std::shared_ptr<Element> create(const std::string& name);
};
```

An XML element. This is constructed with only a name; attributes can be added
after construction.

```c++
class Document: public CompoundNode {
    XmldeclPtr xmldecl() const noexcept;
    DtdPtr dtd() const noexcept;
    static std::shared_ptr<Document> create(Options opt = {});
    static std::shared_ptr<Document> create(std::string_view xml,
        Options opt = {});
};
```

An XML document. The first version of `create()` returns an empty document
(except possibly for an XML declaration, as described below); the second
version parses an existing document.

If the parsed document contains an XML declaration, it will be retained
regardless of the `Options::noxmldecl` flag. Otherwise, a default `Xmldecl`
will be created unless suppressed by the flag.

| Document creation          | Xmldecl in doc?  | Noxmldecl flag?  | Document xmldecl     |
| -----------------          | ---------------  | ---------------  | ----------------     |
| Default create             | N/A              | N/A              | Default xmldecl      |
| Create from options alone  | N/A              | No               | Default xmldecl      |
|                            |                  | Yes              | None                 |
| Create from XML source     | No               | No               | Default xmldecl      |
|                            |                  | Yes              | None                 |
|                            | Yes              | Ignored          | Xmldecl from source  |
