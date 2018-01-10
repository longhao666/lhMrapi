#include "oscilloscope.h"
#include "ui_oscilloscope.h"

#define LHDEBUG 0

#define OSCILLO_SCOPE_INTEVAL 10    // 示波器采样周期（ms）

OscilloScope::OscilloScope(QWidget* parent) :
  QWidget(parent),
  uiOscilloScope(new Ui::OscilloScope)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
  uiOscilloScope->setupUi(this);
  joint = NULL;

}

OscilloScope::~OscilloScope()
{
  delete uiOscilloScope;
}

void OscilloScope::OscilloScopeInitialize(int ID)
{
    joint = jointGetJoint(ID); // 修改当前控制的模块ID
  // initialize Oscilloscope - related content
  tgPOSPushButtonOn = "background-color: rgb(";
  tgPOSPushButtonOn += QString::number(TGPOS_RGB_R) + ',' + QString::number(TGPOS_RGB_G) + ',' + QString::number(TGPOS_RGB_B) + ");";
  tgPOSPushButtonOff = "background-color: rgb(128, 128, 128);";
  tgSPDPushButtonOn = "background-color: rgb(";
  tgSPDPushButtonOn += QString::number(TGSPD_RGB_R) + ',' + QString::number(TGSPD_RGB_G) + ',' + QString::number(TGSPD_RGB_B) + ");";
  tgSPDPushButtonOff = tgPOSPushButtonOff;
  tgCURPushButtonOn = "background-color: rgb(";
  tgCURPushButtonOn += QString::number(TGCUR_RGB_R) + ',' + QString::number(TGCUR_RGB_G) + ',' + QString::number(TGCUR_RGB_B) + ");";
  tgCURPushButtonOff = tgPOSPushButtonOff;
  rlPOSPushButtonOn = "background-color: rgb(";
  rlPOSPushButtonOn += QString::number(RLPOS_RGB_R) + ',' + QString::number(RLPOS_RGB_G) + ',' + QString::number(RLPOS_RGB_B) + ");";
  rlPOSPushButtonOff = tgPOSPushButtonOff;
  rlSPDPushButtonOn = "background-color: rgb(";
  rlSPDPushButtonOn += QString::number(RLSPD_RGB_R) + ',' + QString::number(RLSPD_RGB_G) + ',' + QString::number(RLSPD_RGB_B) + ");";
  rlSPDPushButtonOff = tgPOSPushButtonOff;
  rlCURPushButtonOn = "background-color: rgb(";
  rlCURPushButtonOn += QString::number(RLCUR_RGB_R) + ',' + QString::number(RLCUR_RGB_G) + ',' + QString::number(RLCUR_RGB_B) + ");";
  rlCURPushButtonOff = tgPOSPushButtonOff;
  scopeEnablePushButtonOn = "color: rgb(";
  scopeEnablePushButtonOn += QString::number(SCOPEENABLE_RGB_R) + ',' + QString::number(SCOPEENABLE_RGB_G) + ',' + QString::number(SCOPEENABLE_RGB_B) + ");";
  scopeEnablePushButtonOff = "color: rgb(";
  scopeEnablePushButtonOff += QString::number(SCOPEDISABLE_RGB_R) + ',' + QString::number(SCOPEDISABLE_RGB_G) + ',' + QString::number(SCOPEDISABLE_RGB_B) + ");";

  // 启动刷新控件的定时器
  if (timerOscilloScope == NULL) {
    timerOscilloScope = new QTimer(this);
    connect( timerOscilloScope, SIGNAL(timeout()), this, SLOT(updatePlot()) );
    timerOscilloScope->start(10);
  }

  // 初始化qwt plot
  uiOscilloScope->plot->enableAxis(QwtPlot::xBottom , false);
  uiOscilloScope->plot->setCanvasBackground(Qt::gray);
  uiOscilloScope->plot->setTitle("Current Curve");
  uiOscilloScope->plotSPD->enableAxis(QwtPlot::xBottom , false);
  uiOscilloScope->plotSPD->setCanvasBackground(Qt::gray);
  uiOscilloScope->plotSPD->setTitle("Speed Curve");
  uiOscilloScope->plotPOS->enableAxis(QwtPlot::xBottom , false);
  uiOscilloScope->plotPOS->setCanvasBackground(Qt::gray);
  uiOscilloScope->plotPOS->setTitle("Position Curve");

  if (grid == NULL) {
    grid = new QwtPlotGrid ();
    grid->setPen(Qt::black, 0, Qt::DotLine);
    grid->attach(uiOscilloScope->plot);
    grid = new QwtPlotGrid ();
    grid->setPen(Qt::black, 0, Qt::DotLine);
    grid->attach(uiOscilloScope->plotSPD);
    grid = new QwtPlotGrid ();
    grid->setPen(Qt::black, 0, Qt::DotLine);
    grid->attach(uiOscilloScope->plotPOS);
  }
  if (plotMag == NULL) { // 暂时没用
    plotMag = new QwtPlotMagnifier(uiOscilloScope->plot->canvas());
    plotMag->setAxisEnabled(QwtPlot::xBottom , false);
    plotMag->setAxisEnabled(QwtPlot::yLeft , false);
    plotSPDMag = new QwtPlotMagnifier(uiOscilloScope->plotSPD->canvas());
    plotSPDMag->setAxisEnabled(QwtPlot::xBottom , false);
    plotSPDMag->setAxisEnabled(QwtPlot::yLeft , false);
    plotPOSMag = new QwtPlotMagnifier(uiOscilloScope->plotPOS->canvas());
    plotPOSMag->setAxisEnabled(QwtPlot::xBottom , false);
    plotPOSMag->setAxisEnabled(QwtPlot::yLeft , false);
  }

  if (curveRlCUR == NULL) {
    curveRlCUR = new QwtPlotCurve();
    curveRlCUR->setPen(QColor(RLCUR_RGB_R,RLCUR_RGB_G,RLCUR_RGB_B), 3 );
    curveRlCUR->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curveRlCUR->attach(uiOscilloScope->plot);
  }
  if (curveRlPOS == NULL) {
    curveRlPOS = new QwtPlotCurve();
    curveRlPOS->setPen(QColor(RLPOS_RGB_R,RLPOS_RGB_G,RLPOS_RGB_B), 3 );
    curveRlPOS->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curveRlPOS->attach(uiOscilloScope->plotPOS);
  }
  if (curveRlSPD == NULL) {
    curveRlSPD = new QwtPlotCurve();
    curveRlSPD->setPen(QColor(RLSPD_RGB_R,RLSPD_RGB_G,RLSPD_RGB_B), 3 );
    curveRlSPD->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curveRlSPD->attach(uiOscilloScope->plotSPD);
  }
  if (curveTgCUR == NULL) {
    curveTgCUR = new QwtPlotCurve();
    curveTgCUR->setPen(QColor(TGCUR_RGB_R,TGCUR_RGB_G,TGCUR_RGB_B), 3 );
    curveTgCUR->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curveTgCUR->attach(uiOscilloScope->plot);
  }
  if (curveTgPOS == NULL) {
    curveTgPOS = new QwtPlotCurve();
    curveTgPOS->setPen(QColor(TGPOS_RGB_R,TGPOS_RGB_G,TGPOS_RGB_B), 3 );
    curveTgPOS->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curveTgPOS->attach(uiOscilloScope->plotPOS);
  }
  if (curveTgSPD == NULL) {
    curveTgSPD = new QwtPlotCurve();
    curveTgSPD->setPen(QColor(TGSPD_RGB_R,TGSPD_RGB_G,TGSPD_RGB_B), 3 );
    curveTgSPD->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curveTgSPD->attach(uiOscilloScope->plotSPD);
  }

  if (joint == NULL) {
    return;
  }
  // 开启新线程准备显示示波器曲线
  if (osthread == NULL) {
    osthread = new OscilloScopeThread(this, this);
  }

    uint16_t data_L = 0;
    // 记录对象标志MASK的初始化
    jointGetSCP_MASK(joint, &data_L, 100, NULL);
    osthread->paintArea->Mask = data_L;
    //参数表中的“记录时间间隔（对10kHZ的分频值）”显示到测量条件选项卡中的对应控件里
    jointGetSCP_REC_TIM(joint, &data_L, 100, NULL);
    osthread->paintArea->ScanFrequency = data_L;
  uiOscilloScope->ScanFrequencyComboBox->setVisible(false); // 扫描频率设置改为不可见
  //同样，用参数表中的“记录时间间隔（对10kHZ的分频值）”设置扫描频率，初始化Interval
  osthread->paintArea->Interval = osthread->paintArea->ScanFrequency / 10 / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)
  // EnableScope在paintArea的构造函数初始化了
  // Initializes the color of the buttons
  if ((osthread->paintArea->Mask & MASK_TAGPOS) != 0x00) {
    uiOscilloScope->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOn);
  } else {
    uiOscilloScope->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOff);
  }
  if ((osthread->paintArea->Mask & MASK_TAGSPD) != 0x00) {
    uiOscilloScope->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOn);
  } else {
    uiOscilloScope->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOff);
  }
  if ((osthread->paintArea->Mask & MASK_TAGCUR) != 0x00) {
    uiOscilloScope->tgCURPushButton->setStyleSheet(tgCURPushButtonOn);
  } else {
    uiOscilloScope->tgCURPushButton->setStyleSheet(tgCURPushButtonOff);
  }
  if ((osthread->paintArea->Mask & MASK_MEACUR) != 0x00) {
    uiOscilloScope->rlCURPushButton->setStyleSheet(rlCURPushButtonOn);
  } else {
    uiOscilloScope->rlCURPushButton->setStyleSheet(rlCURPushButtonOff);
  }
  if ((osthread->paintArea->Mask & MASK_MEASPD) != 0x00) {
    uiOscilloScope->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOn);
  } else {
    uiOscilloScope->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOff);
  }
  if ((osthread->paintArea->Mask & MASK_MEAPOS) != 0x00) {
    uiOscilloScope->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOn);
  } else {
    uiOscilloScope->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOff);
  }
  //由记录对象标志初始化要显示的测量曲线
  if (osthread->paintArea->showItems.empty()) {
    qDebug("osthread->paintArea->showItems is empty");
  } else if (osthread->paintArea->showItems.size() == 6){
    if ((osthread->paintArea->Mask & MASK_TAGCUR) != 0x00) {
      osthread->paintArea->showItems[0].IsCheck = true;
    } else {
      osthread->paintArea->showItems[0].IsCheck = false;
    }
    if ((osthread->paintArea->Mask & MASK_TAGPOS) != 0x00) {
      osthread->paintArea->showItems[2].IsCheck = true;
    } else {
      osthread->paintArea->showItems[2].IsCheck = false;
    }
    if ((osthread->paintArea->Mask & MASK_TAGSPD) != 0x00) {
      osthread->paintArea->showItems[1].IsCheck = true;
    } else {
      osthread->paintArea->showItems[1].IsCheck = false;
    }
    if ((osthread->paintArea->Mask & MASK_MEACUR) != 0x00) {
      osthread->paintArea->showItems[3].IsCheck = true;
    } else {
      osthread->paintArea->showItems[3].IsCheck = false;
    }
    if ((osthread->paintArea->Mask & MASK_MEASPD) != 0x00) {
      osthread->paintArea->showItems[4].IsCheck = true;
    } else {
      osthread->paintArea->showItems[4].IsCheck = false;
    }
    if ((osthread->paintArea->Mask & MASK_MEAPOS) != 0x00) {
      osthread->paintArea->showItems[5].IsCheck = true;
    } else {
      osthread->paintArea->showItems[5].IsCheck = false;
    }
  }
  // Initializes the color of the enable button
  if (osthread->paintArea->EnableScope) {
    uiOscilloScope->scopeEnablePushButton->setText("I>");
  } else {
    uiOscilloScope->scopeEnablePushButton->setText("II");
  }
  // Initializes the pr cr sr
  uiOscilloScope->prComboBox->setCurrentIndex(1);
  uiOscilloScope->crComboBox->setCurrentIndex(2);
  uiOscilloScope->srComboBox->setCurrentIndex(2);
}

