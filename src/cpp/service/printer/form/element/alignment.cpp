#include "alignment.h"
#include "service/printer/form/xfs_form.h"

bool Alignment::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() != 3) {
        return false;
    } else {
        if (!setAlignment(l_listPara[0])) {
            return false;
        }
        m_iXOffset = l_listPara[1].toInt();
        m_iYOffset = l_listPara[2].toInt();
    }
    return true;
}

bool Alignment::setAlignment(const QString &strAlignment)
{
    if (ALIGNMENT.contains(strAlignment)) {
        m_strAlignment = ALIGNMENT.value(strAlignment);
        return true;
    } else {
        return false;
    }
}

bool Alignment::dump2Json(QJsonObject &jsonObject)
{
    jsonObject["alignment"] = m_strAlignment;
    jsonObject["offsetX"] = m_iXOffset;
    jsonObject["offsetX"] = m_iYOffset;
    return true;
}
