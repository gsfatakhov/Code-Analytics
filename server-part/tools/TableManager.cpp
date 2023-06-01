#include "TableManager.h"

TableManager::TableManager(clickhouse::ClientOptions opts) : options_(opts), client_(opts){
    std::cout << "Connection in TableManager created\n";
}


void TableManager::operate_language(const std::string &from_table, const std::string &to_table, const std::string &lang,
                                    int max_len) {
    try {
        std::string createTableQuery = "CREATE TABLE " + to_table +
                                       " (Location VARCHAR, Punctuation String) ENGINE = MergeTree() ORDER BY Location";
        client_.Execute(createTableQuery);
    }
    catch (const std::exception &e) {
        std::cerr << "Error while making new table: " << e.what() << std::endl;
        return;
    }

    std::string get_query = "SELECT Location FROM " + from_table + " WHERE Language = '" + lang + "' AND Lines < " +
                            std::to_string(max_len) + " ;";



    size_t counter = 0;
    size_t batch_size = 1000;

    auto location = std::make_shared<clickhouse::ColumnString>();
    auto punct_col = std::make_shared<clickhouse::ColumnString>();
    clickhouse::Client sender(this->options_);

    client_.Select(get_query, [&](const clickhouse::Block &block) {
        for (size_t i = 0; i < block.GetRowCount(); ++i) {
            std::string path = "/data/git_cache/";
            path += std::string(block[0]->As<clickhouse::ColumnString>()->At(i));
            std::string punctuation = TableManager::parse_file(path);
            if (punctuation.empty()){
                continue;
            }

            location->Append(path);
            punct_col->Append(punctuation);

            counter++;
            if (counter % batch_size == 0){
                clickhouse::Block out_block;

                out_block.AppendColumn("Location", location);
                out_block.AppendColumn("Punctuation", punct_col);

                sender.Insert(to_table, out_block);
                std::cout << "writed " << counter << " rows\n";

                location = std::make_shared<clickhouse::ColumnString>();
                punct_col = std::make_shared<clickhouse::ColumnString>();
            }
        }
    });
    clickhouse::Block out_block;
    out_block.AppendColumn("Location", location);
    out_block.AppendColumn("Punctuation", punct_col);
    sender.Insert(to_table, out_block);

    std::cout << "writed " << counter << " rows\n";
}

std::string TableManager::parse_file(const std::string &path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::exception();
        }
        std::string out;
        std::string line;
        while (std::getline(file, line)) {
            for (char c : line) {
                if (TableManager::isPunctuation(c)) {
                    out += c;
                }
            }
        }
        file.close();
        return out;
    }
    catch (...){
        std::cerr << "Error while parsing file " + path + "\n";
        return "";
    }
}

bool TableManager::isPunctuation(char c) {
    return std::ispunct(static_cast<unsigned char>(c));
}

// std::string stringToHex(const std::string& input) {
//     std::stringstream hexStream;
//     hexStream << std::hex << std::uppercase << std::setfill('0');

//     for (const unsigned char c : input) {
//         hexStream << std::setw(2) << static_cast<int>(c);
//     }

//     return hexStream.str();
// }
