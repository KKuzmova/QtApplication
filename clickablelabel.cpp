#include "clickablelabel.h"
#include "QtNetwork/qnetworkreply.h"

ClickableLabel::ClickableLabel(MovieObject *film, QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    this->film = *film;
    connect(this, SIGNAL(clicked()), this, SLOT(labelClicked()));
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}


void ClickableLabel::labelClicked(){
    widget->setCurrentIndex(1);

    QList<QLabel*> score_label = widget->findChildren<QLabel*>("score_label", Qt::FindChildrenRecursively);
    score_label[0]->setText(this->film.imDbRating);
    QList<QLabel*> movie_title = widget->findChildren<QLabel*>("movie_title", Qt::FindChildrenRecursively);
    movie_title[0]->setText(this->film.title);

}

void ClickableLabel::setWi(QStackedWidget *widgetP)
{
    widget = widgetP;

}

