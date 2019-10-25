#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Main_App; }
QT_END_NAMESPACE

struct GeneralVariables
{
    QString str_file_path;
    QString str_text;
};

extern GeneralVariables general_variables;

class Main_App : public QMainWindow
{
    Q_OBJECT

public:
    Main_App(QWidget *parent = nullptr);
    ~Main_App();

private slots:
    void on_file_name_button_clicked();

    void on_save_button_clicked();

    void clean_str();

    void new_table();

    void update_table(QString*,QString*);

    void set_header();

    void onThreadFinished();

    void onThreadStart();

private:
    Ui::Main_App *ui;

    QStandardItemModel * model;

    int count_row_table;

    bool is_thread_active;

    virtual void closeEvent(QCloseEvent *event) override;
};
#endif // MAIN_APP_H



