# Images

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/image.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
using Point = Int2;
```

Used for pixel coordinates.

```c++
enum class ImageFlags: int {
    none    = 0,
    invert  = 1,
    pma     = 2,
}
```

Bitmask flags indicating properties of an image. Images are normally stored
top down; they will be stored bottom up if the `invert` flag is used. Pixel
data will be assumed to include premultiplied alpha if the `pma` flag is
used.

```c++
enum class ImageResize: int {
    none    = 0,
    unlock  = 1,
    wrap    = 2,
}
```

Bitmask flags controlling the behaviour of the `resize()` function.

```c++
class ImageIoError:
public std::runtime_error {
    Path file() const noexcept;
};
```

This is thrown when one of the image I/O functions encounters an error. The
name of the offending file will be supplied if possible.

```c++
struct ImageInfo {
    Point shape = Point::null();
    int channels = 0;
    int bits_per_channel = 0;
    bool has_alpha = false;
    bool is_hdr = false;
    explicit operator bool() const noexcept;
    std::string str() const;
};
std::ostream& operator<<(std::ostream& out, const ImageInfo& info);
```

Information about an image file. The boolean operator is true if the shape is
not a null vector, indicating that a file has been successfully queried.

## Image class

```c++
template <ColourType CT, ImageFlags Flags = ImageFlags::none> class Image;
```

The image class.

### Member types

```c++
class Image::iterator {
    iterator& move(int axis, int distance = 1) noexcept;
    Point pos() const noexcept;
};
class Image::const_iterator {
    const_iterator& move(int axis, int distance = 1) noexcept;
    Point pos() const noexcept;
};
```

Bidirectional iterators over the image's pixels. Iterators have two additional
member functions:

* `move()` moves the iterator along the given axis by the given number of
  pixels. The axis is 0 for x, 1 for y; you can also use 'x' and 'y'.
  Behaviour is undefined if the axis is not one of these values.
* `pos()` returns the position vector corresponding to the iterator's current
  position.

Iterators can be considered to exist in an infinite plane (bounded in practise
by the range of an `int`); moving an iterator outside the bounds of the image
using `move()` is safe. Behaviour is undefined if any of `operator*()`,
`operator++()`, `pos()`, or the comparison operators are called on an
off-image iterator.

```c++
using Image::channel_type = CT::value_type;
using Image::colour_space = CT::colour_space;
using Image::colour_type = CT;
```

Properties of the pixel type.

### Constants

```c++
static constexpr int channels = CT::channels;
static constexpr ColourLayout colour_layout = CL;
static constexpr bool has_alpha = CT::has_alpha;
static constexpr bool is_hdr = CT::is_hdr;
static constexpr bool is_linear = CT::is_linear;
```

Properties of the pixel type.

```c++
static constexpr bool is_bottom_up;
static constexpr bool is_top_down;
```

Indicate whether the image is laid out top-down or bottom-up in memory.

```c++
static constexpr bool is_premultiplied;
```

True if the image uses premultiplied alpha.

### Life cycle functions

```c++
Image::Image() noexcept;
```

The default constructor creates an empty image with zero width and height.

```c++
explicit Image::Image(Point shape);
Image::Image(Point shape, CT c);
Image::Image(int w, int h);
Image::Image(int w, int h, CT c);
```

Create an image with the specified dimensions. If a colour is supplied, the
new image will be filled with that colour; otherwise, the pixel data is left
uninitialized. These will throw `std::invalid_argument` if either dimension
is negative, or if one is zero but the other is not.

```c++
Image::Image(const Image& img);
Image::Image(Image&& img) noexcept;
Image::~Image() noexcept;
Image& Image::operator=(const Image& img);
Image& Image::operator=(Image&& img) noexcept;
```

Other life cycle functions.

### Pixel access functions

```c++
CT& Image::operator[](Point p) noexcept;
const CT& Image::operator[](Point p) const noexcept;
CT& Image::operator()(int x, int y) noexcept;
const CT& Image::operator()(int x, int y) const noexcept;
```

Reference to a specific pixel. Behaviour is undefined if the coordinates are
out of bounds.

```c++
iterator Image::begin() noexcept;
const_iterator Image::begin() const noexcept;
iterator Image::end() noexcept;
const_iterator Image::end() const noexcept;
```

Iterators over the image's pixels. The `begin()` iterator is the same pixel
returned by `locate(0,0)`.

```c++
iterator Image::bottom_left() noexcept;
const_iterator Image::bottom_left() const noexcept;
iterator Image::bottom_right() noexcept;
const_iterator Image::bottom_right() const noexcept;
iterator Image::top_left() noexcept;
const_iterator Image::top_left() const noexcept;
iterator Image::top_right() noexcept;
const_iterator Image::top_right() const noexcept;
```

These return iterators pointing to the pixel at one corner of the image.
Behaviour is undefined if the image is empty.

```c++
channel_type* Image::data() noexcept;
const channel_type* Image::data() const noexcept;
CT* Image::pixels() noexcept;
const CT* Image::pixels() const noexcept;
```

Pointers to the image data.

```c++
iterator Image::locate(Point p) noexcept;
const_iterator Image::locate(Point p) const noexcept;
iterator Image::locate(int x, int y) noexcept;
const_iterator Image::locate(int x, int y) const noexcept;
```

These return an iterator pointing to the pixel at the specified coordinates.
The iterator returned by `locate(0,0)` will match either `top_left()` or
`bottom_left()`, depending on whether the image is stored top-down or
bottom-up. Behaviour is undefined if the coordinates are out of bounds.

### Conversion functions

```c++
Image<CT, [modified flags]> Image::multiply_alpha() const;
Image<CT, [modified flags]> Image::unmultiply_alpha() const;
```

Convert a non-premultiplied image into a premultiplied-alpha image, or vice
versa. The returned image type has the opposite premultiplication flag. These
are only defined if `CT::can_premultiply` is true.

```c++
template <typename C1, ImageFlags F1, typename C2, ImageFlags F2>
    void convert_image(const Image<C1, F1>& in, Image<C2, F2>& out);
