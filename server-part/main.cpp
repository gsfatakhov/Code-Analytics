#pragma Once

#include "tools/TableManager.h"
#include "tools/MinHashMaker.h"

int main(int argc, char *argv[]) {
    clickhouse::ClientOptions options;
    options.SetHost("localhost");
    std::string source_table = "default.code_metrics";
    std::string punct_table = "default.cpp_punctuation";
    std::string signature_table = "default.cpp_signature";

    TableManager manager(options);
    manager.operate_language(source_table, punct_table, "C++", 100);

    MinHashMaker maker = MinHashMaker(options);
    maker.make_signature(punct_table, signature_table, 100);
    return 0;
}