#include "qthread.h"
#include "service/cardreader/cardreader_service.h"
#include "service_manager.h"
#include <QCoreApplication>
#include <QLibrary>
#include "utils/json_utils.h"
#include "mrw_logger.h"

QDir g_dirRun;

typedef int (*GetStatus)();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MRWLogger::setRootDir(QDir("logs"));
    ThreadFacotry gThreadFactory;
    RunableObject::setThreadFacotry(&gThreadFactory);
    ServiceManager service;
    service.log("");
    service.log("###############################################################################################");
    service.log(
            QString("Application running at dir [%1], Main thread id [%2]")
                    .arg(g_dirRun.absolutePath())
                    .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()), QT_POINTER_SIZE * 2, 16, QChar('0')));

    service.loadConfig(JSonUtils::loadJsonObject("MRWXFSIoT.json"));
    service.init();
    service.start();

    //    signal(SIGTERM, [](int sig) {
    //        qDebug() << "SIGNAL received: SIGTERM |" << sig;
    //        qApp->quit();
    //    });
    //    signal(SIGABRT, [](int sig) {
    //        qDebug() << "SIGNAL received: SIGABRT |" << sig;
    //        qApp->quit();
    //    });
    //    signal(SIGINT, [](int sig) {
    //        qDebug() << "SIGNAL received: SIGTERM |" << sig;
    //        qApp->quit();
    //    });

    qDebug() << "Starting main EVENT LOOP process";
    return a.exec();
}
