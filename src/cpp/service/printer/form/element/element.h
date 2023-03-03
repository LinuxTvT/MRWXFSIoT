#ifndef ELEMENT_H
#define ELEMENT_H

#include <QJsonObject>
#include <QString>

namespace xfs_printer {

#define KEYWORD(x) inline static const QString KW_##x = QStringLiteral(#x)

class Element
{

public:
    KEYWORD(XFSFORM);
    KEYWORD(XFSFIELD);
    KEYWORD(XFSMEDIA);
    KEYWORD(BEGIN);
    KEYWORD(END);
    KEYWORD(UNIT);
    KEYWORD(SIZE);
    KEYWORD(ALIGNMENT);
    KEYWORD(VERSION);
    KEYWORD(SKEW);
    KEYWORD(ORIENTATION);
    KEYWORD(POSITION);
    KEYWORD(CLASS);
    KEYWORD(ACCESS);
    KEYWORD(LANGUAGE);
    KEYWORD(COPYRIGHT);
    KEYWORD(COMMENT);
    KEYWORD(FOLLOWS);
    KEYWORD(OVERFLOW);
    KEYWORD(STYLE);
    KEYWORD(VERTICAL);
    KEYWORD(INITIALVALUE);
    KEYWORD(HORIZONTAL);
    KEYWORD(TYPE);
    KEYWORD(SCALING);
    KEYWORD(FONT);
    KEYWORD(CPI);
    KEYWORD(LPI);
    KEYWORD(TITLE);
    KEYWORD(POINTSIZE);

    // XFSSUBFORM
    KEYWORD(XFSSUBFORM);
    // XFSFRAME
    KEYWORD(XFSFRAME);
    KEYWORD(FRAMES);
    KEYWORD(COLOR);
    KEYWORD(FILLCOLOR);
    KEYWORD(FILLSTYLE);
    // XFSMEIDA
    KEYWORD(INDEX);
    KEYWORD(PRINTAREA);
    KEYWORD(RESTRICTED);
    KEYWORD(STAGGERING);
    KEYWORD(FOLD);
    KEYWORD(LINES);

    explicit Element(const QString &strKeyWord, int iNumOfPara = 1, const QString &strJsonKey = QString{});
    virtual ~Element();

    inline const QString &keyWord() const { return m_strKeyWord; }
    inline const QString &jsonKey() const { return m_strJsonKey; }
    inline int numOfPara() const { return m_iNumOfPara; }

    virtual bool load(const QString &strPara) = 0;
    virtual bool dump2Json(QJsonObject &jsonObject) const;

private:
    QString m_strKeyWord;
    QString m_strJsonKey;
    int m_iNumOfPara = 0;
};

}

#endif // ELEMENT_H
