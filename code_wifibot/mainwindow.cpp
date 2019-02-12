#include "mainwindow.h"
#include "ui_mainwindow.h"


//******************CONSTRUCTEUR FENETRE******************//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Initialisation du graphique
    ui->setupUi(this);
    tcpSocket = new TcpConnection();

    //Camera :
    streamView = new QWebEngineView();
    ui->gridLayout_6->addWidget(streamView);
    streamView->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    streamView->show();

    //Signals :
    connect(ui->forwardButton, SIGNAL(released()), this, SLOT (releaseButton()));
    connect(ui->backwardButton, SIGNAL(released()), this, SLOT (releaseButton()));
    connect(ui->leftButton, SIGNAL(released()), this, SLOT (releaseButton()));
    connect(ui->rightButton, SIGNAL(released()), this, SLOT (releaseButton()));
    connect(ui->turnAwayButton, SIGNAL(released()), this, SLOT (releaseButton()));
    connect(ui->boost, SIGNAL(released()), this, SLOT (releaseButton()));
    connect(tcpSocket, SIGNAL(sensorsValues(Sensors)), this, SLOT(updateSensorsValues(Sensors)));

    //WebsiteAccess :
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

//******************DECONSTRUCTEUR FENETRE******************//
MainWindow::~MainWindow()
{
    delete ui;
    delete tcpSocket;
}

void MainWindow::on_connectButton_clicked(){
    bool ok;
    if(ui->connectButton->text() == "Connect"){
        tcpSocket->connectTo(ui->hostEdit->text(), ui->portEdit->text().toInt(&ok, 10));
        ui->connectButton->setText("Disconnect");
    } else{
        tcpSocket->disconnect();
        ui->connectButton->setText("Connect");
    }
}

void MainWindow::on_forwardButton_pressed(){
    tcpSocket->moveRobot("forward", ui->speedSlider->value());
    std::cout << "[Info] Robot go forward." << std::endl;
}

void MainWindow::on_backwardButton_pressed(){
    tcpSocket->moveRobot("backward", ui->speedSlider->value());
    std::cout << "[Info] Robot go backward." << std::endl;
}

void MainWindow::on_leftButton_pressed(){
    tcpSocket->moveRobot("left", ui->speedSlider->value());
    std::cout << "[Info] Robot go to the left." << std::endl;
}

void MainWindow::on_rightButton_pressed(){
    tcpSocket->moveRobot("right", ui->speedSlider->value());
    std::cout << "[Info] Robot go to the right." << std::endl;
}


void MainWindow::on_turnAwayButton_pressed(){
    tcpSocket->moveRobot("turnaway", ui->speedSlider->value());
    std::cout << "[Info] Robot turnaway." << std::endl;
}

void MainWindow::on_boost_pressed()
{
    boolSpeed = true;
    previousSpeed = ui->speedSlider->value();
    ui->speedSlider->setValue(speedMax);
    std::cout << "[Info] Boost : vitesse maximale." << std::endl;
}

void MainWindow::releaseButton(){
    //Condition pour BOOST
    if (boolSpeed){
        ui->speedSlider->setValue(previousSpeed);
        boolSpeed = false;
    }else{
        tcpSocket->moveRobot("stop");
        std::cout << "[Info] Stop robot." << std::endl;
    }
}

void MainWindow::on_camUpButton_clicked(){
    std::cout << "[Info] Camera robot up." << std::endl;
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-250")));
}

void MainWindow::on_camDownButton_clicked(){
    std::cout << "[Info] Camera robot down." << std::endl;
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=250")));
}

void MainWindow::on_camLeftButton_clicked(){
    std::cout << "[Info] Camera robot left." << std::endl;
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=250")));
}

void MainWindow::on_camRightButton_clicked(){
    std::cout << "[Info] Camera robot right." << std::endl;
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-250")));
}

void MainWindow::on_camResetButton_clicked(){
    std::cout << "[Info] Camera robot reset." << std::endl;
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094855&group=1&value=1")));
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094854&group=1&value=1")));
}

void MainWindow::on_filter1Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' grayscale(100%)';");
}
void MainWindow::on_filter2Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' invert(100%)';");
}

void MainWindow::on_filter3Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' sepia(100%)';");
}

void MainWindow::on_filter4Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' blur(5px)';");
}
void MainWindow::on_filter5Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' saturate(500%)';");
}
void MainWindow::on_filter6Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' hue-rotate(180deg)';");
}
void MainWindow::on_filter7Button_clicked(){
    streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' brightness(50%)';");
}
void MainWindow::on_resetFilterButton_clicked(){
    streamView->page()->runJavaScript("document.body.firstChild.style.webkitFilter = ''");
}

