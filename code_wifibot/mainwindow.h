#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QKeyEvent>
#include <string>
#include <QTimer>
#include <QtWebEngineWidgets>
#include <QIcon>
#include <tcpconnection.h>
#include <sensors.h>

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
    void on_connectButton_clicked();
    void on_forwardButton_pressed();
    void on_backwardButton_pressed();
    void on_leftButton_pressed();
    void on_rightButton_pressed();
    void on_boost_pressed();
    void on_turnAwayButton_pressed();
    void releaseButton();
    void on_filter1Button_clicked();
    void on_filter2Button_clicked();
    void on_filter3Button_clicked();
    void on_filter4Button_clicked();
    void on_filter5Button_clicked();
    void on_filter6Button_clicked();
    void on_filter7Button_clicked();
    void on_resetFilterButton_clicked();
    void on_speedSlider_valueChanged();
    void on_camUpButton_clicked();
    void on_camDownButton_clicked();
    void on_camLeftButton_clicked();
    void on_camRightButton_clicked();
    void on_camResetButton_clicked();
    void updateSensorsValues(Sensors s);




    void on_capture_clicked();

private:
    Ui::MainWindow *ui;
    TcpConnection* tcpSocket;
    QWebEngineView* streamView;

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

    //Accès Network : pour camera
    QNetworkAccessManager *manager;
    int speedMax = 100;

    //Acceleration Vitesse
    int previousSpeed;
    bool boolSpeed = false;

    //Capture
    QString path;
    QString fileName;
    QString extension;
    QString pathCapture;
    QDateTime nowDate;
    QString fileNameDate;
};

#endif // MAINWINDOW_H
