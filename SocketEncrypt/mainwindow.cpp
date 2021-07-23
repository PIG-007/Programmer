#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "dialog.h"
#include <QStyleFactory>
#include "fileclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginDlg = new Login;
    loginDlg->show();
    connect(loginDlg,SIGNAL(login()),this,SLOT(show()));

    dialogDlg = new dialog;
    dialogFileDlg = new dialogFile;
    exchangeDHDlg = new exchangeDH;
    cryptDlgDlg = new cryptDlg;

//    clientDlg = new client;
//    clientDlg->show();

    client* clientDlg2 = new client;
    clientDlg2->show();


//    fileClient *fileclientDlg = new fileClient;
//    fileclientDlg->show();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showDialogDlg()
{
    dialogDlg->setWindowModality(Qt::ApplicationModal);
    dialogDlg->show();
}

void MainWindow::showDialogFileDlg()
{
    dialogFileDlg->setWindowModality(Qt::ApplicationModal);
    dialogFileDlg->show();
}

void MainWindow::showExchangeDHDlg()
{
    exchangeDHDlg->setWindowModality(Qt::ApplicationModal);
    exchangeDHDlg->show();
}

void MainWindow::showCryptDlg()
{
    cryptDlgDlg->setWindowModality(Qt::ApplicationModal);
    cryptDlgDlg->show();
}


void MainWindow::on_communicationBtn_clicked()
{
    showDialogDlg();
}


void MainWindow::on_transFileBtn_clicked()
{
    showDialogFileDlg();
}


void MainWindow::on_cryptBtn_clicked()
{
    showCryptDlg();
}


void MainWindow::on_DH_ExchangeBtn_clicked()
{
    showExchangeDHDlg();
}

