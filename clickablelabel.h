#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include "ui_mainwindow.h"
#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QStackedWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "movieobject.h"

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(MovieObject *film, QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();
    void setWi(QStackedWidget* widgetP);
    void getPages(QWidget* page);
    void getUi(Ui::MainWindow* u);
    void getMovieTitle(QString *moTitle);
    void setMovieTitle(QString *moTitle);

signals:
    void clicked();
public slots:
    void labelClicked();

protected:
    void mousePressEvent(QMouseEvent* event);
private:
    MovieObject film;
    QStackedWidget* widget;
    void download_finished(QNetworkReply *reply);
};

#endif // CLICKABLELABEL_H
