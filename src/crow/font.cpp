#include "crow/font.hpp"
#include "crow/binary.hpp"
#include "crow/stdio.hpp"
#include "crow/unicode.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <utility>

#ifdef _MSC_VER
    #pragma warning(push, 1)
#else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC

#include "crow/stb/stb_truetype.h"

#ifdef _MSC_VER
    #pragma warning(pop)
#else
    #pragma GCC diagnostic pop
#endif

#ifdef _WIN32
    #include <windows.h>
#endif

namespace Crow {

    namespace {

        // The stb_truetype library doesn't provide any way to verify that a
        // font file is valid before trying to parse it, so we have to verify
        // the format ourselves, at least to the extent necessary to be
        // confident that stb_truetype won't try to read past the end of our
        // buffer.

        // https://docs.microsoft.com/en-us/typography/opentype/spec/otff

        constexpr uint32_t otf_magic = 0x4f54544ful; // "OTTO"
        constexpr uint32_t ttc_magic = 0x74746366ul; // "ttcf"
        constexpr uint32_t ttf_magic = 0x00010000ul;

        struct FontCoreInfo {
            stbtt_fontinfo info;
            std::string family;
            std::string subfamily;
            std::string name;
            int ascent;
            int descent;
            int line_gap;
            float units_per_em; // Font native units per em
        };

        struct FontNameParams {
            int platform;
            int encoding;
            int language;
        };

        CROW_ENUM_CLASS(FontNameId, int, 0,
            copyright,
            family,
            subfamily,
            unique_subfamily_id,
            full_name,
            version,
            postscript_name,
            trademark,
            manufacturer,
            designer,
            description,
            vendor_url,
            designer_url,
            license,
            license_url,
            reserved,
            typographic_family,
            typographic_subfamily,
            compatible_full,
            sample_text,
            postscript_cid,
            wws_family,
            wws_subfamily,
            light_background,
            dark_background,
            postscript_variation_prefix
        )

        enum class MicrosoftLanguageId: int {
            english_uk = 0x0809,
            english_us = 0x0409,
        };

        bool decode_latin_1(const std::string& latin, std::u32string& utf32) {
            std::u32string out(latin.size(), '\0');
            std::transform(latin.begin(), latin.end(), out.begin(), [] (char c) { return char32_t(uint8_t(c)); });
            utf32 = std::move(out);
            return true;
        }

        bool decode_mac_roman(const std::string& roman, std::u32string& utf32) {

            static constexpr std::array<char32_t, 256> code_table = {{

                0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
                0x0010, 0x2318, 0x2713, 0x2666, 0xF8FF, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
                0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
                0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
                0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
                0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
                0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
                0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
                0x00C4, 0x00C5, 0x00C7, 0x00C9, 0x00D1, 0x00D6, 0x00DC, 0x00E1, 0x00E0, 0x00E2, 0x00E4, 0x00E3, 0x00E5, 0x00E7, 0x00E9, 0x00E8,
                0x00EA, 0x00EB, 0x00ED, 0x00EC, 0x00EE, 0x00EF, 0x00F1, 0x00F3, 0x00F2, 0x00F4, 0x00F6, 0x00F5, 0x00FA, 0x00F9, 0x00FB, 0x00FC,
                0x2020, 0x00B0, 0x00A2, 0x00A3, 0x00A7, 0x2022, 0x00B6, 0x00DF, 0x00AE, 0x00A9, 0x2122, 0x00B4, 0x00A8, 0x2260, 0x00C6, 0x00D8,
                0x221E, 0x00B1, 0x2264, 0x2265, 0x00A5, 0x00B5, 0x2202, 0x2211, 0x220F, 0x03C0, 0x222B, 0x00AA, 0x00BA, 0x03A9, 0x00E6, 0x00F8,
                0x00BF, 0x00A1, 0x00AC, 0x221A, 0x0192, 0x2248, 0x2206, 0x00AB, 0x00BB, 0x2026, 0x00A0, 0x00C0, 0x00C3, 0x00D5, 0x0152, 0x0153,
                0x2013, 0x2014, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x25CA, 0x00FF, 0x0178, 0x2044, 0x20AC, 0x2039, 0x203A, 0xFB01, 0xFB02,
                0x2021, 0x00B7, 0x201A, 0x201E, 0x2030, 0x00C2, 0x00CA, 0x00C1, 0x00CB, 0x00C8, 0x00CD, 0x00CE, 0x00CF, 0x00CC, 0x00D3, 0x00D4,
                0xF8FF, 0x00D2, 0x00DA, 0x00DB, 0x00D9, 0x0131, 0x02C6, 0x02DC, 0x00AF, 0x02D8, 0x02D9, 0x02DA, 0x00B8, 0x02DD, 0x02DB, 0x02C7,

            }};

            std::u32string out(roman.size(), '\0');
            std::transform(roman.begin(), roman.end(), out.begin(), [] (char c) { return code_table[uint8_t(c)]; });
            utf32 = std::move(out);

            return true;

        }

