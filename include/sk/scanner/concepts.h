#ifndef SK_LIB_SCANNER_CONCEPTS_H
#define SK_LIB_SCANNER_CONCEPTS_H

#include "results.h"

#include <optional>
#include <string_view>


namespace sk::scanner::concepts {
    using namespace sk::scanner::types;

    //
    // Use case 1: Retrieve a character and buffer it internally
    // Use case 2: Remove a string of X characters from the internal buffer and return it
    // Use case 3: Drop the internal internal buffer
    //
    template<typename T>
    concept BufferedInput = requires(T a, std::streamsize n) {
        { a.get_char() }
        ->std::is_same<std::optional<char>>;

        { a.get_string_from_buffer(n) }
        ->std::is_same<std::string>;

        { a.buffer_size() }
        ->std::is_same<std::streamsize>;

        { a.putback_buffer() }
        ->std::is_save<void>
    };

}  // namespace sk::scanner::concepts

#endif
