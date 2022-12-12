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
    isPowered(false),
    isRecording(false)
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

	// initialize session timer
	sessionTimer = new QTimer(this);
	connect(sessionTimer, SIGNAL(timeout()), this, SLOT(updateSessionTimer()));

	//initialize startSessionTimer
	startSessionTimer = new QTimer(this);
	startSessionTimer->setSingleShot(true);
	connect(startSessionTimer, SIGNAL(timeout()), this, SLOT(startSession()));

	//initialize blink timer
	sessionBlinkTimer = new QTimer(this);
	connect(sessionBlinkTimer, SIGNAL(timeout()), this, SLOT(sessionBlink()));
	CESModeBlink = true;

    // initialize batteryDisplayBar
    batteryDisplayBar = ui->batteryDisplayBar;
    batteryDisplayBar->setValue(100);

    // initialize the users
    for (int i = 1; i <= NUM_USERS; ++i) {
        users.append(new User(i));

        QLabel *user = new QLabel(this);
        user->setText(QString("U%1").arg(i));
        user->setAlignment(Qt::AlignCenter);
        ui->userList->layout()->addWidget(user);

        userLabels.append(user);
    }

    // connect user buttons
    connect(ui->switchUserBtn, SIGNAL(pressed()), this, SLOT(cycleUsers()));
    connect(ui->recordSessionBtn, SIGNAL(pressed()), this, SLOT(recordSession()));
    connect(ui->saveBtn, SIGNAL(pressed()), this, SLOT(saveSession()));

	// countdown timer label
	sessionTimeLabel = ui->sessionTimerDisplayLabel;

	userDesignatedSpinBox = ui->userDesignatedTimeBox;

    // dev mode
    test();
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < NUM_USERS; ++i) {
        delete users[i];
    }
    delete ui;
    delete powerButtonTimer;
    delete idleTimer;
	delete startSessionTimer;
	delete sessionTimer;
	delete sessionBlinkTimer;
}



void MainWindow::setupGridWrappers() {
    setupIntensityLevelDisplayWrapper();
    setupButtons();
	setupSessionGroupDisplayWrapper();
	setupSessionTypeDisplayWrapper();
	setuptDCSDisplayWrapper();
	setupCESModeDisplayWrapper();
}

void MainWindow::setuptDCSDisplayWrapper() {
	tDCSLabels.append(ui->tDCS025Label);
	tDCSLabels.append(ui->tDCS050Label);
	tDCSLabels.append(ui->tDCS075Label);
	tDCSLabels.append(ui->tDCS100Label);
}

