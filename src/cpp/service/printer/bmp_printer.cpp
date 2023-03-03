#include "bmp_printer.h"
#include "qnamespace.h"

using namespace xfs_printer;

BMPPrinter::BMPPrinter(const Unit &oUnit, const Size &oSize, SelfServiceObject *pParent)
    : SelfServiceObject(pParent), m_oUnit(oUnit), m_oSize(oSize)
{
    if (m_oUnit.base() == "rowColumn") {
        qreal l_rLineHeight = (25.4 / m_iLPI) / m_rPixelSize; // pixel
        qreal l_rCharacterWidth = (25.4 / m_iCPI) / m_rPixelSize; // pixel
        m_unitSize = QSizeF{ l_rCharacterWidth / m_oUnit.x(), //
                             l_rLineHeight / m_oUnit.y() };
    } else {
        qreal l_realPixelOfBase = 0;
        if (m_oUnit.base() == "inch") {
            l_realPixelOfBase = 2.54 / m_rPixelSize;
        } else {
            l_realPixelOfBase = 1 / m_rPixelSize;
        }
        m_unitSize = QSizeF{ l_realPixelOfBase / m_oUnit.x(), //
                             l_realPixelOfBase / m_oUnit.y() };
    }
    m_pImage = new QImage(toPixel(oSize.qSize()), QImage::Format_Mono);
    m_pImage->fill(Qt::color1);
    m_pPainter = new QPainter(m_pImage);
    m_pPainter->setPen(QPen(Qt::color1));

    //    m_pPainter->setFont(QFont("Tahoma", 16));
}

BMPPrinter::~BMPPrinter()
{
    if (m_pImage != nullptr) {
        delete m_pImage;
    }
    if (m_pPainter != nullptr) {
        delete m_pPainter;
    }
}

void BMPPrinter::printString(int x, int y, const QString &str)
{
    m_pPainter->drawText(x, y, str);
}

void BMPPrinter::printString(const QString &str)
{
    printString(m_iCurX, m_iCurY, str);
}

void BMPPrinter::printField(const XFSField *pField, const QMap<int, QString> &values, bool isFollows)
{
    QRect l_rectField = pField->rect();
    if (isFollows) {
        l_rectField.moveBottom(m_iCurY + 1);
    }
    if (values.isEmpty()) {
        warn(QString("List values of field [%1] is EMPTY").arg(pField->name()));
    } else {
        int l_iMax = values.lastKey();
        for (int i = 0; i <= l_iMax; i++) {
            printField(pField, l_rectField, values.value(i, QString{}));
            l_rectField.translate(pField->index().xOffset(), pField->index().yOffset());
        }
    }
}

void BMPPrinter::printField(const XFSField *pField, const QString &value, bool isFollows)
{
    QRect l_rectField = pField->rect();
    if (isFollows) {
        l_rectField.moveBottom(m_iCurY + 1);
    }
    printField(pField, l_rectField, value);
}

void BMPPrinter::printField(const XFSField *pField, const QRect &rect, const QString &value)
{
    if (pField->type().is("text")) {
        QFont l_curFont = m_pPainter->font();
        QFont l_fieldFont = l_curFont;
        // Set style of field
        const QStringList &l_refListStyles = pField->style().listValues();
        for (auto itr = l_refListStyles.constBegin(); itr != l_refListStyles.constEnd(); itr++) {
            if ((*itr) == "BOLD") {
                l_fieldFont.setBold(true);
            } else if ((*itr) == "ITALIC") {
                l_fieldFont.setItalic(true);
            }
        }

        if (!pField->pointSize().isNull()) {
            l_fieldFont.setPointSize(pField->pointSize().value());
            l_fieldFont.setBold(true);
        }
        m_pPainter->setFont(l_fieldFont);
        m_pPainter->drawText(toPixel(rect), pField->qtHorizontalAlign(), value);
        m_pPainter->setFont(l_curFont);
    } else if (pField->type().is("graphic")) {
        QImage l_imageField = QImage{ value };
        m_pPainter->drawImage(toPixel(rect), l_imageField);
    }
    m_iCurY = rect.bottom();
}

void BMPPrinter::printFrame(int iWidth, int iHeight)
{
    m_pPainter->setBrush(QBrush{ Qt::color0 });
    m_pPainter->drawRect(m_iCurX + 130, m_iCurY - 15, 240, 30);

    m_pPainter->setBrush(Qt::BDiagPattern);
    m_pPainter->drawRect(m_iCurX, m_iCurY, toPixelWidth(iWidth), toPixelHeight(iHeight));
}

void BMPPrinter::printFrame(const QSize &size)
{
    printFrame(size.width(), size.height());
}

void BMPPrinter::goTo(int x, int y)
{
    m_iCurX = toPixelWidth(x);
    m_iCurY = toPixelHeight(y + 1);
}

void BMPPrinter::printToFile(const QString &strPath)
{
    m_pImage->save(strPath, 0, -1);
}

QSize BMPPrinter::toPixel(const QSize &size)
{
    return QSize{ toPixelWidth(size.width()), toPixelHeight(size.height()) };
}

QPoint BMPPrinter::toPixel(const QPoint &point)
{
    return QPoint{ toPixelWidth(point.x()), toPixelHeight(point.y() + 1) };
}

QRect BMPPrinter::toPixel(const QRect &rect)
{
    return QRect{ toPixel(rect.topLeft()), toPixel(rect.size()) };
}
