#include "string_element.h"

using namespace xfs_printer;

StringElement::StringElement(const QString &strKeyWord, const QString &strJsonKey, QHash<QString, QString> *pValues)
    : Element{ strKeyWord, strJsonKey }, m_pValues(pValues)
{
    if (pValues != nullptr && pValues->size() > 0) {
        m_strValue = pValues->constBegin().value();
    }
}

StringElement::StringElement(const QString &strKeyWord, QHash<QString, QString> *pValues)
    : StringElement{ strKeyWord, QString(), pValues }
{
}

bool StringElement::load(const QString &strPara)
{
    Element::load(strPara);
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

bool StringElement::dump2Json(QJsonObject &jsonObject)
{
    if (!this->jsonKey().isEmpty()) {
        jsonObject[this->jsonKey()] = this->m_strValue;
    }
    return true;
}
