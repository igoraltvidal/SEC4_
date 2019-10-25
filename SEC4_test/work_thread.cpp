#include "work_thread.h"

Work_Thread::Work_Thread(QMutex * mutex)
{
    general_mutex = mutex;
}

/* Thread create a file with the file path and a message */
void Work_Thread::create_file(QString * fileNamePtr, QString * textPtr)
{
    general_mutex->lock();
    QString fileName = *fileNamePtr;
    QString text = *textPtr;
    general_mutex->unlock();

    QFile file(fileName);
    qDebug() << fileName << endl;
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out << text;
        file.close();
    }

    Work_Thread::quit();
}
