#include "sk/scanner/token_map.h"

#include <cassert>


namespace sk::scanner {

    TokenId token_map::add_token(const std::string &tokenStr) {
        assert(!resolve_token_id(nextTokenId_));
        tokenMap_[nextTokenId_] = tokenStr;
        return nextTokenId_++;
    }

    std::optional<std::string> token_map::resolve_token_id(const TokenId id) const {
        const auto it {tokenMap_.find(id)};
        if (it == std::cend(tokenMap_)) {
            return std::nullopt;
        }
        return it->second;
    }

    size_t token_map::get_token_count() const { return static_cast<size_t>(nextTokenId_); }

}  // namespace sk::scanner
