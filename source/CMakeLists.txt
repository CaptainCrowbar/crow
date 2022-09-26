cmake_minimum_required(VERSION 3.16)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# The version number here is the canonical one for the project.
# Run rs-bump-version to increment the patch number
# and propagate the new version to version.hpp.

project(Crow
    VERSION 2.0.16
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
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
find_package(Threads REQUIRED)

set(executable PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

add_library(${library} STATIC

    ${library}/channel.cpp
    ${library}/colour.cpp
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
    ${library}/image.cpp
    ${library}/interval.cpp
    ${library}/markup.cpp
    ${library}/mp-integer.cpp
    ${library}/named-mutex.cpp
    ${library}/net.cpp
    ${library}/options.cpp
    ${library}/path.cpp
    ${library}/process.cpp
    ${library}/progress.cpp
    ${library}/rational.cpp
    ${library}/regex.cpp
    ${library}/signal.cpp
    ${library}/stdio.cpp
    ${library}/string.cpp
    ${library}/table.cpp
    ${library}/terminal.cpp
    ${library}/text-gen.cpp
    ${library}/thread-pool.cpp
    ${library}/time.cpp
    ${library}/unicode.cpp
    ${library}/uri.cpp
    ${library}/uuid.cpp

)

add_executable(${unittest}

    test/log-test.cpp
    test/types-test.cpp

    test/algorithm-test.cpp
    test/binary-test.cpp
    test/bounded-array-construction-test.cpp
    test/bounded-array-insertion-test.cpp
    test/bounded-array-misc-test.cpp
    test/bounded-array-tracking-test.cpp
    test/channel-classes-test.cpp
    test/channel-dispatch-test.cpp
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
    test/enum-test.cpp
    test/fixed-binary-large-binary-100-test.cpp
    test/fixed-binary-large-binary-35-test.cpp
    test/fixed-binary-misc-test.cpp
    test/fixed-binary-small-binary-35-test.cpp
    test/fixed-binary-small-binary-5-test.cpp
    test/font-test.cpp
    test/format-floating-test.cpp
    test/format-integer-test.cpp
    test/format-null-test.cpp
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
    test/interval-continuous-arithmetic-test.cpp
    test/interval-continuous-basic-test.cpp
    test/interval-continuous-boundary-addition-test.cpp
    test/interval-continuous-boundary-basic-test.cpp
    test/interval-continuous-boundary-comparison-test.cpp
    test/interval-continuous-boundary-multiplication-test.cpp
    test/interval-continuous-map-test.cpp
    test/interval-continuous-set-test.cpp
    test/interval-integral-arithmetic-test.cpp
    test/interval-integral-basic-test.cpp
    test/interval-integral-boundary-addition-test.cpp
    test/interval-integral-boundary-basic-test.cpp
    test/interval-integral-boundary-comparison-test.cpp
    test/interval-integral-boundary-multiplication-test.cpp
    test/interval-integral-map-test.cpp
    test/interval-integral-set-test.cpp
    test/interval-ordered-basic-test.cpp
    test/interval-ordered-boundary-basic-test.cpp
    test/interval-ordered-boundary-comparison-test.cpp
    test/interval-ordered-map-test.cpp
    test/interval-ordered-set-test.cpp
    test/interval-stepwise-basic-test.cpp
    test/interval-stepwise-boundary-basic-test.cpp
    test/interval-stepwise-boundary-comparison-test.cpp
    test/interval-stepwise-map-test.cpp
    test/interval-stepwise-set-test.cpp
    test/interval-types-test.cpp
    test/iterator-test.cpp
    test/linear-map-test.cpp
    test/markov-test.cpp
    test/markup-test.cpp
    test/maths-test.cpp
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
    test/named-mutex-test.cpp
    test/net-address-test.cpp
    test/net-dns-test.cpp
    test/net-tcp-test.cpp
    test/noise-test.cpp
    test/options-anonymous-test.cpp
    test/options-boolean-test.cpp
    test/options-container-test.cpp
    test/options-enumeration-test.cpp
    test/options-help-test.cpp
    test/options-mutual-exclusion-test.cpp
    test/options-parsing-test.cpp
    test/options-pattern-match-test.cpp
    test/options-required-test.cpp
    test/options-traits-test.cpp
    test/path-directory-test.cpp
    test/path-file-system-test.cpp
    test/path-name-test.cpp
    test/prime-fixed-binary-test.cpp
    test/prime-int-factors-test.cpp
    test/prime-int-next-prev-test.cpp
    test/prime-int-test.cpp
    test/prime-mp-integer-test.cpp
    test/process-test.cpp
    test/progress-test.cpp
    test/projection-test.cpp
    test/quaternion-test.cpp
    test/random-adapter-test.cpp
    test/random-continuous-test.cpp
    test/random-discrete-test.cpp
    test/random-lcg-test.cpp
    test/random-non-arithmetic-test.cpp
    test/random-pcg-test.cpp
    test/random-spatial-test.cpp
    test/random-xoshiro-test.cpp
    test/rational-test.cpp
    test/regex-basic-test.cpp
    test/regex-compile-flags-test.cpp
    test/regex-grep-test.cpp
    test/regex-match-test.cpp
    test/regex-replace-test.cpp
    test/regex-runtime-flags-test.cpp
    test/root-finding-test.cpp
    test/signal-test.cpp
    test/stack-test.cpp
    test/statistics-test.cpp
    test/stdio-test.cpp
    test/string-casing-test.cpp
    test/string-character-test.cpp
    test/string-formatting-test.cpp
    test/string-literals-test.cpp
    test/string-manipulation-test.cpp
    test/string-parsing-test.cpp
    test/string-type-test.cpp
    test/string-wrapping-test.cpp
    test/table-test.cpp
    test/terminal-test.cpp
    test/text-gen-test.cpp
    test/thread-pool-test.cpp
    test/thread-test.cpp
    test/time-test.cpp
    test/topological-order-test.cpp
    test/transform-test.cpp
    test/unicode-test.cpp
    test/uri-general-test.cpp
    test/uri-schemes-test.cpp
    test/uuid-test.cpp
    test/vector-test.cpp

    test/version-test.cpp
    test/unit-test.cpp

)

target_link_libraries(${unittest}
    PRIVATE ${library}
    PRIVATE pcre2-8
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

install(FILES ${library}.hpp DESTINATION include)
install(DIRECTORY ${library} DESTINATION include PATTERN "*.cpp" EXCLUDE)
install(TARGETS ${library} LIBRARY DESTINATION lib)
install(FILES ../bin/rs-bump-version DESTINATION bin ${executable})
install(FILES ../bin/rs-update-tests DESTINATION bin ${executable})