        void free_stb_bitmap(unsigned char* ptr) noexcept {
            if (ptr != nullptr)
                stbtt_FreeBitmap(ptr, nullptr);
        }

        size_t get_file_size(Cstdio& io) noexcept {
            if (! io.is_open())
                return 0;
            io.seek(0, SEEK_END);
            auto size = size_t(io.tell());
            io.seek(0, SEEK_SET);
            return size;
        }

        bool decode_utf16_be(std::string raw, std::u32string& utf32) {
            if (raw.size() % 2 != 0)
                return false;
            std::u16string utf16(raw.size() / 2, 0);
            std::memcpy(utf16.data(), raw.data(), raw.size());
            for (auto& c: utf16)
                c = (c >> 8) + (c << 8);
            try {
                utf32 = decode_string(utf16);
                return true;
            }
            catch (const std::invalid_argument&) {
                return false;
            }
        }

        std::string get_font_name(const FontCoreInfo& font, FontNameId id, const std::vector<FontNameParams>& params) {

            int len = 0;

            for (auto& item: params) {

                auto cptr = stbtt_GetFontNameString(&font.info, &len, item.platform, item.encoding, item.language, int(id));

                if (cptr && len) {

                    std::string raw_name(cptr, len);
                    std::u32string utf32_name;
                    bool ok = false;

                    if (item.platform == STBTT_PLATFORM_ID_MAC)
                        ok = decode_mac_roman(raw_name, utf32_name);
                    else if (raw_name.size() % 2 == 0)
                        ok = decode_utf16_be(raw_name, utf32_name);

                    if (! ok)
                        decode_latin_1(raw_name, utf32_name);

                    return to_utf8(utf32_name);

                }

            }

            return {};

        }

        void init_font_names(FontCoreInfo& font) {

            static const std::vector<FontNameParams> params = {

                { STBTT_PLATFORM_ID_UNICODE,    STBTT_UNICODE_EID_UNICODE_1_0,       0                                     },
                { STBTT_PLATFORM_ID_UNICODE,    STBTT_UNICODE_EID_UNICODE_1_1,       0                                     },
                { STBTT_PLATFORM_ID_UNICODE,    STBTT_UNICODE_EID_ISO_10646,         0                                     },
                { STBTT_PLATFORM_ID_UNICODE,    STBTT_UNICODE_EID_UNICODE_2_0_BMP,   0                                     },
                { STBTT_PLATFORM_ID_UNICODE,    STBTT_UNICODE_EID_UNICODE_2_0_FULL,  0                                     },
                { STBTT_PLATFORM_ID_MICROSOFT,  STBTT_MS_EID_UNICODE_FULL,           int(MicrosoftLanguageId::english_us)  },
                { STBTT_PLATFORM_ID_MICROSOFT,  STBTT_MS_EID_UNICODE_FULL,           int(MicrosoftLanguageId::english_uk)  },
                { STBTT_PLATFORM_ID_MICROSOFT,  STBTT_MS_EID_UNICODE_BMP,            int(MicrosoftLanguageId::english_us)  },
                { STBTT_PLATFORM_ID_MICROSOFT,  STBTT_MS_EID_UNICODE_BMP,            int(MicrosoftLanguageId::english_uk)  },
                { STBTT_PLATFORM_ID_MAC,        STBTT_MAC_EID_ROMAN,                 STBTT_MAC_LANG_ENGLISH                },

            };

            font.family = get_font_name(font, FontNameId::family, params);
            font.subfamily = get_font_name(font, FontNameId::subfamily, params);
            font.name = get_font_name(font, FontNameId::full_name, params);

        }

