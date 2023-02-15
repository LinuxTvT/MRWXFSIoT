#include "unit.h"
#include "service/printer/form/xfs_form.h"

bool Unit::setBase(const QString &strBase)
{
    if (BASE.contains(strBase)) {
        m_strBase = BASE.value(strBase);
        return true;
    } else {
        return false;
    }
}

bool Unit::load(const QString &strPara)
{
    Element::load(strPara);
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() != numOfPara()) {
        return false;
    } else {
        if (!setBase(l_listPara[0])) {
            return false;
        }
        m_iX = l_listPara[1].toInt();
        m_iY = l_listPara[2].toInt();
    }
    return true;
}

bool Unit::dump2Json(QJsonObject &jsonObject)
{
    jsonObject["base"] = m_strBase;
    jsonObject["unitX"] = m_iX;
    jsonObject["unitY"] = m_iY;
    return true;
}
