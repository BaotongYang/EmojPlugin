#ifndef YEMOJTABLEVIEW_H
#define YEMOJTABLEVIEW_H

#include <QTableView>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPixmap>
#include <QMap>
#include <QDir>
#include <QCoreApplication>
#include <cstdint>

class YEmojItemDelegate;
class YEmojStandardItemModel;

// view类
class YEmojTableView : public QTableView
{
public:
    YEmojTableView(QWidget * parent=0);
    virtual ~YEmojTableView();//

    // 添加项到TableView
    void AddItem(const QString& picName,int32_t row, int32_t col);

    // 添加类别单元格
    void AddTypeItem(const QString& typeName,int32_t row, int32_t col);

    // 遍历文件夹下所有图片文件
    void TraversePicFile(void);

    // 最大列数
    int32_t m_MaxCol = 10;


protected:
    void mouseMoveEvent(QMouseEvent *event);
private:
    YEmojItemDelegate * m_delegate;
    YEmojStandardItemModel *m_model;
};


// 委托类
class YEmojItemDelegate:public QItemDelegate
{
public:
    YEmojItemDelegate(QObject* parent=0);
    virtual ~YEmojItemDelegate(){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QPixmap m_pixmap1;
    QPixmap m_pixmap2;
};


// 标准项模型
class YEmojStandardItemModel:public QStandardItemModel
{
public:
    YEmojStandardItemModel(QObject * parent=0):QStandardItemModel(parent){};
    virtual ~YEmojStandardItemModel(){}
    //QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // 存储文件夹下的所有pixmap
    QMap<QString,QPixmap*> m_mapPixmap;
};

#endif // YEMOJTABLEVIEW_H
