#pragma once
#include <QGraphicsBlurEffect>

class BlurPanel : public QGraphicsBlurEffect
{
	Q_OBJECT

public:
	void draw(QPainter* painter) override;
};