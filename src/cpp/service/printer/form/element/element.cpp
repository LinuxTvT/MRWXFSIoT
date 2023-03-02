#include "element.h"
#include "qglobal.h"
#include "service/printer/form/xfs_form.h"

Element::Element(const QString &strKeyWord, //
                 int iNumOfPara, //
                 const QString &strJsonKey)
    : m_strKeyWord(strKeyWord), //
      m_strJsonKey(strJsonKey), //
      m_iNumOfPara(iNumOfPara)
{
}

Element::~Element() { }

bool Element::parsePara(const QString &strPara, QStringList &strListRes)
{
    strListRes = strPara.split(XFSForm::REGX_PARA_SEP);
    if (strListRes.size() != this->numOfPara()) {
        return false;
    } else {
        return true;
    }
}

bool Element::load(const QString &strPara)
{
    Q_UNUSED(strPara)
    m_isLoaded = true;
    return true;
}

bool Element::dump2Json(QJsonObject &jsonObject)
{
    Q_UNUSED(jsonObject)
    return true;
}
