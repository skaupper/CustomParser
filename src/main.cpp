#include "sk/parser.h"
#include "sk/utils.h"
#include <iostream>

#include <spdlog/spdlog.h>

using namespace sk::parser;
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


void try_parse(const std::string_view s) {
    std::cout << "Try parsing string: '" << s << "'" << std::endl;

    const auto res {parse_arith_expr(s)};

    if (const auto good {std::get_if<0>(&res)}) {
        std::cout << "Tokens: ";
        for (const auto &child : good->children) {
            std::cout << static_cast<int>(child.token) << " ";
        }
        std::cout << std::endl;

        std::cout << "Remaining: " << good->remaining << std::endl;
    } else if (const auto bad {std::get_if<1>(&res)}) {
        std::cerr << "Error: " << bad->msg << std::endl;
    }

    std::cout << std::endl;
}


int main() {

    spdlog::set_level(spdlog::level::info);

    try_parse("1+1"sv);
    try_parse("-1*55"sv);
    try_parse("+0**123"sv);
    try_parse("12345//123"sv);
    try_parse("4678+0asdf"sv);
    try_parse("0-0"sv);
    try_parse("0f-0"sv);

    return 0;
}
