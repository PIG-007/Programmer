#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "dialog.h"
#include "dialogfile.h"
#include "exchangedh.h"
#include "cryptdlg.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showDialogDlg();
    void showDialogFileDlg();
    void showExchangeDHDlg();
    void showCryptDlg();




private slots:


    void on_communicationBtn_clicked();

    void on_transFileBtn_clicked();

    void on_cryptBtn_clicked();

    void on_DH_ExchangeBtn_clicked();

private:
    Ui::MainWindow *ui;
    dialog* dialogDlg;
    Login* loginDlg;
    cryptDlg* cryptDlgDlg;
    dialogFile* dialogFileDlg;
    exchangeDH* exchangeDHDlg;
    client* clientDlg;

};
#endif // MAINWINDOW_H
