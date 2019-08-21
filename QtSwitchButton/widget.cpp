#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QColor>

//https://blog.csdn.net/xiezhongyuan07/article/details/93217781

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);



    switchComponentInit();


}

Widget::~Widget()
{
    delete ui;
}

//1. 背景
//背景可以分解为 两侧的两个半圆 + 中间的矩形块 + “开启”文字 + “关闭文字”
void Widget::drawBackGround(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QColor bgColor = m_checked ? m_bgColorOn : m_bgColorOff;

    if (isEnabled()) {
          bgColor.setAlpha(150);
    }

    painter->setBrush(bgColor);

//    QRect rect(0, 0, width(), height());
//    int side = qMin(width(), height());

   QRect rect(swBtn_loc_offset_x, swBtn_loc_offset_y, swWidth, swHeight);
   int side = qMin(swWidth, swHeight);

    //左侧半圆
    QPainterPath path1;
    path1.addEllipse(rect.x(), rect.y(), side, side);

    //右侧半圆
    QPainterPath path2;
    path2.addEllipse(rect.width() - side + swBtn_loc_offset_x, rect.y(), side, side);
//    path2.addEllipse(rect.width() - side , rect.y(), side, side);

    //中间的矩形
    QPainterPath path3;
    path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, swHeight);
//    path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, height());

    QPainterPath path = path1 + path2 + path3;
    painter->drawPath(path);


    //Andy added 0727
    //滑鼠觸控反應範圍
    mouseTouchRange = QRect(swBtn_loc_offset_x, swBtn_loc_offset_y, rect.width(), swHeight);

    //绘制文本
    //滑块半径
    int sliderWidth = qMin(swHeight, swWidth) - m_space * 2 - 5;
//    int sliderWidth = qMin(height(), width()) - m_space * 2 - 5;
    if (!m_checked){
        QRect textRect(0 + swBtn_loc_offset_x - 3, 0 + swBtn_loc_offset_y - 3, swWidth - sliderWidth, swHeight);
//        QRect textRect(0, 0, width() - sliderWidth, height());

        painter->setPen(QPen(m_textColor));
        painter->setFont(QFont("Times", stringSize, QFont::Bold));
        painter->drawText(textRect, Qt::AlignCenter, m_textOn);

    } else {
        QRect textRect(sliderWidth + swBtn_loc_offset_x - 3, 0 + swBtn_loc_offset_y - 3, swWidth - sliderWidth, swHeight);
//        QRect textRect(sliderWidth, 0, width() - sliderWidth, height());

        painter->setPen(QPen(m_textColor));
        painter->setFont(QFont("Times", stringSize, QFont::Bold));
        painter->drawText(textRect, Qt::AlignCenter, m_textOff);

    }

    painter->restore();
}

//2. 滑动块
//滑动块比较简单，就是画一个圆形。不过要考虑动画的因素。
void Widget::drawSlider(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QColor color = m_checked ? m_sliderColorFrontSide : m_sliderColorFrontSide;

    painter->setBrush(QBrush(color));

    int sliderWidth = qMin(swWidth, swHeight) - m_space * 2;
//    int sliderWidth = qMin(width(), height()) - m_space * 2;

    QRect rect(m_space + m_startX + swBtn_loc_offset_x, m_space + swBtn_loc_offset_y, sliderWidth, sliderWidth);
    painter->drawEllipse(rect);

    painter->restore();
}
//3. 动画效果
//动画就是使用的定时器QTimer， 进行设置滑动块的开始位置。

//还有一点
//定时器链接的信号槽，就应该是移动滑块，也就是动画的效果。我们该怎么实现呢？
void Widget::updateValue()
{
   if (m_checked) {
       if (m_startX < m_endX) {
           m_startX += m_step;
       } else {
           m_startX = m_endX;
//           m_timer->stop();
       }
   } else {
       if (m_startX > m_endX) {
           m_startX -= m_step;
       } else {
           m_startX = m_endX;
//           m_timer->stop();
       }
   }

   update();
}


//改变状态：

//这时候重点的问题我们都已经解决了，不过我们是在一个QWidget上来绘画的，
//并没有点击事件。所以我们应该重写QWidget的鼠标按下事件mousePressEvent。
//在鼠标按下事件中，我们需要做以下几件事：

//1. 改变控件的属性 m_checked
//2. 发送状态改变信号(类似QPushButton的clicked(bool))信号
//3. 根据控件属性m_checked 计算滑块开始位置m_startX, 与 滑块结束位置 m_endX
//4. 启动定时器开始




void Widget::mousePressEvent(QMouseEvent *ev)
{
//   Q_UNUSED(ev)


   //Andy added 0727

   if (!mouseTouchRange.contains(ev->x(),ev->y()))
   {
       return;
   }


   m_checked = !m_checked;
//   emit statusChanged(m_checked);

   //计算步长
   m_step = width() / 10;

   //计算滑块X轴终点坐标
   if (m_checked) {
       m_endX = swWidth - swHeight;
//       m_endX = width() - height();
   } else {
       m_endX = 0;
   }

   //判断是否使用动画
   if (m_animation) {
//       m_timer->start();
   } else{
       m_startX = m_endX;
       update();
   }
}
void Widget::paintEvent(QPaintEvent *)
{

    QPainter *painter = new QPainter(this);

    drawBackGround(painter);

//    painter = new QPainter(this);
    drawSlider(painter);
}
void Widget::switchComponentInit()
{
    QPainter *painter = new QPainter(this);

    drawBackGround(painter);

//    painter = new QPainter(this);
    drawSlider(painter);


    m_checked = !m_checked;
 //   emit statusChanged(m_checked);

    //计算步长
    m_step = width() / 10;

    //计算滑块X轴终点坐标
    if (m_checked) {
        m_endX = swWidth - swHeight;
 //       m_endX = width() - height();
    } else {
        m_endX = 0;
    }

    //判断是否使用动画
    if (m_animation) {
 //       m_timer->start();
    } else{
        m_startX = m_endX;
        update();
    }
}
