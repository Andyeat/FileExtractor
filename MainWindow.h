#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSourceBrowser_clicked();

    void on_btnTargetBrowser_clicked();

    void on_btnFile2Suffix_clicked();

    void on_btnStart_clicked();

    void setCnrrentPertentage(const QString &fileName, const double &pertentage);

    void on_actAbout_clicked();

    void on_thread_finished();
private:
    Ui::MainWindow *ui;

    void startWorkInAThread();
};

#endif // MAINWINDOW_H
