#include <types.hpp>

namespace utils
{
   asset get_supply(const name &token_contract_account, const symbol_code &sym_code)
   {
      stats statstable(token_contract_account, sym_code.raw());
      string err_msg = "invalid token contract: ";
      err_msg.append(token_contract_account.to_string());
      const auto &st = statstable.require_find(sym_code.raw(), err_msg.c_str());
      return st->supply;
   }

   asset get_balance(const name &token_contract_account, const name &owner, const symbol_code &sym_code)
   {
      accounts accountstable(token_contract_account, owner.value);
      auto itr = accountstable.find(sym_code.raw());
      if (itr == accountstable.end())
      {
         asset supply = get_supply(token_contract_account, sym_code);
         return asset(0, supply.symbol);
      }
      else
      {
         return itr->balance;
      }
   }

   void inline_transfer(name contract, name from, name to, asset quantity, string memo)
   {
      action(permission_level{from, "active"_n}, contract, name("transfer"), make_tuple(from, to, quantity, memo)).send();
   }

} // namespace utils