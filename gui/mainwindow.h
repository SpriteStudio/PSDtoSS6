#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>
#include <QProcess>
#include <QUrl>
#include <QMimeData>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QScrollBar>
#include <QThread>
#include <QDesktopServices>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setText_to_List(QStringList list);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void buttonEnable( bool flg );

private slots:
    void on_pushButton_exit_clicked();
    void on_pushButton_listclear_clicked();
    void on_pushButton_convert_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *cnvProcess;
    QProcess *imgProcess;
    QString data_path;
    void loadConfig(const QString & fileName);
    void saveConfig(const QString & fileName);
    void pushButton_enableset();
    void templistsave(void);
    void save_convert_info();
    void delete_convert_info();
    void templistload();
    template<typename T1, typename T2>
    T1 getKey(const std::map<T1, T2> & map, const T2 & value) const ;

private slots:
    void processErrOutput();
    void processFinished( int exitCode, QProcess::ExitStatus exitStatus);
    void on_pushButton_output_clicked();
    void on_pushButton_listsave_clicked();
    void on_pushButton_listload_clicked();
    void on_pushButton_fileadd_clicked();

    void on_pushButton_settingsave_clicked();
    void on_pushButton_open_help_clicked();
    void on_pushButton_Preview_clicked();

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
