#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QColor>
#include <QColorDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentColor(QColor(0,0,0)),
    game(new GameWidget(this))
{
    ui->setupUi(this);
    QPixmap icon(16, 16);
    icon.fill(currentColor);
    ui->Button_Select_Color->setIcon(QIcon(icon));

    connect(ui->Button_Start, SIGNAL(clicked()), game,SLOT(startGame()));
    connect(ui->Button_Stop, SIGNAL(clicked()), game,SLOT(stopGame()));
    connect(ui->Button_Clear, SIGNAL(clicked()), game,SLOT(clear()));
    connect(ui->IterationTime, SIGNAL(valueChanged(int)), game, SLOT(setInterval(int)));
    connect(ui->FieldSize, SIGNAL(valueChanged(int)), game, SLOT(setCellNumber(int)));
    connect(game,SIGNAL(environmentChanged(bool)),ui->FieldSize,SLOT(setDisabled(bool)));
    connect(game,SIGNAL(gameEnds(bool)),ui->FieldSize,SLOT(setEnabled(bool)));
    connect(ui->Button_Select_Color, SIGNAL(clicked()), this, SLOT(SelectColor()));

    connect(ui->Button_Save, SIGNAL(clicked()), this, SLOT(SaveGame()));
    connect(ui->Button_Load, SIGNAL(clicked()), this, SLOT(LoadGame()));

    ui->mainLayout->setStretchFactor(ui->gameLayout, 8);
    ui->mainLayout->setStretchFactor(ui->settingLayout, 2);
    ui->gameLayout->addWidget(game);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SaveGame()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save your game"), QDir::homePath(), tr("GameLive *.life Files (*.life)"));
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QString s = QString::number(game->cellNumber())+"\n";
    file.write(s.toUtf8());
    file.write(game->dump().toUtf8());
    QColor color = game->masterColor();
    QString buf = QString::number(color.red())+" "+
                  QString::number(color.green())+" "+
                  QString::number(color.blue())+"\n";
    file.write(buf.toUtf8());
    buf.clear();
    buf = QString::number(ui->FieldSize->value())+"\n";
    file.write(buf.toUtf8());
    file.close();
}
void MainWindow::LoadGame()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open saved game"), QDir::homePath(), tr("GameLive File (*.life)"));
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);

    int sv;
    in >> sv;
    ui->FieldSize->setValue(sv);

    game->setCellNumber(sv);
    QString dump="";
    for(int k=0; k != sv; k++) {
        QString t;
        in >> t;
        dump.append(t+"\n");
    }
    game->setDump(dump);

    int r,g,b; // RGB color
    in >> r >> g >> b;
    currentColor = QColor(r,g,b);
    game->setMasterColor(currentColor); // sets color of the dots
    QPixmap icon(16, 16); // icon on the button
    icon.fill(currentColor); // fill with new color
    ui->Button_Select_Color->setIcon( QIcon(icon) ); // set icon for button
    in >> r; // r will be interval number
    ui->FieldSize->setValue(r);
    game->setInterval(r);
}
void MainWindow::SelectColor()
{
    QColor color = QColorDialog::getColor(currentColor, this, tr("Select color of figures"));
    if(!color.isValid())
        return;
    currentColor = color;
    game->setMasterColor(color);
    QPixmap icon(16, 16);
    icon.fill(color);
    ui->Button_Select_Color->setIcon( QIcon(icon) );
}
