#ifndef BMPPRINTER_H
#define BMPPRINTER_H

#include "selfservice_object.h"
#include "service/printer/form/element/size.h"
#include "service/printer/form/element/unit.h"
#include "service/printer/form/xfs_field.h"
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QSize>

namespace xfs_printer {

class BMPPrinter : public SelfServiceObject
{
public:
    explicit BMPPrinter(const Unit &oUnit, const Size &oSize, SelfServiceObject *pParent);
    virtual ~BMPPrinter();
    void printField(const XFSField *pField, const QMap<int, QString> &values, bool isFollows = false);
    void printField(const XFSField *pField, const QString &value, bool isFollows = false);
    void printFrame(XFSFrame *pFrame);
    void printToFile(const QString &strPath);

protected:
    void printString(int x, int y, const QString &str);
    void printString(const QString &str);
    void printField(const XFSField *pField, const QRect &rect, const QString &value);

private:
    QSize toPixel(const QSize &size);
    QPoint toPixel(const QPoint &point);
    QRect toPixel(const QRect &rect);
    inline int toPixelHeight(int iHeight) { return iHeight * m_unitSize.height(); }
    inline int toPixelWidth(int iWidth) { return iWidth * m_unitSize.width(); }

private:
    Unit m_oUnit;
    Size m_oSize;
    int m_iCurX;
    int m_iCurY;
    int m_iCPI = 15;
    int m_iLPI = 8;
    qreal m_rPixelSize = 0.125; //    mm
    QSizeF m_unitSize;
    QImage *m_pImage = nullptr;
    QPainter *m_pPainter = nullptr;
};

}

#endif // BMPPRINTER_H
