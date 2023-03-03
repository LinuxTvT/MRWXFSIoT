#ifndef XFSFORM_H
#define XFSFORM_H

#include "selfservice_object.h"
#include "service/printer/form/element/alignment.h"
#include "service/printer/form/element/number_element.h"
#include "service/printer/form/element/unit.h"
#include "service/printer/form/element/version.h"
#include "service/printer/form/text_line_producer.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_frame.h"
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
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
    bool build();

    inline const QMap<Position, XFSField *> &fieldMapByPosition() const { return m_fieldsMapByPosition; };
    inline const QHash<QString, XFSFrame *> &frames() const { return m_framesHashByName; }
    inline const QHash<QString, XFSField *> &fields() const { return m_fieldsHashByName; }
    inline const Unit &unit() const { return m_oUnit; }
    inline const Size &size() const { return m_oSize; }
    inline const Alignment &alignment() const { return m_oAlignment; }

    XFSField *field(const QString &strName) const;
    QStringList fieldsName();
    int dumpFieldsName2Json(QJsonArray &jaResults) const;
    int dumpFileds2Json(QJsonObject &joResults) const;
    int dumpFields2StringList(QStringList &strLst) const;

    // BlockElement interface
public:
    bool dump2Json(QJsonObject &jsonObject) const override;
    bool parse(TextLineProducer &lineProducer) override;

private:
    // UNIT
    Unit m_oUnit;
    // SIZE
    Size m_oSize;
    // ALIGNMENT
    Alignment m_oAlignment;
    // ORIENTATION
    StringElement m_strOrientation{ Element::KW_ORIENTATION };
    // SKEW
    NumberElement m_iSkew{ Element::KW_SKEW, "skew" };
    // VERSION
    Version m_oVersion;
    // CPI
    NumberElement m_iCPI{ Element::KW_CPI };
    // LPI
    NumberElement m_iLPI{ Element::KW_CPI };
    // POINTSIZE
    NumberElement m_iPointSize{ Element::KW_POINTSIZE };
    // COPYRIGHT
    StringElement m_strCopyRight{ Element::KW_COPYRIGHT };
    // TITLE
    StringElement m_strTitle{ Element::KW_TITLE };
    // LANGUAGE
    NumberElement m_iLanguage{ Element::KW_LANGUAGE };
    // COMMENT
    StringElement m_strComment{ Element::KW_COMMENT };

    bool addField(XFSField *pNewField);
    bool addFrame(XFSFrame *pNewFrame);

    QHash<QString, XFSField *> m_fieldsHashByName;
    QHash<QString, XFSFrame *> m_framesHashByName;
    QMap<Position, XFSField *> m_fieldsMapByPosition;
};

#endif // XFSFORM_H
