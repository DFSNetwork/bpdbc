#include <utils.hpp>
CONTRACT bpdbc : public contract
{
public:
   using contract::contract;

   [[eosio::on_notify("eosio.token::transfer")]] void token_in(name from, name to, asset quantity, string memo);

   ACTION claim();
   ACTION split();
   ACTION allocate();
   ACTION addliquidity();
};