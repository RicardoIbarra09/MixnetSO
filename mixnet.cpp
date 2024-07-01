#include "mixnet.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <chrono>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <filesystem>

using namespace std;
using namespace std::chrono;

void AES::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void AES::generate_key(unsigned char* key) {
    if (!RAND_bytes(key, 32)) {
        handleErrors();
    }
}

void AES::generate_iv(unsigned char* iv) {
    if (!RAND_bytes(iv, 16)) {
        handleErrors();
    }
}

string AES::encrypt(const unsigned char* key, const string& plaintext) {
    EVP_CIPHER_CTX* ctx;
    int len;
    int ciphertext_len;
    unsigned char ciphertext[1024];  
    unsigned char iv[16];

    generate_iv(iv);

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, key, iv)) {
        handleErrors();
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length())) {
        handleErrors();
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        handleErrors();
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    string encrypted_message(reinterpret_cast<char*>(ciphertext), ciphertext_len);
    string iv_str(reinterpret_cast<char*>(iv), 16);

    return iv_str + encrypted_message;
}

string AES::decrypt(const unsigned char* key, const string& ciphertext) {
    EVP_CIPHER_CTX* ctx;
    int len;
    int plaintext_len;
    unsigned char plaintext[1024];
    unsigned char iv[16];

    memcpy(iv, ciphertext.c_str(), 16);

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, key, iv)) {
        handleErrors();
    }

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str() + 16), ciphertext.length() - 16)) {
        handleErrors();
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        handleErrors();
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return string(reinterpret_cast<char*>(plaintext), plaintext_len);
}

MixingNode::MixingNode() {
    AES::generate_key(key);
}

vector<string> MixingNode::mix(const vector<string>& messages) {
    vector<string> encrypted_messages;
    for (const auto& msg : messages) {
        string encrypted_message = AES::encrypt(key, msg);
        encrypted_messages.push_back(encrypted_message);
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(encrypted_messages.begin(), encrypted_messages.end(), g);
    return encrypted_messages;
}

vector<string> MixingNode::demix(const vector<string>& messages) {
    vector<string> decrypted_messages;
    for (const auto& msg : messages) {
        string decrypted_message = AES::decrypt(key, msg);
        decrypted_messages.push_back(decrypted_message);
    }
    return decrypted_messages;
}

vector<Vote> load_votes(const string& directory) {
    vector<Vote> votes;
    for (const auto& entry : filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".json") {
            ifstream file(entry.path());
            if (file.is_open()) {
                json j;
                try {
                    file >> j;
                    Vote vote = j.get<Vote>();
                    votes.push_back(vote);
                } catch (json::parse_error& e) {
                    cerr << "Error parsing file " << entry.path() << ": " << e.what() << endl;
                }
            }
        }
    }
    return votes;
}

void save_encrypted_messages(const vector<string>& messages, const string& output_dir, const string& prefix) {
    if (!filesystem::exists(output_dir)) {
        filesystem::create_directory(output_dir);
    }
    for (size_t i = 0; i < messages.size(); ++i) {
        ofstream file(output_dir + "/" + prefix + to_string(i + 1) + ".enc");
        if (file.is_open()) {
            file << messages[i];
        }
    }
}

void save_decrypted_messages(const vector<Vote>& votes, const string& output_dir, const string& prefix) {
    if (!filesystem::exists(output_dir)) {
        filesystem::create_directory(output_dir);
    }
    for (size_t i = 0; i < votes.size(); ++i) {
        ofstream file(output_dir + "/" + prefix + to_string(i + 1) + ".json");
        if (file.is_open()) {
            file << "{\n";
            file << "    \"voter_id\": \"" << votes[i].voter_id << "\",\n";
            file << "    \"vote\": \"" << votes[i].vote << "\",\n";
            file << "    \"timestamp\": \"" << votes[i].timestamp << "\",\n";
            file << "    \"signature\": \"" << votes[i].signature << "\"\n";
            file << "}\n";
        }
    }
}

vector<string> load_encrypted_messages(const string& directory) {
    vector<string> encrypted_messages;
    for (const auto& entry : filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".enc") {
            ifstream file(entry.path());
            if (file.is_open()) {
                stringstream buffer;
                buffer << file.rdbuf();
                encrypted_messages.push_back(buffer.str());
            }
        }
    }
    return encrypted_messages;
}
