#ifndef XFSFORMREPOSITORY_H
#define XFSFORMREPOSITORY_H

#include <QObject>
#include "selfservice_object.h"
#include "service/printer/form/xfs_form.h"

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
    const XFSForm *form(const QString &strName) const;

private:
    void pushForm(XFSForm *pForm);
    QHash<QString, XFSForm *> m_formsHashByName;
};

#endif // XFSFORMREPOSITORY_H
