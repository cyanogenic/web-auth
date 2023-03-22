#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include <QTimer>
#include <QtDebug>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStatusBar *statusBr = statusBar();
    statusBr->addWidget(new QLabel("互联网连接: ", this));
    statusBr->addWidget(ui->networkLabel);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::isOnline);
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logInButton_clicked()
{
    doLogin(ui->userNameLineEdit->text().toStdString(), ui->userPasswdLineEdit->text().toStdString());
}

void MainWindow::on_logoutButton_clicked()
{
    doLogout();
}

void MainWindow::isOnline()
{
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.msftncsi.com/ncsi.txt"));
    QNetworkReply* reply = nam.get(req);
    QEventLoop loop;
    QTimer timeoutTimer;

    qDebug() << QTime::currentTime();

    connect(&timeoutTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    timeoutTimer.setSingleShot(true);
    timeoutTimer.start(3000);

    loop.exec();

    if (reply->bytesAvailable())
    {
        QString expectMessage = "Microsoft NCSI";
        QString replyMessage = reply->readAll();

        if (!expectMessage.compare(replyMessage))
        {
            ui->networkLabel->setText("已连接");
        }
        else
        {
            ui->networkLabel->setText("未连接");
        }
    }
    else
    {
        ui->networkLabel->setText("未连接");
    }
}
