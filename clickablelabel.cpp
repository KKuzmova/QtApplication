#include "clickablelabel.h"
#include "QtNetwork/qnetworkreply.h"
#include "mainwindow.h"

#include "globals.h"

ClickableLabel::ClickableLabel(MovieObject *film, QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    this->film = *film;
    connect(this, SIGNAL(clicked()), this, SLOT(labelClicked()));
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

void ClickableLabel::download_finished(QNetworkReply *reply){
    QString s = reply->readAll();
    QJsonDocument rating_Reply_read = QJsonDocument::fromJson(s.toUtf8());
//    qDebug()<<rating_Reply_read["totalRating"];
    this->film.imDbRating = rating_Reply_read["totalRating"].toString();
//    qDebug()<<this->film.imDbRating;
    this->film.imDbRatingCount = rating_Reply_read["totalRatingVotes"].toString();
    this->film.imDbRatingCount = rating_Reply_read["year"].toString();
    QList<QLabel*> score_label = widget->findChildren<QLabel*>("score_label", Qt::FindChildrenRecursively);
    score_label[0]->setText(this->film.imDbRating);

    QList<QLabel*> imDbRatingCount_label = widget->findChildren<QLabel*>("num_of_ratings_label", Qt::FindChildrenRecursively);
    imDbRatingCount_label[0]->setText(this->film.imDbRatingCount);


    QList<QLabel*> year_label = widget->findChildren<QLabel*>("year_label", Qt::FindChildrenRecursively);
    year_label[0]->setText(this->film.year);
//    qDebug()<<"Reply in label got";
};

void ClickableLabel::labelClicked(){
    widget->setCurrentIndex(1);
//    qDebug()<<this->film.title;
    QList<QLabel*> movie_title = widget->findChildren<QLabel*>("movie_title", Qt::FindChildrenRecursively);
    movie_title[0]->setText(this->film.title);
    QList<QLabel*> image_label = widget->findChildren<QLabel*>("image_label", Qt::FindChildrenRecursively);
    image_label[0]->setPixmap(this->film.pixmap);
    if(!this->film.full){
        QNetworkAccessManager *client = new QNetworkAccessManager(this);
        connect(client, &QNetworkAccessManager::finished, this, &ClickableLabel::download_finished);
        QUrl data_url = QUrl("https://imdb-api.com/en/API/Wikipedia/"+ api_key +"/"+this->film.id);
        QNetworkRequest request_data(data_url);

//        QNetworkReply *data_Reply = client->get(request_data);
        QUrl rating_url = QUrl("https://imdb-api.com/en/API/UserRatings/"+ api_key +"/"+this->film.id);
        QNetworkRequest request_rating(rating_url);
        QNetworkReply *rating_Reply = client->get(request_rating);
        QEventLoop loop;
//                connect(data_Reply, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(rating_Reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QString s_data;
        QString s_raiting;

//                s_data.fromLatin1(data_Reply->readAll());
                s_raiting.fromLatin1(rating_Reply->readAll());
//        QJsonDocument data_Reply_read = QJsonDocument::fromJson(s_data.toUtf8());
        QJsonDocument rating_Reply_read = QJsonDocument::fromJson(s_raiting.toUtf8());
//        qDebug()<<rating_Reply_read["totalRating"];
        this->film.imDbRating = rating_Reply_read["totalRating"].toString();
//        qDebug()<<this->film.imDbRating;
        this->film.imDbRatingCount = rating_Reply_read["totalRatingVotes"].toString();
        this->film.imDbRatingCount = rating_Reply_read["year"].toString();
    } else {
        QList<QLabel*> score_label = widget->findChildren<QLabel*>("score_label", Qt::FindChildrenRecursively);
        score_label[0]->setText(this->film.imDbRating);

        QList<QLabel*> imDbRatingCount_label = widget->findChildren<QLabel*>("num_of_ratings_label", Qt::FindChildrenRecursively);
        imDbRatingCount_label[0]->setText(this->film.imDbRatingCount);


        QList<QLabel*> year_label = widget->findChildren<QLabel*>("year_label", Qt::FindChildrenRecursively);
        year_label[0]->setText(this->film.year);
    }

//    qDebug()<<"end";

}

void ClickableLabel::setWi(QStackedWidget *widgetP)
{
    widget = widgetP;

}

