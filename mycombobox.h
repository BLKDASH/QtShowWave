#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
#include <QMouseEvent>
//创建一个父类是combobox的类
class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyComboBox(QWidget *parent = nullptr);
    ~MyComboBox();

protected:
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(bool);
};

#endif // MYCOMBOBOX_H
