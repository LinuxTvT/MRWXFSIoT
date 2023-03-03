#include "xfs_form.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form_repository.h"
#include "service/printer/form/xfs_frame.h"
#include <QJsonValue>

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
                        if (!addField(l_pNewField)) {
                            error(QString("Add field [%1] ERROR").arg(l_pNewField->name()));
                            l_pNewField->deleteLater();
                            return false;
                        }
                    } else {
                        error(QString("Parse field [%1] ERROR").arg(l_pNewField->name()));
                        l_pNewField->deleteLater();
                        return false;
                    }
                } else if (l_strKeyWord == Element::KW_XFSFRAME) {
                    l_StrParameter.remove(0, 1);
                    l_StrParameter.chop(1);
                    XFSFrame *l_pNewFrame = new XFSFrame(l_StrParameter, this);
                    if (l_pNewFrame->parse(lineProducer)) {
                        if (!addFrame(l_pNewFrame)) {
                            error(QString("Add frame [%1] ERROR").arg(l_pNewFrame->name()));
                            l_pNewFrame->deleteLater();
                            return false;
                        }
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
    // Check frame link to field
    for (auto itr = m_framesHashByName.constBegin(); itr != m_framesHashByName.constEnd(); itr++) {
        if (!(*itr)->rebuild()) {
            error(QString("Rebuild frame [%1] ERROR").arg((*itr)->name()));
            return false;
        }
    }
    // Check field follows flow
    for (auto itr = m_fieldsHashByName.constBegin(); itr != m_fieldsHashByName.constEnd(); itr++) {
        if (!(*itr)->rebuild()) {
            error(QString("Rebuild field [%1] ERROR").arg((*itr)->name()));
            return false;
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

bool XFSForm::addField(XFSField *pNewField)
{
    if (m_fieldsHashByName.contains(pNewField->name())) {
        error(QString("Form [%1] have duplicate field [%2]").arg(name(), pNewField->name()));
        return false;
    } else {
        m_fieldsHashByName.insert(pNewField->name(), pNewField);
    }

    if (m_fieldsMapByPosition.contains(pNewField->position())) {
        const XFSField *l_pField = m_fieldsMapByPosition.value(pNewField->position());
        error(QString("Form [%1] have field [%2] and field [%3] same position") //
                      .arg(name(), pNewField->name()) //
                      .arg(l_pField->name()));
        return false;
    } else {
        m_fieldsMapByPosition.insert(pNewField->position(), pNewField);
    }
    return true;
}

bool XFSForm::addFrame(XFSFrame *pNewFrame)
{
    if (m_framesHashByName.contains(pNewFrame->name())) {
        error(QString("Form [%1] have duplicate frame [%2]").arg(name(), pNewFrame->name()));
        return false;
    } else {
        m_framesHashByName.insert(pNewFrame->name(), pNewFrame);
        return true;
    }
}
bool XFSForm::dump2Json(QJsonObject &jsonObject) const
{
    jsonObject["formName"] = this->name();
    return BlockElement::dump2Json(jsonObject);
}
