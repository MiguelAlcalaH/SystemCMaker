#include "MainWindow.h"
#include <QSvgRenderer>
#include <QKeyEvent>
#include <PortItem.h>
#include <codebuilder.h>
#include <QtGui>

int n = 0;
/**
 * @brief Default constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new MGraphicsScene();
    scene->setSceneRect(0,0,480,360);
    frame = new frameDrag();
    frame->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    //frame->resize(120,360);
    view = new GraphicsViewDrop(scene);
    setAcceptDrops(true);

    QScrollArea *sc = new QScrollArea;
    sc->setWidget(frame);
    sp = new QSplitter();
    sp->addWidget(sc);
    sp->addWidget(view);
    tb = new QTabWidget();

    QPushButton *closeButt = new QPushButton;
    closeButt->setIcon(QIcon(":/SVG/close_tab.png"));
    closeButt->resize(16,16);
    QRegion *reg = new QRegion(QRect(closeButt->x(),closeButt->y(),32,32),QRegion::Rectangle);
    closeButt->setMask(*reg);

    QPushButton *addBut= new QPushButton;
    addBut->setIcon(QIcon(":/SVG/add_tab.png"));
    addBut->resize(16,16);

    tb->addTab(sp,QString("Circuit"));
    tb->addTab(new QLabel(QString("tab")),QString(""));
    tb->tabBar()->setTabButton(0,QTabBar::RightSide,closeButt);
    tb->tabBar()->setTabButton(1,QTabBar::LeftSide,addBut);

    setCentralWidget(tb);
    QBrush myBrush(Qt::blue, Qt::CrossPattern);
    view->setBackgroundBrush(myBrush);

    selectAll = false;
    resize(580,360);
    //Resources table for shared svg files
    rendererTable.insert(QString("AND_gate"),new QSvgRenderer(QLatin1String(":/SVG/AND_gate.svg")));
    rendererTable.insert(QString("OR_gate"),new QSvgRenderer(QLatin1String(":/SVG/OR_gate.svg")));
    rendererTable.insert(QString("XOR_gate"),new QSvgRenderer(QLatin1String(":/SVG/XOR_gate.svg")));
    rendererTable.insert(QString("NOT_gate"),new QSvgRenderer(QLatin1String(":/SVG/NOT_gate.svg")));
    rendererTable.insert(QString("input"),new QSvgRenderer(QLatin1String(":/SVG/input.svg")));
    rendererTable.insert(QString("output"),new QSvgRenderer(QLatin1String(":/SVG/output.svg")));

    view->setRedererTable(&rendererTable);

    SvgDraggableItem *svgItem = new SvgDraggableItem("AND_gate",rendererTable.value(QString("AND_gate")),
                                                                            PropertyComponent("AND_gate",0));
    scene->addItem(svgItem);
    svgItem->setPos(30,120);
    svgItem->setScale(0.5);

    createMenus();

}

void MainWindow::createMenus()
{
    QMenu * archivo = menuBar()->addMenu("Archivo");
    menuBar()->addMenu("Editar");
    menuBar()->addMenu("Proyecto");
    QMenu *analisis = menuBar()->addMenu("Analisis");
    QAction *genCode = analisis->addAction("Generate");

    QObject::connect(genCode,SIGNAL(triggered()),this,SLOT(generateCode()));

    QMenu * about = menuBar()->addMenu("About");
    about->addMenu("about qt");

}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if(!items.isEmpty())
    {

        qDebug() <<"SelectedItems : "<<scene->selectedItems().size();
        if(event->key() == Qt::Key_Delete)
        {
            while(!items.isEmpty())
            {
                QGraphicsItem *item = items.takeFirst();
                if(item)
                {
                    PolyLinesItem *wire = qgraphicsitem_cast<PolyLinesItem *>(item);
                    if(wire != 0)
                    {
                        qDebug() << "WIRE >>>";
                        wire->detach();
                        qDebug() << "Wire Delete";
                    }
                    else if(qgraphicsitem_cast<SvgDraggableItem*>(item))
                    {
                        qDebug() << "SvgDraggableItem";
                        qgraphicsitem_cast<SvgDraggableItem*>(item)->deleteAllWires(scene);
                    }
                    scene->removeItem(item);
                    delete item;
                }
            }
        }
    }

    if(event->matches(QKeySequence::AddTab))
      {
        tb->addTab(new QLabel(QString("Otro Tab")+QString::number(n)),QString("tab")+QString::number(n));
        n++;
      }

    if(event->matches(QKeySequence::SelectAll))
    {
        qDebug() << "Select ALL\n";
        if(selectAll)
            scene->clearSelection();
        else
        {
            QPainterPath path;
            path.addRect(view->rect());
            scene->setSelectionArea(path);
        }

        selectAll = !selectAll;
    }
    switch(event->key())
    {
        case Qt::Key_Escape:
            close();
        break;
        case  Qt::Key_Plus:
            view->zoomIn();
        break;
        case Qt::Key_Minus:
            view->zoomOut();
        break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::generateCode()
{
    QHash<QString,QString> dicClassComp;
    QHash<QString,QString> dicClassHeader;
    dicClassComp.insert("AND_gate","AndGate");
    dicClassComp.insert("OR_gate","OrGate");
    dicClassComp.insert("XOR_gate","XorGate");
    dicClassComp.insert("NOT_gate","NotGate");

    dicClassHeader.insert("AndGate","and.h");
    dicClassHeader.insert("OrGate","or.h");
    dicClassHeader.insert("XorGate","xor.h");
    dicClassHeader.insert("NotGate","not.h");

    CodeBuilder builder(this->view,QString("design"));
    builder.componentClass = dicClassComp;
    builder.classHeader = dicClassHeader;
    builder.generate("salida/",true);
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QSize nSize = event->size();
    scene->setSceneRect(QRectF(QPointF(0.0,0.0),QSizeF(nSize)));
    //frame->resize(120,nSize.height()+20);
}
