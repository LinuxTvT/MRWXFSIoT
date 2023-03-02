#include "position.h"
#include "service/printer/form/xfs_form.h"

Position::Position(const Position &other) : Position{}
{
    m_qPoint = other.m_qPoint;
    m_iZ = other.m_iZ;
}

bool Position::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() == 2) {
        m_qPoint.setX(l_listPara[0].toInt());
        m_qPoint.setY(l_listPara[1].toInt());
        return true;
    } else if (l_listPara.size() == 3) {
        m_qPoint.setX(l_listPara[0].toInt());
        m_qPoint.setY(l_listPara[1].toInt());
        m_iZ = l_listPara[2].toInt();
        return true;
    } else {
        return false;
    }
}

bool Position::operator<(const Position &obj) const
{
    if (m_qPoint.y() == obj.m_qPoint.y()) {
        return (m_qPoint.x() < obj.m_qPoint.x());
    } else {
        return m_qPoint.y() < obj.m_qPoint.y();
    }
}
