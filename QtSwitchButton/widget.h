#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QMouseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;


     bool m_checked = true; // 正面
     QColor m_bgColorOn = QColor(130, 210, 230);
     QColor m_bgColorOff = QColor(255, 70, 30);


     QColor m_sliderColorBackSide = QColor(0,0,0);
     QColor m_sliderColorFrontSide = QColor(250,250,250);
     QColor m_textColor = QColor(30,10,10);


    //Switch location
     int swBtn_loc_offset_x = 290; //50;
     int swBtn_loc_offset_y = 290; //50;

     //Switch width/height
     const int swWidth = 110; //180;
     const int swHeight = 48; //80;

     //Slide circle size, less then "min(swWidth, swHeight)"
     int m_space = swHeight - 3; //40; //75;
     int m_startX = 0;
     int m_endX = 50;

     int m_step = 5;

     //String size
     const int stringSize = 16;


     QString m_textOn = QString("正面");
     QString m_textOff = QString("反面");

     bool m_animation = false;

     QRect mouseTouchRange;

public:
    void updateValue();
    void drawSlider(QPainter *painter);
    void drawBackGround(QPainter *painter);
    void mousePressEvent(QMouseEvent *ev);
    void switchComponentInit();

protected:
    void paintEvent(QPaintEvent *);



};

#endif // WIDGET_H