// 更新示波器显示
void OscilloScope::updatePlot() {
  uiOscilloScope->plot->replot();
  uiOscilloScope->plotSPD->replot();
  uiOscilloScope->plotPOS->replot();
}

void OscilloScope::on_tgPOSPushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->showItems[2].IsCheck == true) { // showItems[2] is tgPOS
    uiOscilloScope->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOff);
  } else {
    uiOscilloScope->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOn);
  }
  osthread->changeTgPOS();
}

void OscilloScope::on_tgSPDPushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->showItems[1].IsCheck == true) { // showItems[1] is tgSPD
    uiOscilloScope->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOff);
  } else {
    uiOscilloScope->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOn);
  }
  osthread->changeTgSPD();
}

void OscilloScope::on_tgCURPushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->showItems[0].IsCheck == true) { // showItems[0] is tgCUR
    uiOscilloScope->tgCURPushButton->setStyleSheet(tgCURPushButtonOff);
  } else {
    uiOscilloScope->tgCURPushButton->setStyleSheet(tgCURPushButtonOn);
  }
  osthread->changeTgCUR();
}

void OscilloScope::on_rlPOSPushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->showItems[5].IsCheck == true) { // showItems[5] is rlPOS
    uiOscilloScope->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOff);
  } else {
    uiOscilloScope->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOn);
  }
  osthread->changeRlPOS();
}

