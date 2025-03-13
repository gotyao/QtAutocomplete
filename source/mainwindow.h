#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QStringDecoder>
#include <QFileDialog>
#include "heap.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void beginload();
    void appendline(QString& str);

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QString filename;
    Data* fdata;
    int N;
};

int datastrcmp(const string& a, const string & b);
#endif // MAINWINDOW_H
