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

// QString PrinterWorker::getFieldValue(XFSField *pField, //
//                                     const QJsonValue &jvValue, //
//                                     bool *pIsSuccess, //
//                                     QString &strFailure)
//{
//    if (pField->fieldClass().is("static")) {
//        *pIsSuccess = false;
//        strFailure = "staticOverwrite";
//    } else {
//        *pIsSuccess = true;
//        strFailure.clear();
//    }

//    if (jvValue.isUndefined()) {
//        if (pField->fieldClass().is("time")) {
//            *pIsSuccess = true;
//            return QDateTime::currentDateTime().toString();
//        } else if (pField->fieldClass().is("required")) {
//            *pIsSuccess = false;
//            strFailure = "required";
//            return QString{};
//        } else {
//            *pIsSuccess = true;
//            strFailure.clear();
//            return pField->initialvalue();
//        }
//    } else {
//        if (jvValue.isString()) {
//            if (pField->fieldClass().is("static")) {
//                *pIsSuccess = false;
//                strFailure = "staticOverwrite";
//            } else {
//                *pIsSuccess = true;
//                strFailure.clear();
//            }
//            return jvValue.toString();
//        } else {
//            *pIsSuccess = false;
//            strFailure = "notSupported";
//            return QString{};
//        }
//    }
//}

// bool PrinterWorker::getFieldValues(XFSField *pField, const QJsonObject &fields, QMap<int, QString> &strListValues,
//                                   QString &strFailure)
//{
//    QRegularExpression l_regx = pField->fieldNameRegularExpression();
//    for (auto itr = fields.constBegin(); itr != fields.constEnd(); itr++) {
//        QRegularExpressionMatch l_matcher = l_regx.match(itr.key());
//        if (l_matcher.hasMatch()) {
//            strListValues.insert(l_matcher.captured(1).toInt(), itr->toString());
//        }
//    }

//    //    qDebug() << l_regx.pattern();
//    //    QStringList l_listKeys = fields.keys().filter(l_regx);
//    //    l_listKeys.sort();
//    //    for (auto itr = l_listKeys.constBegin(); itr != l_listKeys.constEnd(); itr++) {
//    //        strListValues.append(fields[*itr].toString());
//    //    }
//    strFailure = QString{};
//    return true;
//}

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
