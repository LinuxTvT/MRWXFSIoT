#include "xfs_form_repository.h"
#include "service/printer/form/text_line_producer.h"
#include "service/printer/form/xfs_media.h"
#include "service/printer/printer_service.h"
#include <QStack>

XFSFormRepository::XFSFormRepository(PrinterService *parent) : SelfServiceObject{ parent }
{
    setLogger(parent->logger());
}

XFSFormRepository::~XFSFormRepository()
{
    debug("Destroy XFSFormRepository");
}

bool XFSFormRepository::loadForms(const QString &strFile)
{
    QFile file(strFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    } else {
        QTextStream l_txtStream{ &file };
        bool l_iSuccess = loadForms(l_txtStream);
        file.close();
        return l_iSuccess;
    }
}

bool XFSFormRepository::loadForms(QTextStream &txtStream)
{
    TextLineProducer l_oLineProducer(&txtStream);
    QString &l_strCurLine = l_oLineProducer.nextLine();
    while (!l_strCurLine.isEmpty()) {
        QRegularExpressionMatch match = XFSForm::REGX_KEYWORD.match(l_strCurLine);
        if (match.hasMatch()) {
            QString l_strKeyWord = match.captured(1);
            QString l_StrParameter = match.captured(3);
            debug(QString("part: [%1] [%2]").arg(l_strKeyWord, l_StrParameter));
            if (l_strKeyWord == Element::KW_XFSFORM) {
                XFSForm *l_pNewForm = new XFSForm(l_StrParameter.remove("\""), this);
                if (l_pNewForm->parse(l_oLineProducer)) {
                    if (l_pNewForm->build()) {
                        pushForm(l_pNewForm);
                    } else {
                        error(QString("build form [%1] ERROR").arg(l_pNewForm->name()));
                        l_pNewForm->deleteLater();
                        return false;
                    }
                } else {
                    error(QString("Load form [%1] ERROR").arg(l_pNewForm->name()));
                    l_pNewForm->deleteLater();
                    return false;
                }
            } else if (l_strKeyWord == Element::KW_XFSMEDIA) {
                XFSMedia *l_pNewMedia = new XFSMedia(l_StrParameter.remove("\""), this);
                if (l_pNewMedia->parse(l_oLineProducer)) {
                    pushMedia(l_pNewMedia);
                } else {
                    error(QString("Load media [%1] ERROR").arg(l_pNewMedia->name()));
                    l_pNewMedia->deleteLater();
                    return false;
                }
            } else {
                error(QString("Key Word [%1] out off form define").arg(l_strKeyWord));
                return false;
            }
        } else {
            error(QString("Line invalid [%1]").arg(l_strCurLine));
            return false;
        }
        l_strCurLine = l_oLineProducer.nextLine();
    }
    return true;
}

int XFSFormRepository::dumpFormsName2Json(QJsonArray &jaResults)
{
    for (auto itr = m_formsHashByName.constBegin(); itr != m_formsHashByName.constEnd(); itr++) {
        jaResults.append((*itr)->name());
    }
    return m_formsHashByName.size();
}

int XFSFormRepository::dumpMediasName2Json(QJsonArray &jaResults)
{
    for (auto itr = m_mediasHashByName.constBegin(); itr != m_mediasHashByName.constEnd(); itr++) {
        jaResults.append((*itr)->name());
    }
    return m_formsHashByName.size();
}

const XFSForm *XFSFormRepository::form(const QString &strName) const
{
    return m_formsHashByName.value(strName, nullptr);
}

void XFSFormRepository::pushForm(XFSForm *pForm)
{
    if (m_formsHashByName.contains(pForm->name())) {
        warn(QString("Form [%1] ready push").arg(pForm->name()));
        m_formsHashByName.take(pForm->name())->deleteLater();
    }
    m_formsHashByName.insert(pForm->name(), pForm);
}

void XFSFormRepository::pushMedia(XFSMedia *pMedia)
{
    if (m_mediasHashByName.contains(pMedia->name())) {
        warn(QString("Form [%1] ready push").arg(pMedia->name()));
        m_mediasHashByName.take(pMedia->name())->deleteLater();
    }
    m_mediasHashByName.insert(pMedia->name(), pMedia);
}
