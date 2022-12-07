#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isPowered(false),
    battery(MAX_BATTERY) // device starts at 100? or should we "remember" the battery level
{
    ui->setupUi(this);

    setupGridWrappers();
    setupLightColours();

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
    delete powerButtonTimer;
    delete idleTimer;
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
        intensityLevelLabel->setStyleSheet(DEFAULT_INTENSITY_COLOUR);

        // add QLabel to wrapper
        intensityLevelDisplayWrapper->addWidget(intensityLevelLabel);
        intensityLabels.append(intensityLevelLabel);
    }
    std::reverse(intensityLabels.begin(), intensityLabels.end());
}

void MainWindow::setupLightColours() {
    lightColours = QVector<QString>({
        "background-color: green",
        "background-color: green",
        "background-color: green",
        "background-color: yellow",
        "background-color: yellow",
        "background-color: yellow",
        "background-color: red",
        "background-color: red",
    });
}

void MainWindow::setupButtons() {
    // get power button
    QPushButton* powerButton = ui->powerButton;

    connect(powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPressed()));
    connect(powerButton, SIGNAL(released()), this, SLOT(powerButtonReleased()));
}

// helper functions to light up the numbers
//
void MainWindow::turnOnIntensityNum(int start, int end) {
    while (start < end) {
        intensityLabels[start]->setStyleSheet(lightColours[start]);
        start++;
    }
}
void MainWindow::turnOffIntensityNum(int start, int end) {
    while (start < end) {
        intensityLabels[start]->setStyleSheet(DEFAULT_INTENSITY_COLOUR);
        start++;
    }
}
void MainWindow::displayBattery() {
    int batteryLevel = (MAX_INTENSITY_LEVEL * battery) / MAX_BATTERY;
    turnOnIntensityNum(0, batteryLevel);

    // TODO: low battery stuff
}

// turn on/off
//

void MainWindow::powerOn() {
    isPowered = true;
    ui->powerLED->setStyleSheet("background-color: green");
    idleTimer->start(IDLE_TIME);

    // show battery
    displayBattery();
}

void MainWindow::powerOff() {
    isPowered = false;
    ui->powerLED->setStyleSheet("");
    idleTimer->stop();

    // turn off all lights
    turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
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
