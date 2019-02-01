#include <bos.pegtoken/bos.pegtoken.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/action.hpp>
#include "def.cpp"

namespace eosio {
    void pegtoken::create_v2( symbol sym, name issuer, name address_style){
        require_auth(get_self());

        ACCOUNT_CHECK(issuer);
        eosio_assert(sym.is_valid(), "invalid symbol");

        eosio_assert(address_style == "bitcoin"_n || address_style == "ethereum"_n || address_style == "tether"_n ||
                     address_style == "other"_n,
                     "address_style must be one of bitcoin, ethereum, tether or other");

        auto info_table = infos(get_self(),sym.code().raw());
        eosio_assert(info_table.find(sym.code().raw()) == info_table.end(), "token with symbol already exists (info)");

        info_table.emplace(get_self(),[&](auto &p) {
            p.supply = eosio::asset(0,sym);
            p.issuer = issuer;
            p.address_style = address_style;
            p.active = true; 
        });

        auto summary_table = summaries(get_self(),sym.code().raw());
        eosio_assert(summary_table.find(sym.code().raw()) == summary_table.end(), "token with symbol already exists (summary)");
        summary_table.emplace(get_self(),[&](auto &p){
            /* do nothing */
        });

        auto limit_table = limits(get_self(),sym.code().raw());
        eosio_assert(limit_table.find(sym.code().raw()) == limit_table.end(), "token with symbol already exists (limit)");
        limit_table.emplace(get_self(),[&](auto &p){
            p.maximum_limit = eosio::asset(0,sym);
            p.minimum_limit = eosio::asset(0,sym);
            p.total_limit = eosio::asset(0,sym);

            p.frequency_limit = 0;
            p.interval_limit = 300;
            p.reset_limit = 30 * ONE_DAY;
        });
    }

    void pegtoken::setissuer_v2( symbol_code sym_code, name issuer ) {
        require_auth(get_self());
        ACCOUNT_CHECK(issuer);

        auto sym_raw = sym_code.raw();
        auto info_table = infos(get_self(), sym_raw);
        auto iter = info_table.find(sym_raw);
        eosio_assert(iter != info_table.end(), "token not exist");
        
        info_table.modify(iter,same_payer,[&](auto & p) {
            p.issuer = issuer;
        });
    }

    void pegtoken::update_v2( symbol_code sym_code, string organization, string website ){
        STRING_LEN_CHECK(organization, 256)
        STRING_LEN_CHECK(website, 256)

        auto info_table = infos(get_self(),sym_code.raw());
        auto val = info_table.get(sym_code.raw(), "token with symbol already exists");
        require_auth(val.issuer);

        auto summary_table = summaries(get_self(),sym_code.raw());
        auto iter = summary_table.begin();
        eosio_assert( iter != summary_table.end(), "token with symbol not exists");
        summary_table.modify(iter,same_payer,[&](auto &p){
            p.organization = organization;
            p.website = website;
        });
    }

    void pegtoken::setlimit_v2( asset max_limit, asset min_limit, asset total_limit, uint64_t frequency_limit, uint64_t interval_limit ) {
        eosio_assert(min_limit.amount >= 0 && max_limit >= min_limit && total_limit >= max_limit,
             "constrict mismatch: total_limit >= max_limit >= min_limit >= 0");

        auto sym_code = max_limit.symbol.code();
        auto info_table = infos(get_self(),sym_code.raw());
        auto val = info_table.get(sym_code.raw(), "token with symbol not exists(info)");
        require_auth(val.issuer);

        auto limit_table = limits(get_self(),sym_code.raw());
        auto iter = limit_table.find(sym_code.raw());
        eosio_assert( iter != limit_table.end(), "token with symbol not exists(limit)");
        limit_table.modify(iter,same_payer,[&](auto &p){
            p.maximum_limit = max_limit;
            p.minimum_limit = min_limit;
            p.total_limit = total_limit;

            p.frequency_limit = frequency_limit;
            p.interval_limit = interval_limit;
        });
    }

    void pegtoken::setmaxlimit_v2( asset max_limit ){
        auto sym_code = max_limit.symbol.code();
        auto limit_table = limits(get_self(),sym_code.raw());
        auto val = limit_table.get(sym_code.raw(),"token with symbol not exists");
        
        setlimit_v2( max_limit,val.minimum_limit,val.total_limit,val.frequency_limit,val.interval_limit );
    }

