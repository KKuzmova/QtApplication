#include "movieobject.h"

MovieObject::MovieObject(QString imDbRating, QString imDbRatingCount, QString image, QString rank, QString title, QString year, QString id)
{
    this->imDbRating = imDbRating;
    this->imDbRatingCount = imDbRatingCount;
    this->image = image;
    this->rank = rank ;
    this->title = title;
    this->year = year ;
    this->id = id;

}

MovieObject::MovieObject(){;};

MovieObject::~MovieObject()
{

}
