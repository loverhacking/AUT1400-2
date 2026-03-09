#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <iostream>
#include <memory>
#include <map>

class Client;  // forward declaration to avoid circular dependency

// save the pending transactions (declaration only; defined in server.cpp)
extern std::vector<std::string> pending_trxs;

class Server
{
    public:
        Server();

         // create a new Client with the specified id.
         // If this id already exists, the server should add a random 4 digit number at the end of it automatically.
         // each client should be assigned with 5 coins at the begining.
        std::shared_ptr<Client> add_client(std::string id);

        // get a pointer to a Client using its id
        std::shared_ptr<Client> get_client(std::string id) const;

        // return the wallet value of the client with username id
        double get_wallet(std::string id) const;

        // Each transaction has 3 properties:
        //     i) id of the sender
        //     ii) id of the receiver
        //     iii) value of money to transfer.
        // We will show each transaction with a string, concatenating each of these properties with a -.
        // For example if ali sends 1.5 coins to hamed the transaction will be shown by a string "ali-hamed-1.5"
        static bool parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value);

        // Each Client can add a pending transaction using the transaction format described in the above section.
        // Only accept a pending transaction by authenticating the sender's signature and if he has enough money in his wallet.
        bool add_pending_trx(std::string trx, std::string signature) const;

        // To mine transactions first pending transactions together,
        // also add a number called nonce at the end of this string.
        // The server will generate the mempool and asks each Client for a nonce and calculates the sha256 of the final string.
        // For each nonce if the generated sha256 has 3 zeros in a row in the first 10 numbers,
        // then the mine is successful and the client who called the correct nonce will be awarded with 6.25 coins.
        // After a successful mine of the pending transactions, all the transactions will be removed from pending
        // and the effect of them will be applied on the clients.
        // After a successful mine, print the id of the miner and return the associate nonce.
        size_t mine();

        friend void show_wallets(const Server& server);
    private:
        // Map each client to its wallet. The wallet is the amount of money the client has.
        std::map<std::shared_ptr<Client>, double> clients;
};

void show_wallets(const Server& server);

#endif //SERVER_H