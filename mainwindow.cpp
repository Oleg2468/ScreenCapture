#include "mainwindow.h"
#include "screencaptureitem.h"
#include "bagroundpolygonitem.h"
#include "currentsizeitem.h"
#include "resizegraphicsitem.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setGeometry(QApplication::desktop()->screenGeometry());
    setStyleSheet("background:transparent;");
    setContentsMargins(0, 0, 0, 0);

    QGraphicsView* view = new QGraphicsView(this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setContentsMargins(0, 0, 0, 0);
    view->setGeometry(geometry());
    setCentralWidget(view);

    QGraphicsScene* scene = new QGraphicsScene(view);
    scene->setSceneRect(view->geometry());
    view->setScene(scene);
    view->setBackgroundBrush(QBrush(QColor(0, 0, 0, 0)));

    BackgroundPolygonItem* background_item = new BackgroundPolygonItem(QPolygonF(), nullptr, scene);
    background_item->setBrush(QBrush(QColor(0, 0, 200, 100)));
    background_item->setPen(TransparentColor);
    scene->addItem(background_item);

    ScreenCaptureItem* capture_item = new ScreenCaptureItem(QRectF(200, 200, 200, 200), nullptr, scene);
    capture_item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemSendsGeometryChanges);
    capture_item->setAcceptHoverEvents(true);
    capture_item->setPen(QPen(QBrush(FrameColor), CaptureItemwidth, Qt::DashLine));
    capture_item->setBrush(TransparentColor);
    connect(capture_item, &ScreenCaptureItem::positionRectChanged, background_item, &BackgroundPolygonItem::onItemRectChanged);
    connect(capture_item, &ScreenCaptureItem::sizeRectChanged, background_item, &BackgroundPolygonItem::onItemRectChanged);
    scene->addItem(capture_item);
    capture_item->addResizeItems(scene);

    background_item->onItemRectChanged(capture_item->boundingRect());

    CurrentSizeItem* cur_size_item = new CurrentSizeItem;
    cur_size_item->onItemRectChanged(capture_item->boundingRect());
    connect(capture_item, &ScreenCaptureItem::positionRectChanged, cur_size_item, &CurrentSizeItem::onItemRectChanged);
    connect(capture_item, &ScreenCaptureItem::sizeRectChanged, cur_size_item, &CurrentSizeItem::onItemRectChanged);
    scene->addItem(cur_size_item);
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
        event->accept();
        return;
    }

    QMainWindow::keyPressEvent(event);
}



