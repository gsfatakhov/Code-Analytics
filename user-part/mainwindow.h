#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <clickhouse/client.h>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    clickhouse::Client client_;

    static std::string parse_file(const std::string &inp);
    static std::vector<std::string> make_ngrams(const std::string& sequence, size_t ngram_size);
};
#endif // MAINWINDOW_H
