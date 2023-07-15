#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pEmojForm(nullptr)
{
    ui->setupUi(this);
    //ui->pushButton
    connect(ui->pushButton,&QPushButton::clicked,[this](){
        if(m_pEmojForm == nullptr)
        {
            m_pEmojForm = new EmojForm(this);
        }
        m_pEmojForm->show();
    });

    connect(ui->pushButton_close,&QPushButton::clicked,[this](){
        if(m_pEmojForm != nullptr)
        {
            m_pEmojForm->close();
            delete m_pEmojForm;
            m_pEmojForm = nullptr;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

