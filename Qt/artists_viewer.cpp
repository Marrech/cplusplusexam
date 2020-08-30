#include "artists_viewer.h"
#include "ui_artists_viewer.h"

artists_viewer::artists_viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::artists_viewer)
{
    connect(&downloader_tool, SIGNAL(data_ready(QByteArray)), SLOT(use_downloaded_data(QByteArray)));
    ui->setupUi(this);
}

//Metodo che controlla se i file sono già presenti della directory
//   altrimenti prima di avviare il programma si occuperà del download
void artists_viewer::start_prog()
{
    if(QFileInfo::exists(universal_txt) && QFileInfo::exists(emi_txt))
    {
        load_file();
    }else
    {
        if(!(QFileInfo::exists(universal_txt)))
        {
            downloader_tool.make_request(universal_url);
        }
    }
}

//Distruttore
artists_viewer::~artists_viewer()
{
    QMapIterator<QChar, int> u(universal_map);
    while (u.hasNext())
    {
        u.next();
        universal_map.remove(u.key());
    }
    QMapIterator<QChar, int> e(emi_map);
    while (e.hasNext())
    {
        e.next();
        universal_map.remove(e.key());
    }
    universal_map.clear();
    emi_map.clear();
    delete ui;
}

//Metodo che si occupa della creazione dei grafici che riportano il numero
//  di artisti per singola lettera (su nuova finestra)
void artists_viewer::create_chart(bool universal_artist)
{
    QStringList categories;

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    QMapIterator<QChar, int> m(universal_map);
    if(universal_artist)
    {
        m = universal_map;
    }else
    {
        m = emi_map;
    }
    QBarSet *set = new QBarSet("Quantità");
    while (m.hasNext())
    {
        m.next();
        QString str(QChar(m.key()));
        categories << str;
        QString s = QString::number(m.value());
        *set << m.value();
    }
    series->append(set);
    series->setLabelsVisible(true);
    series->setBarWidth(0.5);

    QChart *chart = new QChart();
    chart->setY(1000);

    chart->addSeries(series);
    if(universal_artist)
    {
        chart->setTitle("Grafico Universal:");
    }else
    {
        chart->setTitle("Grafico EMI:");
    }
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QValueAxis *axisX = new QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisX->applyNiceNumbers();

    QAbstractAxis *xAxis = chart->axisX();
    xAxis->setRange(0,100);

    QMainWindow *windows = new QMainWindow();

    QChartView *chartsView = new QChartView(chart);
    chartsView->resize(400,900);
    chartsView->setParent(windows);
    if(universal_artist)
    {
        windows->setWindowTitle("Grafico Universal:");
    }else
    {
        windows->setWindowTitle("Grafico EMI:");
    }

    windows->resize(400,900);
    windows->show();
}

//Metodo che si occupa della creazione del grafico relativo alla quantità
//  di artisti per singola etichetta (su nuova finestra)
void artists_viewer::size_chart()
{
    QStringList categories;
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Quantità");

    int universal_artists_quantity = 0;
    int emi_artists_quantity = 0;

    QMapIterator<QChar, int> m(universal_map);
    while (m.hasNext())
    {
        m.next();
        universal_artists_quantity += m.value();
    }
    m = emi_map;
    while (m.hasNext())
    {
        m.next();
        emi_artists_quantity += m.value();
    }

    categories << "Universal" << "EMI";
    *set << universal_artists_quantity << emi_artists_quantity;
    series->append(set);
    series->setLabelsVisible(true);
    series->setBarWidth(0.5);

    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->setTitle("Numero di artisti per etichetta:");

    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axisX, series);

    QAbstractAxis *yAxis = chart->axisY();
    yAxis->setRange(0, 1000);

    QMainWindow *windows = new QMainWindow();

    QChartView *chartsView = new QChartView(chart);
    chartsView->resize(650,390);
    chartsView->setParent(windows);

    windows->setWindowTitle("Numero di artisti per etichetta:");

    windows->resize(650,390);
    windows->show();
}

