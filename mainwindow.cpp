#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "count.h"
#include <iostream>
#include <string>
#include <QTextStream>
#include <QElapsedTimer>
#include <QTableView>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tableView);
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
    model = new QStandardItemModel(rowC,colC,this);
    ui->tableView->setModel(model);

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
                QModelIndex index = model->index(r,c,QModelIndex());
                model->setData(index,cur.c_str());
                cur = "";
                c++;
            }
            //if not a comma, build cell value
            //this statement is also run when a comma is reached but the comma is between double quotes. thus adding the comma into the cell value
            else{
                cur += currentLine[i];
            }
        }
        //appends the last value of each row to the table
        QModelIndex index = model->index(r,c,QModelIndex());
        model->setData(index,cur.c_str());
        r++;
    }
    std::cout << timer.elapsed() << std::endl;
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    QFile file(currentFile);
    //clear contents of file to prevent overwriting errors
    //eventually replace this with better method, this method could be dangerous
    file.resize(0);
    int rowC = model->rowCount();
    int colC = model->columnCount();
    if (file.open(QIODevice::ReadWrite | QFile::Text)){
        QTextStream stream(&file);
        for (int i = 0; i<rowC; i++){
            for (int j = 0; j<colC; j++){
                std::cout<<model->index(i, j).data().toString().toStdString()<<std::endl;
                stream<<model->index(i, j).data().toString();
                //checks if the cursor is on the last word, if so, don't add comma to end of word
                if(j!=colC-1){
                    stream<<",";
                }
            }
            //after entire row is read, add new line
            if (i != rowC-1){
                stream<<"\n";
            }
        }
    }

    file.close();
}


void MainWindow::on_actionAdd_Row_triggered()
{
    model->insertRow(model->rowCount(QModelIndex()));
}


void MainWindow::on_actionAdd_Column_triggered()
{
    model->insertColumn(model->columnCount(QModelIndex()));
}


void MainWindow::on_actionRemove_Row_triggered()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    while (!selected.isEmpty()){
        model->removeRows(selected.last().row(),1);
        selected.removeLast();
    }
}


void MainWindow::on_actionRemove_Column_triggered()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedColumns();
    while (!selected.isEmpty()){
        model->removeColumns(selected.last().column(),1);
        selected.removeLast();
    }

}

