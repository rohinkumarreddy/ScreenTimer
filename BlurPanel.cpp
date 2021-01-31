#include "BlurPanel.h"
#include <QPainter>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter* p, QImage& blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

void BlurPanel::draw(QPainter* painter)
{
	QPixmap pix;
	QPoint offset;
    if (sourceIsPixmap()) // No point in drawing in device coordinates (pixmap will be scaled anyways)
        pix = sourcePixmap(Qt::LogicalCoordinates, &offset);
    else // Draw pixmap in device coordinates to avoid pixmap scaling;
    {
        pix = sourcePixmap(Qt::DeviceCoordinates, &offset);
        painter->setWorldTransform(QTransform());
    }
    /*blurs the widget coontents too!*/
    //QPixmap pixA(pix.size());
    //pixA.fill(Qt::transparent);
    //QPainter pixPainter(&pixA);
    //qt_blurImage(&pixPainter, pix.toImage(), 5, true, false);

    painter->setBrush(QColor(75, 75, 75, 100)); // white bg
    painter->drawRect(pix.rect().adjusted(-1,-1,0,0));
    //painter->setOpacity(0.5);
    painter->drawPixmap(offset, pix);

    //blurs the widget contents too!
    //setBlurRadius(10);//not working?
    //setBlurHints(QGraphicsBlurEffect::BlurHint::QualityHint);
    //QGraphicsBlurEffect::draw(painter);
}