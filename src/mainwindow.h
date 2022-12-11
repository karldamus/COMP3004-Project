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

#include "user.h"
#include "session.h"

// UI CONSTANTS
//

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
	};

    // ui creation methods
    void setupGridWrappers();
    void setupIntensityLevelDisplayWrapper();
    void setupButtons();
	void setupSessionGroupDisplayWrapper();


    // button handling
    void powerButtonClicked();

    // other initializations


    // dev testing
    void test();

private:
    Ui::MainWindow *ui;
    int battery; // ranges from 0 to 100
    float batteryDrain; // rate at which the battery drains
    bool isPowered;
	Session* currentSession;
	bool isSessionRunning;

    // timers
    QTimer* powerButtonTimer;
    QTimer* idleTimer;
    QTimer* batteryTimer;

    // background color of each number
    QVector<QLabel*> intensityLabels;
    QVector<QString> lightColours;

	// session group labels
	QVector<sessionGroupLabelStruct> sessionGroupLabels;

    // ui elements
    QProgressBar* batteryDisplayBar;

    // helper functions
    void turnOnIntensityNum(int, int);
    void turnOffIntensityNum(int, int);
    void displayBattery();
    void delay();
	void cycleSessionGroups();
	void colourSessionGroup(Session::SessionGroup);


    User user;


private slots:
    void powerButtonPressed();
    void powerButtonReleased();
    void powerButtonHeld();

    // power on/off
    void powerOn();
    void powerOff();
    void softOff();

    // battery decay
    void drainBattery();

    void clearIntensityNum();
};

#endif // MAINWINDOW_H
