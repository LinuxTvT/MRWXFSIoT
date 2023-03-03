#ifndef VERSION_H
#define VERSION_H

#include "element.h"

namespace xfs_printer {

class Version : public Element
{
public:
    explicit Version();
    virtual ~Version();

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
    virtual bool dump2Json(QJsonObject &jsonObject) const override;

private:
    int m_iMajor;
    int m_iMinor;
    QString m_strDate;
    QString m_strAuthor;
};

}

#endif // VERSION_H
