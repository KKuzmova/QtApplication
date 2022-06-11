#include "mainwindow.h"
#include "QtNetwork/qnetworkreply.h"
#include "ui_mainwindow.h"


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
    const QUrl url = QUrl("https://imdb-api.com/en/API/Top250Movies/k_u0nldr66");
    QNetworkRequest request(url);

    QNetworkReply *netReply = man->get(request);
    QEventLoop loop;
    connect(netReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    create_labels(netReply, ui->gridLayout);

    }


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
            MovieObject *film = new MovieObject(j["imDbRating"].toString(), j["imDbRatingCount"].toString(), url, j["rank"].toString(), j["title"].toString(), j["year"].toString(), j["id"].toString());
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
            if(url.contains("original")){
                const QUrl url_min = QUrl(url.replace(28, 8, "128x176"));
                QNetworkRequest request(url_min);
                QNetworkReply *imageNetReply = client->get(request);
                QEventLoop loop;
                connect(imageNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
                QPixmap pix;
                pix.loadFromData(imageNetReply->readAll());
                label->setPixmap(pix);

            }else{
                QNetworkRequest request(url);
                QNetworkReply *imageNetReply = client->get(request);
                QEventLoop loop;
                connect(imageNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();

                QPixmap pix;
                pix.loadFromData(imageNetReply->readAll());
                label->setPixmap(pix);
            }
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
    qDebug()<<"Reply got!";
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

void MainWindow::on_button_mo_completed_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->movie_CM_page);
}

void MainWindow::on_button_mo_plan_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->movie_PLW_page);
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
    QString search_url = "https://imdb-api.com/en/API/SearchTitle/k_u0nldr66/" + movieName;

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