    void pegtoken::setminlimit_v2( asset min_limit ){
        auto sym_code = min_limit.symbol.code();
        auto limit_table = limits(get_self(),sym_code.raw());
        auto val = limit_table.get(sym_code.raw(),"token with symbol not exists");
        
        setlimit_v2( val.maximum_limit,min_limit,val.total_limit,val.frequency_limit,val.interval_limit );
    }

    void pegtoken::settotlimit_v2( asset total_limit ){
        auto sym_code = total_limit.symbol.code();
        auto limit_table = limits(get_self(),sym_code.raw());
        auto val = limit_table.get(sym_code.raw(),"token with symbol not exists");
        
        setlimit_v2( val.maximum_limit,val.minimum_limit,total_limit,val.frequency_limit,val.interval_limit );
    }

    void pegtoken::setfrelimit_v2( symbol_code sym_code, uint64_t frequency_limit) {
        auto limit_table = limits(get_self(),sym_code.raw());
        auto val = limit_table.get(sym_code.raw(),"token with symbol not exists");
        
        setlimit_v2( val.maximum_limit,val.minimum_limit,val.total_limit,frequency_limit,val.interval_limit );
    }

    void pegtoken::setintlimit_v2( symbol_code sym_code, uint64_t interval_limit) {
        auto limit_table = limits(get_self(),sym_code.raw());
        auto val = limit_table.get(sym_code.raw(),"token with symbol not exists");
        
        setlimit_v2( val.maximum_limit,val.minimum_limit,val.total_limit,val.frequency_limit, interval_limit );
    }

    void pegtoken::setreslimit_v2( symbol_code sym_code, uint64_t reset_limit) {
        auto info_table = infos(get_self(),sym_code.raw());
        auto val = info_table.get(sym_code.raw(), "token with symbol not exists(info)");
        require_auth(val.issuer);

        auto limit_table = limits(get_self(),sym_code.raw());
        auto iter = limit_table.find(sym_code.raw());
        eosio_assert( iter != limit_table.end(), "token with symbol not exists(limit)");
        limit_table.modify(iter,same_payer,[&](auto &p){
            p.reset_limit = reset_limit;
        });
    }

    void pegtoken::setviplimit_v2(name vip, asset maximum_limit, asset minimum_limit ,asset total_limit,uint64_t frequency_limit, uint64_t interval_limit,uint64_t reset_limit){
        eosio_assert( maximum_limit >= minimum_limit && total_limit >= maximum_limit && minimum_limit.amount >=0, "mismatch: total >= max >= min >= 0");

        auto sym_raw = maximum_limit.symbol.code().raw();
        
        auto info_table = infos(get_self(),sym_raw);
        auto val = info_table.get(sym_raw, "token with symbol not exists(info)");
        require_auth(val.issuer);

        auto vip_table = vips(get_self(),sym_raw);
        auto viplimit_table = viplimits(get_self(),sym_raw);
        auto vipfee_table = vipfees(get_self(),sym_raw);
        
        auto iter_vip = vip_table.find(vip.value);
        if(iter_vip == vip_table.end()){
            vip_table.emplace(get_self(),[&](auto &p){
                p.vip = vip;
                p.create_time = time_point_sec(now());
            });
            viplimit_table.emplace(get_self(),[&](auto &p){
                p.owner = vip;
                p.maximum_limit = maximum_limit;
                p.minimum_limit = minimum_limit;
                p.total_limit = total_limit;
                p.frequency_limit = frequency_limit;
                p.interval_limit = interval_limit;
                p.reset_limit = reset_limit;
            });
            vipfee_table.emplace(get_self(),[&](auto &p){
                p.owner = vip;
                p.service_fee_rate = 0;
                p.min_service_fee = eosio::asset(0,maximum_limit.symbol);
                p.miner_fee = eosio::asset(0,maximum_limit.symbol);
            });
        } else {
            viplimit_table.modify(
                viplimit_table.find(vip.value),
                    same_payer,[&](auto &p){
                p.owner = vip;
                p.maximum_limit = maximum_limit;
                p.minimum_limit = minimum_limit;
                p.total_limit = total_limit;
                p.frequency_limit = frequency_limit;
                p.interval_limit = interval_limit;
                p.reset_limit = reset_limit;
            });
        }
    }

