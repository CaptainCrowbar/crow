# HTML/XML Markup

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/markup.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## HTML/XML Markup

The `Htag` and `Xtag` classes write an HTML or XML tag in the constructor,
then write the corresponding closing tag in the destructor. Output can go to
a string or an output stream, defaulting to standard output.

The opening tag can be supplied with or without enclosing angle brackets. The
constructor does not attempt any validation of the tag's format; the output
will be nonsense if the element argument is not a valid HTML/XML tag.

If the opening tag is standalone, the text will simply be written as is, and
no closing tag will be written. Standalone tags can be identified by a
closing slash.

The `Htag` class is also aware of the
[standard HTML5 list of self closing tags](https://www.w3.org/TR/html5/syntax.html#void-elements);
this is the only difference between `Htag` and `Xtag`.

If the element string ends with one line feed, a line feed will be written
after the closing tag, but not the opening one; if it ends with two line
feeds, one will be written after both tags.

The `next()` function closes the current tag and opens a new one. If no
argument is supplied, the new tag will be the same as the previous one.

The `nest()` function can be used to write a piece of literal text enclosed in
one or more tags. The arguments are the output destination, a list of tags
(using the same rules as the `Xtag` constructor), and a string that will be
written enclosed by the tags.

Example:

```c++
Xtag::nest(out, "p\n", "code", "Hello world");
```

Output:

```html
<p><code>Hello world</code></p>\n
```

### XML class

```c++
class Xtag {
    Xtag();
    explicit Xtag(const std::string& element);
    Xtag(const std::string& element, std::string& out);
    Xtag(const std::string& element, std::ostream& out);
    Xtag(Xtag&& t) noexcept;
    virtual ~Xtag() noexcept;
    Xtag& operator=(Xtag&& t) noexcept;
    std::string tag() const;
    void next(const std::string& element = {});
    template <typename Out, typename... Args>
        static void nest(Out& out, const Args&... args);
};
```

### HTML class

```c++
class Htag: public Xtag {
    Htag();
    explicit Htag(const std::string& element);
    Htag(const std::string& element, std::string& out);
    Htag(const std::string& element, std::ostream& out);
    Htag(Htag&& t) = default;
    virtual ~Htag() noexcept;
    Htag& operator=(Htag&& t) = default;
};
```
