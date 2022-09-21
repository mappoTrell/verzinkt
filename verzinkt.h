#ifndef VERZINKT_H
#define VERZINKT_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QStandardItemModel>
#include <QtMath>
#include <QScrollBar>
#include <QTimeLine>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>




QT_BEGIN_NAMESPACE
namespace Ui { class verzinkt; }
QT_END_NAMESPACE

class verzinkt : public QMainWindow
{
    Q_OBJECT

public:
    verzinkt(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent* event);
    ~verzinkt();

private slots:


    void on_pushButtonAdd_clicked();

    void on_pushButtonRemove_clicked();

    void on_pushButtonGenerate_clicked();

    void on_pushButtonTest_clicked();

    void on_actionRemove_image_triggered();

    void on_actionSave_selected_image_triggered();

private:
    Ui::verzinkt *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *map;

    QList<QGraphicsPixmapItem*> maps;

    QImage *image;


    QStandardItemModel* model;
    QList<QString> horizontalHeader;

    int width = 10000;

    int height = 10000;

    int factor = 100;



public slots:



};





#endif // VERZINKT_H
