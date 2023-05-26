#include "MinHashMaker.h"

MinHashMaker::MinHashMaker(clickhouse::ClientOptions opts) : options_(opts), client_(opts) {
    std::cout << "Connection in MinHashMaker created\n";
}


void MinHashMaker::make_signature(const std::string &from_table, const std::string &to_table,
                                  int function_count) {
    try {
        // Создание SQL-запроса для создания таблицы
        std::string query = "CREATE TABLE IF NOT EXISTS " + to_table + " ( Location VARCHAR, ";

        // Добавление столбцов
        for (int i = 1; i < function_count; ++i) {
            query += "hash_" + std::to_string(i) + " FixedString(16),";
        }
        query += "hash_" + std::to_string(function_count) + " FixedString(16)) ";
        query += "ENGINE = MergeTree() ORDER BY Location ;";

        client_.Execute(query);
    }
    catch (const std::exception &e) {
        std::cerr << "Error while making new table: " << e.what() << std::endl;
        return;
    }

    clickhouse::Client sender(this->options_);

    std::string begining = "INSERT INTO " + to_table + " ( Location VARCHAR, ";
    for (int i = 1; i < function_count; ++i) {
        begining += "hash_" + std::to_string(i) + ",";
    }
    begining += "hash_" + std::to_string(function_count) + " ) VALUES (";

    size_t counter = 0;
    size_t ngram_size = 3;
    std::string get_query = "SELECT * FROM " + from_table + " ;";
    client_.Select(get_query, [&](const clickhouse::Block &block) {
        for (size_t i = 0; i < block.GetRowCount(); ++i) {
            std::string path = std::string(block[0]->As<clickhouse::ColumnString>()->At(i));
            std::string sequence = std::string(block[1]->As<clickhouse::ColumnString>()->At(i));
            std::vector<std::string> ngrams = make_ngrams(sequence, ngram_size);

            std::string vals;
            for (size_t hash_number = 0; hash_number < function_count; hash_number++) {
                std::string min_hash;
                for (const std::string &item: ngrams) {
                    std::string itemHash = sip128(item + std::to_string(i), &sender, ngram_size + 1);
                    if (min_hash.empty() || item < min_hash) {
                        min_hash = itemHash;
                    }
                }
                vals += min_hash;
                if (hash_number != function_count - 1) {
                    vals += ", ";
                }
            }
            vals += ");";

            sender.Execute(begining + vals);

            counter++;
        }
    });
    std::cout << "writed " << counter << " rows\n";
}


std::vector<std::string> MinHashMaker::make_ngrams(const std::string &sequence, size_t ngram_size) {
    size_t n = sequence.size();

    if (n <  ngram_size){
        return std::vector<std::string>(0);
    }

    std::vector<std::string> ngrams;
    ngrams.reserve(n - ngram_size + 1);

    for (size_t i = 0; i < n - ngram_size + 1; ++i) {
        std::string ngram;
        for (size_t shift = 0; shift < ngram_size; ++shift) {
            if (sequence[i + shift] == '\'' || sequence[i + shift] == '\"' || sequence[i + shift] == '\\') {
                ngram += '\\';
            }
            ngram += sequence[i + shift];
        }
        ngrams.push_back(ngram);
    }

    return ngrams;
}

std::string MinHashMaker::sip128(const std::string &input, clickhouse::Client *sender, size_t size) {
    std::string get_query = "SELECT hex(sipHash128('" + input + "', '\\x01', " + std::to_string(size) + " )) ;";
    std::string out;
    sender->Select(get_query, [&](const clickhouse::Block &block) {
        if (block.GetColumnCount() > 0 && block.GetRowCount() > 0) {
            out = std::string(block[0]->As<clickhouse::ColumnFixedString>()->At(0));
        }
    });
    return out;
}




/* Code to use SHA256 hashing */
//void MinHashMaker::make_signature_custom_hash(const std::string &from_table, const std::string &to_table,
//                                    int function_count) {
//    try {
//        // Создание SQL-запроса для создания таблицы
//        std::string query = "CREATE TABLE IF NOT EXISTS " +  to_table + " ( Location VARCHAR, ";
//
//        // Добавление столбцов типа Int32
//        for (int i = 1; i < function_count; ++i) {
//            query += "hash_" + std::to_string(i) + " VARCHAR,";
//        }
//        query += "hash_" + std::to_string(function_count) + " VARCHAR";
//        query += ") ENGINE = MergeTree() ORDER BY Location ;";
//
//        client_.Execute(query);
//    }
//    catch (const std::exception &e) {
//        std::cerr << "Error while making new table: " << e.what() << std::endl;
//        return;
//    }
//
//    std::string get_query = "SELECT * FROM " + from_table + " ;";
//
//
//    auto location = std::make_shared<clickhouse::ColumnString>();
//    std::vector<std::shared_ptr<clickhouse::ColumnString>> hashes(function_count);
//
//    clickhouse::Client sender(this->options_);
//    size_t counter = 0;
//    size_t batch_size = 10000;
//
//    client_.Select(get_query, [&](const clickhouse::Block &block) {
//        for (size_t i = 0; i < block.GetRowCount(); ++i) {
//            std::string path = std::string(block[0]->As<clickhouse::ColumnString>()->At(i));
//            std::string sequence = std::string(block[1]->As<clickhouse::ColumnString>()->At(i));
//            std::vector<std::string> punctuation = make_ngrams(sequence);
//
//
//            location->Append(path);
//
//            for (size_t hash_number = 0; hash_number < function_count; hash_number++) {
//                std::string min_hash = std::string(SHA256_DIGEST_LENGTH * 2, 'f');
//                for (const std::string& item : punctuation) {
//                    std::string itemHash = sha256(item + std::to_string(i));
//                    if (item < min_hash) {
//                        min_hash = itemHash;
//                    }
//                }
//                hashes[hash_number]->Append(min_hash);
//            }
//            counter++;
//            if (counter % batch_size == 0){
//                clickhouse::Block out_block;
//
//                out_block.AppendColumn("Location", location);
//                for (size_t hash_number = 0; hash_number < function_count; hash_number++) {
//                    out_block.AppendColumn("hash_" + std::to_string(hash_number), hashes[hash_number]);
//                }
//
//                sender.Insert(to_table, out_block);
//                std::cout << "writed " << counter << " rows\n";
//
//                location = std::make_shared<clickhouse::ColumnString>();
//                hashes = std::vector<std::shared_ptr<clickhouse::ColumnString>>(function_count);
//            }
//        }
//    });
//
//    clickhouse::Block out_block;
//    out_block.AppendColumn("Location", location);
//    for (size_t hash_number = 0; hash_number < function_count; hash_number++) {
//        out_block.AppendColumn("hash_" + std::to_string(hash_number), hashes[hash_number]);
//    }
//
//    sender.Insert(to_table, out_block);
//    std::cout << "writed " << counter << " rows\n";
//}



//std::string MinHashMaker::sha256(const std::string& input) {
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    SHA256_CTX sha256;
//    SHA256_Init(&sha256);
//    SHA256_Update(&sha256, input.c_str(), input.length());
//    SHA256_Final(hash, &sha256);
//
//    std::string hashStr;
//    char hex[3];
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//        sprintf(hex, "%02x", hash[i]);
//        hashStr += hex;
//    }
//    return hashStr;
//}
