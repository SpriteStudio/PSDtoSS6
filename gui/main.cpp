#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include <QTranslator>

QTranslator translator;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

        QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	// 翻訳
        if(QLocale::system().name()=="ja_JP")
        {
            translator.load("translate_ja");
        }
        else
        {
            translator.load("translate_en");
        }
        a.installTranslator(&translator);

    MainWindow w;
    w.setText_to_List(a.arguments());
    w.show();

    return a.exec();
}
