
#include "mainwindow.h"
#include "QtNetwork/qnetworkreply.h"
#include "ui_mainwindow.h"

#include "globals.h"
int label_list_size;
int label_list_sizeS;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget::setWindowTitle ("MovieManager");
    ui->stackedWidget->setCurrentWidget(ui->main_page);

    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::downloadFinished);
    const QUrl url = QUrl("https://imdb-api.com/en/API/Top250Movies/"+api_key);
//    qDebug()<<url;
    QNetworkRequest request(url);

    QNetworkReply *netReply = man->get(request);
    QEventLoop loop;
    connect(netReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    create_labels(netReply, ui->gridLayout);
    }

//clears lay
void MainWindow::clear_layout(QLayout *layout){

        QLayoutItem* child;
        while ( layout->count() != 0 ) {
            child = layout->takeAt ( 0 );
            if ( child->layout() != 0 ) {
                clear_layout( child->layout() );
            } else if ( child->widget() != 0 ) {
                delete child->widget();
            }
            delete child;
    }
}

//creates labels
void MainWindow::create_labels(QNetworkReply *reply, QGridLayout *grid_layout) {
    this->clear_layout(grid_layout);
    QByteArray str;
    str = reply->readAll();
    QString s;
    s = s.fromLatin1(str);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(s.toUtf8());

    QNetworkAccessManager *client = new QNetworkAccessManager(this);
    connect(client, &QNetworkAccessManager::finished, this,  &MainWindow::downloadFinished);
    int rowNum = 0;
    int colNum = 0;
    QJsonArray arr;
    if (grid_layout->objectName() != "gridLayout_4"){
        arr = jsonResponse["items"].toArray();
    } else {
        arr = jsonResponse["results"].toArray();
    }
    label_list_size = arr.size();
    QString row = QString::number(rowNum);
    QString col = QString::number(colNum);
    for(const QJsonValue j : arr){

            QString url = j["image"].toString();

            movieTitle = (j["title"]).toString();
            MovieObject *film;
            QPixmap pix;
            if(url.contains("original")){
                const QUrl url_min = QUrl(url.replace(28, 8, "128x176"));
                QNetworkRequest request(url_min);
                QNetworkReply *imageNetReply = client->get(request);
                QEventLoop loop;
                connect(imageNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();

                pix.loadFromData(imageNetReply->readAll());

            }else{
                QNetworkRequest request(url);
                QNetworkReply *imageNetReply = client->get(request);
                QEventLoop loop;
                connect(imageNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();

                pix.loadFromData(imageNetReply->readAll());
            }
            if(j["imDbRating"].isUndefined()){
                film = new MovieObject(j["title"].toString(), j["id"].toString(), pix);

            } else {
                film = new MovieObject(j["imDbRating"].toString(), j["imDbRatingCount"].toString(), j["title"].toString(), j["year"].toString(), j["id"].toString(),pix, true);
            }
            ClickableLabel *label = new ClickableLabel(film, ui->main_scroll_area);
            label->setWi(ui->stackedWidget);
            label->setToolTip(movieTitle);
            label->setMinimumSize(QSize(128,176));
            label->setMaximumSize(QSize(128,176));
            label->setStyleSheet("QLabel{"
                                    "background-color: rgb(44, 50, 77);}"
                                 "QLabel:hover{"
                                    "border-color: rgb(255, 255, 255);"
                                    "border-width: 1.2px;"
                                    "border-style: inset;}");
            label->setFrameShape(QLabel::StyledPanel);
            label->setFrameShadow(QLabel::Raised);
            auto newName = "label" + row + "__" + col;
            label->setObjectName(newName);

            label->setPixmap(pix);
            qDebug()<<colNum<<"\t"<<rowNum;
            grid_layout->addWidget(label,rowNum, colNum, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter);
            colNum++;
            if(colNum == 4){
               rowNum++;
               colNum = 0;
            }
    }
}

void MainWindow::downloadFinished(QNetworkReply *reply){
//    qDebug()<<"Reply got!";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logo_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->main_page);
}

void MainWindow::on_exit_button_clicked()
{
    QApplication::quit();
}

void MainWindow::on_filter_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->filter_page);
}


