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
#include <QInputDialog>
#include <QTextStream>
#include <QDebug>






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

    void on_pushButtonSave_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::verzinkt *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *map;

    QList<QGraphicsPixmapItem*> maps;

    QImage *image;


    QStandardItemModel* model;
    QList<QString> horizontalHeader;

    int width = 300;

    int height = 300;

    int factor = 10;

    int** keime;

    int*** imageMa;

    void checkPixel(QPoint posN, int ox, int oy, int round);

    float calcT(QPoint pos, int keimOld);

    void enableUi(bool act);

public slots:



};





#endif // VERZINKT_H
