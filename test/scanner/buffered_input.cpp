#include "sk/scanner/buffered_input.h"
#include "sk/utils.h"

#include <catch2/catch.hpp>

#include <array>
#include <string_view>

using namespace std::literals::string_view_literals;
using namespace sk::scanner::types;


TEST_CASE("Reading from source stream", "[scanner][scanner.buffered_input]") {
    static const std::string TEST_STRING {"1234567890 ABCDEF\n!\"§$&/('"};
    std::stringstream source(TEST_STRING);
    buffered_input stream(source);


    REQUIRE(stream.buffer_size() == 0);
    for (size_t i {0}; i < std::size(TEST_STRING); ++i) {
        CAPTURE(i);
        REQUIRE(stream.get_char() == TEST_STRING[i]);
        REQUIRE(stream.buffer_size() == i + 1);
    }
    REQUIRE(stream.buffer_size() == std::size(TEST_STRING));

    REQUIRE(stream.get_char() == std::nullopt);
}


TEST_CASE("Check internal buffer", "[scanner][scanner.buffered_input]") {
    static const std::string TEST_STRING {"1234567890 ABCDEF\n!\"§$&/('"};
    static const std::array END_INDICES {
        std::size(TEST_STRING) / 2,               //
        std::size(TEST_STRING) / 2,               //
        size_t {std::size(TEST_STRING) / 2 + 1},  //
        std::size(TEST_STRING) / 3 * 2,           //
        std::size(TEST_STRING)                    //
    };

    std::stringstream source(TEST_STRING);
    buffered_input stream(source);

    // Read the whole string
    while (stream.get_char()) {
    }
    REQUIRE(stream.buffer_size() == std::size(TEST_STRING));


    for (size_t i {0}; i < std::size(END_INDICES); ++i) {
        const size_t startIndex {std::size(TEST_STRING) - static_cast<size_t>(stream.buffer_size())};
        const size_t endIndex {END_INDICES[i]};
        const size_t len {endIndex - startIndex};
        const size_t bufferSizeBefore {static_cast<size_t>(stream.buffer_size())};


        CAPTURE(startIndex, endIndex, len, bufferSizeBefore);
        REQUIRE(stream.remove_string_from_buffer(static_cast<std::streamsize>(len))
                == TEST_STRING.substr(startIndex, len));
        REQUIRE(stream.buffer_size() == bufferSizeBefore - len);
    }
}


TEST_CASE("Check buffer putback", "[scanner][scanner.buffered_input]") {
    static const std::string TEST_STRING {"1234567890 ABCDEF\n!\"§$&/('"};

    std::stringstream source(TEST_STRING);
    buffered_input stream(source);


    SECTION("Read the whole string") {
        while (stream.get_char()) {
        }
        REQUIRE(stream.buffer_size() == std::size(TEST_STRING));


        SECTION("Putback everything") {
            stream.putback_buffer();

            REQUIRE(stream.buffer_size() == 0);
            for (size_t i {0}; i < std::size(TEST_STRING); ++i) {
                CAPTURE(i);
                REQUIRE(stream.get_char() == TEST_STRING[i]);
                REQUIRE(stream.buffer_size() == i + 1);
            }
            REQUIRE(stream.buffer_size() == std::size(TEST_STRING));
        }

        SECTION("Putback half of the string") {
            const size_t len {std::size(TEST_STRING) / 2};

            REQUIRE(stream.remove_string_from_buffer(static_cast<std::streamsize>(len)) == TEST_STRING.substr(0, len));

            stream.putback_buffer();

            REQUIRE(stream.buffer_size() == 0);
            for (size_t i {len}; i < std::size(TEST_STRING); ++i) {
                CAPTURE(i);
                REQUIRE(stream.get_char() == TEST_STRING[i]);
                REQUIRE(stream.buffer_size() == (i + 1) - len);
            }
            REQUIRE(stream.buffer_size() == std::size(TEST_STRING) - len);
        }
    }

    SECTION("Read half of the string") {
        const size_t len {std::size(TEST_STRING) / 2};
        while (stream.get_char() && stream.buffer_size() < static_cast<std::streamsize>(len)) {
        }
        REQUIRE(stream.buffer_size() == len);


        SECTION("Putback half of the string") {
            const size_t putbackLen {std::size(TEST_STRING) / 2};

            REQUIRE(stream.remove_string_from_buffer(static_cast<std::streamsize>(putbackLen))
                    == TEST_STRING.substr(0, putbackLen));

            stream.putback_buffer();

            REQUIRE(stream.buffer_size() == len - putbackLen);
            for (size_t i {putbackLen}; i < len; ++i) {
                CAPTURE(i);
                REQUIRE(stream.get_char() == TEST_STRING[i]);
                REQUIRE(stream.buffer_size() == (i + 1) - putbackLen);
            }
            REQUIRE(stream.buffer_size() == len - putbackLen);
        }
    }

    SECTION("Putback nothing") {
        REQUIRE(stream.buffer_size() == 0);

        stream.putback_buffer();

        REQUIRE(stream.buffer_size() == 0);
        for (size_t i {0}; i < std::size(TEST_STRING); ++i) {
            CAPTURE(i);
            REQUIRE(stream.get_char() == TEST_STRING[i]);
            REQUIRE(stream.buffer_size() == i + 1);
        }
        REQUIRE(stream.buffer_size() == std::size(TEST_STRING));
    }
}