void OscilloScope::on_rlSPDPushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->showItems[4].IsCheck == true) { // showItems[4] is rlSPD
    uiOscilloScope->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOff);
  } else {
    uiOscilloScope->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOn);
  }
  osthread->changeRlSPD();
}

void OscilloScope::on_rlCURPushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->showItems[3].IsCheck == true) { // showItems[3] is rlCUR
    uiOscilloScope->rlCURPushButton->setStyleSheet(rlCURPushButtonOff);
  } else {
    uiOscilloScope->rlCURPushButton->setStyleSheet(rlCURPushButtonOn);
  }
  osthread->changeRlCUR();
}

void OscilloScope::on_scopeEnablePushButton_clicked()
{
  if (osthread == NULL) {
    return;
  }
  if (osthread->paintArea->EnableScope) {
    osthread->paintArea->EnableScope = false;
    uiOscilloScope->scopeEnablePushButton->setText("II");
  } else {
    osthread->paintArea->EnableScope = true;
    uiOscilloScope->scopeEnablePushButton->setText("I>");
  }
}

void OscilloScope::on_offset_POSLineEdit_editingFinished()
{
  if (osthread == NULL) {
    return;
  }
  osthread->paintArea->positionOffset = uiOscilloScope->offset_POSLineEdit->text().toDouble();
}

