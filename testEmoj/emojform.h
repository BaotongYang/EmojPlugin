#ifndef EMOJFORM_H
#define EMOJFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <cstdint>
namespace Ui {
class EmojForm;
}

class EmojForm : public QWidget
{
    Q_OBJECT

public:
    explicit EmojForm(QWidget *parent = nullptr);
    ~EmojForm();

    //QStandardItemModel * m_emojModel = nullptr;

    QStandardItemModel * m_emojTableModel = nullptr;

    QItemSelectionModel *m_emojTableSelection;//Item选择模型

    int m_MaxCol = 10;//一行放几个表情

private:
    Ui::EmojForm *ui;
};

#endif // EMOJFORM_H
