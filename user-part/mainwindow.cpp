#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <vector>




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
    std::string inp = ui->textEdit->toPlainText().toStdString();
    std::string punct = parse_file(inp);
    size_t ngram_size = 3;
    std::vector<std::string> ngrams = make_ngrams(punct, ngram_size);




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

std::vector<std::string> MainWindow::make_ngrams(const std::string& sequence, size_t ngram_size){
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
