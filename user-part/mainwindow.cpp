#include "mainwindow.h"
#include "./ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    client_(clickhouse::ClientOptions().SetHost("localhost")) //указываем Host и Port нужной нам бд
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    std::string language = ui->lang->currentText().toStdString();
    if (language == "--"){
        ui->textBrowser->insertPlainText(QString::fromStdString("Input langeage."));
        return;
    }
    std::string table_name;
    if ( language == "C++" ){
        table_name = "default.cpp_signatures";
    }
    
    std::string inp = ui->textEdit->toPlainText().toStdString();
    std::string punct = parse_file(inp);
    
    size_t ngram_size = 3;
    size_t function_count = 100;
    
    std::vector<std::string> ngrams = make_ngrams(punct, ngram_size);
    std::vector<std::string> min_hashed = get_hashes(ngrams, ngram_size, function_count);
    
    
    

    ui->textBrowser->insertPlainText(QString::fromStdString(punct));

    /*for (char i : inp) {
        std::string out = "";
        out += i;
        out += '\n';
        ui->textBrowser->insertPlainText(QString::fromStdString(out));
        ui->textBrowser->insertPlainText(ui->lang->currentText());

    }*/
    ui->textEdit->clear();
}

std::string MainWindow::parse_file(const std::string &inp) {
    std::string out;
    for (char c : inp) {
        if (c == '\n') {
            continue;
        }
        if (std::ispunct(static_cast<unsigned char>(c))) {
            out += c;
        }
    }
    return out;
}

std::vector<std::string> MainWindow::make_ngrams(const std::string& sequence, size_t ngram_size) {
    size_t n = sequence.size();
    if (n <  ngram_size){
        return std::vector<std::string>(0);
    }

    std::vector<std::string> ngrams(n - ngram_size + 1);

    for (size_t i = 0; i < n - ngram_size + 1; ++i) {
        std::string ngram;
        for (size_t shift = 0; shift < ngram_size; ++shift) {
            if (sequence[i + shift] == '\'' || sequence[i + shift] == '\"' || sequence[i + shift] == '\\') {
                ngram += '\\';
            }
            ngram += sequence[i + shift];
        }
        ngrams[i] = ngram;
    }
    return ngrams;
}

std::vector<std::string> MainWindow::get_hashes(const std::vector<std::string>& ngrams, size_t ngram_size, size_t function_count) {
    std::vector<std::string> minhashes(function_count);
    for (size_t hash_number = 0; hash_number < function_count; hash_number++) {
        for (const std::string &item: ngrams) {
            std::string itemHash = sip128(item + std::to_string(hash_number), ngram_size + 1);
            if (minhashes[hash_number].empty() || item < minhashes[hash_number]) {
                minhashes[hash_number] = itemHash;
            }
        }
    }
    return minhashes;
}

std::string MainWindow::sip128(const std::string &input, size_t size) {
    std::string get_query = "SELECT hex(sipHash128('" + input + "', '\\x01', " + std::to_string(size) + " )) ;";
    std::string out;
    client_.Select(get_query, [&](const clickhouse::Block &block) {
        if (block.GetColumnCount() > 0 && block.GetRowCount() > 0) {
            out = std::string(block[0]->As<clickhouse::ColumnFixedString>()->At(0));
        }
    });
    return out;
}

