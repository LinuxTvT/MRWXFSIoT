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
    explicit Unit();
    explicit Unit(const Unit &other);
    virtual ~Unit();

    inline const QString &base() const { return m_strBase; }
    inline int x() const { return m_iX; }
    inline int y() const { return m_iY; }
    bool setBase(const QString &strBase);

    // Element interface
public:
    virtual bool load(const QString &strPara) override;
    virtual bool dump2Json(QJsonObject &jsonObject) const override;

private:
    QString m_strBase = "mm";
    int m_iX = 1;
    int m_iY = 1;
};

}

#endif // UNIT_H
