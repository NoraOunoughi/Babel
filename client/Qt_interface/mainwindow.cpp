#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(std::string &tcpIp, std::size_t portTCP, std::size_t portUdp, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new BabelClient(tcpIp, portTCP, portUdp);
    client->run();
    _startCallback = true;
    getThread_ = new(std::nothrow) std::thread([&] { getCallback(); });
    current = "start";
}

void MainWindow::setCurrent(std::string &where)
{
    current = where;
}

void MainWindow::getCallback()
{
    std::string where = "";
    while (_startCallback) {
        where = client->getWhereAmI();
        if (where != current) {
            if (where == "home") {
                ui->stackedWidget->setCurrentIndex(2);
                setCurrent(where);
            }
            if (where == "calling") {
                // std::cout << "someone call you" << std::endl;
                // ui->stackedWidget->setCurrentIndex(3);
                ui->stackedWidget->setCurrentIndex(4);
                setCurrent(where);
            }
            if (where == "call") {
                ui->stackedWidget->setCurrentIndex(3);
                setCurrent(where);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    client->stop();
    _startCallback = false;
    if (getThread_) {
        getThread_->join();
        delete(getThread_);
    }
    delete ui;
    std::cout << "Delete window" << std::endl;
}

void MainWindow::on_LoginpushButton_clicked()
{
    std::string username;
    // ui->stackedWidget->setCurrentIndex(2);
    username = ui->LoginlineEdit->text().toStdString();
    client->login(username);
}

void MainWindow::on_RegisterPushButton_2_clicked()
{
    std::string username = ui->id_lineEdit->text().toStdString();
    std::string passwd = ui->Pwd_lineEdit->text().toStdString();
    std::string confpasswd = ui->PwdConf_lineEdit->text().toStdString();
    ui->stackedWidget->setCurrentIndex(1);
    std::string where = "register";
    setCurrent(where);
    client->setWhereAmI(where);
}

void MainWindow::on_BackLogin_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    std::string where = "start";
    setCurrent(where);
    client->setWhereAmI(where);
}

void MainWindow::on_Call_pushButton_clicked()
{
    // ui->stackedWidget->setCurrentIndex(3);
    std::string destidString = ui->Call_id_lineEdit->text().toStdString();
    std::size_t destId = std::atoi(destidString.c_str());
    client->sendCallRequest(destId);
    //new window avec call lancé
    //si pas accepté : Afficher text : call denied
    ///Lorsque call termniné : revenir sur page call donc currentIndex : 2
}

void MainWindow::on_pushButton_3_clicked()
{
    client->sendHangUp();
    ui->stackedWidget->setCurrentIndex(2);
    std::string where = "home";
    setCurrent(where);
}

void MainWindow::on_DenyCall_pushButton_clicked()
{
    std::size_t _callerId = client->getCallerId();
    client->acceptDenyCall(false, _callerId);
    ui->stackedWidget->setCurrentIndex(2);
    std::string where = "home";
    setCurrent(where);
}

void MainWindow::on_AcceptCall_pushButton_clicked()
{
    std::size_t _callerId = client->getCallerId();
    client->acceptDenyCall(true, _callerId);
    ui->stackedWidget->setCurrentIndex(3);
    std::string where = "call";
    setCurrent(where);
}

void MainWindow::on_LogOut_pushButton_clicked()
{
    client->logout(true, client->getId());
    ui->stackedWidget->setCurrentIndex(0);
    std::string where = "start";
    setCurrent(where);
}