        void init_font_metrics(FontCoreInfo& font) {
            font.units_per_em = 1.0f / stbtt_ScaleForMappingEmToPixels(&font.info, 1.0f);
            stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.line_gap);
            int x0, y0, x1, y1;
            stbtt_GetFontBoundingBox(&font.info, &x0, &y0, &x1, &y1);
        }

        bool init_font(FontCoreInfo& font, const unsigned char* data, size_t offset) {
            if (! stbtt_InitFont(&font.info, data, int(offset)))
                return false;
            init_font_names(font);
            init_font_metrics(font);
            return true;
        }

        template <typename T>
        T read_be(Cstdio& io) {
            std::string buf = io.reads(sizeof(T));
            std::reverse(buf.begin(), buf.end());
            T t = 0;
            std::memcpy(&t, buf.data(), buf.size());
            return t;
        }

        bool verify_ttf(Cstdio& io, size_t base_offset, size_t file_size) noexcept {
            if (file_size == npos)
                file_size = get_file_size(io);
            size_t ttf_size = file_size - base_offset;
            if (ttf_size < 12)
                return false;
            io.seek(4, SEEK_CUR);
            size_t num_tables = read_be<uint16_t>(io);
            if (16 * num_tables + 12 > ttf_size)
                return false;
            io.seek(6, SEEK_CUR);
            for (size_t i = 0; i < num_tables; ++i) {
                io.seek(8, SEEK_CUR);
                size_t offset = read_be<uint32_t>(io);
                size_t length = read_be<uint32_t>(io);
                if (offset + length > file_size)
                    return false;
            }
            return true;
        }

        bool verify_ttc(Cstdio& io) noexcept {
            size_t file_size = get_file_size(io);
            if (file_size < 12)
                return false;
            io.seek(8, SEEK_CUR);
            size_t num_fonts = read_be<uint32_t>(io);
            if (4 * num_fonts + 12 > file_size)
                return false;
            std::vector<size_t> offsets(num_fonts);
            for (size_t& ofs: offsets) {
                ofs = read_be<uint32_t>(io);
                if (ofs >= file_size)
                    return false;
            }
            for (size_t ofs: offsets) {
                io.seek(ofs, SEEK_SET);
                if (! verify_ttf(io, ofs, file_size))
                    return false;
            }
            return true;
        }

        bool verify_font_data(Cstdio& io) noexcept {
            auto magic = read_be<uint32_t>(io);
            io.seek(-4, SEEK_CUR);
            switch (magic) {
                case ttf_magic:
                case otf_magic:
                    return verify_ttf(io, 0, npos);
                case ttc_magic:
                    return verify_ttc(io);
                default:
                    return false;
            }
        }

        #ifdef _WIN32

            Path get_windows_dir() {
                std::wstring wdir;
                unsigned size = 100;
                for (;;) {
                    wdir.resize(size);
                    size = GetWindowsDirectoryW(reinterpret_cast<wchar_t*>(wdir.data()), size);
                    if (size < wdir.size())
                        break;
                }
                wdir.resize(size);
                if (wdir.empty())
                    return "C:\\Windows";
                else
                    return wdir;
            }

        #else

            Path get_home_dir() {
                auto home_env = std::getenv("HOME");
                Path path;
                if (home_env != nullptr)
                    path = home_env;
                return path;
            }

