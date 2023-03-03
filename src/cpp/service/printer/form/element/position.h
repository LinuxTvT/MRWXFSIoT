#ifndef POSITION_H
#define POSITION_H

#include "element.h"
#include <QPoint>

namespace xfs_printer {

class Position : public Element
{
public:
    Position() : Element(Element::KW_POSITION, 2) { }
    Position(const Position &other);

    bool operator<(Position const &obj) const;
    inline int x() const { return m_qPoint.x(); }
    inline int y() const { return m_qPoint.y(); }
    inline int z() const { return m_iZ; }
    inline const QPoint &qPoint() const { return m_qPoint; }

    // Element interface
public:
    virtual bool load(const QString &strPara) override;

private:
    QPoint m_qPoint;
    int m_iZ;
};

}
#endif // POSITION_H
