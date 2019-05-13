#include <QApplication>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QFileInfoList>
#include <QDir>

#include "MainWindow.h"
#include "log/log4z.h"

using namespace zsummer::log4z;
using namespace std;


void setQssFile(const QString &dirPath)
{
    //qss模板
    QString qss = "";
    //设置qss文件夹路径(相对路径和绝对路径都可)
    QDir dir(dirPath);
    QFileInfoList fileList = dir.entryInfoList();

    qDebug() << QStringLiteral("--------从资源目录读取qss文件begin--------");
    qDebug() << QString("Dir is exists. dirPath=%1").arg(dir.absolutePath()) << dir.exists();
    foreach(QFileInfo fileItem, fileList) {
        if (fileItem.isFile() && fileItem.suffix() == QString("qss")) {
            qDebug() << "read file. filePath="<<fileItem.absoluteFilePath();
            QFile specifyStylefile(fileItem.absoluteFilePath());
            specifyStylefile.open(QFile::ReadOnly);
            qss += QString::fromUtf8(specifyStylefile.readAll());
            specifyStylefile.close();
        }
    }
    qDebug() << QStringLiteral("--------从资源目录读取qss文件end--------");

    qApp->setStyleSheet(qss);
}

void initLog() {
    //启动LOG4Z
    //没有添加任何日志记录器的情况下 LOG4Z会默认构建一个.
    ILog4zManager::getInstance()->start();

    //使用快速日志记录宏 可以快速把日志写入默认日志记录器中.
    //LOGD对应 LOG DEBUG, LOGI对应LOG INFO 类推...
    //该宏采用流的形式录入日志信息
    LOGD(" *** " << "hellow wolrd" <<" *** ");
    LOGI("loginfo");
    LOGW("log warning");
    LOGE("log err");
    LOGA("log alarm");
    LOGF("log fatal");

    QString str = QStringLiteral("测试一下日志系统");
    int i = 321;

    LOGFMTF("text log4z input *** %s *** %d ***", str.toStdString().c_str(), i);
    LOGFMTF("text log4z input *** %s *** %d ***", str.toUtf8().data(), i);


    //begin test stream log input....
    LOGT("stream input *** " << "LOGT LOGT LOGT LOGT" << " *** ");
    LOGD("stream input *** " << "LOGD LOGD LOGD LOGD" << " *** ");
    LOGI("stream input *** " << "LOGI LOGI LOGI LOGI" << " *** ");
    LOGW("stream input *** " << "LOGW LOGW LOGW LOGW" << " *** ");
    LOGE("stream input *** " << "LOGE LOGE LOGE LOGE" << " *** ");
    LOGA("stream input *** " << "LOGA LOGA LOGA LOGA" << " *** ");
    LOGF("stream input *** " << "LOGF LOGF LOGF LOGF" << " *** ");

    // cannot support VC6 or VS2003
    //begin test format log input....
    LOGFMTT("format input *** %s *** %d ***", "LOGFMTT", 123456);
    LOGFMTD("format input *** %s *** %d ***", "LOGFMTD", 123456);
    LOGFMTI("format input *** %s *** %d ***", "LOGFMTI", 123456);
    LOGFMTW("format input *** %s *** %d ***", "LOGFMTW", 123456);
    LOGFMTE("format input *** %s *** %d ***", "LOGFMTE", 123456);
    LOGFMTA("format input *** %s *** %d ***", "LOGFMTA", 123456);
    LOGFMTF("format input *** %s *** %d ***", "LOGFMTF", 123456);

    //停止LOG4Z
    //如果不停止 将在程序退出时会安全销毁
    //ILog4zManager::getInstance()->stop();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载QSS样式
    setQssFile(":/qss");
    MainWindow w;
    w.show();
    initLog();
    return a.exec();
}
