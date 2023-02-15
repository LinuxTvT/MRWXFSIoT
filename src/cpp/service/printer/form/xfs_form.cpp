#include "xfs_form.h"
#include "qjsonobject.h"

#include "service/printer/form/xfs_form_repository.h"

XFSForm::XFSForm(const QString &strName, XFSFormRepository *parent) : BlockElement{ strName, parent }
{
    setLogger(parent->logger());
    addElement(&m_oUnit);
    addElement(&m_oSize);
    addElement(&m_oAlignment);
    addElement(&m_iSkew);
    addElement(&m_oVersion);
    addElement(&m_iLanguage);
    addElement(&m_strCopyRight);
}

XFSForm::~XFSForm() { }

bool XFSForm::load(TextLineProducer &lineProducer)
{
    if (lineProducer.nextLine() == "BEGIN") {
        while (lineProducer.nextLine() != "END") {
            QRegularExpressionMatch match = XFSForm::REGX_KEYWORD.match(lineProducer.currentLine());
            if (match.hasMatch()) {
                QString l_strKeyWord = match.captured(1);
                QString l_StrParameter = match.captured(3);
                if (l_strKeyWord == Element::KW_XFSFIELD) {
                    l_StrParameter.remove(0, 1);
                    l_StrParameter.chop(1);

                    XFSField *l_pNewField = new XFSField(l_StrParameter, this);
                    if (l_pNewField->load(lineProducer)) {
                        addField(l_pNewField);
                    } else {
                        error(QString("Load field [%1] ERROR").arg(l_pNewField->name()));
                        l_pNewField->deleteLater();
                        return false;
                    }
                } else {
                    if (this->loadElement(l_strKeyWord, l_StrParameter)) {
                        debug(QString("Load Element [%1] success").arg(l_strKeyWord));
                    } else {
                        error(QString("Load Element [%1] ERROR").arg(l_strKeyWord));
                        return false;
                    }
                }
            } else {
                error(QString("Line invalid [%1]").arg(lineProducer.currentLine()));
                return false;
            }
        }
    } else {
        error(QString("Require BEGIN at [%1]").arg(lineProducer.currentLine()));
        return false;
    }
    return true;
}

QStringList XFSForm::fieldsName()
{
    QStringList l_strListTemp;
    for (auto itr = m_fieldsHashByName.constBegin(); itr != m_fieldsHashByName.constEnd(); itr++) {
        l_strListTemp.append((*itr)->name());
    }
    return l_strListTemp;
}

int XFSForm::dumpFieldsName2Json(QJsonArray &jaResults) const
{
    for (auto itr = m_fieldsHashByName.constBegin(); itr != m_fieldsHashByName.constEnd(); itr++) {
        jaResults.append((*itr)->name());
    }
    return m_fieldsHashByName.size();
}

int XFSForm::dumpFileds2Json(QJsonObject &joResults) const
{
    for (auto itr = m_fieldsHashByName.constBegin(); itr != m_fieldsHashByName.constEnd(); itr++) {
        QJsonObject l_joField;
        (*itr)->dump2Json(l_joField);
        joResults[(*itr)->name()] = l_joField;
    }
    return m_fieldsHashByName.size();
}

int XFSForm::dumpFields2StringList(QStringList &strLst) const
{
    for (auto itr = m_fieldsMapByPosition.constBegin(); itr != m_fieldsMapByPosition.constEnd(); itr++) {
        strLst.append((*itr)->initialvalue());
    }
    return m_fieldsHashByName.size();
}

const XFSField *XFSForm::field(const QString &strName) const
{

    return m_fieldsHashByName.value(strName, nullptr);
}

void XFSForm::addField(XFSField *pNewField)
{
    m_fieldsHashByName.insert(pNewField->name(), pNewField);
    m_fieldsMapByPosition.insert(pNewField->position(), pNewField);
}

bool XFSForm::dump2Json(QJsonObject &jsonObject) const
{
    jsonObject["formName"] = this->name();
    return BlockElement::dump2Json(jsonObject);
}
