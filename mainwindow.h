#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QStackedWidget>
#include <QList>
#include <QLabel>
#include <QJsonArray>
#include <QPushButton>

#include "clickablelabel.h"
#include "movieobject.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void downloadFinished(QNetworkReply *reply);
    void search_movies(QNetworkReply *reply);
    void clear_layout(QLayout *layout);
    void create_labels(QNetworkReply *reply, QGridLayout *grid_layout);

private slots:
    void on_logo_button_clicked();

    void on_exit_button_clicked();

    void on_filter_button_clicked();

    void on_button_mo_completed_clicked();

    void on_button_mo_plan_clicked();

    void on_button_tv_watching_clicked();

    void on_button_tv_completed_clicked();

    void on_button_tv_plan_clicked();

    void on_search_button_clicked();

    void on_search_bar_returnPressed();

private:

    Ui::MainWindow *ui;
    int rowNum, colNum, rowNumS, colNumS;
    QString movieTitle;
    QList<ClickableLabel*> label_list;
    QList<QString> label_list_data;

};
#endif // MAINWINDOW_H
