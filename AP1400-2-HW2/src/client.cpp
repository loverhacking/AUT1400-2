#include "client.h"

#include <random>
#include <limits>

Client::Client(std::string id, const Server& server) : id(id), server(&server) {
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const {
    return id;
}

std::string Client::get_publickey() const {
    return public_key;
}

double Client::get_wallet() const {
    return server->get_wallet(id);
}

std::string Client::sign(std::string txt) const {
    std::string signature = crypto::signMessage(private_key, txt);
    return signature;
}

bool Client::transfer_money(std::string receiver, double value) const{
    std::string trx = id + "-" + receiver + "-" + std::to_string(value);
    std::string signature = sign(trx);
    return server->add_pending_trx(trx, signature);
}

size_t Client::generate_nonce() const{
    static std::random_device rd;
    // Use the random device to seed the Mersenne Twister generator
    static std::mt19937_64 gen(rd());
    // evenly distribute results across the entire range of possible size_t values
    static std::uniform_int_distribution<size_t> dist(
        0, std::numeric_limits<size_t>::max()
    );
    return dist(gen);
}

