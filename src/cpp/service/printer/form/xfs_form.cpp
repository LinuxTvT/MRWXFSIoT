#include "xfs_form.h"
#include "qjsonobject.h"

#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form_repository.h"
#include "service/printer/form/xfs_frame.h"

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

bool XFSForm::parse(TextLineProducer &lineProducer)
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
                    if (l_pNewField->parse(lineProducer)) {
                        addField(l_pNewField);
                    } else {
                        error(QString("Load field [%1] ERROR").arg(l_pNewField->name()));
                        l_pNewField->deleteLater();
                        return false;
                    }
                } else if (l_strKeyWord == Element::KW_XFSFRAME) {
                    l_StrParameter.remove(0, 1);
                    l_StrParameter.chop(1);
                    XFSFrame *l_pNewFrame = new XFSFrame(l_StrParameter, this);
                    if (l_pNewFrame->parse(lineProducer)) {
                        addFrame(l_pNewFrame);
                    } else {
                        error(QString("Load field [%1] ERROR").arg(l_pNewFrame->name()));
                        l_pNewFrame->deleteLater();
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

bool XFSForm::build()
{
    for (auto itr = m_framesHashByName.constBegin(); itr != m_framesHashByName.constEnd(); itr++) {
        QString l_strFieldName = (*itr)->fieldName();
        if (!l_strFieldName.isEmpty()) {
            XFSField *l_pField = field(l_strFieldName);
            if (l_pField == nullptr) {
                error(QString("Can not find field [%1] for frame [%2]").arg(l_strFieldName, (*itr)->name()));
                return false;
            } else {
                l_pField->setFrame(*itr);
            }
        }
        QString l_strTileFieldName = (*itr)->tileFieldName();
        if (!l_strTileFieldName.isEmpty()) {
            XFSField *l_pField = field(l_strTileFieldName);
            if (l_pField == nullptr) {
                error(QString("Can not find field [%1] for tile of frame [%2]").arg(l_strFieldName, (*itr)->name()));
                return false;
            } else {
                l_pField->setTileOfFrame(*itr);
            }
        }
    }
    // Check field flow
    for (auto itr = m_fieldsHashByName.constBegin(); itr != m_fieldsHashByName.constEnd(); itr++) {
        QString l_strFieldFollowsName = (*itr)->follows().value();
        if (!l_strFieldFollowsName.isEmpty()) {
            XFSField *l_pField = field(l_strFieldFollowsName);
            if (l_pField == nullptr) {
                error(QString("Can not find field [%1] to follows [%2]").arg(l_strFieldFollowsName, (*itr)->name()));
                return false;
            } else {
                (*itr)->setFieldFollows(l_pField);
            }
        }
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

XFSField *XFSForm::field(const QString &strName) const
{

    return m_fieldsHashByName.value(strName, nullptr);
}

void XFSForm::addField(XFSField *pNewField)
{
    m_fieldsHashByName.insert(pNewField->name(), pNewField);
    m_fieldsMapByPosition.insert(pNewField->position(), pNewField);
}

void XFSForm::addFrame(XFSFrame *pNewFrame)
{
    m_framesHashByName.insert(pNewFrame->name(), pNewFrame);
}

bool XFSForm::dump2Json(QJsonObject &jsonObject) const
{
    jsonObject["formName"] = this->name();
    return BlockElement::dump2Json(jsonObject);
}
