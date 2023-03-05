#ifndef XFSFRAME_H
#define XFSFRAME_H

#include "service/printer/form/element/block_element.h"
#include "service/printer/form/element/position.h"
#include "service/printer/form/element/size.h"
#include "service/printer/form/element/string_element.h"
#include <QObject>
#include <QPoint>
#include <QRect>

class XFSForm;
class XFSField;

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
    inline const QPoint &topLeft() const { return m_oPosition.qPoint(); }
    inline const Size &size() const { return m_oSize; }
    inline const QSize &qSize() const { return m_oSize.qSize(); }
    inline int width() const { return qSize().width(); }
    inline int height() const { return qSize().height(); }
    inline QRect rect() const { return QRect{ topLeft(), qSize() }; }

    inline const XFSField *titleField() const { return m_pTitleField; }

    QRect titlelRect(const QSize &titleSize) const;

    bool rebuild();

private:
    void setContentField(XFSField *pContentField);
    void setTitleField(XFSField *pTitleField);

private:
    // POSITION
    Position m_oPosition;
    // FRAMES
    StringElement m_strFrames{ Element::KW_FRAMES };
    // HEADER
    // FOOTER
    // SIDE
    // SIZE
    Size m_oSize{ false };
    // REPEATONX
    // REPEATONY
    // TYPE
    // CLASS
    // OVERFLOW
    // STYLE
    // COLOR
    StringElement m_strColor{ Element::KW_COLOR, "black", &COLOR };
    // RGBCOLOR
    // FILLCOLOR
    StringElement m_strFillColor{ Element::KW_FILLCOLOR, "white", &COLOR };
    // RGBFILLCOLOR
    // FILLSTYLE
    StringElement m_strFillStyle{ Element::KW_FILLSTYLE, "none", &FIELD_STYLE };
    // SUBSTSIGN
    // TILE
    StringElement m_strTitle{ Element::KW_TITLE };
    // HORIZONTAL
    // VERTICAL

    XFSForm *m_pForm = nullptr;
    XFSField *m_pContentField = nullptr;
    XFSField *m_pTitleField = nullptr;
};

#endif // XFSFRAME_H
