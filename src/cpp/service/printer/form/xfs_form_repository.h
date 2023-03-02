#ifndef XFSFORMREPOSITORY_H
#define XFSFORMREPOSITORY_H

#include "selfservice_object.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/form/xfs_media.h"
#include <QObject>

class PrinterService;

class XFSFormRepository : public SelfServiceObject
{
    Q_OBJECT
public:
    explicit XFSFormRepository(PrinterService *parent);
    virtual ~XFSFormRepository();
    bool loadForms(const QString &strFile);
    bool loadForms(QTextStream &txtStream);
    int dumpFormsName2Json(QJsonArray &jaResults);
    int dumpMediasName2Json(QJsonArray &jaResults);
    const XFSForm *form(const QString &strName) const;

private:
    void pushForm(XFSForm *pForm);
    void pushMedia(XFSMedia *pMedia);
    QHash<QString, XFSForm *> m_formsHashByName;
    QHash<QString, XFSMedia *> m_mediasHashByName;
};

#endif // XFSFORMREPOSITORY_H
