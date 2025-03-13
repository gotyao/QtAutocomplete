#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBox->setValue(5);
    filename = QFileDialog::getOpenFileName(this,
                                            "open a file",
                                            "C:",
                                            "txt(*.txt) ;; all (*.*)");
    if(filename.isEmpty()){
        ui->pushButton->setText("当前无文件打开。\n单击添加文件");
        return ;
    }

    clock_t startload, endload, endsort;
    startload = clock();

    QFile dataFile(filename);
    dataFile.open(QIODevice::ReadOnly);

    QByteArray array;
    array = dataFile.readLine();
    auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
    QString info = toUtf8(array);
    info.erase(info.cend()-1);

    N = info.toInt();
    fdata = new Data[N+1];
    int i = 1;

    do{
        array = dataFile.readLine();
        auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
        QString info = toUtf8(array);
        info.erase(info.cend()-1);

        while(!info[0].isDigit()) info.erase(info.begin());
        string str = info.toStdString();
        long long t = stoll(str);

        while(info[0].isDigit()) info.erase(info.begin());
        info.erase(info.begin());
        str = info.toStdString();

        Data temp(t, str);
        fdata[i++] = temp;
    }while(!(dataFile.atEnd()));
    qDebug() << "load finish";
    endload = clock();

    dataFile.close();
    HeapSort temp(fdata, N);
    temp.acHeapSort();
    qDebug() << "sort finish";
    endsort = clock();
    qDebug() << endload - startload << endsort - endload;
    ui->pushButton->setText("当前文件为：\n"+ filename + "\n单击切换文件");

    ui->label->installEventFilter(this);
    connect(this, beginload, [=](){
        statusBar()->showMessage("打开文件中……");
        repaint();
    });
    connect(this, appendline, [=](QString& str){
        ui->textBrowser->append(str);
        repaint();
    });
    statusBar()->showMessage("打开文件用时：" + QString::number(endsort - startload) + "ms");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->statusbar){
        return true;
    }
    else if(obj == ui->textBrowser){
        return true;
    }
    else return QWidget::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete[] fdata;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    ui->lineEdit->clear();
    QString t = QFileDialog::getOpenFileName(this,
                                                "open a file",
                                                "C:",
                                                "txt(*.txt) ;; all (*.*)");
    if(t.isEmpty()) return ;
    clock_t startload, endload, endsort;
    startload = clock();
    emit beginload();

    delete[] fdata;
    fdata = nullptr;

    filename = t;
    QFile dataFile(filename);
    dataFile.open(QIODevice::ReadOnly);

    QByteArray array;
    array = dataFile.readLine();
    auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
    QString info = toUtf8(array);
    info.erase(info.cend()-1);

    N = info.toInt();
    fdata = new Data[N+1];
    int i = 1;

    do{
        array = dataFile.readLine();
        auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
        QString info = toUtf8(array);
        info.erase(info.cend()-1);

        while(!info[0].isDigit()) info.erase(info.begin());
        string str = info.toStdString();
        long long t = stoll(str);

        while(info[0].isDigit()) info.erase(info.begin());
        info.erase(info.begin());
        str = info.toStdString();

        Data temp(t, str);
        fdata[i++] = temp;
    }while(!(dataFile.atEnd()));
    qDebug() << "load finish";
    endload = clock();

    dataFile.close();
    HeapSort temp(fdata, N);
    temp.acHeapSort();
    qDebug() << "sort finish";
    endsort = clock();
    qDebug() << endload - startload << endsort - endload;
    ui->pushButton->setText("当前文件为：\n"+ filename + "\n单击切换文件");

    statusBar()->showMessage("打开文件用时：" + QString::number(endsort - startload) + "ms");
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(filename.isEmpty()) return ;

    clock_t start, end;
    start = clock();
    ui->textBrowser->clear();
    int l{1}, h{N};
    string prefix = arg1.toStdString();
    if(prefix.empty()) return ;

    int mid1{1};
    while(l <= h){
        mid1 = l + ((h-l)>>1);
        int Case = datastrcmp(prefix, fdata[mid1].s);

        if(!Case) h = mid1-1; // <
        else if(Case == 2) l = mid1+1; // >
        else{ // ==
            if(mid1 == 1 || datastrcmp(prefix, fdata[mid1-1].s) == 2){
                break;
            }
            else {
                h = mid1-1;
            }
        }
    }

    if(l > h && datastrcmp(prefix, fdata[mid1].s) != 1){
        qDebug() << "cool1" << mid1;
        end = clock();
        statusBar()->showMessage("搜索时间：" + QString::number(end - start) + "ms", 3000);
        return ;
    }

    l = mid1, h = N;
    int mid2{l};
    while(l <= h){
        mid2 = l + ((h-l)>>1);
        int Case = datastrcmp(prefix, fdata[mid2].s);

        if(!Case) h = mid2-1; // <
        else if(Case == 2) l = mid2+1; // >
        else{ // ==
            if(mid2 == N || !datastrcmp(prefix, fdata[mid2+1].s)){
                break;
            }
            else {
                l = mid2+1;
            }
        }
    }

    if(l > h && datastrcmp(prefix, fdata[mid2].s) != 1){
        qDebug() << "cool2"<< mid1 << mid2;
        end = clock();
        statusBar()->showMessage("搜索时间：" + QString::number(end - start) + "ms", 3000);
        return ;
    }

    heap re(fdata + mid1, fdata + mid2);
    int k = ui->spinBox->value();
    int n = mid2-mid1+1;
    k = (k > n ? n : k);
    qDebug() << mid1 << mid2 << k;
    re.acHeapSort(k);
    end = clock();

    statusBar()->showMessage("搜索时间：" + QString::number(end - start) + "ms", 3000);
    for(int i = 0; i < k; i++){
        QString temp = "<font style = 'color:#000000;' >" + QString::fromStdString(re.pa[n - i].a->s) +  "<font>" + " "
                       + "<font style = 'color:#CDC1A7;' style = 'text-align:right;'>" + QString::number(re.pa[n - i].a->f) + "<font>";
        emit appendline(temp);
    }
}