    void pegtoken::setvipmaxlim_v2(name vip, asset maximum_limit ) {
        auto sym_raw = maximum_limit.symbol.code().raw();
        auto viplimit_table = viplimits(get_self(),sym_raw);
        auto iter = viplimit_table.find(vip.value);
        if (iter == viplimit_table.end() ){
            auto zero_asset = eosio::asset(0,maximum_limit.symbol);
            setviplimit_v2(vip,maximum_limit,zero_asset,zero_asset,0,0,0);
        } else {
            setviplimit_v2(vip,maximum_limit,iter->minimum_limit,iter->total_limit,
            iter->frequency_limit,iter->interval_limit,iter->reset_limit);
        }
    }

    void pegtoken::setvipminlim_v2(name vip, asset minimum_limit ) {
        auto sym_raw = minimum_limit.symbol.code().raw();
        auto viplimit_table = viplimits(get_self(),sym_raw);
        auto iter = viplimit_table.find(vip.value);
        if (iter == viplimit_table.end() ){
            auto zero_asset = eosio::asset(0,minimum_limit.symbol);
            setviplimit_v2(vip,zero_asset,minimum_limit,zero_asset,0,0,0);
        } else {
            setviplimit_v2(vip,iter->maximum_limit,minimum_limit,iter->total_limit,
            iter->frequency_limit,iter->interval_limit,iter->reset_limit);
        }
    }

    void pegtoken::setviptotlim_v2(name vip, asset total_limit ) {
        auto sym_raw = total_limit.symbol.code().raw();
        auto viplimit_table = viplimits(get_self(),sym_raw);
        auto iter = viplimit_table.find(vip.value);
        if (iter == viplimit_table.end() ){
            auto zero_asset = eosio::asset(0,total_limit.symbol);
            setviplimit_v2(vip,zero_asset,zero_asset,total_limit,0,0,0);
        } else {
            setviplimit_v2(vip,iter->maximum_limit,iter->minimum_limit,total_limit,
            iter->frequency_limit,iter->interval_limit,iter->reset_limit);
        }
    }

    void pegtoken::setfee_v2( double service_fee_rate, asset min_service_fee, asset miner_fee ){
        eosio_assert(min_service_fee.symbol == miner_fee.symbol, "different symbol");
        auto sym_raw = min_service_fee.symbol.code().raw();

        auto info_table = infos(get_self(),sym_raw);
        auto val = info_table.get(sym_raw, "token with symbol not exists(info)");
        require_auth(val.issuer);

        auto fee_table = fees(get_self(),sym_raw);
        if( fee_table.begin() == fee_table.end()) {
            fee_table.emplace(get_self(),[&](auto &p){
                p.service_fee_rate = service_fee_rate;
                p.min_service_fee = min_service_fee;
                p.miner_fee = miner_fee;
            });
        } else {
            fee_table.modify(fee_table.begin(), same_payer ,[&](auto &p){
                p.service_fee_rate = service_fee_rate;
                p.min_service_fee = min_service_fee;
                p.miner_fee = miner_fee;
            });
        }
    }

    void pegtoken::setsrvfeerat_v2(symbol_code sym_code, double service_fee_rate){
        auto fee_table = fees(get_self(),sym_code.raw());
        auto iter = fee_table.begin();
        if( iter == fee_table.end()) {
            auto zero_asset = eosio::asset(0,eosio::symbol(sym_code.raw()));
            setfee_v2(service_fee_rate,zero_asset,zero_asset);
        } else {
            setfee_v2(service_fee_rate,iter->min_service_fee,iter->miner_fee);
        }
    }

    void pegtoken::setminsrvfee_v2(asset min_service_fee){
        auto fee_table = fees(get_self(),min_service_fee.symbol.code().raw());
        auto iter = fee_table.begin();
        if( iter == fee_table.end()) {
            auto zero_asset = eosio::asset(0,min_service_fee.symbol);
            setfee_v2(0,min_service_fee,zero_asset);
        } else {
            setfee_v2(iter->service_fee_rate,min_service_fee,iter->miner_fee);
        }
    }

    void pegtoken::setminerfee_v2(asset miner_fee){
        auto fee_table = fees(get_self(),miner_fee.symbol.code().raw());
        auto iter = fee_table.begin();
        if( iter == fee_table.end()) {
            auto zero_asset = eosio::asset(0,miner_fee.symbol);
            setfee_v2(0,zero_asset,miner_fee);
        } else {
            setfee_v2(iter->service_fee_rate,iter->min_service_fee,miner_fee);
        }
    }
    
