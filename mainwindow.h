#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionAdd_Row_triggered();

    void on_actionAdd_Column_triggered();

    void on_actionRemove_Row_triggered();

    void on_actionRemove_Column_triggered();

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QStandardItemModel * model;
};
#endif // MAINWINDOW_H
