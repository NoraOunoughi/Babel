#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <ostream>
#include "../BabelClient/BabelClient.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::string &tcpIp, std::size_t portTCP, std::size_t portUdp, QWidget *parent = nullptr);
    ~MainWindow();

    void getCallback();
    void setCurrent(std::string &where);

    BabelClient *client;
    std::thread *getThread_;
    bool _startCallback;
    std::string current;

private slots:
    void on_LoginpushButton_clicked();

    void on_BackLogin_pushButton_clicked();

    void on_RegisterPushButton_2_clicked();

    void on_Call_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_DenyCall_pushButton_clicked();

    void on_AcceptCall_pushButton_clicked();

    void on_LogOut_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
