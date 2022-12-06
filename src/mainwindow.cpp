#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <iostream>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isPowered(false)
{
    ui->setupUi(this);

    setupGridWrappers();

    // initialize power button timer
    powerButtonTimer = new QTimer(this);
    powerButtonTimer->setSingleShot(true);
    connect(powerButtonTimer, SIGNAL(timeout()), this, SLOT(powerButtonHeld()));

    // initialize idle timer
    idleTimer = new QTimer(this);
    idleTimer->setSingleShot(true);
    connect(idleTimer, SIGNAL(timeout()), this, SLOT(powerOff()));



    // dev mode
    test();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGridWrappers() {
    setupIntensityLevelDisplayWrapper();
    setupButtons();
}

void MainWindow::setupIntensityLevelDisplayWrapper() {
    // get the intensityLevelDisplayWrapper QVBoxLayout
    QVBoxLayout* intensityLevelDisplayWrapper = ui->intensityLevelDisplayWrapper;

    for (int i = MAX_INTENSITY_LEVEL; i >= MIN_INTENSITY_LEVEL; i--) {
        // create new QLabel with `i` as the display text
        QLabel* intensityLevelLabel = new QLabel();
        intensityLevelLabel->setNum(i);

        // style the QLabel
        intensityLevelLabel->setAlignment(Qt::AlignCenter);
        intensityLevelLabel->setStyleSheet("background-color: white");

        // add QLabel to wrapper
        intensityLevelDisplayWrapper->addWidget(intensityLevelLabel);
    }
}

void MainWindow::setupButtons() {
    // get power button
    QPushButton* powerButton = ui->powerButton;

    connect(powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPressed()));
    connect(powerButton, SIGNAL(released()), this, SLOT(powerButtonReleased()));
}

// turn on/off
//

void MainWindow::powerOn() {
    isPowered = true;
    ui->powerLED->setStyleSheet("background-color: green");
    idleTimer->start(IDLE_TIME);
}

void MainWindow::powerOff() {
    isPowered = false;
    ui->powerLED->setStyleSheet("");
    idleTimer->stop();
}

// button handling
//

void MainWindow::powerButtonClicked() {
    if (!isPowered) return;
    cout << "Power button was clicked once!" << endl;
}

void MainWindow::powerButtonHeld() {
    if (isPowered) {
        powerOff();
    } else {
        powerOn();
    }
}

// signals
//

void MainWindow::powerButtonPressed() {
    powerButtonTimer->start(POWER_BUTTON_LONG_PRESS_TIME);
}

void MainWindow::powerButtonReleased() {
    if (powerButtonTimer->isActive()) {
        powerButtonTimer->stop();
        powerButtonClicked();
    }
}

// dev
//

void MainWindow::test() {

}
