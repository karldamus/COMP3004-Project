#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <iostream>
#include <stdlib.h>

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

    // initialize batteryDisplayBar
    batteryDisplayBar = ui->batteryDisplayBar;

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
	setupSessionGroupDisplayWrapper();
}

void MainWindow::setupSessionGroupDisplayWrapper() {
	// get sessionGroupDisplayWrapper from ui
	QHBoxLayout* sessionGroupDisplayWrapper = ui->sessionGroupDisplayWrapper;

	// create labels for the 3 session groups, 20 min, 45 min, User Designed and add to wrapper and vector
	// 20 min
	QLabel* sessionGroupLabel20 = new QLabel();
	QString path20min = QString(":/icons/20min.png");
	QPixmap pixmap20Min(path20min);
	QPixmap scaledPixmap20Min = pixmap20Min.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionGroupLabel20->setPixmap(scaledPixmap20Min);
	sessionGroupDisplayWrapper->addWidget(sessionGroupLabel20);
	struct sessionGroupLabelStruct sessionGroupLabel20Struct = {sessionGroupLabel20, Session::TWENTY_MINUTES};
	sessionGroupLabels.append(sessionGroupLabel20Struct);

	// 45 min
	QLabel* sessionGroupLabel45 = new QLabel();
	QString path45min = QString(":/icons/45min.png");
	QPixmap pixmap45Min(path45min);
	QPixmap scaledPixmap45Min = pixmap45Min.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionGroupLabel45->setPixmap(scaledPixmap45Min);
	sessionGroupDisplayWrapper->addWidget(sessionGroupLabel45);
	struct sessionGroupLabelStruct sessionGroupLabel45Struct = {sessionGroupLabel45, Session::FORTY_FIVE_MINUTES};
	sessionGroupLabels.append(sessionGroupLabel45Struct);

	QLabel* sessionGroupLabelUserDesigned = new QLabel();
	QString pathUserDesigned = QString(":/icons/UserDesignated.png");
	QPixmap pixmapUserDesigned(pathUserDesigned);
	QPixmap scaledpixmapUserDesigned = pixmapUserDesigned.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionGroupLabelUserDesigned->setPixmap(scaledpixmapUserDesigned);
	sessionGroupDisplayWrapper->addWidget(sessionGroupLabelUserDesigned);
	struct sessionGroupLabelStruct sessionGroupLabelUserDesignedStruct = {sessionGroupLabelUserDesigned, Session::USER_DESIGNATED};
	sessionGroupLabels.append(sessionGroupLabelUserDesignedStruct);

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


//
// Functions for selecting sessions
//
void MainWindow::cycleSessionGroups() {
	// when the power button is pressed for the first time in a blank session, it will start the cycle at 20 min (first in list)
	// if the current session type is User Designed, it will cycle back to 20 min
	// c++ enums are annoying, can't ++ it, using this
	switch (this->currentSession->getSessionGroup()) {
		case Session::NULL_SESSION_GROUP:
			this->currentSession->setSessionGroup(Session::TWENTY_MINUTES);
			colourSessionGroup(Session::TWENTY_MINUTES);
			break;
		case Session::USER_DESIGNATED:
			this->currentSession->setSessionGroup(Session::TWENTY_MINUTES);
			colourSessionGroup(Session::TWENTY_MINUTES);
			break;
		case Session::TWENTY_MINUTES:
			this->currentSession->setSessionGroup(Session::FORTY_FIVE_MINUTES);
			colourSessionGroup(Session::FORTY_FIVE_MINUTES);
			break;
		case Session::FORTY_FIVE_MINUTES:
			this->currentSession->setSessionGroup(Session::USER_DESIGNATED);
			colourSessionGroup(Session::USER_DESIGNATED);
			break;
		default:
			assert( ! "cycleSessionGroup given invalid enum");
	}
	cout << "Session Group was changed to " << this->currentSession->getSessionGroup() << endl;

}

// given sessionGroup enum, it will highlight that groups lebel in the ui, and dehighlight all others
void MainWindow::colourSessionGroup(Session::SessionGroup sessionGroup) {
	// iterate through sessionGroupLabelStructs
	QVectorIterator<sessionGroupLabelStruct> itSessionGroupLabels(sessionGroupLabels);
	while (itSessionGroupLabels.hasNext()){
		sessionGroupLabelStruct currStruct = itSessionGroupLabels.next();
		if (currStruct.sessionGroup == sessionGroup){
			currStruct.sessionGroupLabel->setStyleSheet("QLabel { background-color : rgba(0, 255, 0, 50) }");
		} else {
			currStruct.sessionGroupLabel->setStyleSheet("QLabel { background-color : rgba(0, 0, 0, 0) }");
		}
	}
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
	isSessionRunning = false;
    ui->powerLED->setStyleSheet("background-color: green");

    // start revelant timers
    idleTimer->start(IDLE_TIME); // turns off device after set duration if idle
    batteryTimer->start(BATTERY_DRAIN_RATE); // drains the battery periodically

    // show battery
    displayBattery();

	// create default session with null values for the current session
	currentSession = new Session();
}

void MainWindow::powerOff() {
    // turn off all lights
    turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
	colourSessionGroup(Session::NULL_SESSION_GROUP); // this will remove all colour from group icons

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
    cout << "Power button was clicked once!" << endl;
    if (!isPowered) return;

	// change between session groups if a session isn't running
	if (!isSessionRunning) cycleSessionGroups();
}



void MainWindow::powerButtonHeld() {
    cout << "Power button was held down!" << endl;
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

    // battery -= batteryDrain;
    battery -= 10;
    if (battery <= 0)
        powerOff();

    batteryDisplayBar->setValue(battery);
}

// dev
//

void MainWindow::test() {
    // test drainbattery
    // for (int i = 0; i <= 10; i++) {
    //     delay();
    //     drainBattery();
    // }

    User user = User(1);
    user.test();
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
