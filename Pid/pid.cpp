#include "pid.h"
#include "ui_pid.h"
#include <QDebug>

#define LHDEBUG 0

Pid::Pid(QWidget *parent) :
    QWidget(parent),
    uiPid(new Ui::Pid)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uiPid->setupUi(this);
    joint = NULL;
}

Pid::~Pid()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    delete uiPid;
}

void Pid::pidInit(int ID)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    this->joint = jointGetJoint(ID);
    if(!joint) {
        return ;
    }
    on_adjustGroupComboBox_currentIndexChanged(uiPid->adjustGroupComboBox->currentIndex());
    showMaxLimit();
}

void Pid::showSPID()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    uint16_t data16 = 0;
    // 位置环P
    jointGetPositionP(joint, &data16, 100, NULL);
    uiPid->POS_PSpinBox->setValue(data16);
    // 位置环I
    jointGetS_POSITION_I(joint, &data16, 100, NULL);
    uiPid->POS_ISpinBox->setValue(data16);
    // 位置环D
    jointGetS_POSITION_D(joint, &data16, 100, NULL);
    uiPid->POS_DSpinBox->setValue(data16);
    // 位置死区
    jointGetPositionDs(joint, &data16, 100, NULL);
    uiPid->POS_DSSpinBox->setValue(data16);
    // 速度环P
    jointGetSpeedP(joint, &data16, 100, NULL);
    uiPid->SPD_PSpinBox->setValue(data16);
    // 速度环I
    jointGetSpeedI(joint, &data16, 100,NULL);
    uiPid->SPD_ISpinBox->setValue(data16);
    // 速度环D
    jointGetS_SPEED_D(joint, &data16, 100, NULL);
    uiPid->SPD_DSpinBox->setValue(data16);
    // 速度死区
    jointGetS_SPEED_DS(joint, &data16, 100, NULL);
    uiPid->SPD_DSSpinBox->setValue(data16);
    // 电流环P
    jointGetCurrP(joint, &data16, 100, NULL);
    uiPid->CUR_PSpinBox->setValue(data16);
    // 电流环I
    jointGetCurrI(joint, &data16, 100, NULL);
    uiPid->CUR_ISpinBox->setValue(data16);
    // 电流环D
    jointGetS_CURRENT_D(joint, &data16, 100, NULL);
    uiPid->CUR_DSpinBox->setValue(data16);
}

void Pid::showMaxLimit()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uint16_t data16 = 0;
    // 最大加速度
    jointGetMaxAcceleration(joint, &data16, 100, NULL);
    uiPid->maxAccLineEdit->setText(QString::number(data16));
//    qDebug() << data16;
    // 最大速度
    jointGetMaxSpeed(joint, &data16, 100, NULL);
    uiPid->maxSpdLineEdit->setText(QString::number(data16));
//    qDebug() << data16;
}

void Pid::on_POS_PSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->POS_PSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->POS_PSpinBox->text();
    jointSetPositionP(joint, str.toShort(), 100, NULL);
}


void Pid::on_SPD_PSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->SPD_PSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->SPD_PSpinBox->text();
    jointSetSpeedP(joint, str.toShort(), 100, NULL);
}

void Pid::on_SPD_ISpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->SPD_ISpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->SPD_ISpinBox->text();
    jointSetSpeedI(joint, str.toShort(), 100, NULL);
}

void Pid::on_CUR_PSpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->CUR_PSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->CUR_PSpinBox->text();
    jointSetCurrP(joint, str.toShort(), 100, NULL);
}

void Pid::on_CUR_ISpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->CUR_ISpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->CUR_ISpinBox->text();
    jointSetCurrI(joint, str.toShort(), 100, NULL);
}

void Pid::on_POS_DSSpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->POS_DSSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->POS_DSSpinBox->text();
    jointSetPositionDs(joint, str.toShort(), 100, NULL);
}

void Pid::on_POS_DSpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->POS_DSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->POS_DSpinBox->text();
    jointSetS_POSITION_D(joint, str.toShort(), 100, NULL);
}

void Pid::on_POS_ISpinBox_editingFinished()
{
    if(!joint) {
        uiPid->POS_ISpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->POS_ISpinBox->text();
    jointSetS_POSITION_I(joint, str.toShort(), 100, NULL);
}

void Pid::on_SPD_DSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->SPD_DSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->SPD_DSpinBox->text();
    jointSetS_SPEED_D(joint, str.toShort(), 100, NULL);
}

void Pid::on_SPD_DSSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->SPD_DSSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->SPD_DSSpinBox->text();
    jointSetS_SPEED_DS(joint, str.toShort(), 100, NULL);
}

void Pid::on_CUR_DSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->CUR_DSpinBox->setValue(0);
        return ;
    }
    QString str = uiPid->CUR_DSpinBox->text();
    jointSetS_CURRENT_D(joint, str.toShort(), 100, NULL);
}

void Pid::on_adjustGroupComboBox_currentIndexChanged(int index)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    switch(index) {
        case 0:
            showSPID();
            break;
        case 1:
            break;
        case 2:
            break;
        default :
             break;
    }
}

void Pid::on_maxSpdLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxSpdLineEdit->setText("");
        return ;
    }
    QString str = uiPid->maxSpdLineEdit->text();
    jointSetMaxSpeed(joint, str.toShort(), 100, NULL);
}

void Pid::on_maxAccLineEdit_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->maxAccLineEdit->setText("");
        return ;
    }
    QString str = uiPid->maxAccLineEdit->text();
    jointSetMaxAcceleration(joint, str.toShort(), 100, NULL);
}

/**
 * @brief Pid::on_maxCurLineEdit_editingFinished
 */
void Pid::on_maxCurLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxCurLineEdit->setText("");
        return ;
    }
    QString str = uiPid->maxCurLineEdit->text();
    // 设置最大的电流
}

void Pid::on_minPosLLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->minPosLLineEdit->setText("");
        return ;
    }
}

void Pid::on_minPosHLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->minPosHLineEdit->setText("");
        return ;
    }
}

void Pid::on_maxPosLLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxPosLLineEdit->setText("");
        return ;
    }
}

void Pid::on_maxPosHLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxPosHLineEdit->setText("");
        return ;
    }
}
