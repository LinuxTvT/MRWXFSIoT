#ifndef UNIT_H
#define UNIT_H

#include "element.h"

namespace xfs_printer {

class Unit : public Element
{

public:
    inline static QHash<QString, QString> BASE{ { "MM", "mm" }, //
                                                { "INCH", "inch" }, //
                                                { "ROWCOLUMN", "rowColumn" } };
    Unit();
    Unit(const Unit &other);

    inline const QString &base() const { return m_strBase; }
    inline int x() const { return m_iX; }
    inline int y() const { return m_iY; }
    bool setBase(const QString &strBase);

    virtual bool load(const QString &strPara) override;

private:
    QString m_strBase = "mm";
    int m_iX = 1;
    int m_iY = 1;

    // Element interface
public:
    virtual bool dump2Json(QJsonObject &jsonObject) override;
};

}

#endif // UNIT_H
