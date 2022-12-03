#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <iostream>

using namespace std;

// UI CONSTANTS
//

#define MIN_INTENSITY_LEVEL 1
#define MAX_INTENSITY_LEVEL 8

#define POWER_BUTTON_LONG_PRESS_TIME 300

//
// END UI CONSTANTS

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupGridWrappers();


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

// button handling
//

void MainWindow::powerButtonClicked() {
    cout << "Power button was clicked once!" << endl;
}

void MainWindow::powerButtonHeld() {
    cout << "Power button was held down!" << endl;
}

// signals
//

void MainWindow::powerButtonPressed() {
    powerButtonTimer = new QElapsedTimer();
    powerButtonTimer->start();
}

void MainWindow::powerButtonReleased() {
    if (powerButtonTimer->elapsed() > POWER_BUTTON_LONG_PRESS_TIME) {
        powerButtonHeld();
    } else {
        powerButtonClicked();
    }

    delete powerButtonTimer;
}

// dev
//

void MainWindow::test() {

}