#include "mixnet.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <numero_de_nodos> <ruta_votos> <ruta_cifrados> <ruta_descifrados>" << std::endl;
        return 1;
    }

    int num_nodos = std::stoi(argv[1]);
    std::string votes_directory = argv[2];
    std::string encrypted_output_directory = argv[3];
    std::string decrypted_output_directory = argv[4];

    std::vector<Vote> votes = load_votes(votes_directory);
    std::vector<std::string> votes_str;
    for (const auto& vote : votes) {
        json j = vote;
        votes_str.push_back(j.dump());
    }

    std::vector<MixingNode> nodes(num_nodos);
    auto start_total = std::chrono::high_resolution_clock::now();

    auto start_encrypt_mix = std::chrono::high_resolution_clock::now();
    std::vector<std::string> mixed_messages = votes_str;
    for (auto& node : nodes) {
        mixed_messages = node.mix(mixed_messages);
    }
    auto end_encrypt_mix = std::chrono::high_resolution_clock::now();

    save_encrypted_messages(mixed_messages, encrypted_output_directory, "encrypted_vote_");

    auto start_decrypt = std::chrono::high_resolution_clock::now();
    std::vector<std::string> encrypted_messages = load_encrypted_messages(encrypted_output_directory);
    std::vector<std::string> demixed_messages = encrypted_messages;
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
        demixed_messages = it->demix(demixed_messages);
    }
    auto end_decrypt = std::chrono::high_resolution_clock::now();

    std::vector<Vote> decrypted_votes;
    for (const auto& msg : demixed_messages) {
        try {
            json j = json::parse(msg);
            Vote vote = j.get<Vote>();
            decrypted_votes.push_back(vote);
        } catch (json::parse_error& e) {
            std::cerr << "Error parsing decrypted message: " << e.what() << std::endl;
        }
    }

    save_decrypted_messages(decrypted_votes, decrypted_output_directory, "decrypted_vote_");

    auto end_total = std::chrono::high_resolution_clock::now();

    auto encrypt_mix_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_encrypt_mix - start_encrypt_mix).count();
    auto decrypt_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_decrypt - start_decrypt).count();
    auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_total - start_total).count();

    std::cout << "Tiempo de cifrado y mezcla: " << encrypt_mix_time << " segundos" << std::endl;
    std::cout << "Tiempo de descifrado: " << decrypt_time << " segundos" << std::endl;
    std::cout << "Tiempo total: " << total_time << " segundos" << std::endl;

    return 0;
}
