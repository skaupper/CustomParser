// #include "sk/parser.h"

#include "sk/scanner/output_operators.h"
#include "sk/utils/output_operators.h"

#include <iostream>
#include <sstream>

#include <spdlog/spdlog.h>

// using namespace sk::parser;
using namespace std::literals::string_view_literals;


// const auto lambda {[](const auto &e, auto &&self) {
//     if (std::size(e.children) != 0) {
//         recursive_for_each_vector(
//             e.children, [&](auto &&e2) { self(std::forward<decltype(e2)>(e2), std::forward<decltype(self)>(self));
//             });
//     } else {
//         std::cout << e.result << " ";
//     }
// }};

// const auto indirect_lambda {[&](auto &&e) {
//     lambda(std::forward<decltype(e)>(e), lambda);
// }};

// template<typename Parser>
// void try_parse(const std::string_view s, Parser &&parse = parse_arith_expr) {
//     std::cout << "Try parsing string: '" << s << "'" << std::endl;
//     std::vector<sk::parser::types::ParseStackEntry> bt;

//     const auto res {parse(s, bt)};

//     if (const auto good {std::get_if<0>(&res)}) {
//         std::cout << "Tokens: ";
//         for (const auto &child : good->children) {
//             // std::cout << child.token << " ";
//             std::cout << "'" << child.result << "' ";
//         }
//         std::cout << std::endl;

//         std::cout << "Remaining: " << good->remaining << std::endl;
//     } else if (const auto bad {std::get_if<1>(&res)}) {
//         std::cerr << "Error: " << bad->msg << std::endl;
//     }

//     std::cout << std::endl;
// }


#include "sk/scanner/defaults.h"
#include "sk/scanner/scanner.h"
// #include "sk/scanner/types.h"

using namespace sk::scanner::types;
using namespace sk::scanner;


class op_matcher final {
    enum class State { INIT, OP, ERROR } state;

public:
    ScanToken get_token() const { return static_cast<ScanToken>(-1); }

    StepResult step(int c) {

        switch (state) {
        case State::INIT:
            if (c == '+' || c == '-' || c == '*' || c == '/') {
                spdlog::debug("op_matcher::step: OP, {}", c);
                state = State::OP;
                return StepResult::MATCH;
            }
            break;

        case State::OP:
        case State::ERROR:
            break;
        }

        spdlog::debug("op_matcher::step: ERROR, {}", c);
        state = State::ERROR;
        return StepResult::ERROR;
    }

    void reset() { state = State::INIT; }
};


int main() {

    spdlog::set_level(spdlog::level::debug);


    scanner<op_matcher, default_matcher_t> sc;

    const auto result {sc.scan(std::cin)};
    if (const auto good {std::get_if<GoodScanResult>(&result)}) {
        for (const auto &s : *good) {
            std::cout << "'" << s.str << "'" << std::endl;
            // std::cout << static_cast<int>(s.token) << " ";
        }

        if (std::size(*good) == 0) {
            std::cout << "No symbols found!" << std::endl;
        } else {
            std::cout << std::endl;
        }
    } else {
        std::cout << "Scanning failed" << std::endl;
    }

    // char c;
    // std::cin.get(c);
    // std::cout << c << std::endl;
    // std::cin.unget();
    // std::cin.get(c);
    // std::cout << c << std::endl;


    // for (int i = 0; i < 10; ++i) {
    //     std::cout << i << std::endl;
    //     input.unget();
    //     if (!input) {
    //         std::cout << "invalid stream state" << std::endl;
    //         return 1;
    //     }
    // }

    // std::string s;
    // if (!std::getline(input, s)) {
    //     std::cout << "invalid stream state" << std::endl;
    // }
    // std::cout << s << std::endl;

    // std::cout << std::get<0>(scanner(p)).str << std::endl;

    // TODO: This should produce 23 tokens (one for each digit)
    // but there are only 3, since the tree still has a
    // separate node for the `repeated` in it, which should be flattened out
    // try_parse("1234567891 12345", parse_integer);
    // try_parse("1+1", parse_arith_expr);
    // try_parse("1324* asdfw", parse_arith_expr);

    // try_parse("1+1"sv);
    // try_parse("-1*55"sv);
    // try_parse("+0**123"sv);
    // try_parse("12345//123"sv);
    // try_parse("4678+0asdf"sv);
    // try_parse("0-0"sv);
    // try_parse("0f-0"sv);

    return 0;
}
