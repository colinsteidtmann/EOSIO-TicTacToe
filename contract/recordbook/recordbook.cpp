#include "recordbook.hpp"

void recordbook::adduser(const name &user) 
{
    require_auth( user );
    recordbook_type records(_code, _code.value);
    auto iterator = records.find(user.value);
    eosio_assert(iterator == records.end(), "User already exists");
    records.emplace(user, [&]( auto& row ) {
        row.key = user;
        row.wins = 0;
        row.losses = 0;
        row.ties = 0;
    });
}

void recordbook::eraseuser(const name &user) 
{
    require_auth(user);

    recordbook_type records(_self, _code.value);

    auto iterator = records.find(user.value);
    eosio_assert(iterator != records.end(), "User does not exist");
    records.erase(iterator);
}

void recordbook::addrecord(const name &winner, const name &loser, const bool &tie) {
    
    recordbook_type records(_self, _code.value);
    
    auto winnerIter = records.find(winner.value);
    auto loserIter = records.find(loser.value);
    
    eosio_assert(winnerIter != records.end(), "winnerIter does not exist");
    eosio_assert(loserIter != records.end(), "loserIter does not exist");
    
    if (tie) {  

        records.modify(winnerIter,  _self, [&]( auto& row ) {
            row.gamesPlayed=(winnerIter->gamesPlayed)+1;
            row.ties=(winnerIter->ties)+1;
        });

        records.modify(loserIter, _self, [&]( auto& row ) {
            row.gamesPlayed=(loserIter->gamesPlayed)+1;
            row.ties=(loserIter->ties)+1;
        });

    } else {

        records.modify(winnerIter,  _self, [&]( auto& row ) {
            row.gamesPlayed=(winnerIter->gamesPlayed)+1;
            row.wins=(winnerIter->wins)+1;
        });

        records.modify(loserIter,  _self, [&]( auto& row ) {
            row.gamesPlayed=(loserIter->gamesPlayed)+1;
            row.losses=(loserIter->losses)+1;
        });
    }
}

uint32_t recordbook::getgamestot(const name &user){
    recordbook_type records(_self, _code.value);
    auto iterator = records.find(user.value);
    eosio_assert(iterator != records.end(), "User does not exist");
    return iterator->gamesPlayed;
}

uint32_t recordbook::getwins(const name &user){
    recordbook_type records(_self, _code.value);
    auto iterator = records.find(user.value);
    eosio_assert(iterator != records.end(), "User does not exist");
    return iterator->wins;
}

uint32_t recordbook::getlosses(const name &user){
    recordbook_type records(_self, _code.value);
    auto iterator = records.find(user.value);
    eosio_assert(iterator != records.end(), "User does not exist");
    return iterator->losses;
}

uint32_t recordbook::getties(const name &user){
    recordbook_type records(_self, _code.value);
    auto iterator = records.find(user.value);
    eosio_assert(iterator != records.end(), "User does not exist");
    return iterator->ties;
}