void MainWindow::on_button_tv_watching_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->tv_W_page);
}

void MainWindow::on_button_tv_completed_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->tv_CM_page);
}

void MainWindow::on_button_tv_plan_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->tv_PLW_page);
}

void MainWindow::on_search_button_clicked()
{
    on_search_bar_returnPressed();
}

void MainWindow::on_search_bar_returnPressed()
{
    QString movieName = ui->search_bar->text();
    ui->search_bar->clear();
    QString search_url = "https://imdb-api.com/en/API/SearchTitle/"+api_key+"/" + movieName;

    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::downloadFinished);
    const QUrl url = QUrl(search_url);
    QNetworkRequest request(url);
    ui->stackedWidget->setCurrentWidget(ui->search_page);
    QNetworkReply *netReply = man->get(request);

    QEventLoop loop;
    connect(netReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    create_labels(netReply, ui->gridLayout_4);
}

void MainWindow::on_list_box_currentIndexChanged(int index)
{
//    qDebug()<< "current index changed";
}


void MainWindow::on_list_box_activated(int index)
{
//    qDebug()<< "activated";
    QJsonDocument doc;
    QJsonObject json_object;

    json_object["title"] = ui->movie_title->text();

    doc.setObject(json_object);
    QByteArray byte_arr = doc.toJson(QJsonDocument::Indented);
    QFile json_file("/Users/mac/Qt/Projects/Manager/completed.json");
    if( json_file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        json_file.write(byte_arr);
        json_file.close();
    }
}

void MainWindow::on_rating_box_activated(int index)
{
    //TODO: when rated it automaticcaly adds to the completed list
    //adds to the list by type: Movie, TV

}

void MainWindow::load_data(){

}

void MainWindow::on_button_mo_completed_clicked()
{
    QString jsonStr;
    QFile jsonFile("/Users/mac/Qt/Projects/Manager/completed.json");
    if(jsonFile.exists()){
        if(jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            jsonStr = jsonFile.readAll();
            jsonFile.close();
        }

        QJsonDocument json = QJsonDocument::fromJson(jsonStr.toUtf8());

        ui->movie_CM_table->setItem(0, 0, new QTableWidgetItem(json["title"].toString()));
    }

//    ui->movie_CM_table->setColumnWidth(0, 350);
//    ui->movie_CM_table->setColumnWidth(1, 50);
    ui->stackedWidget->setCurrentWidget(ui->movie_CM_page);


}

void MainWindow::on_button_mo_plan_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->movie_PLW_page);
}


void MainWindow::on_filter_search_button_clicked()
{
    QString search_url = "https://imdb-api.com/API/AdvancedSearch/"+api_key+"?genres=";

    QList<QCheckBox*> list = ui->genres_frame->findChildren<QCheckBox*>();
    foreach (auto child, list){
       if (child->isChecked()){
           search_url += child->objectName();
           search_url += ",";
           child->setChecked(false);
       }
   }
   if(!search_url.endsWith("=")){
//       url.remove(url.back());
       search_url.chop(1);
//       qDebug()<< url;
       ui->stackedWidget->setCurrentWidget(ui->search_page);

       QNetworkAccessManager *man = new QNetworkAccessManager(this);
       connect(man, &QNetworkAccessManager::finished, this, &MainWindow::downloadFinished);
       const QUrl url = QUrl(search_url);
       QNetworkRequest request(url);
       ui->stackedWidget->setCurrentWidget(ui->search_page);
       QNetworkReply *netReply = man->get(request);

       QEventLoop loop;
       connect(netReply, SIGNAL(finished()), &loop, SLOT(quit()));
       loop.exec();
       create_labels(netReply, ui->gridLayout_4);
   };
}
