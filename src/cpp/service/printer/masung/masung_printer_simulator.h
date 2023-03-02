#ifndef MASUNG_PRINTER_SIMULATOR_H
#define MASUNG_PRINTER_SIMULATOR_H
#include "mrw_logger.h"
#include "qglobal.h"
#include "service/printer/masung/masung_printer_worker.h"
#include <QDebug>
#include <QStringList>

// QStringList g_strTestBuffer;
int g_iCurLine = 0;
QString g_strCurrentLine;

MRWLogger *TEST_OUT = MRWLogger::createLogger("MS_RECIEPT");

int testSetInit()
{
    return 0;
}
int testPrintString(const char *strData, int iImme)
{
    if (iImme == 0) {
        TEST_OUT->i(strData);
    } else {
        g_strCurrentLine.append(strData);
    }
    return 0;
}

int testPrintDiskbmpfile(const char *filePath)
{
    QFile l_file(filePath);
    if (!l_file.exists()) {
        TEST_OUT->i(QString("File: [%1] not found").arg(filePath));
    } else {
        TEST_OUT->i(l_file.fileName());
    }
    return 0;
}

int testPrintFeedline(int iLine)
{
    TEST_OUT->i(g_strCurrentLine);
    g_strCurrentLine.clear();
    for (int i = 0; i < iLine; i++) {
        TEST_OUT->i("__");
    }
    return 0;
}

int testPrintChangeRow()
{
    TEST_OUT->i(g_strCurrentLine);
    return 0;
}

int testPrintCutpaper(int mode)
{
    if (mode == 0) {
        TEST_OUT->i("########################################");
    } else {
        TEST_OUT->i("========================================");
    }
    return 0;
}

int testGetStatus()
{
    return 8;
}

int testGetProductinformation(int iFstype, char *bFiddata, int iFidlen)
{
    Q_UNUSED(iFidlen)
    switch (iFstype) {
        case MS_INFO_HEAD_MODEL:
            strncpy(bFiddata, "Simulator", 32);
            return 0;
        case MS_INFO_BCB_BOARD_VERSION:
            strncpy(bFiddata, "Simulator", 32);
            return 0;
        case MS_INFO_FIRMWARE_VERSION:
            strncpy(bFiddata, "TaiTV_20220222", 32);
            return 0;
        case MS_INFO_MANUFACTURER:
            strncpy(bFiddata, "Miraway", 32);
            return 0;
        case MS_INFO_MODEL:
            strncpy(bFiddata, "Simulator", 32);
            return 0;
        case MS_INFO_CHINESE_ENCODE_FORMAT:
            strncpy(bFiddata, "Simulator", 32);
            return 0;
        case MS_INFO_CHECKSUM_VALUE:
            strncpy(bFiddata, "Simulator", 32);
            return 0;
        default:
            strncpy(bFiddata, "Simulator", 32);
            return 0;
    }
}

#endif // MASUNG_PRINTER_SIMULATOR_H
