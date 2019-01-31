#pragma once

#include "decoder.hpp"
#include <eosiolib/asset.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/time.hpp>
#include <string>
#include <vector>


namespace eosio {

using std::string;
using std::vector;

using transaction_id_type = capi_checksum256;

constexpr uint32_t ONE_DAY = 24 * 60 * 60;

enum withdraw_state : uint64_t {
    INITIAL_STATE = 0,
    FEED_BACK = 2,
    SEND_BACK = 3,
    ROLL_BACK = 5,
};

class[[eosio::contract("bos.pegtoken")]] pegtoken : public contract
{
public:
    using contract::contract;

    [[eosio::action]] void create( symbol sym, name issuer, name address_style, uint64_t peg );
    void create_v1( symbol sym, name issuer, name acceptor, name address_style, string organization, string website );

    [[eosio::action]] void update( symbol_code sym_code, string organization, string website );
    void update_v1( symbol_code sym_code, string organization, string website );

    [[eosio::action]] void setlimit( asset max_limit, asset min_limit, asset total_limit, uint64_t frequency_limit, uint64_t interval_limit );
    void setlimit_v1( asset max_limit, asset min_limit, asset total_limit, uint64_t frequency_limit, uint64_t interval_limit );

    // FIXME: setauditor is removed
    // [[eosio::action]] void setauditor( symbol_code sym_code, string action, name auditor );
    void setauditor_v1( symbol_code sym_code, string action, name auditor );

    [[eosio::action]] void setissuer( symbol_code sym_code, name issuer );
    void setissuer_v1( symbol_code sym_code, name issuer );

    [[eosio::action]] void setfee( double service_fee_rate, asset min_service_fee, asset miner_fee );
    void setfee_v1( double service_fee_rate, asset min_service_fee, asset miner_fee );

    [[eosio::action]] void issue( asset quantity, string memo );
    void issue_v1( asset quantity, string memo );

    [[eosio::action]] void retire( asset quantity, string memo );
    void retire_v1( asset quantity, string memo );

    [[eosio::action]] void applyaddr( symbol_code sym_code, name to );
    void applyaddr_v1( symbol_code sym_code, name to );

    [[eosio::action]] void assignaddr( symbol_code sym_code, name to, string address );
    void assignaddr_v1( symbol_code sym_code, name to, string address );

    [[eosio::action]] void withdraw( name from, string to, asset quantity, string memo );
    void withdraw_v1( name from, string to, asset quantity, string memo );

    [[eosio::action]] void deposit( name to, asset quantity, string memo );
    void deposit_v1( name to, asset quantity, string memo );

    [[eosio::action]] void transfer( name from, name to, asset quantity, string memo );
    void transfer_v1( name from, name to, asset quantity, string memo );

    [[eosio::action]] void clear( symbol_code sym_code, uint64_t num );
    void clear_v1( symbol_code sym_code, uint64_t num );

    [[eosio::action]] void feedback( symbol_code sym_code, transaction_id_type trx_id, string remote_trx_id, string memo );
    void feedback_v1( symbol_code sym_code, transaction_id_type trx_id, string remote_trx_id, string memo );

    [[eosio::action]] void rollback( symbol_code sym_code, transaction_id_type trx_id, string memo );
    void rollback_v1( symbol_code sym_code, transaction_id_type trx_id, string memo );

    [[eosio::action]] void setacceptor( symbol_code sym_code, name acceptor );
    void setacceptor_v1( symbol_code sym_code, name acceptor );

    [[eosio::action]] void setdelay( symbol_code sym_code, uint64_t delayday );
    void setdelay_v1( symbol_code sym_code, uint64_t delayday );

    [[eosio::action]] void lockall( symbol_code sym_code, name auditor );
    void lockall_v1( symbol_code sym_code, name auditor );

    [[eosio::action]] void unlockall( symbol_code sym_code, name auditor );
    void unlockall_v1( symbol_code sym_code, name auditor );

    [[eosio::action]] void approve( symbol_code sym_code, name auditor, transaction_id_type trx_id, string memo );
    void approve_v1( symbol_code sym_code, name auditor, transaction_id_type trx_id, string memo );

    [[eosio::action]] void unapprove( symbol_code sym_code, name auditor, transaction_id_type trx_id, string memo );
    void unapprove_v1( symbol_code sym_code, name auditor, transaction_id_type trx_id, string memo );

    [[eosio::action]] void sendback( /*name auditor,*/ transaction_id_type trx_id, name to, asset quantity, string memo );
    void sendback_v1(/* name auditor,*/ transaction_id_type trx_id, name to, asset quantity, string memo );

