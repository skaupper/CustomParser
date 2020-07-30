#ifndef SK_LIB_SCANNER_TOKEN_MAP_H
#define SK_LIB_SCANNER_TOKEN_MAP_H

#include "types.h"

#include <map>
#include <optional>
#include <string>


namespace sk::scanner {
    using namespace types;

    class token_map {
        TokenId nextTokenId_ {1};
        std::map<TokenId, std::string> tokenMap_ {{0, "INVALID_TOKEN"}};

    public:
        TokenId add_token(const std::string &);
        std::optional<std::string> resolve_token_id(const TokenId) const;
        size_t get_token_count() const;
    };

}  // namespace sk::scanner


#endif
