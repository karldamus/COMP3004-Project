#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

// UI CONSTANTS
//

#define MIN_INTENSITY_LEVEL 1
#define MAX_INTENSITY_LEVEL 8

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

    // ui creation methods
    void setupGridWrappers();
    void setupIntensityLevelDisplayWrapper();
    void setupButtons();


    // button handling
    void powerButtonClicked();

    // other initializations


    // dev testing
    void test();

private:
    Ui::MainWindow *ui;
    bool isPowered;

    // timers
    QTimer* powerButtonTimer;
    QTimer* idleTimer;


private slots:
    void powerButtonPressed();
    void powerButtonReleased();
    void powerButtonHeld();

    // power on/off
    void powerOn();
    void powerOff();
};

#endif // MAINWINDOW_H
