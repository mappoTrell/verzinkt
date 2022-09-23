#include "verzinkt.h"

#include "ui_verzinkt.h"



verzinkt::verzinkt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::verzinkt)
{

    Q_INIT_RESOURCE(icons);

    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->qWidgetSaveFile->hide();

    ui->graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->graphicsView->setResizeAnchor(QGraphicsView::NoAnchor);

    QImage *back =  new QImage(width, height,  QImage::Format_Indexed8);

    map = scene->addPixmap(QPixmap::fromImage(*back));

    map->setFlag(QGraphicsItem::ItemIsMovable);

    map->setFlag(QGraphicsItem::ItemIsSelectable);



    ui->graphicsView->fitInView(map,Qt::KeepAspectRatio);

    int factor = 10;

    ui->graphicsView->scale(factor, factor);

    scene->removeItem(map);


    scene->update();

    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);


    // Create a new model
    // QStandardItemModel(int rows, int columns, QObject * parent = 0)
    model = new QStandardItemModel(0,7,this);

    horizontalHeader = {"X","Y","Vx","Vy","V-x","V-y","birth"};


    model->setHorizontalHeaderLabels(horizontalHeader);


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ui->tableView->setModel(model);



    // Generate data


}

verzinkt::~verzinkt()
{
    delete ui;
}



void verzinkt::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        // zoom
        ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = event->angleDelta().y();
        qreal factor;
        if (angle > 0) {
            factor = 1.1;
        } else {
            factor = 0.9;
        }
        ui->graphicsView->scale(factor, factor);
        ui->graphicsView->setTransformationAnchor(ui->graphicsView->transformationAnchor());
    }
}




void verzinkt::on_pushButtonAdd_clicked()
{
    QStandardItem *rowlist = new QStandardItem();



    model->appendRow(rowlist);

    for(int row = model->rowCount()-1; row < model->rowCount(); row++)
        {
            for(int col = 0; col < 7; col++)
            {

                int value = 1;

                switch(col){
                case 0:value = (QRandomGenerator::global()->generate() % width);break;
                case 1:value = (QRandomGenerator::global()->generate() % height);break;
                case 2:value = (QRandomGenerator::global()->generate() % (width/factor)+1);break;
                case 3:value = (QRandomGenerator::global()->generate() % (height/factor)+1);break;
                case 4:value = (QRandomGenerator::global()->generate() % (width/factor)+1);break;
                case 5:value = (QRandomGenerator::global()->generate() % (height/factor)+1);break;
                case 6:value = (QRandomGenerator::global()->generate() % (10));break;
                default: break;
                }


                QModelIndex index
                        = model->index(row,col,QModelIndex());
                // 0 for all data
                model->setData(index,value);

            }


        }

    ui->tableView->setModel(model);

}


void verzinkt::on_pushButtonRemove_clicked()
{
    model->removeRows(model->rowCount()-1,1);
}


void verzinkt::on_pushButtonGenerate_clicked()
{

    ui->verticalLayout->setEnabled(false);

    int rows = model->rowCount();

    if(rows == 0){
        QMessageBox::warning(this, "Warning", "Cannot create image with 0 seeds");
        return;

    }



    int** keime = new int*[rows];
    for(int row = 0; row < rows; row++)
        {

            keime[row] = new int[7];

        }



    for(int row = 0; row < rows; row++)
        {

            for(int col = 0; col < model->columnCount(); col++)
            {
                QModelIndex index
                        = model->index(row,col,QModelIndex());


                bool ok;

                QString str = model->itemData(index).value(0).toString();

                int number = str.toInt(&ok);

                if(ok == false){
                    //ui->lineEdit->setText(QString("false"));

                    for (int i = 0; i < rows; i++) {
                        delete[] keime[i];
                    }
                    delete[] keime;
                    keime = NULL;

                    QMessageBox::warning(this, "Warning", "Some of the data in the table are no ints");

                    return;
                }

                switch(col){
                case 0:number %= width;break;
                case 1:number %= height;break;
                default:break;
                }

                keime[row][col] = number;

            }

       }




    image =  new QImage(width, height,  QImage::Format_Indexed8);




    for(int i = 0; i < rows; i++){
        std::srand(i+1);
        int rand = (std::rand() % 256);
        QRgb value = qRgb(rand, rand, rand);
        image->setColor(i, value);

    }


    for(int w = 0; w < width; w++){

        for(int h = 0; h < height; h++){

            int k = 0;
            int kR = -1;

            for(int i = 0; i < rows; i++){

                int dx = w - keime[i][0];
                int dy = h - keime[i][1];

                float rx, ry, r;

                if(dx<0){
                    dx *= -1;
                    rx = float(dx)/float(keime[i][4]);
                }else{
                    rx = float(dx)/float(keime[i][2]);
                }

                if(dy<0){
                    dy *= -1;
                    ry = float(dy)/float(keime[i][5]);
                }else{
                    ry = float(dy)/float(keime[i][3]);
                }

                if(rx<ry){

                    r = ry + keime[i][6];
                }else{
                    r = rx + keime[i][6];
                }

                if((r <= kR) | (kR == -1)){
                    k = i;
                    kR = r;
                }

            }

            image->setPixel(w, h, k);

        }

    }

    maps.append(scene->addPixmap(QPixmap::fromImage(*image)));
    maps.back()->setData(42, maps.size()-1);

    scene->update();


    maps.back()->setFlag(QGraphicsItem::ItemIsSelectable);

    maps.back()->setFlag(QGraphicsItem::ItemIsMovable);


    for (int i = 0; i < rows; i++) {
        delete[] keime[i];
    }
    delete[] keime;
    keime = NULL;

    ui->lineEdit->setText(maps.back()->data(42).toString());

    ui->verticalLayout->setEnabled(true);

}


void verzinkt::on_pushButtonTest_clicked()
{

}


void verzinkt::on_actionRemove_image_triggered()
{
    if(scene->selectedItems().size()==0){

        QMessageBox::warning(this, "                 Warning                 ", "No image selected");
        return;

    }

    QGraphicsItem *rem = scene->selectedItems().at(0);

    maps.removeAt(rem->data(42).toInt());

    scene->removeItem(rem);
    delete rem;
    rem = NULL;
}


void verzinkt::on_actionSave_selected_image_triggered()
{

    QString warning = "No Image selected";

    if(scene->selectedItems().size()==0){

        QMessageBox::warning(this, "                  Warning                        ", warning);
        return;

    }

    ui->qWidgetSaveFile->show();

    enableUi(false);


}


void verzinkt::on_pushButtonSave_clicked()
{
    auto img = maps.at(scene->selectedItems().at(0)->data(42).toInt())->pixmap().toImage();

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");

    QString fName = ui->lineEditFileName->text()+".png";

    QFileDialog::saveFileContent(ba, fName);

    ui->qWidgetSaveFile->hide();

    ui->lineEditFileName->clear();

    enableUi(true);
}


void verzinkt::on_pushButtonCancel_clicked()
{
    ui->qWidgetSaveFile->hide();

    ui->lineEditFileName->clear();

    enableUi(true);
}


void verzinkt::enableUi(bool act){

    ui->actionSave_selected_image->setEnabled(act);

    ui->actionRemove_image->setEnabled(act);

    ui->verticalLayout->setEnabled(act);

}
