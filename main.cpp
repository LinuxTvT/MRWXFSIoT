#include "mrw_logger.h"
#include "qcolor.h"
#include "qnamespace.h"
#include "qthread.h"
#include "service/cardreader/cardreader_service.h"
#include "service_manager.h"
#include "utils/json_utils.h"
#include <QBitmap>
#include <QBrush>
#include <QCoreApplication>
#include <QFont>
#include <QGuiApplication>
#include <QLibrary>
#include <QPainter>
#include <QPen>

QDir g_dirRun;

typedef int (*GetStatus)();

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
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

    //    QDateTime local(QDateTime::currentDateTime());
    //    QDateTime UTC(local.toUTC());
    //    qDebug() << "Local time is:" << local;
    //    qDebug() << "UTC time is:" << UTC;
    //    qDebug() << "No difference between times:" << local.secsTo(UTC);

    qDebug() << "Starting main EVENT LOOP process";

    // QBitmap m_hBitmap(476, 568);
    QPen m_hPen(Qt::SolidLine);
    QBrush m_hBrush(Qt::SolidPattern);
    QImage image(QSize(400, 300), QImage::Format_Mono);
    image.fill(Qt::color1);

    QPainter painter{ &image };
    // painter.setLayoutDirection(Qt:);
    // painter.begin(&image);

    // painter.setBrush(QBrush(Qt::green));
    // painter.fillRect(QRectF(0, 0, 400, 300), Qt::green);
    // painter.fillRect(QRectF(100, 100, 200, 100), Qt::white);
    painter.setPen(QPen(Qt::color1));
    painter.drawText(
            0, 20,
            QString::fromUtf8("Trịnh Văn Tài TThe core functionality of QPainter is drawing, but the class also "
                              "provide several functions that allows you to customize QPainter's settings and its "
                              "rendering quality, and others that enable clipping. In addition you can control how "
                              "different shapes are merged together by specifying the painter's composition mode. "));

    // painter.end();
    //    // Reverse the backgrond & text color
    //    if (m_hBrush.color() != Qt::color1)
    //        m_hBrush.setColor(Qt::color0);
    //    else
    //        m_hBrush.setColor(Qt::color1);
    //    m_hdcMem.setBrush(m_hBrush);
    //    m_hdcMem.setBackground(m_hBrush);

    //    if (m_hPen.color() != Qt::color1)
    //        m_hPen.setColor(Qt::color1);
    //    else
    //        m_hPen.setColor(Qt::color0);
    //    m_hdcMem.setPen(m_hPen);
    //    // m_hdcMem.drawText(rect_rev, QString("My test text"));

    // Save as image
    // image = m_hBitmap.toImage();
    image.save("test1.bmp", 0, -1); // Testing

    //    // end
    //    if (m_hBitmap.isNull() != true && m_hdcMem.isActive()) {
    //        m_hdcMem.end();
    //    }
    //    m_hBitmap.clear();
    return a.exec();
}
