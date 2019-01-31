#include <bos.pegtoken/bos.pegtoken.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/action.hpp>

#include "bos.pegtoken_v1.cpp"

namespace eosio {

////////////////////////
// actions
////////////////////////

    void
    pegtoken::create(symbol sym, name issuer, name address_style, uint64_t peg) {
        switch (peg)
        {
            case 1:
                create_v1(sym,issuer,issuer,address_style,"","");
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

    void pegtoken::update(symbol_code sym_code, string organization, string website) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            update_v1(sym_code,organization,website);
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
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    // FIXME: setauditor is removed
    // void pegtoken::setauditor(symbol_code sym_code, string action, name auditor) { }

    void pegtoken::setfee(double service_fee_rate, asset min_service_fee, asset miner_fee) {
        auto sym_code = min_service_fee.symbol.code();
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setfee_v1(service_fee_rate,min_service_fee,miner_fee);
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::setissuer( symbol_code sym_code, name issuer )
    {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            setissuer_v1(sym_code,issuer);
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
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

    void pegtoken::applyaddr(symbol_code sym_code, name to) {
        auto peg_table = pegs(get_self(),sym_code.raw());
        auto pegval = peg_table.get(sym_code.raw(),"no such peg");
        switch (pegval.peg)
        {
        case 1:
            applyaddr_v1(sym_code,to);
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
        default:
            eosio_assert(false, "peg should be 1 or 2");
            break;
        }
    }

} // namespace eosio

// FIXME: setauditor is removed
EOSIO_DISPATCH( eosio::pegtoken, ( create )(update)(setlimit)/*(setauditor)*/(setissuer)(setfee)(issue)(retire)(applyaddr)(assignaddr)(
        withdraw)(deposit)(transfer)(clear)(feedback)(rollback)(setacceptor)(setdelay)(lockall)(unlockall)(approve)(
        unapprove)(sendback)
( rmwithdraw ));
