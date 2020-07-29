#include "sk/scanner/buffered_input.h"


namespace sk::scanner::types {

    namespace detail {
        std::streamsize string_stream::size() const { return size_; }


        std::optional<std::string> string_stream::get_string(std::streamsize amount) {
            if (amount == -1) {
                amount = size();
            }

            if (size() < amount) {
                spdlog::error("buffered_input::string_stream::get_n_chars: More characters requested than the stream "
                              "has in it.");
                return std::nullopt;
            }

            std::string res;
            res.resize(static_cast<size_t>(amount));

            if (!ss_.read(std::data(res), amount)) {
                flush();
                spdlog::error("buffered_input::string_stream::get_n_chars: Unable to receive character string stream.");
                return std::nullopt;
            }

            spdlog::debug("buffered_input::string_stream::get_n_chars: '{}'", res);

            size_ -= amount;
            return res;
        }

        std::optional<char> string_stream::get_char() {
            if (size() == 0) {
                spdlog::error("buffered_input::string_stream::get_char: Unable to receive a character from the "
                              "string stream since its size == 0.");
                return std::nullopt;
            }

            char c;
            if (!ss_.get(c)) {
                flush();
                spdlog::error("buffered_input::string_stream::get_char: Unable to receive character string stream "
                              "although its size != 0.");
                return std::nullopt;
            }

            --size_;
            return c;
        }


        void string_stream::put_char(char c) {
            ss_ << c;
            ++size_;
        }

        void string_stream::put_string(const std::string &s) {
            ss_ << s;
            size_ += static_cast<std::streamsize>(std::size(s));
        }


        void string_stream::flush() {
            size_ = 0;
            ss_   = std::stringstream {};
        }

    }  // namespace detail


    // -----------------------------------------------------------------------------------------------------------------


    std::streamsize buffered_input::buffer_size() const { return alreadyReadBuffer_->size(); }


    std::optional<char> buffered_input::get_char() {
        if (putbackBuffer_->size() > 0) {
            auto optChar {putbackBuffer_->get_char()};
            if (optChar) {
                alreadyReadBuffer_->put_char(*optChar);
            } else {
                spdlog::error("buffered_input::get_char: putbackBuffer_ did not return a character although its "
                              "size is != 0.");
            }
            return optChar;
        }

        char c;
        if (!input_.get(c)) {
            if (input_.eof()) {
                spdlog::debug("buffered_input::get_char: EOF");
            } else {
                spdlog::error("buffered_input::get_char: Unable to receive character from input stream.");
            }
            return std::nullopt;
        }

        alreadyReadBuffer_->put_char(c);
        return c;
    }

    std::optional<std::string> buffered_input::remove_string_from_buffer(std::streamsize amount) {
        return alreadyReadBuffer_->get_string(amount);
    }


    void buffered_input::putback_buffer() {
        if (const auto s {putbackBuffer_->get_string()}) {
            alreadyReadBuffer_->put_string(*s);
        } else {
            putbackBuffer_->flush();
            spdlog::error("buffered_input::flush: Could not receive string of putbackBuffer_. "
                          "Some data may be lost.");
        }

        std::swap(alreadyReadBuffer_, putbackBuffer_);
    }

}  // namespace sk::scanner::types