int datastrcmp(const string& a, const string& b){
    if(a > b) return 2;
    int n = a.length();
    for(int i = 0; i < n; i++){
        if(a[i] != b[i]){
            return 0;
        }
    }

    return 1;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if(filename.isEmpty()) return ;

    clock_t start, end;
    start = clock();
    ui->textBrowser->clear();
    int l{0}, h{N-1};
    string prefix = ui->lineEdit->text().toStdString();
    if(prefix.empty()) return ;

    int mid1{0};
    while(l <= h){
        mid1 = l + ((h-l)>>1);

        if(datastrcmp(prefix, fdata[mid1].s) == 2) l = mid1+1; // >
        else h = mid1-1; // <=
    }

    if(l < N-1 && datastrcmp(prefix, fdata[l].s) == 1){
        mid1 = l;
    }
    else{
        end = clock();
        statusBar()->showMessage("搜索时间：" + QString::number(end - start) + "ms", 3000);
        return ;
    }

    qDebug() << "*******************************************************8";
    l = mid1, h = N-1;
    int mid2{l};
    while(l <= h){
        mid2 = l + ((h-l)>>1);

        if(!datastrcmp(prefix, fdata[mid2].s)) h = mid2-1; // <
        else l = mid2+1; // >=
    }

    if(h >= 0 && datastrcmp(prefix, fdata[h].s) == 1){
        mid2 = h;
    }
    else{
        end = clock();
        statusBar()->showMessage("搜索时间：" + QString::number(end - start) + "ms", 3000);
        return ;
    }

    heap re(fdata + mid1, fdata + mid2);
    int k = arg1;
    int n = mid2-mid1+1;
    k = (k > n ? n : k);
    qDebug() << mid1 << mid2 << k;
    re.acHeapSort(k);
    end = clock();

    statusBar()->showMessage("搜索时间：" + QString::number(end - start) + "ms", 3000);
    for(int i = 0; i < k; i++){
        QString temp = "<font style = 'color:#000000;' >" + QString::fromStdString(re.pa[n - i].a->s) +  "<font>" + " "
                       + "<font style = 'color:#CDC1A7;' style = 'text-align:right;'>" + QString::number(re.pa[n - i].a->f) + "<font>";
        emit appendline(temp);
    }
}
