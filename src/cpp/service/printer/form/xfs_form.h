#ifndef XFSFORM_H
#define XFSFORM_H

#include "qjsonarray.h"
#include "qjsonvalue.h"
#include "selfservice_object.h"
#include "service/printer/form/element/alignment.h"
#include "service/printer/form/element/number_element.h"
#include "service/printer/form/element/unit.h"
#include "service/printer/form/element/version.h"
#include "service/printer/form/text_line_producer.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_frame.h"
#include <QHash>
#include <QObject>

class XFSFormRepository;

class XFSForm : public BlockElement
{
    Q_OBJECT
public:
    inline static QRegularExpression REGX_KEYWORD{ "(^\\w+)(\\s+)(.+)" };
    inline static QRegularExpression REGX_PARA_SEP{ "\\s*,\\s*" };

    explicit XFSForm(const QString &strName, XFSFormRepository *parent);
    virtual ~XFSForm();
    bool parse(TextLineProducer &lineProducer) override;
    bool build();

    inline const QMap<Position, XFSField *> &fieldMapByPosition() const { return m_fieldsMapByPosition; };
    inline const QHash<QString, XFSFrame *> &frames() const { return m_framesHashByName; }
    inline const QHash<QString, XFSField *> &fields() const { return m_fieldsHashByName; }
    inline const Unit &unit() const { return m_oUnit; }
    inline const Size &size() const { return m_oSize; }
    inline const Alignment &alignment() const { return m_oAlignment; }

    QStringList fieldsName();
    // QStringList fields();
    int dumpFieldsName2Json(QJsonArray &jaResults) const;
    int dumpFileds2Json(QJsonObject &joResults) const;
    int dumpFields2StringList(QStringList &strLst) const;
    XFSField *field(const QString &strName) const;

private:
    Unit m_oUnit;
    Size m_oSize;
    Alignment m_oAlignment;
    StringElement m_strOrientation{ Element::KW_ORIENTATION };
    NumberElement m_iSkew{ Element::KW_SKEW, "skew" };
    Version m_oVersion;
    int m_iCpi;
    int m_iLpi;
    int m_pointSize;
    NumberElement m_iLanguage{ Element::KW_LANGUAGE };
    StringElement m_strCopyRight{ Element::KW_COPYRIGHT };
    QString m_strTitle{};

    void addField(XFSField *pNewField);
    void addFrame(XFSFrame *pNewFrame);

    QHash<QString, XFSField *> m_fieldsHashByName;
    QHash<QString, XFSFrame *> m_framesHashByName;
    QMap<Position, XFSField *> m_fieldsMapByPosition;

    // BlockElement interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) const override;
};

#endif // XFSFORM_H