    [[eosio::action]] void rmwithdraw( uint64_t id, symbol_code sym_code );
    void rmwithdraw_v1( uint64_t id, symbol_code sym_code );

private:
    void verify_address( name style, string address );
    void add_balance( name owner, asset value, name ram_payer );
    void sub_balance( name owner, asset value );
    asset calculate_service_fee( asset sum, double service_fee_rate, asset min_service_fee );

    bool balance_check( symbol_code sym_code, name user );
    bool addr_check( symbol_code sym_code, name user );

    struct [[eosio::table]] symbol_ts {
        symbol sym;

        uint64_t primary_key() const { return sym.code().raw(); }
    };

    struct [[eosio::table]] addr_ts {
        name owner;
        string address;
        uint64_t state;

        time_point_sec assign_time;
        time_point_sec create_time;

        uint64_t primary_key() const { return owner.value; }

        uint64_t by_addr() const { return hash64( address ); }

        uint64_t by_state() const { return state; }
    };

    struct [[eosio::table]] operate_ts {
        uint64_t id;
        name to;
        asset quantity;
        uint64_t type;
        string memo;
        time_point_sec operate_time;

        uint64_t primary_key() const { return id; }
    };

    struct [[eosio::table]] withdraw_ts {
        uint64_t id;
        transaction_id_type trx_id;
        name from;
        string to;
        asset quantity;
        uint64_t state;

        bool enable;
        name auditor;
        string remote_trx_id;
        string msg;

        time_point_sec create_time;
        time_point_sec update_time;

        uint64_t primary_key() const { return id; }

        fixed_bytes< 32 > by_trxid() const { return trxid( trx_id ); }

        uint128_t by_delindex() const
        {
            uint128_t index = ( state == 2 || state == 3 ) ? 1 : 2;
            index <<= 64;
            index += quantity.amount;
            return index;
        }

        uint128_t by_queindex() const
        {
            uint128_t index = enable ? 1 : 0;
            index <<= 32;
            index += state;
            index <<= 64;
            index += id;
            return index;
        }

        static fixed_bytes< 32 > trxid( transaction_id_type trx_id ) { return fixed_bytes< 32 >( trx_id.hash ); }
    };

    struct [[eosio::table]] deposit_ts {
        uint64_t id;
        transaction_id_type trx_id;
        name from;
        string to;
        asset quantity;
        uint64_t state;
        string remote_trx_id;
        string msg;

        time_point_sec create_time;
        time_point_sec update_time;

        uint64_t primary_key() const { return id; }

        uint64_t by_delindex() const { return create_time.utc_seconds; }
    };

    struct [[eosio::table]] statistic_ts {
        name owner;
        time_point_sec last_time;
        uint64_t frequency;
        asset total;
        time_point_sec update_time;

        uint64_t primary_key() const { return owner.value; }
    };

    struct [[eosio::table]] account_ts {
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };

    struct [[eosio::table]] stat_ts {
        asset supply;
        asset max_limit;
        asset min_limit;
        asset total_limit;
        uint64_t frequency_limit;
        uint64_t interval_limit;
        uint64_t delayday;
        name issuer;
        name acceptor;
        name address_style;
        string organization;
        string website;
        double service_fee_rate;
        asset min_service_fee;
        asset miner_fee;
        bool active;

        uint64_t primary_key() const { return supply.symbol.code().raw(); }

        uint64_t by_issuer() const { return issuer.value; }

        uint64_t by_acceptor() const { return acceptor.value; }
    };

    struct [[eosio::table]] auditor_ts {
        name auditor;

        uint64_t primary_key() const { return auditor.value; }
    };

    using symbols = eosio::multi_index< "symbols"_n, symbol_ts >;

    using addrs = eosio::multi_index< "addrs"_n, addr_ts,
        indexed_by< "addr"_n, const_mem_fun< addr_ts, uint64_t, &addr_ts::by_addr > >,
        indexed_by< "state"_n, const_mem_fun< addr_ts, uint64_t, &addr_ts::by_state > > >;

    using operates = eosio::multi_index< "operates"_n, operate_ts >;

    using withdraws = eosio::multi_index< "withdraws"_n, withdraw_ts,
        indexed_by< "trxid"_n, const_mem_fun< withdraw_ts, fixed_bytes< 32 >, &withdraw_ts::by_trxid > >,
        indexed_by< "delindex"_n, const_mem_fun< withdraw_ts, uint128_t, &withdraw_ts::by_delindex > >,
        indexed_by< "queindex"_n, const_mem_fun< withdraw_ts, uint128_t, &withdraw_ts::by_queindex > > >;

