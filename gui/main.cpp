#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    w.setText_to_List(a.arguments());
    w.show();

    return a.exec();
}