void MainWindow::setupCESModeDisplayWrapper() {
	QHBoxLayout* CESModeDisplayWrapper = ui->CESModeDisplayWrapper;

	QLabel* CESModeLabelShortPulse = new QLabel();
	QPixmap pixmapShortPulseOff = QPixmap(QString::fromStdString(":/icons/shortPulseOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapShortPulseOn = QPixmap(QString::fromStdString(":/icons/shortPulseOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	CESModeLabelShortPulse->setPixmap(pixmapShortPulseOff);
	CESModeDisplayWrapper->addWidget(CESModeLabelShortPulse);
	CESshortPulse = {CESModeLabelShortPulse,  pixmapShortPulseOn, pixmapShortPulseOff};

	QLabel* CESModeLabelDutyCycle = new QLabel();
	QPixmap pixmapDutyCycleOff = QPixmap(QString::fromStdString(":/icons/dutyCycleOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapDutyCycleOn = QPixmap(QString::fromStdString(":/icons/dutyCycleOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	CESModeLabelDutyCycle->setPixmap(pixmapDutyCycleOff);
	CESModeDisplayWrapper->addWidget(CESModeLabelDutyCycle);

}

void MainWindow::setupSessionTypeDisplayWrapper() {
	QHBoxLayout* sessionTypeDisplayWrapper = ui->sessionTypeDisplayWrapper;

	QLabel* sessionTypeLabelDelta = new QLabel();
	QPixmap pixmapDeltaOff = QPixmap(QString::fromStdString(":/icons/deltaOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapDeltaOn = QPixmap(QString::fromStdString(":/icons/deltaOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionTypeLabelDelta->setPixmap(pixmapDeltaOff);
	sessionTypeDisplayWrapper->addWidget(sessionTypeLabelDelta);
	struct sessionTypeLabelStruct sessionTypeLabelDeltaStruct = {sessionTypeLabelDelta, Session::DELTA, pixmapDeltaOn, pixmapDeltaOff};
	sessionTypeLabels.append(sessionTypeLabelDeltaStruct);

	QLabel* sessionTypeLabelAlpha = new QLabel();
	QPixmap pixmapAlphaOff = QPixmap(QString::fromStdString(":/icons/alphaOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapAlphaOn = QPixmap(QString::fromStdString(":/icons/alphaOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionTypeLabelAlpha->setPixmap(pixmapAlphaOff);
	sessionTypeDisplayWrapper->addWidget(sessionTypeLabelAlpha);
	struct sessionTypeLabelStruct sessionTypeLabelAlphaStruct = {sessionTypeLabelAlpha, Session::ALPHA, pixmapAlphaOn, pixmapAlphaOff};
	sessionTypeLabels.append(sessionTypeLabelAlphaStruct);

	QLabel* sessionTypeLabelBeta1 = new QLabel();
	QPixmap pixmapBeta1Off = QPixmap(QString::fromStdString(":/icons/beta1Off.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapBeta1On = QPixmap(QString::fromStdString(":/icons/beta1On.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionTypeLabelBeta1->setPixmap(pixmapBeta1Off);
	sessionTypeDisplayWrapper->addWidget(sessionTypeLabelBeta1);
	struct sessionTypeLabelStruct sessionTypeLabelBeta1Struct = {sessionTypeLabelBeta1, Session::BETA1, pixmapBeta1On, pixmapBeta1Off};
	sessionTypeLabels.append(sessionTypeLabelBeta1Struct);

	QLabel* sessionTypeLabelBeta2 = new QLabel();
	QPixmap pixmapBeta2Off = QPixmap(QString::fromStdString(":/icons/beta2Off.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapBeta2On = QPixmap(QString::fromStdString(":/icons/beta2On.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionTypeLabelBeta2->setPixmap(pixmapBeta2Off);
	sessionTypeDisplayWrapper->addWidget(sessionTypeLabelBeta2);
	struct sessionTypeLabelStruct sessionTypeLabelBeta2Struct = {sessionTypeLabelBeta2, Session::BETA2, pixmapBeta2On, pixmapBeta2Off};
	sessionTypeLabels.append(sessionTypeLabelBeta2Struct);
}

void MainWindow::setupSessionGroupDisplayWrapper() {
	// get sessionGroupDisplayWrapper from ui
	QHBoxLayout* sessionGroupDisplayWrapper = ui->sessionGroupDisplayWrapper;

	// create labels for the 3 session groups, 20 min, 45 min, User Designed and add to wrapper and vector
	// 20 min
	QLabel* sessionGroupLabel20 = new QLabel();
	QPixmap pixmap20minOff = QPixmap(QString::fromStdString(":/icons/20minOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmap20minOn = QPixmap(QString::fromStdString(":/icons/20minOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionGroupLabel20->setPixmap(pixmap20minOff);
	sessionGroupDisplayWrapper->addWidget(sessionGroupLabel20);
	struct sessionGroupLabelStruct sessionGroupLabel20Struct = {sessionGroupLabel20, Session::TWENTY_MINUTES, pixmap20minOn, pixmap20minOff};
	sessionGroupLabels.append(sessionGroupLabel20Struct);

	// 45 min
	QLabel* sessionGroupLabel45 = new QLabel();
	QPixmap pixmap45minOff = QPixmap(QString::fromStdString(":/icons/45minOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmap45minOn = QPixmap(QString::fromStdString(":/icons/45minOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionGroupLabel45->setPixmap(pixmap45minOff);
	sessionGroupDisplayWrapper->addWidget(sessionGroupLabel45);
	struct sessionGroupLabelStruct sessionGroupLabel45Struct = {sessionGroupLabel45, Session::FORTY_FIVE_MINUTES, pixmap45minOn, pixmap45minOff};
	sessionGroupLabels.append(sessionGroupLabel45Struct);

	QLabel* sessionGroupLabelUserDesigned = new QLabel();
	QPixmap pixmapUserDesignatedOff = QPixmap(QString::fromStdString(":/icons/UserDesignatedOff.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	QPixmap pixmapUserDesignatedOn = QPixmap(QString::fromStdString(":/icons/UserDesignatedOn.png")).scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation);
	sessionGroupLabelUserDesigned->setPixmap(pixmapUserDesignatedOff);
	sessionGroupDisplayWrapper->addWidget(sessionGroupLabelUserDesigned);
	struct sessionGroupLabelStruct sessionGroupLabelUserDesignedStruct = {sessionGroupLabelUserDesigned, Session::USER_DESIGNATED, pixmapUserDesignatedOn, pixmapUserDesignatedOff};
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
	QPushButton* increaseIntensityButton = ui->increaseIntensityButton;
	QPushButton* decreaseIntensityButton = ui->decreaseIntensityButton;
	QPushButton* sessionStartButton = ui->sessionStartButton;


    connect(powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPressed()));
    connect(powerButton, SIGNAL(released()), this, SLOT(powerButtonReleased()));
	connect(increaseIntensityButton, SIGNAL(pressed()), this, SLOT(increaseIntensityButtonPressed()));
	connect(decreaseIntensityButton, SIGNAL(pressed()), this, SLOT(decreaseIntensityButtonPressed()));
	connect(sessionStartButton, SIGNAL(pressed()), this, SLOT(sessionStartButtonPressed()));
}


//
// Functions for selecting sessions
//
void MainWindow::cycleSessionGroups() {
	// when the power button is pressed for the first time in a blank session, it will start the cycle at 20 min (first in list)
	// if the current session type is User Designed, it will cycle back to 20 min
	// c++ enums are annoying, can't ++ it, using this
	cout << "cycleSession" << endl;
	switch (this->currentSession->getSessionGroup()) {
		case Session::NULL_SESSION_GROUP:
			this->currentSession->setSessionGroup(Session::TWENTY_MINUTES);
			colourSessionGroup(Session::TWENTY_MINUTES);
			//this->currentSession->setSessionType(Session::ALPHA); not sure how session type logic should work
			//colourSessionType(Session::ALPHA);                    can you only select a type once you've selected a group?
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
			currStruct.sessionGroupLabel->setPixmap(currStruct.on);
		} else {
			currStruct.sessionGroupLabel->setPixmap(currStruct.off);
		}
	}
}

void MainWindow::cycleSessionTypesUp() {
	switch (this->currentSession->getSessionType()){
	case Session::NULL_SESSION_TYPE:
		this->currentSession->setSessionType(Session::DELTA);
		colourSessionType(Session::DELTA);
		colourtDCSNumber(0);
		break;
	case Session::DELTA:
		this->currentSession->setSessionType(Session::ALPHA);
		colourSessionType(Session::ALPHA);
		colourtDCSNumber(1);
		break;
	case Session::ALPHA:
		this->currentSession->setSessionType(Session::BETA1);
		colourSessionType(Session::BETA1);
		colourtDCSNumber(2);
		break;
	case Session::BETA1:
		this->currentSession->setSessionType(Session::BETA2);
		colourSessionType(Session::BETA2);
		colourtDCSNumber(3);
		break;
	case Session::BETA2:
		break;
	default:
		assert( ! "cycleSessionType given invalid enum");
	}
}

void MainWindow::cycleSessionTypesDown() {
	switch (this->currentSession->getSessionType()){
	case Session::NULL_SESSION_TYPE:
		this->currentSession->setSessionType(Session::DELTA);
		colourSessionType(Session::DELTA);
		colourtDCSNumber(0);
		break;
	case Session::DELTA:
		break;
	case Session::ALPHA:
		this->currentSession->setSessionType(Session::DELTA);
		colourSessionType(Session::DELTA);
		colourtDCSNumber(0);
		break;
	case Session::BETA1:
		this->currentSession->setSessionType(Session::ALPHA);
		colourSessionType(Session::ALPHA);
		colourtDCSNumber(1);
		break;
	case Session::BETA2:
		this->currentSession->setSessionType(Session::BETA1);
		colourSessionType(Session::BETA1);
		colourtDCSNumber(2);
		break;
	default:
		assert( ! "cycleSessionType given invalid enum");
	}
}

// given sessionType enum, it will change to On pictures
void MainWindow::colourSessionType(Session::SessionType sessionType) {
	// iterate through sessionTypeLabelStructs
	QVectorIterator<sessionTypeLabelStruct> itSessionTypeLabels(sessionTypeLabels);
	while (itSessionTypeLabels.hasNext()){
		sessionTypeLabelStruct currStruct = itSessionTypeLabels.next();
		if (currStruct.sessionType == sessionType){
			currStruct.sessionTypeLabel->setPixmap(currStruct.on);
		} else {
			currStruct.sessionTypeLabel->setPixmap(currStruct.off);
		}
	}
}

int MainWindow::gettDCSNumber(){
	switch (this->currentSession->getSessionType()){
	case Session::NULL_SESSION_TYPE:
		return -1;
		break;
	case Session::DELTA:
		return 0;
		break;
	case Session::ALPHA:
		return 1;
		break;
	case Session::BETA1:
		return 2;
		break;
	case Session::BETA2:
		return 3;
	}
}

void MainWindow::colourtDCSNumber(int vectorPos) {
	for (int i = 0; i < tDCSLabels.size(); i++){
		if (i == vectorPos){
			tDCSLabels[i]->setStyleSheet("color: green");
		} else {
			tDCSLabels[i]->setStyleSheet("color: black");
		}
	}
}

void MainWindow::startSession() {
	switch(currentSession->getSessionGroup()){
	case Session::TWENTY_MINUTES:
		sessionTimeLabel->setText(QString::number(20));
		sessionTime = 20;
		break;
	case Session::FORTY_FIVE_MINUTES:
		sessionTimeLabel->setText(QString::number(45));
		sessionTime = 45;
		break;
	case Session::USER_DESIGNATED:
		sessionTimeLabel->setText(QString::number(userDesignatedSpinBox->value()));
		sessionTime = userDesignatedSpinBox->value();

	default:
		break;
	}
	CESshortPulse.CESModeLabel->setPixmap(CESshortPulse.on);
	sessionBlinkTimer->stop();
	//recolour tDCS
	colourtDCSNumber(gettDCSNumber());
	isSessionRunning = true;
	turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
	turnOnIntensityNum(0, currentSession->getSessionIntensity());
	sessionTimer->start(1000);
	//sessionTimer->start();
}

void MainWindow::cycleUsers() {
    if (!isPowered) return;
    int currUserId = currUser->getUserId();

    // unselect user in the ui
    userLabels[currUserId - 1]->setStyleSheet("");

    currUserId++;
    if (currUserId > NUM_USERS) {
        currUserId = 1;
    }
    currUser = users[currUserId - 1]; // array index starts at 0

    // highlight new selected user
    userLabels[currUserId - 1]->setStyleSheet("background-color: #bdffdc;");

    // update the user sessions
    updateUserSessionList();
}

void MainWindow::updateUserSessionList() {
    ui->userSessionList->clear();
    Session* s;
    const QVector<Session*>* savedSessions = currUser->getSavedSessions();
    for (int i = 0; i < savedSessions->size(); ++i) {
        s = savedSessions->at(i);
        ui->userSessionList->addItem(s->toString());
    }
}

void MainWindow::recordSession() {
//    if (isSessionRunning) return;
    isRecording = true;
    ui->recordSessionBtn->setEnabled(false);
    ui->saveBtn->setEnabled(true);
}
void MainWindow::saveSession() {
    // called only after record session button has been pressed
    isRecording = false;
    ui->recordSessionBtn->setEnabled(true);
    ui->saveBtn->setEnabled(false);

    // assuming some group and type are always chosen (non null)
//    currUser->loadSession(currentSession);
//    currUser->saveSession();
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
  
  // select user 1
    currUser = users[NUM_USERS - 1];
    cycleUsers();
}

void MainWindow::powerOff() {
    // turn off all lights
    turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
	colourSessionGroup(Session::NULL_SESSION_GROUP); // this will remove all colour from group icons
	colourSessionType(Session::NULL_SESSION_TYPE);
    ui->userSessionList->clear(); // clear user session list
	colourtDCSNumber(-1);
	isSessionRunning = false;
	sessionTimeLabel->setText(QString::fromStdString(""));
	CESshortPulse.CESModeLabel->setPixmap(CESshortPulse.off);


    // turning off the device
    isPowered = false;
    ui->powerLED->setStyleSheet("");

    // stop the timers
    idleTimer->stop();
    batteryTimer->stop();
	sessionTimer->stop();
	startSessionTimer->stop();
}

void MainWindow::softOff() {
	powerOff();
    turnOnIntensityNum(0, MAX_INTENSITY_LEVEL);
    for (int i = MAX_INTENSITY_LEVEL - 1; i >= 0; --i) {
        QTimer::singleShot(500 * (MAX_INTENSITY_LEVEL - i), this, [this, i]() {turnOffIntensityNum(i, i+1);});
    }

}

void MainWindow::updateSessionTimer() {

	if (sessionTime == 0){
		sessionTimer->stop();
		sessionTimeLabel->setText(QString::fromStdString(""));
		softOff();
	} else {
		sessionTimeLabel->setText(QString::number(sessionTime--));
	}

}

// button handling
//

void MainWindow::powerButtonClicked() {
    cout << "Power button was clicked once!" << endl;
    if (!isPowered) return;
	// change between seisSessionRunningssion groups if a session isn't running
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

void MainWindow::sessionBlink() { //this also does test connection display
	int tDCSNum = gettDCSNumber();


	if (tDCSLabels[tDCSNum]->styleSheet() == "color: green"){
		tDCSLabels[tDCSNum]->setStyleSheet("color: black");
	} else {
		tDCSLabels[tDCSNum]->setStyleSheet("color: green");
	}

	if (CESModeBlink){
		CESshortPulse.CESModeLabel->setPixmap(CESshortPulse.on);
		CESModeBlink = false;
	} else {
		CESshortPulse.CESModeLabel->setPixmap(CESshortPulse.off);
		CESModeBlink = true;
	}



}

void MainWindow::sessionStartButtonPressed() {
	cout << "Start Session button was pressed" << endl;
	if (!isPowered) return;
	if (isSessionRunning) return;
	if (currentSession->isGroupSet() && currentSession->isTypeSet()){
		//start session
		sessionBlinkTimer->start(500);
		startSessionTimer->start(5000);
		turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
		turnOnIntensityNum(0, 3);

	}
}

void MainWindow::powerButtonPressed() {
    powerButtonTimer->start(POWER_BUTTON_LONG_PRESS_TIME);
}

void MainWindow::powerButtonReleased() {
    if (powerButtonTimer->isActive()) {
        powerButtonTimer->stop();
        powerButtonClicked();
    }
}

void MainWindow::increaseIntensityButtonPressed() {
	cout << "Increase Intensity button was pressed" << endl;
	if (!isPowered) return;
	if (isSessionRunning){
		if (currentSession->getSessionIntensity() < MAX_INTENSITY_LEVEL){
			currentSession->setSessionIntensity(currentSession->getSessionIntensity() + 1);
			//turn off al lnumbers
			//turn on right numbers
			turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
			turnOnIntensityNum(0, currentSession->getSessionIntensity());
		}
	} else {
		cycleSessionTypesUp();
	}
}

void MainWindow::decreaseIntensityButtonPressed() {
	cout << "Decrease Intensity button was pressed" << endl;
	if (!isPowered) return;
	if (isSessionRunning){
		if (currentSession->getSessionIntensity() > MIN_INTENSITY_LEVEL){
			currentSession->setSessionIntensity(currentSession->getSessionIntensity() - 1);
			//turn off al lnumbers
			//turn on right numbers
			turnOffIntensityNum(0, MAX_INTENSITY_LEVEL);
			turnOnIntensityNum(0, currentSession->getSessionIntensity());
		}
	} else {
		cycleSessionTypesDown();
	}
}

void MainWindow::drainBattery() {
    if (!isPowered || battery <= 0) return;

     battery -= batteryDrain;
//    battery -= 10;
    if (battery <= 0)
        powerOff();

    batteryDisplayBar->setValue(battery);
}

// dev
//

void MainWindow::test() {
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
