#ifndef BOTTOM_H
#define BOTTOM_H

#include "bottom_global.h"
#include "mrapi.h"
#include <QWidget>

namespace Ui {
class Bottom;
}

class BOTTOMSHARED_EXPORT Bottom : public QWidget
{
    Q_OBJECT
public:

public:
    Bottom(QWidget *parent = 0);
    ~Bottom();

public slots:
    void waitingForWidgetReady();

signals:
    void cmbIDChanged(int ID);

private:

private slots:
    void on_btnUpdateID_clicked();
    void on_enableDriverPushButton_clicked();
    void on_btnFlash_clicked();
    void on_btnLoad_clicked();
    void on_btnSave_clicked();
    void on_cmbID_currentIndexChanged(int index);


private:
    Ui::Bottom *uiBottom;
    JOINT_HANDLE joint;
    bool isCANInitialSucceed;
};

#endif // BOTTOM_H
