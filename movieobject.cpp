#include "movieobject.h"

MovieObject::MovieObject(QString imDbRating, QString imDbRatingCount, QString title, QString year, QString id, QPixmap pixmap, bool full)
{
    this->imDbRating = imDbRating;
    this->imDbRatingCount = imDbRatingCount;
    this->title = title;
    this->year = year ;
    this->id = id;
    this->pixmap = pixmap;
    this->full = full;
}

MovieObject::MovieObject(){;};

MovieObject::MovieObject(QString title, QString id, QPixmap pixmap, bool full){

    this->title = title;
    this->id = id;
    this->pixmap = pixmap;
    this->full = full;
}

MovieObject::~MovieObject()
{

}
