#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    battery(MAX_BATTERY), // device starts at 100? or should we "remember" the battery level
    batteryDrain(1.0),
    isPowered(false)
{
    ui->setupUi(this);

    setupGridWrappers();
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

    // initialize power button timer
    powerButtonTimer = new QTimer(this);
    powerButtonTimer->setSingleShot(true);
    connect(powerButtonTimer, SIGNAL(timeout()), this, SLOT(powerButtonHeld()));

    // initialize idle timer
    idleTimer = new QTimer(this);
    idleTimer->setSingleShot(true);
    connect(idleTimer, SIGNAL(timeout()), this, SLOT(powerOff()));

    // initialize the battery drain timer
    batteryTimer = new QTimer(this);
    connect(batteryTimer, SIGNAL(timeout()), this, SLOT(drainBattery()));



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

void MainWindow::clearIntensityNum() {
    // reset the display
    turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
}

void MainWindow::displayBattery() {
    clearIntensityNum();

    int batteryLevel = (MAX_INTENSITY_LEVEL * battery) / MAX_BATTERY;

    turnOnIntensityNum(0, batteryLevel);
    QTimer::singleShot(BATTERY_DISPLAY_DURATION, this, SLOT(clearIntensityNum()));

    // low battery?
}

// turn on/off
//

void MainWindow::powerOn() {
    if (battery <= 0) return;

    isPowered = true;
    ui->powerLED->setStyleSheet("background-color: green");

    // start revelant timers
    idleTimer->start(IDLE_TIME); // turns off device after set duration if idle
    batteryTimer->start(BATTERY_DRAIN_RATE); // drains the battery periodically

    // show battery
    displayBattery();
}

void MainWindow::powerOff() {
    // turn off all lights
    turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);

    isPowered = false;
    ui->powerLED->setStyleSheet("");

    idleTimer->stop();
    batteryTimer->stop();
}

void MainWindow::softOff() {
    powerOff();
    turnOnIntensityNum(0, MAX_INTENSITY_LEVEL);
    for (int i = MAX_INTENSITY_LEVEL - 1; i >= 0; --i) {
        QTimer::singleShot(500 * (MAX_INTENSITY_LEVEL - i), this, [this, i]() {turnOffIntensityNum(i, i+1);});
    }
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

void MainWindow::drainBattery() {
    if (!isPowered || battery <= 0) return;

    battery -= batteryDrain;
    if (battery <= 0)
        powerOff();
}

// dev
//

void MainWindow::test() {
}
