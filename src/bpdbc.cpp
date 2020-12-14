#include <bpdbc.hpp>

void bpdbc::claim()
{
   action(permission_level{get_self(), "active"_n}, name("eosio"), name("claimrewards"), make_tuple(get_self())).send();
}

void bpdbc::token_in(name from, name to, asset quantity, string memo)
{
   if (from == _self || to != _self)
      return;
   action(permission_level{get_self(), "active"_n}, get_self(), name("split"), make_tuple()).send();
}

void bpdbc::split()
{
   require_auth(get_self());
   asset all = utils::get_balance(name("eosio.token"), get_self(), symbol_code("EOS"));
   asset fundation_bal = all / 2;
   asset granary_bal = all - fundation_bal;
   utils::inline_transfer(name("eosio.token"), get_self(), name("dfsfundation"), fundation_bal, string("dbc.tag白菜将军每日乐捐:节点收益50%乐捐给社区票仓(dfsfundation);50%回购做市TAG(granary.tag);以上由智能合约自动执行.合约代码已开源:https://github.com/defis-net/bpdbc"));
   utils::inline_transfer(name("eosio.token"), get_self(), name("granary.tag"), granary_bal, string(""));
   action(permission_level{get_self(), "active"_n}, get_self(), name("allocate"), make_tuple()).send();
}

void bpdbc::allocate()
{
   require_auth(get_self());
   asset granary_bal = utils::get_balance(name("eosio.token"), name("granary.tag"), symbol_code("EOS"));
   if (granary_bal.amount > 0)
   {
      asset buy_tag_fund = granary_bal / 2; // 一半买入tag
      utils::inline_transfer(name("eosio.token"), name("granary.tag"), name("defisswapcnt"), buy_tag_fund, string("swap:602:0"));
      action(permission_level{get_self(), "active"_n}, get_self(), name("addliquidity"), make_tuple()).send();
   }
}

void bpdbc::addliquidity()
{
   require_auth(get_self());
   asset granary_bal = utils::get_balance(name("eosio.token"), name("granary.tag"), symbol_code("EOS"));
   asset tag_bal = utils::get_balance(name("tagtokenmain"), name("granary.tag"), symbol_code("TAG"));
   asset tag_fund = granary_bal;
   if (tag_fund.amount > 0 && tag_bal.amount > 0)
   {
      uint64_t mid = 602; // TAG/EOS 做市
      action(permission_level{name("granary.tag"), "active"_n}, name("defisswapcnt"), name("deposit"), make_tuple(name("granary.tag"), mid)).send();
      utils::inline_transfer(name("eosio.token"), name("granary.tag"), name("defisswapcnt"), tag_fund, string("deposit"));
      utils::inline_transfer(name("tagtokenmain"), name("granary.tag"), name("defisswapcnt"), tag_bal, string("deposit"));
   }
}