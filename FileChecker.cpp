#include <QFileDialog>
#include <QDebug>

#include "FileChecker.h"

FileChecker::FileChecker(QObject *parent) : QObject(parent)
{

}

void FileChecker::process()
{
    // 这里，使用 new 分配资源
    qDebug("start process...");
    flieCnt = 0;
    copyNum = 0;
    ergodicDir(sourceDir, targetDir, false);
    ergodicDir(sourceDir, targetDir, true);
    emit finished();
}

/**
* @brief    遍历源文件夹，根据isCopy参数决定是否拷贝指定文件到目标文件夹
* @para     sourceDir：  源文件夹
*           targetDir：  目标文件夹
*           isCopy：     是否拷贝
* @author   Andy
* @date     2019-04-30
*/
void FileChecker::ergodicDir(const QString &sourceDir, const QString &targetDir, bool isCopy){
    QDir dir(sourceDir);
    foreach(QFileInfo mfi ,dir.entryInfoList()) {
        QString fileName = mfi.fileName();
        //如果是文件则进入拷贝流程，如果是文件夹则进入
        if(mfi.isFile()) {
//            if(!fileName.endsWith(suffix)) continue;
            if(!isEndWithSuffix(fileName)) continue;
            if(!isCopy) {
                ++flieCnt;
                qDebug()<<QString("read file:%1").arg(fileName);
                continue;
            }
            double percentage = (++copyNum + 0.0) / flieCnt * 100;
            QString source = sourceDir + "/" + fileName;
            QString target = targetDir + "/" + fileName;
            if(copyFileToPath(source, target)) {
                qDebug()<<QString("copy success. file=%1,pertent=%2").arg(fileName).arg(percentage);
            }
            emit cnrrentPertentage(fileName, percentage);
        } else {
            if(fileName=="." || fileName == "..") continue;
            qDebug() << "Entry Dir" << mfi.absoluteFilePath();
            ergodicDir(mfi.absoluteFilePath(), targetDir, isCopy);
        }
    }
}


//拷贝文件：
bool FileChecker::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir) {
        qDebug()<<QString("copy faild. sourceDir == toDir");
        return true;
    }
    if (!QFile::exists(sourceDir)) {
        qDebug()<<QString("copy faild. sourceDir is not exists");
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist && coverFileIfExist) {
        createfile->remove(toDir);
    }//end if

    if(!QFile::copy(sourceDir, toDir)) {
        qDebug()<<QString("copy faild. QFile copy faild");
        return false;
    }

    return true;
}

/**
* @brief    是否以指定字符串结尾
* @para     str：要查看的字符串
* @author   Andy
* @date     2019-05-07
*/
bool FileChecker::isEndWithSuffix(QString str)
{
    if(suffix.isEmpty()) return true;
    QStringList list = suffix.split(";");

    for(QString n: list) {
        if(n.isEmpty()) continue;
        if(str.endsWith(n)) return true;
    }
    return false;
}
