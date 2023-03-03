#include "alignment.h"
#include "service/printer/form/xfs_form.h"

Alignment::Alignment() : Element(Element::KW_ALIGNMENT, 3) { }

Alignment::~Alignment()
{

}

bool Alignment::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() != 3) {
        return false;
    } else {
        if (!setAlignment(l_listPara[0])) {
            return false;
        }
        m_pointOffset.setX(l_listPara[1].toInt());
        m_pointOffset.setY(l_listPara[2].toInt());
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

bool Alignment::dump2Json(QJsonObject &jsonObject) const
{
    jsonObject["alignment"] = m_strAlignment;
    jsonObject["offsetX"] = m_pointOffset.x();
    jsonObject["offsetY"] = m_pointOffset.y();
    return true;
}
