#ifndef ARTISTS_VIEWER_H
#define ARTISTS_VIEWER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QList>
#include "downloader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class artists_viewer; }
QT_END_NAMESPACE

class artists_viewer : public QMainWindow
{
    Q_OBJECT

private:
    QMap <QChar,int> universal_map;
    QMap <QChar,int> emi_map;
    downloader downloader_tool;

    QString universal_txt = QDir::currentPath() +"/universal.txt";
    QString emi_txt = QDir::currentPath() +"/EMI.txt";

    const QString universal_url = "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt";
    const QString emi_url = "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt";

public:
    artists_viewer(QWidget *parent = nullptr);
    void start_prog();

    ~artists_viewer();

private slots:
    void use_downloaded_data(QByteArray data); //

private:
    Ui::artists_viewer *ui;
    void load_file();
    void create_chart(bool universal_artist);
    void size_chart();
    void addArtist(QString &element, QListWidget &list, bool universal_artist);
    void analyzeData(QString artist, bool universal_artist);
    void download_next_file();
};
#endif // ARTISTS_VIEWER_H