```

Convert an image from one format to another.

### I/O functions

The current implementation uses
[Sean Barrett's STB library](https://github.com/nothings/stb)
for image I/O.

```c++
void Image::load(const Path& file);
```

Load an image from a file. Supported image types are BMP, GIF, HDR/RGBE, JPEG,
PIC, PNG, PNM, PSD, and TGA (not all features are supported for some
formats). Input channel data can be 32-bit floating point for HDR/RGBE, 8-bit
or 16-bit integer for all other formats. This will throw `ImageIoError` if
the file does not exist, the image was not in a supported format, the image
was too big for the STB library to load (the size limit is about 1-2 GB
depending on format), or an I/O error occurs.

```c++
void Image::save(const Path& file, int quality = 90) const;
```

Save an image to a file. The image format is deduced from the file name
suffix. Supported formats are BMP, HDR/RGBE, JPEG, PNG, and TGA. For JPEG
images, the quality setting is clamped to `[1,100]`; for other formats the
quality argument is ignored. This will throw `ImageIoError` if the image
format is not supported or an I/O error occurs.

```c++
ImageInfo query_image(const Path& file) noexcept;
```

Queries an image file for information about the stored image. File formats
supported are the same as for `Image::load()`. This will return a null
`ImageInfo` if the file is not found or is not in a recognisable format.

### Image size and shape functions

```c++
bool Image::empty() const noexcept;
```

True if the image is empty (both dimensions are zero).

```c++
void Image::reset(Point new_shape);
void Image::reset(Point new_shape, CT c);
void Image::reset(int w, int h);
void Image::reset(int w, int h, CT c);
```

Replace the image with a new image with the specified dimensions, discarding
the image's current contents. If a colour is supplied, the new image will be
filled with that colour; otherwise, the pixel data is left uninitialized.
These will throw `std::invalid_argument` if either dimension is negative, or
if one is zero but the other is not.

```c++
void Image::resize(Point new_shape, ImageResize flags = ImageResize::none);
void Image::resize(double scale, ImageResize flags = ImageResize::none);
Image Image::resized(Point new_shape, ImageResize flags = ImageResize::none) const;
Image Image::resized(double scale, ImageResize flags = ImageResize::none) const;
```

Resample the image to a new set of dimensions. The `resize()` functions modify
the image in place; the `resized()` functions return a new image. The first
version of each function accepts a new width and height in pixels; the second
version accepts a floating point scale factor(the new dimensions will be
rounded to the nearest integer).

In the absence of the `unlock` flag, either of the `new_shape` dimensions
(but not both) can be zero; the zero dimension will be scaled in proportion
to the other dimension (rounded to the nearest integer). If both dimensions
are set, the default behaviour is to scale the image to the largest size that
will fit within the given dimensions. If the `unlock` flag is set, both of
the `new_shape` dimensions are taken literally, which may change the aspect
ratio of the image. The `unlock` flag has no effect on the `scale` based
functions.

The `wrap` flag treats the image as wrapped around in both directions when
interpolating edge values.

The `new_shape` based functions will throw `std::invalid_argument` if both
dimensions are zero or either dimension is negative, or if either dimension
is zero when the `unlock` flag is used. The `scale` based functions will
throw `std::invalid_argument` if the scale is zero or negative.

```c++
Point Image::shape() const noexcept;
int Image::width() const noexcept;
int Image::height() const noexcept;
```

Query the dimensions of the image.

```c++
size_t Image::size() const noexcept;
size_t Image::bytes() const noexcept;
```

The size of the image, in pixels or bytes.

### Other member functions

```c++
void Image::clear() noexcept;
```

Resets the `Image` object to an empty image. Equivalent to `reset(0,0)`.

```c++
void Image::fill(CT c) noexcept;
```

Fills all pixels with a uniform colour.

```c++
void Image::swap(Image& img) noexcept;
void swap(Image& a, Image& b) noexcept;
```

Swap two images.

### Comparison operators

```c++
bool operator==(const Image& a, const Image& b) noexcept;
bool operator!=(const Image& a, const Image& b) noexcept;
```

These perform a full comparison on the pixel data of the images.
