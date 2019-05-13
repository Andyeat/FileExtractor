#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QIcon>
#include <QTimer>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FileChecker.h"
#include "log/log4z.h"

using namespace zsummer::log4z;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("文件提取器"));
    setWindowIcon( QIcon(":/image/icon.ico"));

    connect(ui->actAbout, &QAction::triggered, this, &MainWindow::on_actAbout_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSourceBrowser_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择文件路径..."), QStringLiteral("./"));
    if(file_path.isEmpty()) return;

    qDebug()<<"file_path:" <<file_path;
    LOGFMTD("Source file Browser. path=%s", file_path.toStdString().c_str());
    ui->letSourcePath->setText(file_path);
}

void MainWindow::on_btnTargetBrowser_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择文件路径..."), QStringLiteral("./"));
    if(file_path.isEmpty()) return;

    qDebug()<<"file_path:" <<file_path;
    LOGFMTD("Target file Browser. path=%s", file_path.toStdString().c_str());
    ui->letTargetPath->setText(file_path);
}

void MainWindow::on_btnFile2Suffix_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), NULL, tr("*.*"));
    if(!fileName.contains(".")) return;
    //后缀名
    int suffixIndex = fileName.lastIndexOf(".");
    qDebug()<<"fileName:" <<fileName;
    LOGFMTD("Suffix file Browser. path=%s", fileName.toStdString().c_str());
    ui->letSuffix->insert(";");
    ui->letSuffix->insert(fileName.mid(suffixIndex, fileName.size() - suffixIndex));
}

void MainWindow::on_btnStart_clicked()
{
    QString sourceDir = ui->letSourcePath->text();
    if(sourceDir.isEmpty()) sourceDir = QCoreApplication::applicationDirPath();
    QString targetDir = ui->letTargetPath->text();
    if(sourceDir.isEmpty() || targetDir.isEmpty()) {
        qDebug()<<QString("Dir path is null");
        LOGFMTE("Dir path is null");
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("错误"));
        msgBox.setText(QStringLiteral("源文件夹或目标文件夹为空！"));
        msgBox.exec();
        return;
    }

    QDir dir(sourceDir);
    if(!dir.exists()) {
        qDebug()<<QString("Dir not exists. targetDir=%1").arg(targetDir);
        LOGFMTE("Dir not exists. targetDir=%s", targetDir.toStdString().c_str());
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("错误"));
        msgBox.setText(QStringLiteral("源文件夹不存在！"));
        msgBox.exec();
        return;
    }
    dir.setPath(targetDir);
    if(!dir.exists()) {
        qDebug()<<QString("Dir not exists. sourceDir=%1").arg(sourceDir);
        LOGFMTE("Dir not exists. sourceDir=%s", sourceDir.toStdString().c_str());
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("错误"));
        msgBox.setText(QStringLiteral("目标文件夹不存在！"));
        msgBox.exec();
        return;
    }
    startWorkInAThread();
}



void MainWindow::startWorkInAThread()
{
    QThread *thread = new QThread();
    FileChecker *worker = new FileChecker();
    worker->moveToThread(thread);

    // 错误处理
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));

    // 处理数据
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, &FileChecker::cnrrentPertentage, this, &MainWindow::setCnrrentPertentage);

    // 退出、删除
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    worker->setSourceDir(ui->letSourcePath->text());
    worker->setTargetDir(ui->letTargetPath->text());
    worker->setSuffix(ui->letSuffix->text());

    // 启动线程
    thread->start();
}

void MainWindow::setCnrrentPertentage(const QString &fileName, const double &pertentage) {
    ui->lblFileName->setText(fileName);
    ui->progressBar->setValue(pertentage);
}

void MainWindow::on_actAbout_clicked()
{
    QFile loadFile(":/readme.txt");
    QString msg = QStringLiteral("软件信息文件损坏");
    if (loadFile.open(QIODevice::ReadOnly)) {
        qDebug()<<"file open success.";
        LOGFMTE("file open success.");
        QByteArray allData = loadFile.readAll();
        msg = QString(allData);
    }
    loadFile.close();

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("软件信息"));
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::on_thread_finished()
{
    LOGFMTD("file trans finished.");
    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("提示"));
    msgBox.setText(QStringLiteral("文件传输完成！"));
    msgBox.exec();
    ui->progressBar->setValue(0);
    ui->lblFileName->setText(QStringLiteral("无"));
}
