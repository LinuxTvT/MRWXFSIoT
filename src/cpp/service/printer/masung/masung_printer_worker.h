#ifndef MASUNG_PRINTER_WORKER_H
#define MASUNG_PRINTER_WORKER_H

#include "device/abstract_device_worker.h"
#include "device/device_worker.h"
#include "qlibrary.h"
#include "runable_object.h"
#include "service/printer/bmp_printer.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_frame.h"
#include "service/printer/printer_worker.h"
#include <QObject>

// Information type
//    1 printer head model ID
//    2 printer PCB board version
//    3 firmware version
//    4 manufacturer information
//    5 printer model
//    6 chinese encode format
//    7 check sum value

#define MS_INFO_HEAD_MODEL 1
#define MS_INFO_BCB_BOARD_VERSION 2
#define MS_INFO_FIRMWARE_VERSION 3
#define MS_INFO_MANUFACTURER 4
#define MS_INFO_MODEL 5
#define MS_INFO_CHINESE_ENCODE_FORMAT 6
#define MS_INFO_CHECKSUM_VALUE 7

#define MS_SUCCESS 0

class PrinterService;

class MasungPrinterWorker : public PrinterWorker
{
    Q_OBJECT

    Q_PROPERTY(QString connectType MEMBER m_strConnectType)

private:
    inline static QHash<int, EDeviceStatus> DEVICE_STATUS_MAP = {
        { 0, ST_online }, // Printer is ready, Power is correct and paper is enough
        { 1, ST_offline }, // Printer is offline or no power
        { 2, ST_noDevice }, // Printer called unmatched library
        { 3, ST_userError }, // Printer head is opened
        { 4, ST_hardwareError }, // Cutter is not reset
        { 5, ST_hardwareError }, // Printer head temp is abnormal (overheating or too low)
        { 6, ST_userError }, // Printer does not detect blackmark
        { 7, ST_userError }, // Paper out
        { 8, ST_online } // Paper low
    };
    inline static QHash<int, QString> DEVICE_STATUS_MAP_STR = {
        { 0, "Printer is ready, Power is correct and paper is enough" },
        { 1, "Printer is offline or no power" },
        { 2, "Printer called unmatched library" },
        { 3, "Printer head is opened" },
        { 4, "Cutter is not reset" },
        { 5, "Printer head temp is abnormal (overheating or too low)" },
        { 6, "Printer does not detect blackmark" },
        { 7, "Paper out" },
        { 8, "Paper low" }
    };

public:
    explicit MasungPrinterWorker(const QString &strFileConfig, PrinterService *pParent);
    virtual ~MasungPrinterWorker();

    virtual void idleProcess() override;

protected:
    virtual bool init() override;

    // AbstractDeviceWorker interface
protected:
    virtual bool executeCommand(XFSIoTCommandEvent *pCommandEvent) override;
    void doPrintForm(XFSIoTCommandEvent *pCommandEvent);
    void doCommonStatus(XFSIoTCommandEvent *pCommandEvent);
    void doReset(XFSIoTCommandEvent *pCommandEvent);

    virtual void printField(XFSField *pField, const QJsonObject &joPayload);
    virtual void printField(const XFSField *pField, //
                            const QJsonObject &joFieldsPayload, //
                            QSet<QString> &setPrintedList, //
                            BMPPrinter *pBMPPrinter, bool isFollows);
    virtual void printField(XFSField *pField, const QString &strValue);
    virtual void printField(XFSField *pField, const QString &strValue, BMPPrinter *pBMPPrinter);
    virtual void printFrame(XFSFrame *pFrame, BMPPrinter *pBMPPrinter);

    virtual bool loadConfig(const QJsonObject &config) override;

    // Private Function
private:
    bool loadSDK();
    void initDevice();
    void updateDeviceInfo();
    void updateDeviceStatus();
    void cutPaper();
    bool printBitmap(const QString &filePath);
    bool printString(const QString &str);
    bool goTo(int iX, int iY);
    bool feedLine(int iLines);

private:
    // Masung SDK properties
    QLibrary *m_pSDKLib;
    QString m_strSDKDllFileName = QStringLiteral("Msprintsdk");
    QString m_strSDKVersion;
    QString m_strConnectType;
    QString m_strModel;
    QString m_strManufacturer;
    QString m_strFirmwareVersion;

    int m_iLineNo = 0;

    int m_iCurX = 0;
    int m_iCurY = 0;

    int m_iHardwareStatus;
    double m_dMMUnit = 0.125;
};

#endif // MASUNG_PRINTER_WORKER_H
