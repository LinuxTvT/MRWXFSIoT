#ifndef TEXTLINEPRODUCER_H
#define TEXTLINEPRODUCER_H

#include "selfservice_object.h"
#include <QObject>

class TextLineProducer : public SelfServiceObject
{
    Q_OBJECT

public:
    explicit TextLineProducer(QTextStream *pStream);
    virtual QString &nextLine();
    inline QString &currentLine() { return m_strCurLine; }

private:
    QTextStream *m_pStream;
    QString m_strCurLine;
};

#endif // TEXTLINEPRODUCER_H
