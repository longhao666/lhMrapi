#ifndef SETANDHEALTH_H
#define SETANDHEALTH_H

#include "setandhealth_global.h"
#include <QWidget>
#include <QTimer>
#include "mrapi.h"

namespace Ui {
class SetAndHealth;
}

class SETANDHEALTHSHARED_EXPORT SetAndHealth : public QWidget
{
    Q_OBJECT
public:


public:
    SetAndHealth(QWidget *parent = 0);
    ~SetAndHealth();


public slots:
    void SetAndHealthIint(int ID);

private slots:
    void myTimerSlot();

    void on_IDPushButton_clicked();

    void on_setZeroPushButton_clicked();


private:
    Ui::SetAndHealth *uiSetAndHealth;
    JOINT_HANDLE joint;
    QTimer *timer;


};

#endif // SETANDHEALTH_H
