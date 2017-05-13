#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->pd = new PaintDevice();
    ui->verticalLayout->addWidget(this->pd);
    QObject::connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(raytrace(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::raytrace(bool x)
{
    this->pd->draw();
}
