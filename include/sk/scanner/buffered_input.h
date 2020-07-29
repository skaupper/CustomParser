#ifndef SK_LIB_SCANNER_BUFFERED_INPUT_H
#define SK_LIB_SCANNER_BUFFERED_INPUT_H

#include <optional>
#include <sstream>

#include <spdlog/spdlog.h>


namespace sk::scanner::types {
    namespace detail {

        class string_stream final {
            std::stringstream ss_;
            std::streamsize size_ {0};

        public:
            std::streamsize size() const;

            std::optional<std::string> get_string(std::streamsize amount = -1);
            std::optional<char> get_char();

            void put_char(char c);
            void put_string(const std::string &s);

            void flush();
        };

    }  // namespace detail


    class buffered_input final {
    private:
        std::istream &input_;

        detail::string_stream ss1_, ss2_;
        detail::string_stream *alreadyReadBuffer_;
        detail::string_stream *putbackBuffer_;

    public:
        explicit buffered_input(std::istream &in) : input_(in), alreadyReadBuffer_(&ss1_), putbackBuffer_(&ss2_) {}
        buffered_input(const buffered_input &) = delete;
        buffered_input &operator=(const buffered_input &) = delete;


        std::streamsize buffer_size() const;

        std::optional<char> get_char();
        std::optional<std::string> remove_string_from_buffer(std::streamsize amount = -1);

        void putback_buffer();
    };

}  // namespace sk::scanner::types

#endif