void OscilloScope::on_offset_SPDLineEdit_editingFinished()
{
  if (osthread == NULL) {
    return;
  }
  osthread->paintArea->speedOffset = uiOscilloScope->offset_SPDLineEdit->text().toDouble();
}

void OscilloScope::on_offset_CURLineEdit_editingFinished()
{
  if (osthread == NULL) {
    return;
  }
  osthread->paintArea->currentOffset = uiOscilloScope->offset_CURLineEdit->text().toDouble();
}

void OscilloScope::on_prComboBox_currentIndexChanged(int index)
{
  if (osthread == NULL) {
    return;
  }
  switch(index) {
    case 0:
      osthread->paintArea->pr = 10;
      break;
    case 1:
      osthread->paintArea->pr = 50;
      break;
    case 2:
      osthread->paintArea->pr = 100;
      break;
    case 3:
      osthread->paintArea->pr = 1000;
      break;
    default:
      osthread->paintArea->pr = 10;
      break;
  }
}

void OscilloScope::on_srComboBox_currentIndexChanged(int index)
{
  if (osthread == NULL) {
    return;
  }
  switch(index) {
    case 0:
      osthread->paintArea->sr = 0.5;
      break;
    case 1:
      osthread->paintArea->sr = 1.0;
      break;
    case 2:
      osthread->paintArea->sr = 2.0;
      break;
    default:
      osthread->paintArea->sr = 2.0;
      break;
  }
}

void OscilloScope::on_crComboBox_currentIndexChanged(int index)
{
  if (osthread == NULL) {
    return;
  }
  switch(index) {
    case 0:
      osthread->paintArea->cr = 20;
      break;
    case 1:
      osthread->paintArea->cr = 60;
      break;
    case 2:
      osthread->paintArea->cr = 100;
      break;
    case 3:
      osthread->paintArea->cr = 200;
      break;
    default:
      osthread->paintArea->cr = 100;
      break;
  }
}

void OscilloScope::on_ScanFrequencyComboBox_currentIndexChanged(int index)
{
  if (osthread == NULL) {
    return;
  }
  const int MaxFrequency = 10; // kHz
  osthread->paintArea->Interval = osthread->paintArea->ScanFrequency / MaxFrequency / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)
  switch(index) {
    case 0: {// 10(ms) = osthread->paintArea->Interval * OSCILLO_SCOPE_INTEVAL
        osthread->paintArea->Interval = 10 / OSCILLO_SCOPE_INTEVAL;
        osthread->paintArea->ScanFrequency = 10 * MaxFrequency;
        break;
      }
    case 1: // 50ms
      osthread->paintArea->Interval = 50 / OSCILLO_SCOPE_INTEVAL;
      osthread->paintArea->ScanFrequency = 50 * MaxFrequency;
      break;
    case 2: // 100ms
      osthread->paintArea->Interval = 100 / OSCILLO_SCOPE_INTEVAL;
      osthread->paintArea->ScanFrequency = 100 * MaxFrequency;
      break;
  }
//  uint8_t data[2] = {0,0};
//  data[1]= (uint8_t)( (osthread->paintArea->ScanFrequency & 0xff00) >> 8 );
//  data[0]= (uint8_t)( osthread->paintArea->ScanFrequency & 0xff );
    uint16_t data16 = osthread->paintArea->ScanFrequency;
    jointSetScpMask(joint, data16, 100, NULL);

//  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_WR_NR,SCP_REC_TIM,data,0x02);
//  can1->controller.delayMs(5);
}
