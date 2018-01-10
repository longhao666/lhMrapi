#include "setandhealth.h"
#include "ui_setandhealth.h"
#include <QMessageBox>
#include <QDebug>

#define LHDEBUG 0

#define MONITOR_INTEVAL 500         // 监视器更新周期（ms）

SetAndHealth::SetAndHealth(QWidget *parent) :
    QWidget(parent),
    uiSetAndHealth(new Ui::SetAndHealth)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uiSetAndHealth->setupUi(this);
    timer = NULL;
    joint = NULL;
}

SetAndHealth::~SetAndHealth()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    delete uiSetAndHealth;
}

void SetAndHealth::SetAndHealthIint(int ID)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    this->joint = jointGetJoint(ID);
    if(!joint) {
        return ;
    }
    if(!timer) {
        timer = new QTimer;
//        connect(timer,QTimer::timeout(),this,SetAndHealth::myTimerSlot());
        connect(timer,SIGNAL(timeout()),this,SLOT(myTimerSlot()));
        timer->start(MONITOR_INTEVAL);
    }
    uint16_t data16 = 0;
    // 读取ID 不能放在定时器里面,不然一直更新
    jointGetId(joint, &data16, 100, NULL);
    uiSetAndHealth->IDLineEdit->setText(QString::number(data16));
}

void SetAndHealth::myTimerSlot()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint){
        return ;
    }
    uint16_t data16 = 0;
    uint32_t data32 = 0;
    int data_t = 0;
    // 读取速度
    jointGetSpeed(joint,&data32,100,NULL);
    uiSetAndHealth->speedLineEdit->setText(QString::number(data32));
    // 读取电流
    jointGetCurrent(joint, &data32, 100, NULL);
    // 无符号变有符号,不然电流相反后数值特别大
    data_t = (int)data32;
    if(data_t < 0) {
        data_t = -data_t;
    }
    uiSetAndHealth->currentLineEdit->setText(QString::number(data_t) + "mA");
    // 读取位置
    jointGetPosition(joint, &data32, 100, NULL);
    data_t = (int)data32;
    if(data_t < 0) {
        data_t = -data_t;
        uiSetAndHealth->pOS_LLineEdit->setText("-" + QString::number(data_t));
    }else {
        uiSetAndHealth->pOS_LLineEdit->setText(QString::number(data32));
    }
    // 读取温度
    jointGetTemp(joint,&data16,100,NULL);
    uiSetAndHealth->tempLineEdit->setText(QString::number(static_cast<double>(data16) / 10, 'f', 1) + "°C");
    // 读取电压
    jointGetVoltage(joint,&data16,100,NULL);
    uiSetAndHealth->voltageLineEdit->setText(QString::number(data16) + "mV");


}

void SetAndHealth::on_IDPushButton_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    // 不断电重启的话既不能检测到旧ID,也不能用新ID
    if(!joint) {
        return ;
    }
    uint16_t newID = uiSetAndHealth->IDLineEdit->text().toShort();
    jointSetID(joint, newID, 100, NULL);
    jointSetSave2Flash(joint,100,NULL);
    QMessageBox::information(this, "information", QObject::tr("请断电重启，不然新旧ID都不能用了！"), QMessageBox::Ok);
}

void SetAndHealth::on_setZeroPushButton_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    uint16_t data16 = 0;
    int work = jointGetMode(joint, &data16, 20, NULL);
    if(work == 3) {
        jointSetZero(joint,100,NULL);
    }

}


