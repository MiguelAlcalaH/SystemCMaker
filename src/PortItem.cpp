#include <PortItem.h>
#include <QtDebug>
#include <SvgDraggableItem.h>

PortItem::PortItem(const QPointF &position,const qreal &rad, Sig s,QGraphicsItem * parent) : 
                                               QGraphicsEllipseItem(position.x(),position.y(),2*rad,2*rad,parent)
{
  posi = QPointF(position.x(),position.y());
  radius = rad;
  sig = s;
  active = QColor(Qt::blue);
setFlags(QGraphicsItem::ItemIsSelectable);
}

void PortItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  qDebug() << "***************__Press__********************";
  SvgDraggableItem *pp = qgraphicsitem_cast<SvgDraggableItem *>(parentItem());
  if(pp)
    qDebug() << "***Parent Pos" << pp->pos() << "******___";
}


void PortItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  painter->setBrush(QBrush(active));
  painter->drawEllipse(posi.x(),posi.y(),2*radius,2*radius);
}

int PortItem::type() const
{
  return Type;
}
QPointF PortItem::Pos()
{
  QPointF sc = mapToScene(posi+QPointF(radius,radius));
  qDebug() << "Position Port"<<sc;
  return sc;
}

void PortItem::setState(int st)
{
  if(st == 1)
    active = QColor(Qt::red);
  else 
    active = QColor(Qt::blue);

  update();
}