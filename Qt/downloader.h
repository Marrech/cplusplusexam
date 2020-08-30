#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class downloader : public QObject
{
    Q_OBJECT
public:
    explicit downloader(QObject *parent = nullptr);
    void make_request(QString url); // Funzione che invia la richiesta tramite il manager

public slots:
    void read_data(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

signals:
    void data_ready(QByteArray); //<Per segnalare che i dati sono pronti all'utilizzo

};

#endif // DOWNLOADER_H
