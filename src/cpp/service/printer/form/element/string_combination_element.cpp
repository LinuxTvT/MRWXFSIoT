#include "string_combination_element.h"

using namespace xfs_printer;

StringCombinationElement::StringCombinationElement(const QString &strKeyWord, //
                                                   const QString &strDefaultValue, //
                                                   QHash<QString, QString> *pValues, //
                                                   const QString &strJsonKey)
    : StringElement(strKeyWord, strDefaultValue, pValues, strJsonKey)
{
}

StringCombinationElement::~StringCombinationElement() { }

bool StringCombinationElement::load(const QString &strPara)
{
    Element::load(strPara);
    setValue(strPara);
    QStringList l_listKeywords = value().split('|');
    for (auto itr = l_listKeywords.constBegin(); itr != l_listKeywords.constEnd(); itr++) {
        if (defineValues() == nullptr) {
            m_strListValues.append(*itr);
        } else {
            if (defineValues()->contains(*itr)) {
                m_strListValues.append(defineValues()->value(*itr));
            } else {
                return false;
            }
        }
    }
    return true;
}
