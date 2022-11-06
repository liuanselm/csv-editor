#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "count.h"
#include <iostream>
#include <string>
#include <QTextStream>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QElapsedTimer timer;
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    QTextStream in(&file);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
    }
    setWindowTitle(fileName);
    timer.start();

    int r = 0;
    int rowC = rowCount(fileName.toStdString());
    int colC = colCount(fileName.toStdString());
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(colC);

    while (!in.atEnd() && r < 10000){
        string currentLine = in.readLine().toStdString();
        int n = currentLine.length();
        int c = 0;
        bool a = false;
        std::string cur = "";
        for (int i=0; i< n; i++){
            //if reached a double quotes, dont add value to cell value
            //boolean that also dictates whether or not comma should be inside cell value or not
            if (currentLine[i] == '"'){
                a = !a;
                continue;
            }
            //reached the end of a cell, print cell value
            if (currentLine[i] == ',' && a == false){
                ui->tableWidget->setItem(r,c,new QTableWidgetItem(tr("%1").arg(QString::fromStdString(cur))));
                cur = "";
                c++;
            }
            //if not a comma, build cell value
            //this statement is also run when a comma is reached but the comma is between double quotes. thus adding the comma into the cell value
            else{
                cur += currentLine[i];
            }
        }
        ui->tableWidget->setItem(r,c,new QTableWidgetItem(tr("%1").arg(QString::fromStdString(cur))));
        r++;
    }
    std::cout << timer.elapsed() << std::endl;
    file.close();
}