//Metodo che si occupa della lettura dei file scaricati, richiama addArtist e in fine avvia
//  la creazione dei grafici
void artists_viewer::load_file()
{
    QFile file_universal(universal_txt);
    if (!file_universal.open(QIODevice::ReadOnly | QFile::Text))
    {
         ui->messageLabel->setText("Error: Durante il caricamento dei dati Universal!");
         return;
    }
    QString next_line;
    QTextStream u_in(&file_universal);
    while( !u_in.atEnd())
    {
        next_line = u_in.readLine();
        addArtist(next_line, *ui->universalListWidget, true);
    }


    QFile file_emi(emi_txt);
    if (!file_emi.open(QIODevice::ReadOnly | QFile::Text))
    {
         ui->messageLabel->setText("Error: Durante il caricamento dei dati Universal!");
         return;
    }
    QTextStream e_in(&file_emi);
    while( !e_in.atEnd())
    {
        next_line = e_in.readLine();
        addArtist(next_line, *ui->emiListWidget, false);
    }
    create_chart(true);
    create_chart(false);
    size_chart();
    ui->messageLabel->setText("Dati caricati, elaborati e grafici pronti!");
}

//Metodo che si occupa della creazione della QLabel e dell'aggiunta del link
void artists_viewer::addArtist(QString &element, QListWidget &list, bool universal_artist)
{
    QListWidgetItem* item = new QListWidgetItem();
    list.addItem(item);

    QStringList part = element.split(" ");
    if(part.length() != 2){ return; }
    part[0].remove('"');

    QLabel* hyperlinkWidget = new QLabel(this);

    hyperlinkWidget->setText("<a href=\"https://it.wikipedia.org/"+ part[0] +"\">" + part[1] + "</a>");
    hyperlinkWidget->setTextFormat(Qt::RichText);
    hyperlinkWidget->setTextInteractionFlags(Qt::TextBrowserInteraction);
    hyperlinkWidget->setOpenExternalLinks(true);

    list.setItemWidget(item, hyperlinkWidget);

    analyzeData(part[1], universal_artist);

    //connect(hyperlinkWidget, SIGNAL(linkActivated(const QString&)), this, SLOT(onHyperlinkActivated(const QString&)));
}

//Metodo che controlla quale sia il primo carattere dell'artista, e memorizza su uma QMap
void artists_viewer::analyzeData(QString artist, bool universal_artist)
{
    QChar first_char = artist.at(0);

    if(universal_artist)
    {
        if(!universal_map.contains(first_char))
        {
            universal_map.insert(first_char, 1);
        } else
        {
            universal_map[first_char]++;
        }
    }
    else
    {
        if(!emi_map.contains(first_char))
        {
            emi_map.insert(first_char, 1);
        } else
        {
            emi_map[first_char]++;
        }
    }

}

// Metodo che si occupa della ricezione e del salvataggio del file
void artists_viewer::use_downloaded_data(QByteArray data)
{
    QString data_to_string;
    data_to_string = data;
    QFile output_file;
    if(!QFileInfo::exists(universal_txt))
    {
        output_file.setFileName(universal_txt);
    }
    else if(!QFileInfo::exists(emi_txt))
    {
        output_file.setFileName(emi_txt);
    }
    else
    {
        return;
    }
    output_file.open(QIODevice::WriteOnly);
    if(!output_file.isOpen())
    {
            qDebug() << "- Error, unable to open" << output_file << "for output";
    }

    QTextStream out_stream(&output_file);
    out_stream << data_to_string;
    output_file.close();

    if(!QFileInfo::exists(emi_txt))
    {
        downloader_tool.make_request(emi_url);
    }

    if(QFileInfo::exists(universal_txt) && QFileInfo::exists(emi_txt))
    {
        load_file();
    }
}
