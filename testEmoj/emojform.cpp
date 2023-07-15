#include "emojform.h"
#include "ui_emojform.h"
#include <QImage>
#include <QCoreApplication>
#include <QDir>
#include "y_emoj_table_view.h"



EmojForm::EmojForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmojForm)
{
    ui->setupUi(this);

    //ui->emojTableView
    //YEmojTableView yEmojTableView;
    //yEmojTableView.AddItem("d:\\123.png",1,1);

}

EmojForm::~EmojForm()
{
    delete ui;
}


