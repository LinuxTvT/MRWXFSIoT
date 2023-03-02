#ifndef XFSFRAME_H
#define XFSFRAME_H

#include "service/printer/form/element/block_element.h"
#include "service/printer/form/element/position.h"
#include "service/printer/form/element/size.h"
#include "service/printer/form/element/string_element.h"
#include <QObject>

class XFSForm;

class XFSFrame : public BlockElement
{
    Q_OBJECT

    // static
private:
    inline static QHash<QString, QString> COLOR{ { "BLACK", "black" }, //
                                                 { "WHITE", "white" }, //
                                                 { "GRAY", "gray" }, //
                                                 { "RED", "red" }, //
                                                 { "BLUE", "blue" }, //
                                                 { "GREEN", "green" }, //
                                                 { "YELLOW", "yellow" } };

    inline static QHash<QString, QString> FIELD_STYLE{ { "NONE", "none" }, //
                                                       { "SOLID", "solid" }, //
                                                       { "BDIAGONAL", "bDiagonal" }, //
                                                       { "CROSS", "cross" }, //
                                                       { "DIAGCROSS", "diagcross" }, //
                                                       { "FDIAGONAL", "fDiagonal" }, //
                                                       { "HORIZONTAL", "horizontal" }, //
                                                       { "VERTICAL", "vertical" } };

public:
    explicit XFSFrame(const QString &strName, XFSForm *parent);
    virtual ~XFSFrame() { }

    inline const Position &position() const { return m_oPosition; }
    inline const Size &size() const { return m_oSize; }
    inline const QString &fieldName() const { return m_strFrames.value(); }
    inline const QString &tileFieldName() const { return m_strTile.value(); }

private:
    Position m_oPosition;
    Size m_oSize{ false };
    StringElement m_strFrames{ Element::KW_FRAMES };
    StringElement m_strColor{ Element::KW_COLOR, "black", &COLOR };
    StringElement m_strFillColor{ Element::KW_FILLCOLOR, "white", &COLOR };
    StringElement m_strFillStyle{ Element::KW_FILLSTYLE, "none", &FIELD_STYLE };
    StringElement m_strTile{ Element::KW_TILE };
};

#endif // XFSFRAME_H
