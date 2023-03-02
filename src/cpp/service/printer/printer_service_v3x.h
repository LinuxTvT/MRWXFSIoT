#ifndef PRINTERSERVICEV3X_H
#define PRINTERSERVICEV3X_H

#include "XFSPTR.H"
#include "device/v3x/device_worker_v3x.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "service/abstract_service.h"
#include "service/printer/abstract_printer_service.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/form/xfs_form_repository.h"
#include "xfs_iot_standard.h"
#include <QObject>

class PrinterServiceV3x : public AbstractPrinterService
{
    Q_OBJECT

    // static
public:
    inline static QHash<int, QString> FIELD_TYPE_HASH = {
        { WFS_FRM_FIELDTEXT, XFSIoTStandard::JV_FIELD_TYPE_text }, //
        { WFS_FRM_FIELDMICR, XFSIoTStandard::JV_FIELD_TYPE_micr }, //
        { WFS_FRM_FIELDOCR, XFSIoTStandard::JV_FIELD_TYPE_ocr }, //
        { WFS_FRM_FIELDMSF, XFSIoTStandard::JV_FIELD_TYPE_msf }, //
        { WFS_FRM_FIELDBARCODE, XFSIoTStandard::JV_FIELD_TYPE_barcode }, //
        { WFS_FRM_FIELDGRAPHIC, XFSIoTStandard::JV_FIELD_TYPE_graphic }, //
        { WFS_FRM_FIELDPAGEMARK, XFSIoTStandard::JV_FIELD_TYPE_pagemark }
    };

    inline static QHash<int, QString> FIELD_CLASS_HASH = {
        { WFS_FRM_CLASSSTATIC, XFSIoTStandard::JV_FIELD_CLASS_static }, //
        { WFS_FRM_CLASSOPTIONAL, XFSIoTStandard::JV_FIELD_CLASS_optional }, //
        { WFS_FRM_CLASSREQUIRED, XFSIoTStandard::JV_FIELD_CLASS_required }
    };

    inline static QHash<int, QString> FIELD_ACCESS_HASH = {
        { WFS_FRM_ACCESSREAD, XFSIoTStandard::JV_FIELD_ACCESS_read }, //
        { WFS_FRM_ACCESSWRITE, XFSIoTStandard::JV_FIELD_ACCESS_write }, //
        { WFS_FRM_ACCESSREAD | WFS_FRM_ACCESSWRITE, XFSIoTStandard::JV_FIELD_ACCESS_readWrite }
    };

    inline static QHash<int, QString> FIELD_OVERFLOW_HASH = {
        { WFS_FRM_OVFTERMINATE, XFSIoTStandard::JV_FIELD_OVERFLOW_terminate }, //
        { WFS_FRM_OVFTRUNCATE, XFSIoTStandard::JV_FIELD_OVERFLOW_truncate }, //
        { WFS_FRM_OVFBESTFIT, XFSIoTStandard::JV_FIELD_OVERFLOW_bestFit },
        { WFS_FRM_OVFOVERWRITE, XFSIoTStandard::JV_FIELD_OVERFLOW_overwrite }, //
        { WFS_FRM_OVFWORDWRAP, XFSIoTStandard::JV_FIELD_OVERFLOW_wordWrap } //
    };

public:
    Q_INVOKABLE explicit PrinterServiceV3x(const QString &strName, //
                                           const QString &strFileConfig);
    virtual ~PrinterServiceV3x();

    SERVICE_FUNCTION(Common, Status) override;

    // STATUS_FUNCTION(Printer) override;
    SERVICE_FUNCTION(Printer, GetFormList) override;
    SERVICE_FUNCTION(Printer, GetMediaList) override;
    SERVICE_FUNCTION(Printer, GetQueryForm) override;
    SERVICE_FUNCTION(Printer, GetQueryField) override;
    SERVICE_FUNCTION(Printer, PrintForm) override;
    SERVICE_FUNCTION(Printer, Reset) override;

private:
    DeviceWorkerV3x *m_pDeviceWorkerWrap;
    void dumpField(LPWFSFRMFIELD lpWfsFrmField, QJsonObject &jvFields);

    // AbstractService interface
protected:
    virtual AbstractDeviceWorker *loadDeviceWorker(const QJsonValue &joProperties) override;
};

#endif // PRINTERSERVICEV3X_H
