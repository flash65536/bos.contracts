#include <bos.pegtoken/bos.pegtoken.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/action.hpp>

#include "bos.pegtoken_v1.cpp"
#include "bos.pegtoken_v2.cpp"

namespace eosio {

////////////////////////
// actions
////////////////////////

    void pegtoken::create(symbol sym, name issuer, name address_style, uint64_t peg) {
        switch (peg)
        {
            case 1:
                create_v1(sym,issuer,issuer,address_style,"","");
                break;
            case 2:
                create_v2(sym,issuer,address_style);
                break;
            default:
                eosio_assert(false, "peg should be 1 or 2");
                break;
        }

        auto edtion_table = editions(get_self(),sym.code().raw());
        edtion_table.emplace(get_self(),[&](auto &p) {
            p.sym = sym;
            p.edition = 2;
        });

        auto peg_table = pegs(get_self(),sym.code().raw());
        peg_table.emplace(get_self(),[&](auto &p) {
            p.sym = sym;
            p.peg = peg;
        });
    }


    void pegtoken::setissuer( symbol_code sym_code, name issuer )
    {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setissuer_v1(sym_code,issuer);
            break;
        case 2:
            setissuer_v2(sym_code,issuer);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setedition( symbol_code sym_code ) {
        require_auth(get_self());

        auto edtion_table = editions(get_self(),sym_code.raw());
        eosio_assert(edtion_table.begin() == edtion_table.end(), " edition already set" );
        edtion_table.emplace(get_self(),[&](auto &p) {
            p.sym = eosio::symbol(sym_code.raw());
            p.edition = 1;
        });
    }


    void pegtoken::setpeg( symbol_code sym_code ) {
        require_auth(get_self());

        auto peg_table = pegs(get_self(),sym_code.raw());
        eosio_assert(peg_table.begin() == peg_table.end(), " peg already set" );
        peg_table.emplace(get_self(),[&](auto &p) {
            p.sym = eosio::symbol(sym_code.raw());
            p.peg = 1;
        });
    }

    void pegtoken::update(symbol_code sym_code, string organization, string website) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            update_v1(sym_code,organization,website);
            break;
        case 2:
            update_v2(sym_code,organization,website);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setlimit(asset max_limit, asset min_limit, asset total_limit, uint64_t frequency_limit,
                            uint64_t interval_limit) {
        auto sym_code = max_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setlimit_v1(max_limit,min_limit,total_limit,frequency_limit,interval_limit);
            break;
        case 2:
            setlimit_v2(max_limit,min_limit,total_limit,frequency_limit,interval_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setmaxlimit( asset max_limit ) {
        auto sym_code = max_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setmaxlimit_v1(max_limit);
            break;
        case 2:
            setmaxlimit_v2(max_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setminlimit( asset min_limit ) {
        auto sym_code = min_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setminlimit_v1(min_limit);
            break;
        case 2:
            setminlimit_v2(min_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::settotlimit( asset total_limit )
    {
        auto sym_code = total_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            settotlimit_v1(total_limit);
            break;
        case 2:
            settotlimit_v2(total_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setfrelimit( symbol_code sym_code, uint64_t frequency_limit) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setfrelimit_v1(sym_code, frequency_limit);
            break;
        case 2:
            setfrelimit_v2(sym_code, frequency_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setintlimit( symbol_code sym_code, uint64_t interval_limit) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setintlimit_v1(sym_code, interval_limit);
            break;
        case 2:
            setintlimit_v2(sym_code, interval_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setreslimit( symbol_code sym_code, uint64_t reset_limit) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setreslimit_v1(sym_code, reset_limit);
            break;
        case 2:
            setreslimit_v2(sym_code, reset_limit);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setviplimit(name vip, asset maximum_limit, asset minimum_limit ,asset total_limit,uint64_t frequency_limit, uint64_t interval_limit,uint64_t reset_limit) {
        auto sym_code = maximum_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setviplimit_v1(vip,maximum_limit,minimum_limit,total_limit,frequency_limit,interval_limit,reset_limit );
            break;
        case 2:
            setviplimit_v2(vip,maximum_limit,minimum_limit,total_limit,frequency_limit,interval_limit,reset_limit );
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setvipmaxlim(name vip, asset maximum_limit ) {
        auto sym_code = maximum_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setvipmaxlim_v1(vip,maximum_limit );
            break;
        case 2:
            setvipmaxlim_v2(vip,maximum_limit );
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

        void pegtoken::setvipminlim(name vip, asset minimum_limit ) {
        auto sym_code = minimum_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setvipminlim_v1(vip,minimum_limit );
            break;
        case 2:
            setvipminlim_v2(vip,minimum_limit );
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setviptotlim(name vip, asset totimum_limit ) {
        auto sym_code = totimum_limit.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setviptotlim_v1(vip,totimum_limit );
            break;
        case 2:
            setviptotlim_v2(vip,totimum_limit );
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setfee(double service_fee_rate, asset min_service_fee, asset miner_fee) {
        auto sym_code = min_service_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setfee_v1(service_fee_rate,min_service_fee,miner_fee);
            break;
        case 2:
            setfee_v2(service_fee_rate,min_service_fee,miner_fee);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setsrvfeerat(symbol_code sym_code, double service_fee_rate) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setsrvfeerat_v1(sym_code,service_fee_rate);
            break;
        case 2:
            setsrvfeerat_v2(sym_code,service_fee_rate);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setminsrvfee(asset min_service_fee) {
        auto sym_code = min_service_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setminsrvfee_v1(min_service_fee);
            break;
        case 2:
            setminsrvfee_v2(min_service_fee);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setminerfee(asset miner_fee) {
        auto sym_code = miner_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setminerfee_v1(miner_fee);
            break;
        case 2:
            setminerfee_v2(miner_fee);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setvipfee(name vip, double service_fee_rate, asset min_service_fee, asset miner_fee ){
        auto sym_code = min_service_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setvipfee_v1(vip,service_fee_rate, min_service_fee, miner_fee);
            break;
        case 2:
            setvipfee_v2(vip,service_fee_rate, min_service_fee, miner_fee);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setvipsrvfr(symbol_code sym_code, name vip,double service_fee_rate){
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setvipsrvfr_v1(sym_code,vip,service_fee_rate);
            break;
        case 2:
            setvipsrvfr_v2(sym_code,vip,service_fee_rate);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setvipminfee(name vip, asset min_service_fee ) {
        auto sym_code = min_service_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setvipminfee_v1(vip,min_service_fee);
            break;
        case 2:
            setvipminfee_v2(vip,min_service_fee);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setvipminerf(name vip, asset miner_fee ) {
        auto sym_code = miner_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setvipminerf_v1(vip,miner_fee);
            break;
        case 2:
            setvipminerf_v2(vip,miner_fee);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setdelay(symbol_code sym_code, uint64_t delayday) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setdelay_v1(sym_code,delayday);
            break;
        case 2:
            setdelay_v2(sym_code,delayday);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }


    void pegtoken::issue(asset quantity, string memo) {
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            issue_v1(quantity,memo);
            break;
        case 2:
            issue_v2(quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::retire(asset quantity, string memo) {
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            retire_v1(quantity,memo);
            break;
        case 2:
            retire_v2(quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::precast(string to_address, name to_account, string remote_trx_id, uint64_t index, asset quantity, string memo){
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            eosio_assert(false, "precast not suported");
            break;
        case 2:
            precast_v2(to_address,to_account,remote_trx_id,index,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::agreecast(name auditor, string to_address, name to_account, string remote_trx_id, uint64_t index, asset quantity, string memo){
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            eosio_assert(false, "agreecast not suported");
            break;
        case 2:
            agreecast_v2(auditor,to_address,to_account,remote_trx_id,index,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::refusecast(name auditor, string to_address, name to_account, string remote_trx_id, uint64_t index, asset quantity, string memo){
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            eosio_assert(false, "agreecast not suported");
            break;
        case 2:
            refusecast_v2(auditor,to_address,to_account,remote_trx_id,index,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::docast(string to_address, name to_account, string remote_trx_id, uint64_t index, asset quantity, string memo){
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            eosio_assert(false, "agreecast not suported");
            break;
        case 2:
            docast_v2(to_address,to_account,remote_trx_id,index,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }
    //TODO:

    // FIXME: setauditor is removed
    // void pegtoken::setauditor(symbol_code sym_code, string action, name auditor) { }



    void pegtoken::applyaddr(symbol_code sym_code, name to) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            applyaddr_v1(sym_code,to);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::assignaddr(symbol_code sym_code, name to, string address) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            assignaddr_v1(sym_code,to,address);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::withdraw(name from, string to, asset quantity, string memo) {
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            withdraw_v1(from,to,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::deposit(name to, asset quantity, string memo) {
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            deposit_v1(to,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::transfer(name from, name to, asset quantity, string memo) {
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            transfer_v1(from,to,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::clear(symbol_code sym_code, uint64_t num) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            clear_v1(sym_code,num);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::feedback(symbol_code sym_code, transaction_id_type trx_id, string remote_trx_id, string memo) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            feedback_v1(sym_code,trx_id,remote_trx_id,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::rollback(symbol_code sym_code, transaction_id_type trx_id, string memo) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            rollback_v1(sym_code,trx_id,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setacceptor(symbol_code sym_code, name acceptor) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setacceptor_v1(sym_code,acceptor);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }


    void pegtoken::lockall(symbol_code sym_code, name auditor) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            lockall_v1(sym_code,auditor);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::unlockall(symbol_code sym_code, name auditor) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            unlockall_v1(sym_code,auditor);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::approve(symbol_code sym_code, name auditor, transaction_id_type trx_id, string memo) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            approve_v1(sym_code,auditor,trx_id,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::unapprove(symbol_code sym_code, name auditor, transaction_id_type trx_id, string memo) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            unapprove_v1(sym_code,auditor,trx_id,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::sendback(/*name auditor,*/ transaction_id_type trx_id, name to, asset quantity, string memo) {
        auto sym_code = quantity.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            sendback_v1(trx_id,to,quantity,memo);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::rmwithdraw(uint64_t id, symbol_code sym_code) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            rmwithdraw_v1(id,sym_code);
            break;
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

} // namespace eosio

// FIXME: setauditor is removed
EOSIO_DISPATCH( eosio::pegtoken, (create)(setissuer)(setedition)(setpeg)(update)
        (setlimit)(setmaxlimit)(setminlimit)(settotlimit)(setfrelimit)(setintlimit)(setreslimit)
        (setviplimit)(setvipmaxlim)(setvipminlim)(setviptotlim)(setfee)(setsrvfeerat)(setminsrvfee)(setminerfee)
        (setvipfee)(setvipsrvfr)(setvipminfee)(setvipminerf)
        (setdelay)
        (issue)(retire)
        (precast)(agreecast)(refusecast)(docast)
        /*///////*//*(setauditor)*/(applyaddr)(assignaddr)(
        withdraw)(deposit)(transfer)(clear)(feedback)(rollback)(setacceptor)(lockall)(unlockall)(approve)(unapprove)(sendback)(rmwithdraw));
