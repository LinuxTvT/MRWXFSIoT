#include "string_element.h"

using namespace xfs_printer;

StringElement::StringElement(const QString &strKeyWord, //
                             const QString &strDefaultValue, //
                             QHash<QString, QString> *pValues, //
                             const QString &strJsonKey)
    : Element{ strKeyWord, 1, strJsonKey }, m_strValue(strDefaultValue), m_pValues(pValues)
{
}

bool StringElement::load(const QString &strPara)
{
    if (m_pValues == nullptr) {
        m_strValue = strPara.mid(1, strPara.length() - 2);
        return true;
    } else {
        if (m_pValues->contains(strPara)) {
            m_strValue = m_pValues->value(strPara);
            return true;
        } else {
            return false;
        }
    }
}

bool StringElement::dump2Json(QJsonObject &jsonObject) const
{
    if (!jsonKey().isEmpty()) {
        jsonObject[jsonKey()] = this->m_strValue;
    }
    return true;
}
