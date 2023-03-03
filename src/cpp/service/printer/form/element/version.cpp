#include "version.h"
#include "service/printer/form/xfs_form.h"

Version::Version() : Element(Element::KW_VERSION, 4) { }

Version::~Version() { }

bool Version::load(const QString &strPara)
{
    QStringList l_listPara = strPara.split(XFSForm::REGX_PARA_SEP);
    if (l_listPara.size() != numOfPara()) {
        return false;
    } else {
        m_iMajor = l_listPara[0].toInt();
        m_iMinor = l_listPara[1].toInt();
        m_strDate = l_listPara[2];
        m_strAuthor = l_listPara[3];
        return true;
    }
}

bool Version::dump2Json(QJsonObject &jsonObject) const
{
    jsonObject["versionMajor"] = m_iMajor;
    jsonObject["versionMinor"] = m_iMinor;
    return true;
}
