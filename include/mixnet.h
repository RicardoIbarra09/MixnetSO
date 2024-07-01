#ifndef MIXNET_H
#define MIXNET_H

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class AES {
public:
    static void handleErrors();
    static void generate_key(unsigned char* key);
    static void generate_iv(unsigned char* iv);
    static string encrypt(const unsigned char* key, const string& plaintext);
    static string decrypt(const unsigned char* key, const string& ciphertext);
};

struct Vote {
    string voter_id;
    string vote;
    string timestamp;
    string signature;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vote, voter_id, vote, timestamp, signature)
};

class MixingNode {
public:
    unsigned char key[32];
    MixingNode();
    vector<string> mix(const vector<string>& messages);
    vector<string> demix(const vector<string>& messages);
};

vector<Vote> load_votes(const string& directory);
void save_encrypted_messages(const vector<string>& messages, const string& output_dir, const string& prefix);
void save_decrypted_messages(const vector<Vote>& votes, const string& output_dir, const string& prefix);
vector<string> load_encrypted_messages(const string& directory);

#endif // MIXNET_H
