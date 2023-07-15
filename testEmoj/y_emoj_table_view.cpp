#include "y_emoj_table_view.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStringList>
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QClipboard>


YEmojTableView::YEmojTableView(QWidget* parent) : QTableView(parent)
{
    m_delegate = new YEmojItemDelegate;
    m_model    = new YEmojStandardItemModel;
    this->setModel(m_model);
    this->setItemDelegate(m_delegate);


    QStringList listHeader;
    listHeader<<"类别";
    for(int i=0;i<m_MaxCol;i++)
    {
        listHeader << QString("成员%1").arg(i+1);
    }
    m_model->setHorizontalHeaderLabels(listHeader);

    this->resizeColumnsToContents();
    this->resizeRowsToContents();


    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);//


    TraversePicFile();
}

YEmojTableView::~YEmojTableView()
{
    // 删除new出的pixmap

    for(auto it = m_model->m_mapPixmap.begin();it !=  m_model->m_mapPixmap.end(); it++)
    {
        if(it.value() != nullptr)
        {
            delete it.value();
        }
    }
}

void YEmojTableView::TraversePicFile(void )
{
    //获取程序当前运行目录
    QString emojPath = QCoreApplication::applicationDirPath();
    emojPath += "\\emoj\\";

    QDir dir(emojPath);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    int32_t picCount = -1;//确定是否要换行,存储的是一组表情的个数
    int32_t rowOffset = 0;
    //bool    flagNewType = true;//标记是否是新的表情类型
    QString currentType = "";
    foreach (const QString &str, files)
    {
        ++picCount;
        auto subStrs = str.split("_");//face_smile face + smile
        if(subStrs.size() < 2)
        {
            continue;
        }
        if(currentType == "")
        {
            currentType = subStrs[0] ;
            AddTypeItem(currentType, picCount/m_MaxCol, 0);//第一个表情，第一列需要添加类别
        }
        if(currentType != subStrs[0])//类型判断
        {
            currentType = subStrs[0];
            rowOffset++;
            picCount = 0;
            AddTypeItem(currentType, rowOffset + picCount/m_MaxCol, 0);//新类型的表情时，第一列需要添加类别
        }
        if( picCount > m_MaxCol )//当前表情组已满一行需要换行
        {
            rowOffset++;
            picCount = 0;
        }

        m_model->m_mapPixmap[emojPath+str] = new QPixmap(emojPath+str);
        AddItem(emojPath+str, rowOffset + picCount/m_MaxCol, picCount%m_MaxCol+1);
    }

    this->setColumnWidth(0,80);//注意：这个设置列宽一定要加完项之后再调整。
    //for(int index = this->columnSpan() )
    //this->setColumnWidth();
    return;
}

void YEmojTableView::mouseMoveEvent(QMouseEvent *event)
{
    int column = this->columnAt(event->x());
    int row = this->rowAt(event->y());
    if(column == 0 && row == 0)
    {
        this->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        this->setCursor(Qt::PointingHandCursor);
    }
}

void YEmojTableView::AddItem(const QString& picName,int32_t row, int32_t col)
{
    QStandardItem * item = new QStandardItem(picName);//何时delete
    m_model->setItem(row,col,item);
}

void YEmojTableView::AddTypeItem(const QString& typeName,int32_t row, int32_t col)
{
    QStandardItem * item = new QStandardItem(typeName);//何时delete?
    m_model->setItem(row,col,item);
}

YEmojItemDelegate::YEmojItemDelegate(QObject* parent)
    :QItemDelegate(parent)
{

}

void YEmojItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column()==0)//第0列不用委托的方式重绘
    {
        QItemDelegate::paint(painter,option,index);
        return;
    }

    const QAbstractItemModel * pmodel = index.model();
    QVariant varPicPath = pmodel->data(index,Qt::EditRole);

    if(varPicPath.isValid())
    {
        //qDebug() << varPicPath <<endl;
        QString key = varPicPath.toString();
        YEmojStandardItemModel* pYModel = (YEmojStandardItemModel*)pmodel;
        QPixmap* star = pYModel->m_mapPixmap[key];

        QRect rect = option.rect;
        QPixmap scaled = star->scaled(rect.width(),rect.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);

        painter->drawPixmap(rect,scaled);
    }
}

bool YEmojItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type()==QEvent::MouseButtonPress )
    {
        QVariant var=model->data(index,Qt::EditRole);

        //int column = columnAt(event->x());
        //int row = this->rowAt(event->y());

        if(var.isValid() && index.column() != 0)
        {
            QString data        = var.toString();
            auto backslash      = data.lastIndexOf(QString("\\"));
            QString emoj = data.mid(backslash+1);
            emoj = emoj.mid(0,emoj.length()-4);//去掉".jpg" 去掉".png"
            emoj = emoj.mid(emoj.indexOf("_")+1);
            emoj = (QString(":") + emoj + QString(":"));

            //复制到剪贴板
            QClipboard  *clip = QApplication::clipboard();
            if(clip != nullptr)
            {
                clip->setText(emoj);
            }
            else
            {
                qDebug()<<"get QClipboard failed!";
            }
            qDebug() << emoj ;
        }

        return true;//I have handled the event
    }
    return false;
}

// 数据单元格的处理
//QVariant YEmojStandardItemModel::data(const QModelIndex &index, int role) const
//{
//    int column = index.column();
//    if(role == Qt::DisplayRole && column != 0)
//        return column+1;
//    if(role == Qt::ToolTipRole && column == 0)
//        return tr("love");
//    return QStandardItemModel::data(index,role);
//}

// 标题单元格的处理
//QVariant YEmojStandardItemModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    if(section==0 && orientation==Qt::Horizontal)
//    {
//        if(role == Qt::DecorationRole)
//            return QIcon(R"(D:\QT_project\build-testEmoj-Desktop_Qt_5_9_9_MinGW_32bit-Debug\debug\emoj\face_blush.png)");
//        if(role == Qt::DisplayRole)
//            return "";
//        if(role == Qt::ToolTipRole)
//            return tr("love");
//    }
//    return QStandardItemModel::headerData(section,orientation,role);//其他情况交给框架走默认执行
//}