    using deposits = eosio::multi_index< "deposits"_n, deposit_ts,
        indexed_by< "delindex"_n, const_mem_fun< deposit_ts, uint64_t, &deposit_ts::by_delindex > > >;

    using statistics = eosio::multi_index< "statistics"_n, statistic_ts >;

    using accounts = eosio::multi_index< "accounts"_n, account_ts >;

    using stats = eosio::multi_index< "stats"_n, stat_ts,
        indexed_by< "issuer"_n, const_mem_fun< stat_ts, uint64_t, &stat_ts::by_issuer > >,
        indexed_by< "acceptor"_n, const_mem_fun< stat_ts, uint64_t, &stat_ts::by_acceptor > > >;

    using auditors = eosio::multi_index< "auditors"_n, auditor_ts >;


    /////////////////////////////////
    // v2
    /////////////////////////////////
    struct [[eosio::table]] edition_ts{
        symbol sym;
        uint64_t edition;

        uint64_t primary_key() const { return sym.code().raw(); }
    };
    using editions = eosio::multi_index< "editions"_n, edition_ts >;

    struct [[eosio::table]] peg_ts{
        symbol sym;
        uint64_t peg;

        uint64_t primary_key() const { return sym.code().raw(); }
    };
    using pegs = eosio::multi_index< "pegs"_n, peg_ts >;

    struct [[eosio::table]] record_ts{
        name owner;
        string address;
        time_point_sec reset_time;

        uint64_t primary_key() const { return owner.value; }
        uint64_t by_addr() const { return hash64(address); }
    };
    using records = eosio::multi_index< "records"_n, record_ts, 
        indexed_by< "addr"_n, const_mem_fun< record_ts, uint64_t, &record_ts::by_addr > > >;    

    struct [[eosio::table]] melt_ts{
        uint64_t id;
        transaction_id_type trx_id;
        name from;
        string to;
        asset total;
        asset amount;
        asset fee;
        uint64_t state;
        string remote_trx_id;
        uint64_t index;
        uint64_t remote_index;
        string msg;
        time_point_sec create_time;
        time_point_sec update_time;

        uint64_t primary_key() const { return id; }
    };
    using melts = eosio::multi_index< "melts"_n, melt_ts >;

    struct [[eosio::table]] cast_ts {
        uint64_t id;
        transaction_id_type trx_id;
        name to_account;
        asset quantity;
        uint64_t state;        
        string remote_trx_id;
        uint64_t index;
        uint64_t remote_index;

        string msg;
        time_point_sec create_time;
        time_point_sec update_time;

        uint64_t primary_key() const { return id; }
    };
    using casts = eosio::multi_index< "casts"_n, cast_ts >;

    struct [[eosio::table]] newwithdraw_ts{
        uint64_t id;
        transaction_id_type trx_id;
        name from;
        string to;
        
        asset total;
        asset amount;
        asset fee;

        uint64_t state;
        bool enable;

        name auditor;
        string remote_trx_id;

        string msg;
        time_point_sec create_time;
        time_point_sec update_time;

        uint64_t primary_key() const { return id; }
    };
    using newwithdraws = eosio::multi_index< "newwithdraws"_n, newwithdraw_ts >;

    struct [[eosio::table]] vip_ts{
        name vip;
        time_point_sec create_time;

        uint64_t primary_key() const { return vip.value; }
    };
    using vips = eosio::multi_index< "vips"_n, vip_ts >;

    struct [[eosio::table]] viplimit_ts {
        name owner;
        asset maximum_limit;
        asset minimum_limit;
        uint64_t frequency_limit;
        uint64_t interval_limit;
        uint64_t reset_limit;
    
        uint64_t primary_key() const { return owner.value; }
    };
    using viplimits = eosio::multi_index< "viplimits"_n, viplimit_ts >;

    struct [[eosio::table]] vipfee_ts {
        name owner;
        double service_fee_rate;
        asset min_service_fee;
        asset miner_fee;

        uint64_t primary_key() const { return owner.value; }
    };
    using vipfees = eosio::multi_index< "vipfees"_n, vipfee_ts >;

    struct [[eosio::table]] info_ts {
        asset supply;
        name issuer;
        name address_style;
        bool active;

        uint64_t primary_key() const { return supply.symbol.code().raw(); }
    };
    using infos = eosio::multi_index< "infos"_n, info_ts >;

    struct [[eosio::table]] delay_ts {
        uint64_t delay;

