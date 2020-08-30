#include "artists_viewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    artists_viewer w;
    w.show();

    w.start_prog();

    return a.exec();
}
