#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include "gamewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void SelectColor();

    void SaveGame();
    void LoadGame();

private:
    Ui::MainWindow *ui;
    QColor currentColor;
    GameWidget* game;
};
#endif // MAINWINDOW_H
