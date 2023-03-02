#include "xfs_field.h"
#include "qjsonvalue.h"
#include "service/printer/form/xfs_form.h"

XFSField::XFSField(const QString &strName, XFSForm *parent) : BlockElement{ strName, parent }, m_pForm(parent)
{
    setLogger(parent->logger());
    addElement(&m_oPosition);
    addElement(&m_strFollows);
    addElement(&m_strOverflow);
    addElement(&m_strStyle);
    addElement(&m_strVertical);
    addElement(&m_strHorizontal);
    addElement(&m_strInitialvalue);
    addElement(&m_oSize);
    addElement(&m_strType);
    addElement(&m_strScaling);
    addElement(&m_strClass);
    addElement(&m_strAccess);
    addElement(&m_oIndex);
    addElement(&m_strFont);
    addElement(&m_iPointSize);
    addElement(&m_iCPI);
    addElement(&m_iLPI);

    m_regxIndexFieldNameFilter.setPattern(QString(REGX_FIELD_INDEX_KEY_FORMAT).arg(name()));
}

// QRegularExpression XFSField::fieldNameRegularExpression() const
//{
//    return QRegularExpression{ QString(REGX_FIELD_INDEX_KEY_FORMAT).arg(name()) };
//}

QString XFSField::checkFieldValue(const QJsonValue jvValue) const
{
    if (jvValue.isUndefined()) {
        if (m_strClass.is("required")) {
            return QString{ "required" };
        }
    } else {
        if (jvValue.isString()) {
            if (m_strClass.is("static")) {
                return QString{ "staticOverwrite" };
            }
        } else {
            return QString{ "notSupported" };
        }
    }
    return QString{};
}

QString XFSField::checkFieldValue(const QJsonObject joFieldsPayload) const
{
    if (isIndexField()) {
        if (m_strClass.is("required")) {
            if (joFieldsPayload.keys().filter(fieldNameRegularExpression()).isEmpty()) {
                return QString{ "required" };
            }
        }
    } else {
        return checkFieldValue(joFieldsPayload[name()]);
    }
    return QString{};
}

QString XFSField::printValue(const QJsonObject joFieldsPayload) const
{
    if (m_strClass.is("static")) {
        return initialvalue();
    } else if (m_strClass.is("time")) {
        return QDateTime::currentDateTime().toString();
    } else {
        const QJsonValue &l_jvFieldValue = joFieldsPayload[name()];
        if (l_jvFieldValue.isString()) {
            return l_jvFieldValue.toString();
        } else {
            return initialvalue();
        }
    }
}

int XFSField::printValuesList(const QJsonObject joFieldsPayload, //
                              QMap<int, QString> &valuesList) const
{
    if (isIndexField()) {
        int l_iMaxIndex = 0;
        for (auto itr = joFieldsPayload.constBegin(); itr != joFieldsPayload.constEnd(); itr++) {
            QRegularExpressionMatch l_matcher = m_regxIndexFieldNameFilter.match(itr.key());
            if (l_matcher.hasMatch()) {
                int l_iCurIndex = l_matcher.captured(1).toInt();
                valuesList.insert(l_iCurIndex, itr->toString());
                if (l_iCurIndex > l_iMaxIndex) {
                    l_iMaxIndex = l_iCurIndex;
                }
            }
        }
        return l_iMaxIndex;
    } else {
        valuesList.insert(0, printValue(joFieldsPayload));
        return 0;
    }
}

void XFSField::setFrame(XFSFrame *pFrame)
{
    if (m_pFrame != nullptr) {
        warn(QString("Double frame of field [%1]").arg(name()));
    }
    m_pFrame = pFrame;
}

void XFSField::setTileOfFrame(XFSFrame *pFrame)
{
    if (m_pTitleOfFrame != nullptr) {
        warn(QString("Double tile of frame of field [%1]").arg(name()));
    }
    m_pTitleOfFrame = pFrame;
}

void XFSField::setFieldFollows(XFSField *pField)
{
    m_pPreviousFollows = pField;
    pField->m_pNextFollows = this;
}
