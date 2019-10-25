#include "main_app.h"
#include "work_thread.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_App w;
    w.show();

    return a.exec();
}


