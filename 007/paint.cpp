#include "scene.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent){}

Scene::~Scene(){}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  addEllipse(event->scenePos().x()-(size/2),
             event->scenePos().y()-(size/2),
             size, size, QPen(Qt::NoBrush), QBrush(color));
  previousPoint = event->scenePos();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  addLine(previousPoint.x(), previousPoint.y(),
          event->scenePos().x(), event->scenePos().y(),
          QPen(color, size,Qt::SolidLine,Qt::RoundCap));
  previousPoint = event->scenePos();
}

void Scene::setColor(QString color)
{
  this->color = color;
}

void Scene::setSize(QString size)
{
  this->size = size.toInt();
}