    void pegtoken::setvipfee_v2(name vip, double service_fee_rate, asset min_service_fee, asset miner_fee ){
        eosio_assert(min_service_fee.symbol == miner_fee.symbol, "invlaid symbol");
        
        auto sym_raw = min_service_fee.symbol.code().raw();
        
        auto info_table = infos(get_self(),sym_raw);
        auto val = info_table.get(sym_raw, "token with symbol not exists(info)");
        require_auth(val.issuer);

        auto vip_table = vips(get_self(),sym_raw);
        auto viplimit_table = viplimits(get_self(),sym_raw);
        auto vipfee_table = vipfees(get_self(),sym_raw);
        
        auto iter_vip = vip_table.find(vip.value);
        if(iter_vip == vip_table.end()){
            auto zero_asset = eosio::asset(0,min_service_fee.symbol);
            vip_table.emplace(get_self(),[&](auto &p){
                p.vip = vip;
                p.create_time = time_point_sec(now());
            });
            viplimit_table.emplace(get_self(),[&](auto &p){
                p.owner = vip;
                p.maximum_limit = zero_asset;
                p.minimum_limit = zero_asset;
                p.total_limit = zero_asset;
                p.frequency_limit = 0;
                p.interval_limit = 0;
                p.reset_limit = 0;
            });
            vipfee_table.emplace(get_self(),[&](auto &p){
                p.owner = vip;
                p.service_fee_rate = 0;
                p.min_service_fee = zero_asset;
                p.miner_fee = zero_asset;
            });
        } else {
            vipfee_table.modify(
                vipfee_table.find(vip.value),same_payer,
                [&](auto &p){
                p.service_fee_rate = service_fee_rate;
                p.min_service_fee = min_service_fee;
                p.miner_fee = miner_fee;
            });
        }
    }

    void pegtoken::setvipsrvfr_v2(symbol_code sym_code, name vip,double service_fee_rate) {
        auto sym_raw = sym_code.raw();

        auto vipfee_table = vipfees(get_self(), sym_raw);
        auto iter = vipfee_table.find(vip.value);
        if(iter == vipfee_table.end() ) {
            auto zero_asset = eosio::asset(0,eosio::symbol(sym_raw));
            setvipfee_v2(vip,service_fee_rate,zero_asset,zero_asset);
        } else {
            setvipfee_v2(vip,service_fee_rate,iter->min_service_fee ,iter->miner_fee);
        }
    }

    void pegtoken::setvipminfee_v2(name vip, asset min_service_fee ){
        auto sym_raw = min_service_fee.symbol.code().raw();

        auto vipfee_table = vipfees(get_self(), sym_raw);
        auto iter = vipfee_table.find(vip.value);
        if(iter == vipfee_table.end() ) {
            auto zero_asset = eosio::asset(0,eosio::symbol(sym_raw));
            setvipfee_v2(vip,0,min_service_fee,zero_asset);
        } else {
            setvipfee_v2(vip,iter->service_fee_rate,min_service_fee ,iter->miner_fee);
        }
    }

    void pegtoken::setvipminerf_v2(name vip, asset miner_fee ){
        auto sym_raw = miner_fee.symbol.code().raw();

        auto vipfee_table = vipfees(get_self(), sym_raw);
        auto iter = vipfee_table.find(vip.value);
        if(iter == vipfee_table.end() ) {
            auto zero_asset = eosio::asset(0,eosio::symbol(sym_raw));
            setvipfee_v2(vip,0,zero_asset,miner_fee);
        } else {
            setvipfee_v2(vip,iter->service_fee_rate,iter->min_service_fee ,miner_fee);
        }
    }

    void pegtoken::setdelay_v2(symbol_code sym_code, uint64_t delayday) {
        auto sym_raw = sym_code.raw();
        
        auto info_table = infos(get_self(),sym_raw);
        auto val = info_table.get(sym_raw, "token with symbol not exists(info)");
        require_auth(val.issuer);

        auto delay_table = delays(get_self(),sym_raw);
        if(delay_table.begin()==delay_table.end()){
            delay_table.emplace(get_self(),[&](auto &p){
                p.delay = delayday * ONE_DAY;
            });
        } else {
            delay_table.modify(delay_table.begin(),same_payer,[&](auto &p){
                p.delay = delayday * ONE_DAY;
            });
        }
    }
}