cmake_minimum_required(VERSION 3.26)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# The version number here is the canonical one for the project.
# Run rs-bump-version to increment the patch number
# and propagate the new version to version.hpp.

project(Crow
    VERSION 2.0.264
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
    add_compile_options(/EHsc /Gy /MP /O2 /sdl /utf-8 /W4 /WX)
else()
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    add_compile_options(-fdiagnostics-color=always -finput-charset=UTF-8 -march=native -O2 -Wall -Wextra -Wpedantic -Werror)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wsuggest-override -Wsuggest-destructor-override)
    endif()
endif()

if(WIN32)
    add_compile_definitions(NOMINMAX=1 UNICODE=1 _UNICODE=1 WINVER=0x601 _WIN32_WINNT=0x601)
else()
    add_compile_definitions(_REENTRANT=1 _XOPEN_SOURCE=700)
    if(APPLE)
        add_compile_definitions(_DARWIN_C_SOURCE=1)
    endif()
endif()

set(library crow)
set(unittest test-${library})
include_directories(.)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

set(executable PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

add_library(${library} STATIC

    ${library}/approx.cpp
    ${library}/colour.cpp
    ${library}/curl-api.cpp
    ${library}/dice.cpp
    ${library}/dso.cpp
    ${library}/encoding.cpp
    ${library}/english.cpp
    ${library}/font.cpp
    ${library}/format-floating.cpp
    ${library}/format-numeric.cpp
    ${library}/format-range.cpp
    ${library}/format-spec.cpp
    ${library}/format-type.cpp
    ${library}/hash.cpp
    ${library}/hexmap.cpp
    ${library}/http.cpp
    ${library}/image.cpp
    ${library}/log-scale.cpp
    ${library}/markup.cpp
    ${library}/mp-integer.cpp
    ${library}/name.cpp
    ${library}/named-mutex.cpp
    ${library}/options.cpp
    ${library}/path.cpp
    ${library}/probability.cpp
    ${library}/progress.cpp
    ${library}/rational.cpp
    ${library}/regex.cpp
    ${library}/spectrum.cpp
    ${library}/sqlite.cpp
    ${library}/stdio.cpp
    ${library}/string.cpp
    ${library}/table.cpp
    ${library}/terminal.cpp
    ${library}/text-gen.cpp
    ${library}/thread-pool.cpp
    ${library}/time.cpp
    ${library}/unicode.cpp
    ${library}/unicode-normalization.cpp
    ${library}/uri.cpp
    ${library}/uuid.cpp
    ${library}/web-client.cpp
    ${library}/xml.cpp
    ${library}/xml-reference-data.cpp

    ${library}/ucd/canonical-combining-class.cpp
    ${library}/ucd/canonical-decomposition-mapping.cpp
    ${library}/ucd/east-asian-width.cpp
    ${library}/ucd/extended-pictographic.cpp
    ${library}/ucd/full-composition-exclusion.cpp
    ${library}/ucd/general-category.cpp
    ${library}/ucd/grapheme-cluster-break.cpp
    ${library}/ucd/pattern-syntax.cpp
    ${library}/ucd/xid-continue.cpp
    ${library}/ucd/xid-start.cpp

)

add_executable(${unittest}

    test/unit-test.cpp
    test/version-test.cpp
    test/log-test.cpp
    test/types-test.cpp

    test/algorithm-test.cpp
    test/approx-arithmetic-test.cpp
    test/approx-construction-test.cpp
    test/approx-formatting-test.cpp
    test/benchmark-test.cpp
    test/binary-test.cpp
    test/bounded-array-construction-test.cpp
    test/bounded-array-insertion-test.cpp
    test/bounded-array-misc-test.cpp
    test/bounded-array-tracking-test.cpp
    test/cache-test.cpp
    test/colour-alpha-test.cpp
    test/colour-conversion-test.cpp
    test/colour-floating-channel-test.cpp
    test/colour-integer-channel-test.cpp
    test/colour-interpolation-test.cpp
    test/colour-space-test.cpp
    test/colour-string-test.cpp
    test/compact-array-construction-test.cpp
    test/compact-array-insertion-test.cpp
    test/compact-array-misc-test.cpp
    test/compact-array-tracking-test.cpp
    test/constants-test.cpp
    test/dice-test.cpp
    test/dso-test.cpp
    test/encoding-test.cpp
    test/english-test.cpp
    test/enum-arithmetic-test.cpp
    test/enum-bitmask-test.cpp
    test/enum-concepts-test.cpp
    test/enum-definition-test.cpp
    test/enum-internals-test.cpp
    test/fixed-binary-large-binary-100-test.cpp
    test/fixed-binary-large-binary-35-test.cpp
    test/fixed-binary-misc-test.cpp
    test/fixed-binary-small-binary-35-test.cpp
    test/fixed-binary-small-binary-5-test.cpp
    test/flexible-map-test.cpp
    test/flexible-set-test.cpp
    test/font-test.cpp
    test/format-complex-test.cpp
    test/format-floating-test.cpp
    test/format-integer-test.cpp
    test/format-misc-test.cpp
    test/format-numeric-test.cpp
    test/format-print-test.cpp
    test/format-range-test.cpp
    test/format-type-test.cpp
    test/formula-test.cpp
    test/geometry-test.cpp
    test/guard-test.cpp
    test/hash-test.cpp
    test/hexmap-art-test.cpp
    test/hexmap-building-test.cpp
    test/hexmap-geometry-test.cpp
    test/image-io-test.cpp
    test/image-resize-test.cpp
    test/image-test.cpp
    test/index-table-test.cpp
    test/interpolate-concepts-test.cpp
    test/interpolate-functions-test.cpp
    test/interpolate-map-test.cpp
    test/interpolate-segmented-test.cpp
    test/interpolate-spline-test.cpp
    test/iterator-test.cpp
    test/log-scale-arithmetic-test.cpp
    test/log-scale-basics-test.cpp
    test/log-scale-formatting-test.cpp
    test/log-scale-nested-test.cpp
    test/log-scale-parsing-test.cpp
    test/markov-test.cpp
    test/markup-test.cpp
    test/maths-arithmetic-functions-test.cpp
    test/maths-literals-test.cpp
    test/maths-properties-test.cpp
    test/maths-special-functions-test.cpp
    test/matrix-test.cpp
    test/meta-test.cpp
    test/mirror-map-test.cpp
    test/mp-integer-rational-arithmetic-test.cpp
    test/mp-integer-rational-conversion-test.cpp
    test/mp-integer-sign-agnostic-test.cpp
    test/mp-integer-signed-arithmetic-test.cpp
    test/mp-integer-signed-conversion-test.cpp
    test/mp-integer-unsigned-arithmetic-test.cpp
    test/mp-integer-unsigned-conversion-test.cpp
    test/multi-array-test.cpp
    test/name-test.cpp
    test/named-mutex-test.cpp
    test/newtype-test.cpp
    test/noise-test.cpp
    test/options-anonymous-test.cpp
    test/options-boolean-test.cpp
    test/options-container-test.cpp
    test/options-enumeration-test.cpp
    test/options-help-test.cpp
    test/options-mutual-exclusion-test.cpp
    test/options-parsing-test.cpp
    test/options-path-test.cpp
    test/options-pattern-match-test.cpp
    test/options-required-test.cpp
    test/options-traits-test.cpp
    test/options-types-test.cpp
    test/path-directory-test.cpp
    test/path-file-system-test.cpp
    test/path-name-test.cpp
    test/prime-fixed-binary-test.cpp
    test/prime-int-factors-test.cpp
    test/prime-int-next-prev-test.cpp
    test/prime-int-test.cpp
    test/prime-mp-integer-test.cpp
    test/probability-arithmetic-test.cpp
    test/probability-special-functions-test.cpp
    test/probability-values-test.cpp
    test/progress-test.cpp
    test/projection-test.cpp
    test/quaternion-test.cpp
    test/random-adapter-test.cpp
    test/random-algorithm-test.cpp
    test/random-concept-test.cpp
    test/random-continuous-test.cpp
    test/random-discrete-test.cpp
    test/random-lcg-test.cpp
    test/random-non-arithmetic-test.cpp
    test/random-pcg-test.cpp
    test/random-seed-test.cpp
    test/random-spatial-test.cpp
    test/random-squirrel-test.cpp
    test/random-xoshiro-test.cpp
    test/rational-test.cpp
    test/regex-basic-test.cpp
    test/regex-compile-flags-test.cpp
    test/regex-grep-test.cpp
    test/regex-match-test.cpp
    test/regex-replace-test.cpp
    test/regex-runtime-flags-test.cpp
    test/resource-test.cpp
    test/root-finding-test.cpp
    test/spatial-index-test.cpp
    test/spectrum-formatting-test.cpp
    test/spectrum-parsing-test.cpp
    test/spectrum-property-test.cpp
    test/sqlite-test.cpp
    test/stable-map-multimap-test.cpp
    test/stable-map-unique-test.cpp
    test/stack-test.cpp
    test/statistics-test.cpp
    test/stdio-test.cpp
    test/string-casing-test.cpp
    test/string-character-test.cpp
    test/string-formatting-test.cpp
    test/string-literals-test.cpp
    test/string-manipulation-test.cpp
    test/string-parsing-test.cpp
    test/string-query-test.cpp
    test/string-type-test.cpp
    test/string-view-test.cpp
    test/string-wrapping-test.cpp
    test/superposition-test.cpp
    test/table-test.cpp
    test/terminal-test.cpp
    test/text-gen-test.cpp
    test/thread-pool-test.cpp
    test/thread-test.cpp
    test/time-test.cpp
    test/topological-order-test.cpp
    test/transform-test.cpp
    test/unicode-normalization-test.cpp
    test/unicode-test.cpp
    test/uri-general-test.cpp
    test/uri-schemes-test.cpp
    test/uuid-test.cpp
    test/vector-test.cpp
    test/web-client-test.cpp
    test/xml-construction-test.cpp
    test/xml-functions-test.cpp
    test/xml-options-test.cpp
    test/xml-parsing-test.cpp
    test/xml-search-test.cpp

)

target_link_libraries(${unittest}
    PRIVATE ${library}
    PRIVATE pcre2-8
    PRIVATE sqlite3
    PRIVATE CURL::libcurl
    PRIVATE Threads::Threads
)

if("${CMAKE_SYSTEM}" MATCHES "Linux")
    target_link_libraries(${unittest}
        PRIVATE crypto
    )
endif()

if(WIN32)
    target_link_libraries(${unittest}
        PRIVATE ws2_32
    )
endif()

install(DIRECTORY ${library} DESTINATION include PATTERN "*.cpp" EXCLUDE)
install(TARGETS ${library} LIBRARY DESTINATION lib)
install(FILES ../bin/rs-bump-version DESTINATION bin ${executable})
install(FILES ../bin/rs-update-tests DESTINATION bin ${executable})
