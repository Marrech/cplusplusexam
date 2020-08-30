#include "downloader.h"

downloader::downloader(QObject *parent) : QObject(parent)
{
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_data(QNetworkReply*)));
}

void downloader::make_request(QString url)
{
    manager->get(QNetworkRequest(QUrl(url)));
}

void downloader::read_data(QNetworkReply *reply)
{
    QByteArray my_data;
    my_data = reply->readAll();
    emit(data_ready(my_data));
}


