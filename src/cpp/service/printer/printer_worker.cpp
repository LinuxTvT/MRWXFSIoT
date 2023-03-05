#include "printer_worker.h"
#include "service/printer/printer_service.h"

PrinterWorker::PrinterWorker(const QString &strName, //
                             const QString &strFileConfig, //
                             PrinterService *pParent)
    : DeviceWorker{ strName, strFileConfig, pParent }
{
}

PrinterWorker::~PrinterWorker() { }

bool PrinterWorker::loadConfig(const QJsonObject &config)
{
    if (DeviceWorker::loadConfig(config)) {
        m_joPrinterStatus = config["initPrinterStatus"].toObject();
        m_joPaperStatus = config["initPaperStatus"].toObject();
        return true;
    } else {
        return false;
    }
}

void PrinterWorker::printerStatus(QJsonObject &joPayload)
{
    m_joPrinterStatus["paper"] = m_joPaperStatus;
    joPayload["pinter"] = m_joPrinterStatus;
}

bool PrinterWorker::checkFieldValue(const XFSField *pField, QString &strValue)
{
    int l_iFieldWidth = pField->size().qSize().width();
    if (l_iFieldWidth < strValue.length()) {
        strValue.truncate(l_iFieldWidth);
    }
    return true;
}

void PrinterWorker::printField2BMPPrinter(const XFSField *pField, //
                                          const QJsonObject &joFieldsPayload, //
                                          BMPPrinter *pBMPPrinter, //
                                          bool isFollows)
{
    if (pField->isIndexField()) {
        QMap<int, QString> l_listValue;
        pField->printValuesList(joFieldsPayload, l_listValue);
        pBMPPrinter->printField(pField, l_listValue, isFollows);
    } else {
        QString l_strFieldValue = pField->printValue(joFieldsPayload);
        checkFieldValue(pField, l_strFieldValue);
        pBMPPrinter->printField(pField, l_strFieldValue, isFollows);
    }
}
