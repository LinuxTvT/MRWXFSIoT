#ifndef ELEMENT_H
#define ELEMENT_H

#include <QJsonObject>
#include <QString>

namespace xfs_printer {

#define KEYWORD(x) inline static const QString KW_##x = QStringLiteral(#x)

#define SET_ENUM_VALUE(m, e, v)                                                                                        \
    if (v == #e) {                                                                                                     \
        m = e;                                                                                                         \
        return true;                                                                                                   \
    }

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
    KEYWORD(TILE);
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
    inline int numOfPara() const { return m_iNumOfPara; }
    inline const QString &jsonKey() const { return m_strJsonKey; }
    inline bool isLoaded() const { return m_isLoaded; }

    virtual bool parsePara(const QString &strPara, QStringList &strListRes);
    virtual bool load(const QString &strPara);
    virtual bool dump2Json(QJsonObject &jsonObject);

private:
    QString m_strKeyWord;
    QString m_strJsonKey;
    int m_iNumOfPara = 0;
    bool m_isLoaded = false;
};

}

#endif // ELEMENT_H
