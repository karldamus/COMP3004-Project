#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

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
    void powerButtonHeld();
    void powerButtonClicked();

    // other initializations


    // dev testing
    void test();

private:
    Ui::MainWindow *ui;

    // timers
    QElapsedTimer* powerButtonTimer;


private slots:
    void powerButtonPressed();
    void powerButtonReleased();
};

#endif // MAINWINDOW_H
