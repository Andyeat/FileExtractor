#ifndef FILECHECKER_H
#define FILECHECKER_H
#include <QThread>

class FileChecker : public QObject
{
    Q_OBJECT
public:
    explicit FileChecker(QObject *parent = 0);

    QString getSourceDir() { return sourceDir; }
    void setSourceDir(QString tmp) {sourceDir = tmp;}
    QString getTargetDir() { return targetDir; }
    void setTargetDir(QString tmp) {targetDir = tmp;}
    QString getSuffix() { return suffix; }
    void setSuffix(QString tmp) {suffix = tmp;}

signals:
    void cnrrentPertentage(const QString &fileName, const double &pertentage);
    void finished();
    void error(QString err);
public slots:
    void process();

private:
    int flieCnt;
    int copyNum;
    QString sourceDir;  //源文件夹
    QString targetDir;  //目标文件夹
    QString suffix;     //后缀
    void ergodicDir(const QString &sourceDir, const QString &targetDir, bool isCopy);
    bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist = true);
    bool isEndWithSuffix(QString str);
};

#endif // FILECHECKER_H





