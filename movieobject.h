#ifndef MOVIEOBJECT_H
#define MOVIEOBJECT_H

#include <QString>
#include <QPixmap>
class MovieObject
{
public:
    MovieObject(QString imDbRating, QString imDbRatingCount, QString title, QString year, QString id, QPixmap pixmap, bool full=false);
    MovieObject(QString title, QString id, QPixmap pixmap, bool full=false);
    explicit MovieObject();
    ~MovieObject();

QString imDbRating; QString imDbRatingCount; QString image; QString rank; QString title; QString year; QString id;
QPixmap pixmap;
bool full;
};

#endif // MOVIEOBJECT_H
