#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class [[eosio::contract]] recordbook : public eosio::contract {

public:
  using contract::contract;
  
  recordbook(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  // CREATE AND REMOVE USERS
  [[eosio::action]]
  void adduser(const name &user);

  [[eosio::action]]
  void eraseuser(const name &user);

  // UPDATE USER RECORDS AFTER GAME
  [[eosio::action]]
  void addrecord(const name &winner, const name &loser, const bool &tie);

  // GETS
  [[eosio::action]]
  uint32_t getgamestot(const name &user);
  [[eosio::action]]
  uint32_t getwins(const name &user);
  [[eosio::action]]
  uint32_t getlosses(const name &user);
  [[eosio::action]]
  uint32_t getties(const name &user);

private:
  struct [[eosio::table]] user {
    name key;
    uint32_t gamesPlayed;
    uint32_t wins;
    uint32_t losses;
    uint32_t ties;
    uint64_t primary_key() const { return key.value; }
  };
  typedef eosio::multi_index<"users"_n, user> recordbook_type;

};



