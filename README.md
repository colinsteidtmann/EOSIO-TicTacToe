# C++ Smart Contracts
This repository demonstrates three smart contract written in C++ that run on the EOSIO blockchain. 

The 3 contracts: recordbook, talk/chat, tictactoe gameplay. 

The smart contracts exist on their own right now, however, the logical next step would be to integrate all three into a compete decentralized application. The following demonstrates how to deploy and interact with the smart contracts on a local single-node blockchain. These contracts *can* be deployed on the eosio testnet/mainnet blockchain if the contract owner desires. 
  
# Setup 

1. Fork this repo to your personal GitHub account so that you can save your work into your personal Github account.

2. Point your browser to the following URL https://gitpod.io/#https://github.com/your-github-account/EOSIO-TicTacToe to start the IDE. 

  

You can test drive the system by accessing the IDE at https://gitpod.io/#https://github.com/colinsteidtmann/EOSIO-TicTacToe (however you will not be able to save your work into the EOSIO-TicTacToe Github repository)

  

# Instructions

The following instructions assume that the Web IDE was started successfully (see [Setup](#setup)).


## Opening a terminal

To open a terminal, use the Terminal drop-down menu in the IDE user interface.

### Folder Structure

  ```
╠═contract - smart contracts
    ╠══ recordbook - demonstrates keeping track of game records 
    ╠══ talk - demonstrates chats between users and tests 
    ╠══ tictactoe - demonstrates tactoe game
╚═webapp - UI to interact with talk smart contracts
```

# Setting up blockchain users
Run this in a terminal:

```shell
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio jane EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
```
# Using the contracts
## recordbook contract
The source code for the recordbook smartcontract is at `contract/recordbook/recordbook.cpp` within the IDE. To compile the contract, run this in a terminal:


```shell
eosio-cpp contract/recordbook/recordbook.cpp -o contract/recordbook/recordbook.wasm
```

This will produce `recordbook.abi` and `recordbook.wasm` in the `contract/recordbook/` directory.

  **Installing the contract**  

Run this in a terminal:

```shell
cleos create account eosio recordbook EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos set code recordbook contract/recordbook/recordbook.wasm
cleos set abi recordbook contract/recordbook/recordbook.abi
```
**Adding users**
  ```shell
  cleos push action recordbook adduser '[bob]' -p bob
  cleos push action recordbook adduser '[jane]' -p jane
 ```

**View users**
```shell
cleos get table recordbook recordbook users
```
	

> *should output:*
> ``{
  "rows": [{
      "key": "bob",
      "gamesPlayed": 0,
      "wins": 0,
      "losses": 0,
      "ties": 0
    },{
      "key": "jane",
      "gamesPlayed": 0,
      "wins": 0,
      "losses": 0,
      "ties": 0
    }
  ],
  "more": false
}
``

**Add record (win for bob)**
```shell
cleos push action recordbook addrecord '{"winner":"bob", "loser":"jane", "tie":false}' -p recordbook@active
```
**Add record (tie)**
```shell
cleos push action recordbook addrecord '{"winner":"bob", "loser":"jane", "tie":true}' -p recordbook@active
```

**View updated user records**
```shell
cleos get table recordbook recordbook users
```
	

> *should output:*
> ``{
  "rows": [{
      "key": "bob",
      "gamesPlayed": 2,
      "wins": 1,
      "losses": 0,
      "ties": 1
    },{
      "key": "jane",
      "gamesPlayed": 2,
      "wins": 0,
      "losses": 1,
      "ties": 1
    }
  ],
  "more": false
}
``

**Erase user and their record**

```shell
cleos push action recordbook eraseuser '[bob]' -p bob
cleos get table recordbook recordbook users
```
> *should output:*
> ``{
  "rows": [{
      "key": "jane",
      "gamesPlayed": 2,
      "wins": 0,
      "losses": 1,
      "ties": 1
    }
  ],
  "more": false
}
``

## talk contract
**compile**
```shell
eosio-cpp contract/talk/talk.cpp -o contract/talk/talk.wasm
```

**install**
```shell
cleos create account eosio talk EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos set code talk contract/talk/talk.wasm 
cleos set abi talk contract/talk/talk.abi
```
**interact**
```shell
cleos push action talk post '[1000, 0, bob, "This is a new post"]' -p bob
cleos push action talk post '[2000, 0, jane, "This is my first post"]' -p jane
cleos push action talk post '[1001, 2000, bob, "Replying to your post"]' -p bob
cleos get table talk '' message
```
> *should output:*
> ``{
  "rows": [{
      "id": 1000,
      "reply_to": 0,
      "user": "bob",
      "content": "This is a new post"
    },{
      "id": 1001,
      "reply_to": 2000,
      "user": "bob",
      "content": "Replying to your post"
    },{
      "id": 2000,
      "reply_to": 0,
      "user": "jane",
      "content": "This is my first post"
    }
  ],
  "more": false
}
``
  
## tictactoe contract
**compile**
```shell
eosio-cpp contract/tictactoe/tictactoe.cpp -o contract/tictactoe/tictactoe.wasm
```

**install**
```shell
cleos create account eosio tictactoe EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos set code tictactoe contract/tictactoe/tictactoe.wasm 
cleos set abi tictactoe contract/tictactoe/tictactoe.abi
```
**interact**
```shell
#create game
cleos push action tictactoe create '{"challenger":"jane", "host":"bob"}' --permission bob@active

#host (bob) must make first move
cleos push action tictactoe move '{"challenger":"jane", "host":"bob", "by":"bob", "row":1, "column":0}' --permission bob@active

#game play
cleos push action tictactoe move '[jane, bob, jane, 0,0]' -p jane
cleos push action tictactoe move '[jane, bob, bob, 1,1]' -p bob
cleos push action tictactoe move '[jane, bob, jane, 0,1]' -p jane

#winning move!
cleos push action tictactoe move '[jane, bob, bob, 1,2]' -p bob

#error if jane tries to keep playing
cleos push action tictactoe move '[jane, bob, jane, 0,2]' -p jane

#show gameboard
cleos get table tictactoe bob games

#restart game
cleos push action tictactoe restart '{"challenger":"jane", "host":"bob", "by":"bob"}' --permission bob@active

#close game
cleos push action tictactoe close '{"challenger":"jane", "host":"bob"}' --permission bob@active
```

> *the winning gameboard should look like:*
> ``{
  "rows": [{
      "challenger": "jane",
      "host": "bob",
      "turn": "jane",
      "winner": "bob",
      "board": [
        2,
        2,
        0,
        1,
        1,
        1,
        0,
        0,
        0
      ]
    }
  ],
  "more": false
}
``

> *errors for invalid gameplay should look like:*
> ``Error Details:
assertion failure with message: The game has ended.
``
>``Error Details:
assertion failure with message: it's not your turn yet!``

## Running the talk webapp
The source code for the React WebApp is at `webapp/src/index.tsx` within the IDE. To preview the WebApp run this in a terminal:
```
gp preview $(gp url 8000)
```
It should look like:
![talk webapp](https://github.com/colinsteidtmann/EOSIO-TicTacToe/blob/main/webapp/talk-webapp.png?raw=true)
  

## Building and running the talk contract unit test

  

The source code for the unit test is at the `contract/talk/tests` directory within the IDE. To build the tests, run this in the terminal:
```shell
./contract/talk/build-tests
```

This will produce the `contract/talk/tester` binary, which can be run from the terminal to start the actual unit test:
```shell
./contract/talk/tester
```

  

The unit test creates the `talk_tests` test suite and verifies that the following statements are executed without error:

  

1. Create user account `talk`.

2. Load the `talk` smart contract in the `talk` account sandbox.

2. Create user accounts `john` and `jane`.

3. Test the `post` action by performing the following:

1. Push the `post` action from `talk` to `john` with message "`post 1`" identified as `1` and addressed to message `0` (sent by noone).

This posts the message `1` from `john` to noone in the chat.

2. Push the `post` action from `talk` to `jane` with message "`post 2`" identified as `2` and addressed to message `0` (sent by noone).

This posts the message `2` from `jane` to noone in the chat.

3. Push the `post` action from `talk` to `john` with message "`post 3: reply`" identified as `3` and addressed to message `2` (sent by `jane`).

This posts the reply message `3` from `john` to `jane` in the chat.

4. Test failure of the `post` action if message is addressed to a non-existant message id.

  

## Resetting the chain

  

To remove the existing chain and create another:

  

* Switch to the terminal running `nodeos`

* Press `ctrl+c` to stop it

* Run the following

  

```

rm -rf ~/eosio/chain

nodeos --config-dir ~/eosio/chain/config --data-dir ~/eosio/chain/data -e -p eosio --plugin eosio::chain_api_plugin --contracts-console

```

  

Note: if the web app is currently open, then it will cause errors like the following. You may ignore them:

  

```
FC Exception encountered while processing chain.get_table_rows
```

  

## Contributing

  

[Contributing Guide](./CONTRIBUTING.md)

  

[Code of Conduct](./CONTRIBUTING.md#conduct)

  

## License

  

[MIT](./LICENSE)

  

## Important

  

See [LICENSE](LICENSE) for copyright and license terms.

  

All repositories and other materials are provided subject to the terms of this [IMPORTANT](important.md) notice and you must familiarize yourself with its terms. The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements. By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.