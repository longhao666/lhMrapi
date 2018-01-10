#include "bottom.h"
#include "ui_bottom.h"
#include <QMessageBox>
#include <QDebug>
#include <iostream>

using std::vector;
using std::sort;

#define LHDEBUG 0

Bottom::Bottom(QWidget *parent) :
    QWidget(parent),
    uiBottom(new Ui::Bottom)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uiBottom->setupUi(this);
    joint = NULL;
}

Bottom::~Bottom()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    delete uiBottom;
}

void Bottom::waitingForWidgetReady()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    this->on_btnUpdateID_clicked();
}

void Bottom::on_btnUpdateID_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
//    return ;
    uiBottom->cmbID->clear();
    vector<uint32_t> vectID;
    JOINT_HANDLE tempj = NULL;
    uint16_t ID = 0;
    for(int i=1;i<MAX_JOINTS+1;i++) {
        tempj = jointUp(i, masterLoadSendFunction(0));
        if(tempj) {
            jointGetId(tempj, &ID, 100, NULL);
            vectID.push_back(ID);
            qDebug() << "发现这个ID:" << ID;
        }
    }
    sort(vectID.begin(), vectID.end());
    joint = jointGetJoint((int)vectID.size() - 1);
    for(vector<uint32_t>::iterator iter = vectID.begin();
        iter != vectID.end();
        ++iter) {
        uiBottom->cmbID->addItem(QString::number(*iter, 10));
    }
}

void Bottom::on_enableDriverPushButton_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    uint16_t data16 = 0;
    jointGetSYS_ENABLE_DRIVER(joint, &data16, 20, NULL);
    bool isEnable = !data16;
//    qDebug() << isEnable;
    jointSetSYS_ENABLE_DRIVER(joint, isEnable, 20, NULL);
}

void Bottom::on_btnFlash_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    jointSetSave2Flash(joint, 20, NULL);
}

void Bottom::on_btnLoad_clicked()
{
    QMessageBox::information(this, tr("information"), tr("该功能还没实现！"), QMessageBox::Ok);
}

void Bottom::on_btnSave_clicked()
{
    QMessageBox::information(this, tr("information"), tr("该功能还没实现！"), QMessageBox::Ok);
}

void Bottom::on_cmbID_currentIndexChanged(int index)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
//    qDebug("============================2===========================");
//    qDebug() << index;
    emit cmbIDChanged(index);
}


