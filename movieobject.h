#ifndef MOVIEOBJECT_H
#define MOVIEOBJECT_H

#include <QString>

class MovieObject
{
public:
    MovieObject(QString imDbRating, QString imDbRatingCount, QString image, QString rank, QString title, QString year, QString id);
    explicit MovieObject();
    ~MovieObject();

QString imDbRating; QString imDbRatingCount; QString image; QString rank; QString title; QString year; QString id;
};

#endif // MOVIEOBJECT_H