        uint64_t primary_key() const { return delay; }
    };
    using delays = eosio::multi_index< "delays"_n, delay_ts >;

    struct [[eosio::table]] limit_ts {
        asset maximum_limit;
        asset minimum_limit;
        asset total_limit;
        uint64_t interval_limit;
        uint64_t reset_limit;

        uint64_t primary_key() const { return maximum_limit.symbol.code().raw(); }
    };
    using limits = eosio::multi_index< "limits"_n, limit_ts >;

    struct [[eosio::table]] fee_ts {
        double service_fee_rate;
        asset min_service_fee;
        asset miner_fee;

        uint64_t primary_key() const { return static_cast<uint64_t>(service_fee_rate); }
    };
    using fees = eosio::multi_index< "fees"_n, fee_ts >;

    struct [[eosio::table]] summary_ts {
        string organization;
        string website;

        uint64_t primary_key() const { return 0; }
    };
    using summaries = eosio::multi_index< "summaries"_n, summary_ts >;

    struct bill_pair{
        string addr;
        asset amount;

        EOSLIB_SERIALIZE(bill_pair,(addr)(amount)) 
    };
    struct [[eosio::table]] bill_ts {
        vector<bill_pair> sums;
        asset miner_fee;

        uint64_t primary_key() const { return 0; }
    };
    using bills = eosio::multi_index< "bills"_n, bill_ts >;

    struct [[eosio::table]] acceptor_ts {
        name acceptor;

        uint64_t primary_key() const { return acceptor.value; }
    };
    using acceptors = eosio::multi_index< "acceptors"_n, acceptor_ts >;

    struct [[eosio::table]] toller_ts {
        name toller;

        uint64_t primary_key() const { return toller.value; }
    };
    using tollers = eosio::multi_index< "tollers"_n, toller_ts >;

    struct [[eosio::table]] usertoken_ts {
       uint64_t symbol_code;
       name issuer; 
       name acceptor;
       string address; 

       uint64_t primary_key() const { return symbol_code; }
       uint64_t by_issuer() const { return issuer.value; }
    };
    using usertokens = eosio::multi_index< "usertokens"_n, usertoken_ts >;
};


////////////////////////
// private funcs
////////////////////////
    void pegtoken::verify_address(name style, string addr) {
        if (style == "bitcoin"_n) {
            eosio_assert(valid_bitcoin_addr(addr), "invalid bitcoin addr");
        } else if (style == "ethereum"_n) {
            eosio_assert(valid_ethereum_addr(addr), "invalid ethereum addr");
        } else if (style == "tether"_n) {
            auto _ = name(addr);
        } else if (style == "other"_n) {
            // no check
        } else {
            eosio_assert(false,
                         "only USDT, BTC and ETH supported. address style must be one of bitcoin, ethereum, tether or other");
        }
    }

    void pegtoken::sub_balance(name owner, asset value) {
        auto acct = accounts(get_self(), owner.value);
        auto from = acct.find(value.symbol.code().raw());
        eosio_assert(from != acct.end(), "no balance object found");
        eosio_assert(from->balance.amount >= value.amount, "overdrawn balance");
        if (from->balance.amount == value.amount) {
            acct.erase(from);
        } else {
            acct.modify(from, same_payer, [&](auto &p) {
                p.balance -= value;
            });
        }
    }


    void pegtoken::add_balance(name owner, asset value, name ram_payer) {
        auto acct = accounts(get_self(), owner.value);
        auto to = acct.find(value.symbol.code().raw());
        if (to == acct.end()) {
            acct.emplace(ram_payer, [&](auto &p) {
                p.balance = value;
            });
        } else {
            acct.modify(to, same_payer, [&](auto &p) {
                p.balance += value;
            });
        }
    }

    asset pegtoken::calculate_service_fee(asset sum, double service_fee_rate, asset min_service_fee) {
        asset actual_service_fee = sum * service_fee_rate;

        if (actual_service_fee.amount < min_service_fee.amount) {
            return min_service_fee;
        } else {
            return actual_service_fee;
        }
    }

    bool pegtoken::balance_check(symbol_code sym_code, name user) {
        auto acct = accounts(get_self(), user.value);
        auto balance = acct.find(sym_code.raw());
        return balance == acct.end() || balance->balance.amount == 0;
    }

    bool pegtoken::addr_check(symbol_code sym_code, name user) {
        auto addresses = addrs(get_self(), sym_code.raw());
        return addresses.find(user.value) == addresses.end();
    }





} // namespace eosio