        #endif

    }

    // Font class

    struct Font::font_impl:
    FontCoreInfo {};

    Font::Font(const Path& file, int index) {

        if (index < 0)
            return;

        Cstdio io;
        try {
            io = Cstdio(file, "rb");
        }
        catch (const IoError&) {
            return;
        }

        if (! verify_font_data(io))
            return;

        io.seek(0, SEEK_SET);
        auto content = std::make_shared<std::string>();
        *content = io.read_all();
        auto data = reinterpret_cast<const unsigned char*>(content->data());
        int num_fonts = stbtt_GetNumberOfFonts(data);

        if (index >= num_fonts)
            return;

        int offset = stbtt_GetFontOffsetForIndex(data, index);
        auto impl = std::make_shared<font_impl>();

        if (! init_font(*impl, data, offset))
            return;

        content_ = content;
        font_ = impl;

    }

    std::string Font::family() const {
        return font_ ? font_->family : std::string();
    }

    std::string Font::subfamily() const {
        return font_ ? font_->subfamily : std::string();
    }

    std::string Font::name() const {
        return font_ ? font_->name : std::string();
    }

    bool Font::has_glyph(char32_t c) const noexcept {
        return font_ && has_glyph_unchecked(c);
    }

    bool Font::has_glyphs(char32_t first, char32_t last) const noexcept {
        if (! font_)
            return false;
        for (char32_t c = first; c <= last; ++c)
            if (! has_glyph_unchecked(c))
                return false;
        return true;
    }

    std::vector<Font> Font::load(const Path& file) {

        std::vector<Font> fonts;

        Cstdio io;
        try {
            io = Cstdio(file, "rb");
        }
        catch (const IoError&) {
            return fonts;
        }

        if (! verify_font_data(io))
            return fonts;

        io.seek(0, SEEK_SET);
        auto content = std::make_shared<std::string>();
        *content = io.read_all();
        if (content->empty())
            return fonts;

        auto data = reinterpret_cast<const unsigned char*>(content->data());
        int num_fonts = stbtt_GetNumberOfFonts(data);
        Font font;

        for (int i = 0; i < num_fonts; ++i) {
            int offset = stbtt_GetFontOffsetForIndex(data, i);
            auto impl = std::make_shared<font_impl>();
            if (! init_font(*impl, data, offset))
                continue;
            font.content_ = content;
            font.font_ = impl;
            fonts.push_back(font);
        }

        return fonts;

    }

    bool Font::has_glyph_unchecked(char32_t c) const noexcept {
        return is_unicode(c) && stbtt_FindGlyphIndex(&font_->info, int(c)) > 0;
    }

    // ScaledFont class

    struct ScaledFont::scaled_impl {
        int ascent_pixels = 0;
        int descent_pixels = 0;
        int line_gap_pixels = 0;
        Point pixels_per_em = Point::null();
        Float2 pixels_per_unit = Float2::null();
    };

    ScaledFont::ScaledFont(const Font& font, Point scale) noexcept:
    Font(font),
    scaled_(std::make_shared<scaled_impl>()) {
        if (font_) {
            scaled_->pixels_per_em = scale;
            scaled_->pixels_per_unit = Float2(scale) / font_->units_per_em;
            scaled_->ascent_pixels = scale_y(font_->ascent);
            scaled_->descent_pixels = scale_y(font_->descent);
            scaled_->line_gap_pixels = scale_y(font_->line_gap);
        }
    }

    Point ScaledFont::scale() const noexcept {
        return scaled_ ? scaled_->pixels_per_em : Point::null();
    }

    int ScaledFont::ascent() const noexcept {
        return scaled_ ? scaled_->ascent_pixels : 0;
    }

    int ScaledFont::descent() const noexcept {
        return scaled_ ? scaled_->descent_pixels : 0;
    }

    int ScaledFont::line_gap() const noexcept {
        return scaled_ ? scaled_->line_gap_pixels : 0;
    }

    Box_i2 ScaledFont::text_box(const std::string& text, int line_shift) const {

        if (! font_ || text.empty())
            return {};

        auto utext = decode_string(text);
        int x = 0, y = 0;
        int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
        int min_x = 0, min_y = 0, max_x = 0, max_y = 0;
        size_t length = utext.size();
        float sx = scaled_->pixels_per_unit.x();
        float sy = scaled_->pixels_per_unit.y();

        for (size_t i = 0; i < length; ++i) {

            if (utext[i] == '\n') {
                x = 0;
                y += ascent() - descent() + line_gap() + line_shift;
                continue;
            }

            stbtt_GetCodepointBitmapBox(&font_->info, int(utext[i]), sx, sy, &x0, &y0, &x1, &y1);

            if (x0 != 0 || y0 != 0 || x1 != 0 || y1 != 0) {
                min_x = std::min(min_x, x + x0);
                max_x = std::max(max_x, x + x1);
                min_y = std::min(min_y, y + y0);
                max_y = std::max(max_y, y + y1);
            }

            if (i + 1 < length) {
                int advance, left_bearing;
                stbtt_GetCodepointHMetrics(&font_->info, int(utext[i]), &advance, &left_bearing);
                int kern_advance = stbtt_GetCodepointKernAdvance(&font_->info, int(utext[i]), int(utext[i + 1]));
                x += scale_x(advance + kern_advance);
            }

        }

        Box_i2 box = {{min_x, min_y}, {max_x - min_x, max_y - min_y}};

        return box;

    }

    size_t ScaledFont::text_fit(const std::string& text, size_t max_pixels) const {

        if (! font_)
            throw std::invalid_argument("No font");
        if (text.empty())
            return 0;

        auto utext = decode_string(text);
        int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        int x = 0, min_x = 0, max_x = 0;
        size_t length = utext.size();
        size_t i = 0;
        float sx = scaled_->pixels_per_unit.x();
        float sy = scaled_->pixels_per_unit.y();

        for (i = 0; i < length; ++i) {

            if (utext[i] == U'\n')
                throw std::invalid_argument("Multiple lines in text fit test");

            stbtt_GetCodepointBitmapBox(&font_->info, int(utext[i]), sx, sy, &x0, &y0, &x1, &y1);

            if (x0 != 0 || y0 != 0 || x1 != 0 || y1 != 0) {
                min_x = std::min(min_x, x + x0);
                max_x = std::max(max_x, x + x1);
                size_t test_width = size_t(max_x - min_x);
                if (test_width > max_pixels)
                    break;
            }

            if (i + 1 < length) {
                int advance, left_bearing;
                stbtt_GetCodepointHMetrics(&font_->info, int(utext[i]), &advance, &left_bearing);
                int kern_advance = stbtt_GetCodepointKernAdvance(&font_->info, int(utext[i]), int(utext[i + 1]));
                x += scale_x(advance + kern_advance);
            }

        }

        if (i == length)
            return npos;

        auto prefix = to_utf8(utext.substr(0, i));

        return prefix.size();

    }

    size_t ScaledFont::text_wrap(const std::string& text_in, std::string& text_out, size_t max_pixels) const {

        text_out.clear();

        if (! font_)
            throw std::invalid_argument("No font");

        std::vector<std::string> lines;
        auto paras = splitv(text_in, "\n");
        if (paras.empty())
            return 0;

        for (auto& p: paras) {

            for (std::string para(p); ! para.empty();) {

                size_t fit = text_fit(para, max_pixels);
                if (fit == npos) {
                    lines.push_back(para);
                    break;
                }

                size_t cut = para.find_last_of(' ', fit + 1);
                if (cut == 0 || cut == npos)
                    cut = para.find_first_of(' ', fit + 1);
                if (cut == npos) {
                    lines.push_back(para);
                    break;
                }

                lines.push_back(para.substr(0, cut));
                para.erase(0, cut + 1);

            }

        }

        text_out = join(lines, "\n");

        return lines.size();

    }

    Detail::ByteMask ScaledFont::render_glyph_mask(char32_t c, Point& offset) const {
        using namespace Detail;
        offset = Point::null();
        int width, height, xoff, yoff;
        auto bitmap_ptr = stbtt_GetCodepointBitmap(&font_->info, scaled_->pixels_per_unit.x(), scaled_->pixels_per_unit.y(),
            int(c), &width, &height, &xoff, &yoff);
        if (! bitmap_ptr)
            return {};
        ByteMask mask({width, height}, bitmap_ptr, free_stb_bitmap);
        offset = {xoff, yoff};
        return mask;
    }

    Detail::ByteMask ScaledFont::render_text_mask(const std::u32string& utext, int line_shift, Point& offset) const {

        using namespace Detail;

        size_t length = utext.size();
        std::vector<ByteMask> glyph_masks(length);
        std::vector<Point> glyph_offsets(length); // Top left of glyph relative to initial reference point
        int line_delta = line_offset() + line_shift;
        int min_x = 0, max_x = 0, min_y = 0, max_y = 0;
        Point ref_point = Point::null();

        for (size_t i = 0; i < length; ++i) {

            if (utext[i] == U'\n') {

                ref_point.x() = 0;
                ref_point.y() += line_delta;

            } else {

                glyph_masks[i] = render_glyph_mask(utext[i], glyph_offsets[i]);
                glyph_offsets[i] += ref_point;
                min_x = std::min(min_x, glyph_offsets[i].x());
                min_y = std::min(min_y, glyph_offsets[i].y());
                max_x = std::max(max_x, glyph_offsets[i].x() + glyph_masks[i].shape().x());
                max_y = std::max(max_y, glyph_offsets[i].y() + glyph_masks[i].shape().y());
                int advance, left_bearing;
                stbtt_GetCodepointHMetrics(&font_->info, int(utext[i]), &advance, &left_bearing);
                ref_point.x() += scale_x(advance);
                if (i + 1 < length && utext[i + 1] != U'\n')
                    ref_point.x() += scale_x(stbtt_GetCodepointKernAdvance(&font_->info, int(utext[i]), int(utext[i + 1])));

            }

        }

        offset = {min_x, min_y};
        Point text_shape = Point{max_x, max_y} - offset;
        ByteMask text_mask(text_shape);

        for (size_t i = 0; i < length; ++i) {

            if (glyph_masks[i].empty())
                continue;

            auto glyph_offset = glyph_offsets[i] - offset;
            Point shape = glyph_masks[i].shape();

            for (int glyph_y = 0, text_y = glyph_offset.y(); glyph_y < shape.y(); ++glyph_y, ++text_y) {
                auto glyph_ptr = &glyph_masks[i][{0, glyph_y}];
                auto text_ptr = &text_mask[{glyph_offset.x(), text_y}];
                for (int x = 0; x < shape.x(); ++x, ++text_ptr, ++glyph_ptr)
                    *text_ptr = std::max(*text_ptr, *glyph_ptr);
            }

        }

        return text_mask;

    }

    int ScaledFont::scale_x(int x) const noexcept {
        return int(std::lround(scaled_->pixels_per_unit.x() * float(x)));
    }

    int ScaledFont::scale_y(int y) const noexcept {
        return int(std::lround(scaled_->pixels_per_unit.y() * float(y)));
    }

    Box_i2 ScaledFont::scale_box(Box_i2 box) const noexcept {
        int x0 = int(std::floor(scaled_->pixels_per_unit.x() * float(box.base().x())));
        int y0 = int(std::floor(scaled_->pixels_per_unit.y() * float(box.base().y())));
        int x1 = int(std::ceil(scaled_->pixels_per_unit.x() * float(box.apex().x())));
        int y1 = int(std::ceil(scaled_->pixels_per_unit.y() * float(box.apex().y())));
        return {{x0, y0}, {x1 - x0, y1 - y0}};
    }

    // FontMap class

    bool FontMap::contains(const std::string& family, const std::string& subfamily) const noexcept {
        auto it = table_.find(family);
        return it != table_.end() && it->second.count(subfamily) != 0;
    }

    std::vector<std::string> FontMap::families() const {
        std::vector<std::string> vec;
        std::transform(table_.begin(), table_.end(), std::back_inserter(vec),
            [] (auto& pair) { return pair.first; });
        return vec;
    }

    std::vector<std::string> FontMap::subfamilies(const std::string& family) const {
        std::vector<std::string> vec;
        auto it = table_.find(family);
        if (it != table_.end())
            std::transform(it->second.begin(), it->second.end(), std::back_inserter(vec),
                [] (auto& pair) { return pair.first; });
        return vec;
    }

    Font FontMap::find(const std::vector<std::string>& families, FontStyle style) const {

        static const std::vector<std::vector<std::string>> style_names = {
            { // Regular
                "Regular", "Book", "Medium", "Plain", "Roman", "Solid", "Light", "Thin", "W3"
            },
            { // Bold
                "Bold", "Black", "Semibold", "W6"
            },
            { // Italic
                "Italic", "Inclined", "Oblique",
                "Regular Italic", "RegularItalic", "Book Italic", "BookItalic",
                "Medium Italic", "MediumItalic", "Light Italic", "LightItalic"
            },
            { // Bold italic
                "Bold Italic", "BoldItalic", "Bold Inclined", "BoldInclined",
                "Bold Oblique", "BoldOblique", "Black Italic", "BlackItalic"
            },
        };

        int style_index = int(style) & 3;

        for (auto& family: families) {
            auto fam_it = table_.find(family);
            if (fam_it == table_.end())
                continue;
            for (auto& name: style_names[style_index]) {
                auto& subfamilies = fam_it->second;
                auto sub_it = subfamilies.find(name);
                if (sub_it != subfamilies.end()) {
                    auto& result = sub_it->second;
                    return Font(result.file, result.index);
                }
            }
        }

        if (style_index == 0 || has_bit(style, FontStyle::fallback)) {
            for (auto& family: families) {
                auto fam_it = table_.find(family);
                if (fam_it != table_.end()) {
                    auto& subfamilies = fam_it->second;
                    auto& result = subfamilies.begin()->second;
                    return Font(result.file, result.index);
                }
            }
        }

        return {};

    }

    Font FontMap::load(const std::string& family, const std::string& subfamily) const {
        auto i = table_.find(family);
        if (i == table_.end())
            return {};
        auto j = i->second.find(subfamily);
        if (j == i->second.end())
            return {};
        else
            return Font(j->second.file, j->second.index);
    }

    void FontMap::search(const Path& dir, Path::flag_type flags) {
        auto check_file = [this] (const Path& file) {
            if (file.is_file()) {
                int index = 0;
                for (auto& font: Font::load(file))
                    table_[font.family()][font.subfamily()] = {file, index++};
            }
        };
        if (has_bit(flags, Path::recurse)) {
            for (auto& file: dir.deep_search())
                check_file(file);
        } else {
            for (auto& file: dir.directory())
                check_file(file);
        }
    }

    void FontMap::search_system() {

        #ifdef _WIN32

            auto font_dir = Path::join(get_windows_dir(), "Fonts");
            search(font_dir, Path::flag::recurse);

        #else

            static constexpr const char* system_font_dirs[] = {
                #ifdef __APPLE__
                    "/System/Library/Fonts",
                    "/Library/Fonts",
                    "~/Library/Fonts",
                #else
                    "/usr/X11R6/lib/X11/fonts",
                    "/usr/share/fonts",
                    "/usr/local/share/fonts",
                    "~/.fonts",
                    "~/.local/share/fonts",
                #endif
            };

            Path home = get_home_dir();
            Path dir;

            for (auto& cdir: system_font_dirs) {
                if (cdir[0] == '~' && cdir[1] == '/' && ! home.is_empty())
                    dir = home / (cdir + 2);
                else
                    dir = cdir;
                search(dir, Path::recurse);
            }

        #endif

    }

}
