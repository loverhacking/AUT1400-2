#include "server.h"
#include "client.h"

#include <random>
#include <limits>

std::vector<std::string> pending_trxs;

Server::Server() {
    clients = std::map<std::shared_ptr<Client>, double>();
}

std::shared_ptr<Client> Server::add_client(std::string id) {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<size_t> dist(0, std::numeric_limits<size_t>::max());
    while (get_client(id) != nullptr) {
        id += std::to_string(dist(gen) % 9000 + 1000);
    }
    auto client = std::make_shared<Client>(id, *this);
    clients[client] = 5.0;
    return client;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (const auto& [client, wallet] : clients) {
        if (client->get_id() == id) {
            return client;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) const {
    auto client = get_client(id);
    if (client != nullptr) {
        return clients.at(client);
    }
    return 0.0;
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value) {
    // parse the transaction string and fill sender, receiver, value (e.g. "ali-hamed-1.5")
    size_t pos1 = trx.find("-");
    size_t pos2 = trx.find("-", pos1 + 1);

    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        throw std::runtime_error("Invalid transaction format");
    }
    sender = trx.substr(0, pos1);
    receiver = trx.substr(pos1 + 1, pos2 - pos1 - 1);
    value = std::stod(trx.substr(pos2 + 1));
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    // authenticate the sender's signature and check if he has enough money in his wallet
    size_t pos1 = trx.find("-");
    size_t pos2 = trx.find("-", pos1 + 1);

    std::string sender_id, receiver_id;
    double amount;
    if (!parse_trx(trx, sender_id, receiver_id, amount)) {
        return false;
    }
    
    // check if sender and receiver exist
    if (get_client(sender_id) == nullptr || get_client(receiver_id) == nullptr) {
        return false;
    }

    bool authentic = crypto::verifySignature(get_client(sender_id)->get_publickey(), trx, signature);
    if (!authentic) {
        return false;
    }

    if (get_wallet(sender_id) < amount) {
        return false;
    }
    pending_trxs.push_back(trx);
    return true;
}

size_t Server::mine() {
    // generate the mempool (concatenate trx without separator, to match test's hash check)
    std::string mempool;
    for (const auto& trx : pending_trxs) {
        mempool += trx;
    }
    size_t nonce;
    std::string hash;
    // keep trying until a successful mine is found
    while (true) {
        for (const auto& [client, wallet] : clients) {
            nonce = client->generate_nonce();
            hash = crypto::sha256(mempool + std::to_string(nonce));
            if (hash.substr(0, 10).find("000") != std::string::npos) {
                // successful mine
                clients[client] += 6.25; // award the miner with 6.25 coins
                // apply the effect of transactions on the clients
                for (const auto& trx : pending_trxs) {
                    std::string sender_id, receiver_id;
                    double amount;
                    if (!parse_trx(trx, sender_id, receiver_id, amount)) {
                        continue;
                    }
                    clients[get_client(sender_id)] -= amount; // deduct from sender's wallet
                    clients[get_client(receiver_id)] += amount; // add to receiver's wallet
                }
                pending_trxs.clear(); // clear pending transactions after successful mine
                std::cout << "Miner: " << client->get_id() << ", Nonce: " << nonce << std::endl;
                return nonce;
            }
        }
    }   
}

void show_wallets(const Server& server) {
    std::cout << std::string(20, '*') << std::endl;
    for (const auto& client : server.clients)
        std::cout << client.first->get_id() << " : " << client.second << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}