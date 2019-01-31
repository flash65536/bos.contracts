#pragma once

#define STRING_LEN_CHECK(str, len) \
    eosio_assert( ( str ).size() <= len, "param " #str " too long, maximum length is " #len );

#define ACCOUNT_CHECK(account) \
    eosio_assert( is_account( account ), "invalid account " #account );

#define NIL_ACCOUNT "nil"_n
