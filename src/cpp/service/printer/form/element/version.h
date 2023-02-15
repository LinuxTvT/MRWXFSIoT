#ifndef VERSION_H
#define VERSION_H

#include "element.h"

namespace xfs_printer {

class Version : public Element
{
public:
    Version() : Element(Element::KW_VERSION, 4) { }
    virtual bool load(const QString &strPara) override;

private:
    int m_iMajor;
    int m_iMinor;
    QString m_strDate;
    QString m_strAuthor;

    // Element interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};

}

#endif // VERSION_H
