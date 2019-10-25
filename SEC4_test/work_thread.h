#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <QThread>
#include <QString>
#include <QtDebug>
#include <QFile>
#include <QFileInfo>
#include <QMutex>


class Work_Thread : public QThread
{
public:
    Work_Thread(QMutex *);
signals:
    void finished();
private:
    QMutex * general_mutex;
public slots:
    void counter();
    void create_file(QString *, QString *);
};

#endif // WORK_THREAD_H

