#include "main_app.h"
#include "ui_main_app.h"
#include "work_thread.h"

#include <QThread>
#include <QMutex>
#include <QTableView>
#include <QStandardItemModel>

QMutex general_mutex;
GeneralVariables general_variables;

void Main_App::clean_str()
{
    ui->file_name_lineEdit->clear();
    ui->message_textEdit->clear();
}

/* Insert a new line on the table with message and file path */
void Main_App::update_table(QString * file_name_ptr, QString * message_ptr)
{
    general_mutex.lock();
    QString file_name = *file_name_ptr;
    QString message = *message_ptr;
    general_mutex.unlock();

    QStandardItem* item_0 = new QStandardItem(file_name);
    model->setItem(count_row_table, 0, item_0);
    QStandardItem* item_1 = new QStandardItem(message);
    model->setItem(count_row_table, 1, item_1);
    set_header();
    count_row_table++;
}

void Main_App::set_header()
{
    model->setHeaderData(0,Qt::Horizontal, QObject::tr("File Path"));
    model->setHeaderData(1,Qt::Horizontal, QObject::tr("Message"));
    ui->tableView->setColumnWidth(0,ui->tableView->width()/2);
    ui->tableView->setColumnWidth(1,ui->tableView->width()/2);
    ui->tableView->setRowHeight(count_row_table,90);
}

void Main_App::new_table()
{
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    Main_App::model = new QStandardItemModel();
    ui->tableView->setModel(model);
    Main_App::count_row_table = 0;
}

Main_App::Main_App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Main_App)
{
    ui->setupUi(this);
    this->new_table();
    Main_App::is_thread_active = false;
}

Main_App::~Main_App()
{
    delete ui;
}

/*open a window to select the path */
void Main_App::on_file_name_button_clicked()
{

    QString file_name_string = QFileDialog::getSaveFileName(this,"Save file",QDir::currentPath(),
    "Text files (*.txt)", new QString("Text files (*.txt)"));

    ui->file_name_lineEdit->setText(file_name_string);
}

void Main_App::onThreadStart()
{
    Main_App::is_thread_active = true;
}

void Main_App::onThreadFinished()
{
    Main_App::is_thread_active = false;
}

/*create the thread*/
void Main_App::on_save_button_clicked()
{
    if(ui->file_name_lineEdit->text() =="" ||
       ui->message_textEdit->toPlainText() == "")
    {
        QMessageBox msgBox;
        msgBox.setText("Please, fill the forms.");
        msgBox.exec();
    }
    else
    {
        QThread *thread = new QThread;
        Work_Thread *worker = new Work_Thread(&general_mutex);
        worker->moveToThread(thread);
        connect(thread,SIGNAL(started()),this,SLOT(onThreadStart()));
        connect(thread,SIGNAL(finished()), this,SLOT(onThreadFinished()));
        thread->start();

        general_mutex.lock();
        general_variables.str_file_path = ui->file_name_lineEdit->text();
        general_variables.str_text = ui->message_textEdit->toPlainText();
        general_mutex.unlock();

        worker->create_file(& general_variables.str_file_path,
                            & general_variables.str_text);

        thread->exit();

        update_table(& general_variables.str_file_path,
                     & general_variables.str_text);

        this->clean_str();
    }
}

/* avoid close the window while the thread is still runing */
void Main_App::closeEvent(QCloseEvent *event)
{
    if(Main_App::is_thread_active == true)
    {
        QMessageBox msgBox;
        msgBox.setText("You need to wait the thread finish, sorry.");
        msgBox.exec();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