//******************LECTURE TOUCHE CLAVIER PRESSEE --> Directions ******************//
void MainWindow::keyPressEvent(QKeyEvent *ev){
    switch(ev->key()){
    //1 - Directions
    case Qt::Key_Z:
        tcpSocket->moveRobot("forward", ui->speedSlider->value());
        std::cout << "[Info] Robot go forward." << std::endl;
        break;
    case Qt::Key_S:
        tcpSocket->moveRobot("backward", ui->speedSlider->value());
        std::cout << "[Info] Robot go backward." << std::endl;
        break;
    case Qt::Key_Q:
        tcpSocket->moveRobot("left", ui->speedSlider->value());
        std::cout << "[Info] Robot go to the left." << std::endl;
        break;
    case Qt::Key_D:
        tcpSocket->moveRobot("right", ui->speedSlider->value());
        std::cout << "[Info] Robot go to the right." << std::endl;
        break;
    case Qt::Key_W:
        tcpSocket->moveRobot("turnaway", ui->speedSlider->value());
        std::cout << "[Info] Robot go to the right." << std::endl;
        break;
    //2 - Camera
    case Qt::Key_Up:
        std::cout << "[Info] Camera robot up." << std::endl;
        manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-250")));
        break;
    case Qt::Key_Down:
        std::cout << "[Info] Camera robot down." << std::endl;
        manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=250")));
        break;
    case Qt::Key_Left:
        std::cout << "[Info] Camera robot left." << std::endl;
        manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=250")));
        break;
    case Qt::Key_Right:
        std::cout << "[Info] Camera robot right." << std::endl;
        manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-250")));
        break;


    //3 - Boost
    case Qt::Key_Space:
        boolSpeed = true;
        previousSpeed = ui->speedSlider->value();
        ui->speedSlider->setValue(speedMax);
        std::cout << "[Info] Boost : vitesse maximale." << std::endl;
        break;

    //4 - Grey
     case Qt::Key_N:
         streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' invert(100%)';");
         break;

    //5 - Saturation
      case Qt::Key_P:
         streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' saturate(500%)';");
         break;

    //6 - Black&White
      case Qt::Key_G:
         streamView->page()->runJavaScript("var filtres = document.body.firstChild.style.webkitFilter; document.body.firstChild.style.webkitFilter = filtres+' grayscale(100%)';");
         break;

     //6 - Black&White
       case Qt::Key_R:
         streamView->page()->runJavaScript("document.body.firstChild.style.webkitFilter = ''");
         break;

    //7 - Capture
    case Qt::Key_C:
        //Capture
        nowDate = QDateTime::currentDateTime();
        fileNameDate = nowDate.toString("dd_MM_yyyy_hh_mm_ss").toUtf8();
        path = "/Users/esirem/Documents/wifibot2017_VinceAntho/capture/";
        fileName = fileNameDate;
        extension = ".png";

        pathCapture = path + fileName + extension;
        QPixmap pixMap = streamView->grab();
        pixMap.save(pathCapture, "PNG");

        std::cout << "[Info] Capture." << std::endl;
        break;

    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev){

    //Condition pour BOOST
    if (boolSpeed){
        ui->speedSlider->setValue(previousSpeed);
        boolSpeed = false;
    }else{
        tcpSocket->moveRobot("stop");
        std::cout << "[Info] Stop robot." << std::endl;
    }
}

void MainWindow::on_speedSlider_valueChanged(){
    ui->speedLCD->display(ui->speedSlider->value());
    ui->speedBar->setValue(ui->speedSlider->value());
}

void MainWindow::updateSensorsValues(Sensors s){
    ui->odoL->display(s.getOdoL());
    ui->odoR->display(s.getOdoR());
    ui->speedL->display(s.getSpeedL());
    ui->speedR->display(s.getSpeedR());

    if (s.getBattery() > 18.1 && s.getBattery() < 18.5){
        ui->battery->setValue(100);
        ui->battery->setStyleSheet(ui->battery->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: blue; }");
        ui->batteryIcon->setIcon(QIcon(":/img/battery/BatterieC.png"));
    }
    else {
        ui->battery->setValue(floor(((s.getBattery()-11.3)*100)/12.8));
        if(s.getBattery() > 11.3 && s.getBattery() < 12.1){
            //Low battery
            ui->battery->setStyleSheet(ui->battery->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: red; }");
            ui->batteryIcon->setIcon(QIcon(":/img/battery/Batterie2.png"));
        } else if(s.getBattery() > 12 && s.getBattery() < 12.5){
            //Medium battery
            ui->battery->setStyleSheet(ui->battery->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: orange; }");
            ui->batteryIcon->setIcon(QIcon(":/img/battery/Batterie3.png"));
        } else if(s.getBattery() > 12.4 && s.getBattery() < 12,9){
            //Full battery
            ui->battery->setStyleSheet(ui->battery->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: green; }");
            ui->batteryIcon->setIcon(QIcon(":/img/battery/Batterie.png"));
        }
    }
}

void MainWindow::on_capture_clicked()
{
    //Capture
    //Chemin de base : racine
     nowDate = QDateTime::currentDateTime();
     fileNameDate = nowDate.toString("dd_MM_yyyy_hh_mm_ss").toUtf8();
     //qDebug()<<str->toUtf8();

     path = "/Users/esirem/Documents/wifibot2017_VinceAntho/capture/";
     fileName = fileNameDate;
     extension = ".png";

    pathCapture = path + fileName + extension;
    QPixmap pixMap = streamView->grab();
    pixMap.save(pathCapture, "PNG");

    std::cout << "[Info] Capture." << std::endl;
}


