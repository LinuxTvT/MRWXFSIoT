#include "abstract_printer_service.h"

AbstractPrinterService::AbstractPrinterService(const QString &strFileConfig) : AbstractService{ strFileConfig }
{ //
}

AbstractPrinterService::~AbstractPrinterService()
{ //
}

void AbstractPrinterService::notifyFieldEvent(XFSIoTCommandEvent *pCommandEvent, //
                                              const QString &strEventName, //
                                              const QString &strFormName, //
                                              const QString &strFieldName, //
                                              const QString &strFailure)
{
    QJsonObject l_joEventPayload;
    l_joEventPayload["formName"] = strFormName;
    l_joEventPayload["fieldName"] = strFieldName;
    l_joEventPayload["failure"] = strFailure;
    notifyEventEvent(pCommandEvent, //
                     strEventName, //
                     l_joEventPayload);
}

void AbstractPrinterService::notifyFieldWarningEvent(XFSIoTCommandEvent *pCommandEvent, const XFSForm *pForm,
                                                     const QString &strFieldName, const QString &strFailure)
{
    // 13.3.5 Printer.FieldWarningEvent
    notifyFieldEvent(pCommandEvent, //
                     "Printer.FieldWarningEvent", //
                     pForm->name(), //
                     strFieldName, //
                     strFailure);
}

void AbstractPrinterService::notifyFieldWarningEvent(XFSIoTCommandEvent *pCommandEvent, const XFSForm *pForm,
                                                     const XFSField *pField, const QString &strFailure)
{
    notifyFieldWarningEvent(pCommandEvent, pForm, pField->name(), strFailure);
}

void AbstractPrinterService::notifyFieldErrorEvent(XFSIoTCommandEvent *pCommandEvent, //
                                                   const XFSForm *pForm, //
                                                   const QString &strFieldName, //
                                                   const QString &strFailure)
{
    // 13.3.4 Printer.FieldErrorEvent
    notifyFieldEvent(pCommandEvent, //
                     "Printer.FieldErrorEvent", //
                     pForm->name(), //
                     strFieldName, //
                     strFailure);
}

void AbstractPrinterService::notifyFieldErrorEvent(XFSIoTCommandEvent *pCommandEvent, //
                                                   const XFSForm *pForm, //
                                                   const XFSField *pField, //
                                                   const QString &strFailure)
{
    notifyFieldErrorEvent(pCommandEvent, pForm, pField->name(), strFailure);
}
