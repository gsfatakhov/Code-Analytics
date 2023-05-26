#pragma Once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <clickhouse/client.h>
// #include <openssl/sha.h> нужна для имплементации своего hash


class MinHashMaker {
public:
    explicit MinHashMaker(clickhouse::ClientOptions opts);

//    void make_signature_custom_hash(const std::string &from_table, const std::string &to_table,
//                          int function_count);
    void make_signature(const std::string &from_table, const std::string &to_table,
                          int function_count);

private:
    clickhouse::ClientOptions options_;
    clickhouse::Client client_;

    static std::vector<std::string> make_ngrams(const std::string& sequence, size_t ngram_size);
    static std::string sip128(const std::string& input, clickhouse::Client* sender, size_t ngram_size);
    //static std::string sha256(const std::string& input); Функция для вычисления SHA-256 хэша строки
};