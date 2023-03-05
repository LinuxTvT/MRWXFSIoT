#ifndef ABSTRACTPRINTERSERVICE_H
#define ABSTRACTPRINTERSERVICE_H

#include "service/abstract_service.h"
#include "service/printer/form/xfs_form.h"
#include <QObject>

class AbstractPrinterService : public AbstractService
{
    Q_OBJECT
public:
    explicit AbstractPrinterService(const QString &strName, //
                                    const QString &strFileConfig);
    virtual ~AbstractPrinterService();

    // XFS IoT fucntions
    virtual SERVICE_FUNCTION(Printer, GetFormList) = 0;
    virtual SERVICE_FUNCTION(Printer, GetMediaList) = 0;
    virtual SERVICE_FUNCTION(Printer, GetQueryForm) = 0;
    virtual SERVICE_FUNCTION(Printer, GetQueryField) = 0;
    virtual SERVICE_FUNCTION(Printer, PrintForm) = 0;
    virtual SERVICE_FUNCTION(Printer, Reset) = 0;

public:
    void notifyFieldEvent(XFSIoTCommandEvent *pCommandEvent, //
                          const QString &strEventName, //
                          const QString &strFormName, //
                          const QString &strFieldName, //
                          const QString &strFailure);

    void notifyFieldWarningEvent(XFSIoTCommandEvent *pCommandEvent, //
                                 const XFSForm *pForm, //
                                 const XFSField *pField, //
                                 const QString &strFailure);
    void notifyFieldWarningEvent(XFSIoTCommandEvent *pCommandEvent, //
                                 const XFSForm *pForm, //
                                 const QString &strFieldName, //
                                 const QString &strFailure);
    void notifyFieldErrorEvent(XFSIoTCommandEvent *pCommandEvent, //
                               const XFSForm *pForm, //
                               const XFSField *pField, //
                               const QString &strFailure);
    void notifyFieldErrorEvent(XFSIoTCommandEvent *pCommandEvent, //
                               const XFSField *pField, //
                               const QString &strFailure);

    void notifyFieldErrorEvent(XFSIoTCommandEvent *pCommandEvent, //
                               const XFSForm *pForm, //
                               const QString &strFieldName, //
                               const QString &strFailure);
};

#endif // ABSTRACTPRINTERSERVICE_H
