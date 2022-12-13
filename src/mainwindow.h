#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTimeLine>
#include <QProgressBar>
#include <QTime>
#include <QSpinBox>

#include <cassert>

#include "user.h"
#include "session.h"

// UI CONSTANTS
//

#define NUM_USERS 3

#define MIN_INTENSITY_LEVEL 1
#define MAX_INTENSITY_LEVEL 8

#define MAX_BATTERY                 100
#define BATTERY_DRAIN_RATE          10000 // 10s
#define BATTERY_DISPLAY_DURATION    3000 // 3s

#define DEFAULT_INTENSITY_COLOUR    "background-color: white"
#define INTENSITY_BLINK_DURATION    2000 // 0.2s

#define POWER_BUTTON_LONG_PRESS_TIME 1000   // 1s
#define IDLE_TIME                    120000 // 2min


//
// END UI CONSTANTS

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	struct sessionGroupLabelStruct{
		QLabel* sessionGroupLabel;
		Session::SessionGroup sessionGroup;
		QPixmap on;
		QPixmap off;
	};

	struct sessionTypeLabelStruct{
		QLabel* sessionTypeLabel;
		Session::SessionType sessionType;
		QPixmap on;
		QPixmap off;
	};

	struct CESModeLabelStruct{
		QLabel* CESModeLabel;
		QPixmap on;
		QPixmap off;
	};


    // ui creation methods
    void setupGridWrappers();
    void setupIntensityLevelDisplayWrapper();
    void setupButtons();
	void setupSessionGroupDisplayWrapper();
	void setupSessionTypeDisplayWrapper();
	void setuptDCSDisplayWrapper();
	void setupCESModeDisplayWrapper();

    // button handling
    void powerButtonClicked();

private:
    Ui::MainWindow *ui;
    // power and battery
    int battery; // ranges from 0 to 100
    float batteryDrain; // rate at which the battery drains
    bool isPowered;

    // session
	Session* currentSession;
	bool isSessionRunning;
	int sessionTime;
    QLabel* sessionTimeLabel;

    // recording
    QSpinBox* userDesignatedSpinBox;
    bool isRecording;

    // connection test
	struct CESModeLabelStruct CESshortPulse;
	bool CESModeBlink;

    // timers
    QTimer* powerButtonTimer;
    QTimer* idleTimer;
    QTimer* batteryTimer;
	QTimer* sessionTimer;
	QTimer* startSessionTimer;
	QTimer* sessionBlinkTimer;

    // background color of each number
    QVector<QLabel*> intensityLabels;
    QVector<QString> lightColours;

	// session group labels
	QVector<sessionGroupLabelStruct> sessionGroupLabels;
	// session type labels
	QVector<sessionTypeLabelStruct> sessionTypeLabels;

    // tDCS label vector
	QVector<QLabel*> tDCSLabels;

    // ui elements
    QProgressBar* batteryDisplayBar;

    // User details
    User* currUser;
    QVector<User*> users;
    QVector<QLabel*> userLabels;

    // helper functions
    void turnOnIntensityNum(int, int);
    void turnOffIntensityNum(int, int);
    void displayBattery();
	void cycleSessionGroups();
	void colourSessionGroup(Session::SessionGroup);
	void cycleSessionTypesUp();
	void cycleSessionTypesDown();
	void colourSessionType(Session::SessionType);
	void colourtDCSNumber(int vectorPos);
	int gettDCSNumber();

    void updateUserSessionList();



private slots:
    void powerButtonPressed();
    void powerButtonReleased();
    void powerButtonHeld();

    // starting session
	void sessionStartButtonPressed();

	// intensity buttons
	void increaseIntensityButtonPressed();
	void decreaseIntensityButtonPressed();

    // power on/off
    void powerOn();
    void powerOff();
    void softOff();

    // battery decay
    void drainBattery();

    // user slots
    void cycleUsers();
    void recordSession();
    void saveSession();

    void updateSessionInfo(int row);

    void clearIntensityNum();
	void updateSessionTimer();
	void startSession();
	void sessionBlink();

};

#endif // MAINWINDOW_H
