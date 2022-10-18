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


    QRegExp rgx("[a-zA-Z]{1,50}\\d{1,50}");
    QValidator *comValidator = new QRegExpValidator (rgx, this);
    ui->lineEditFileName->setValidator(comValidator);

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

//    if(rows == 0){
//        QMessageBox::warning(this, "Warning", "Cannot create image with 0 seeds");
//        return;

//    }



    keime = new int*[rows];
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

//                if(ok == false){
//                    //ui->lineEdit->setText(QString("false"));

//                    for (int i = 0; i < rows; i++) {
//                        delete[] keime[i];
//                    }
//                    delete[] keime;
//                    keime = NULL;

//                    QMessageBox::warning(this, "Warning", "Some of the data in the table are no ints");

//                    return;
//                }

                switch(col){
                case 0:number %= width;break;
                case 1:number %= height;break;
                default:break;
                }

                keime[row][col] = number;

            }

       }




    image =  new QImage(width, height,  QImage::Format_Indexed8);

    imageMa = new int**[width];
    for(int w = 0; w < width; w++)
    {

        imageMa[w] = new int*[height];
        for(int h = 0; h < height; h++){
            imageMa[w][h] = new int[2];
            imageMa[w][h][0] = 0;
            imageMa[w][h][1] = -1;
        }

    }



    QRgb value = qRgb(0, 0, 0);
    image->setColor(0, value);

    for(int i = 0; i < rows; i++){
        std::srand(i+1);
        int rand = (std::rand() % 256);
        QRgb value = qRgb(rand, rand, rand);
        image->setColor(i+1, value);

    }


    for(int i = 0; i < rows; i++){
        imageMa[keime[i][0]][keime[i][1]][0] = i+1;
        imageMa[keime[i][0]][keime[i][1]][1] = 0;
    }



    for(int rounds = 1; rounds <= 300;rounds++){

        for(int w = 0; w < width; w++){

            for(int h = 0; h < height; h++){

                QPoint pos = QPoint(w, h);


                if(imageMa[pos.x()][pos.y()][0] >= 1 && imageMa[pos.x()][pos.y()][1] < rounds){
                    checkPixel(pos, -1, -1, rounds);
                    checkPixel(pos, 0, -1, rounds);
                    checkPixel(pos, -1, 0, rounds);
                    checkPixel(pos, 1, 1, rounds);
                    checkPixel(pos, 1, 0, rounds);
                    checkPixel(pos, 0, 1, rounds);
                    checkPixel(pos, 1, -1, rounds);
                    checkPixel(pos, -1, 1, rounds);

                }

            }
        }

    }

    for(int w = 0; w < width; w++){

        for(int h = 0; h < height; h++){
            image->setPixel(w, h, imageMa[w][h][0]);

        }

    }

    maps.append(scene->addPixmap(QPixmap::fromImage(*image)));
    maps.back()->setData(42, maps.size()-1);

    scene->update();


    maps.back()->setFlag(QGraphicsItem::ItemIsSelectable);

    maps.back()->setFlag(QGraphicsItem::ItemIsMovable);




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
    ui->pushButtonAdd->setEnabled(act);
    ui->pushButtonRemove->setEnabled(act);
    ui->pushButtonGenerate->setEnabled(act);
    ui->tableView->setEnabled(act);

    return;
}

void verzinkt::checkPixel(QPoint posN, int ox, int oy,int round)
{


    QPoint pos = posN;

    posN += QPoint(ox, oy);



    if(image->valid(posN)){
        if(imageMa[posN.x()][posN.y()][0] == 0){

            imageMa[posN.x()][posN.y()][0] = imageMa[pos.x()][pos.y()][0];
            imageMa[posN.x()][posN.y()][1] = round;
            return;
        }

        if(imageMa[posN.x()][posN.y()][0] != imageMa[pos.x()][pos.y()][0] && imageMa[posN.x()][posN.y()][1] != 0){
            int keimOld = imageMa[posN.x()][posN.y()][0]-1;
            QPoint cvOld = QPoint(posN.x()-keime[keimOld][0], posN.y()-keime[keimOld][1]);

            int keimNew = imageMa[pos.x()][pos.y()][0]-1;
            QPoint cvNew = QPoint(posN.x()-keime[keimNew][0], posN.y()-keime[keimNew][1]);

            if(calcT(cvOld, keimOld) > calcT(cvNew, keimNew)){
                imageMa[posN.x()][posN.y()][0] = imageMa[pos.x()][pos.y()][0];
                imageMa[posN.x()][posN.y()][1] = round;
            }
         return;


        }
    }

}

float verzinkt::calcT(QPoint cvOld, int keimOld)
{

    float rx, ry;

    if(cvOld.x()>0){
        rx = float(cvOld.x()*(-1))/float(keime[keimOld][4]);
    }else{
        rx = float(cvOld.x())/float(keime[keimOld][2]);
    }

    if(cvOld.y()>0){
        ry = float(cvOld.y()*(-1))/float(keime[keimOld][5]);
    }else{
        ry = float(cvOld.y())/float(keime[keimOld][3]);
    }

    return float(sqrt((rx*rx)+(ry*ry))+keime[keimOld][6]);

}

