#pragma Once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <clickhouse/client.h>


class TableManager {
public:
    explicit TableManager(clickhouse::ClientOptions opts);

    void operate_language(const std::string &from_table, const std::string &to_table,
                          const std::string &lang,
                          int max_len);

private:
    clickhouse::ClientOptions options_;
    clickhouse::Client client_;

    static std::string parse_file(const std::string &path);
    static bool isPunctuation(char c);
};