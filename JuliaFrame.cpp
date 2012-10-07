#include <JuliaFrame.h>
#include <math.h>
#include <stdio.h>
// #include <sstream>
// #include <string>

int thresholds[] = {200, 500, 1000, 2000, 5000, 10000};

JuliaFrame::JuliaFrame(QWidget *parent) : QMainWindow(parent) {
//   int i;
  
//   QGridLayout *settingsBox = new QGridLayout();
  QLabel *zooml = new QLabel(tr("Zoom:"));
  QLabel *aal   = new QLabel(tr("AA ="));
  QLabel *cl1   = new QLabel(tr("c ="));
  QLabel *cl2   = new QLabel(tr("+ i *"));
  QLabel *core  = new QLabel(tr("# of Cores ="));
  QLabel *trs   = new QLabel(tr("threshold ="));

//   settings = new QWidget();
  julia = new JuliaWidget(this);
  
  aaCombo = new QComboBox();
  cpugpu  = new QComboBox();
  cpuCores = new QComboBox();
  thresh = new QComboBox();
  smooth = new QComboBox();
  zoomSlider = new QSlider(Qt::Horizontal);
  cReal = new QDoubleSpinBox();
  cImg = new QDoubleSpinBox();
  zoomPlus = new QPushButton(tr("+"));
  zoomMinus = new QPushButton(tr("-"));
  fit = new QPushButton(tr("fit"));

  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomChanged(int)));
  connect(cReal, SIGNAL(valueChanged(double)), this, SLOT(changeC()));
  connect(cImg, SIGNAL(valueChanged(double)), this, SLOT(changeC()));
  connect(julia, SIGNAL(cChanged(double, double)), this, SLOT(cChanged(double, double)));
  connect(fit, SIGNAL(clicked(bool)), julia, SLOT(fit()));
  connect(zoomPlus, SIGNAL(clicked(bool)), this, SLOT(incrementZoom()));
  connect(zoomMinus, SIGNAL(clicked(bool)), this, SLOT(decrementZoom()));
//   QObject::connect(smooth, SIGNAL(
  
  aaCombo->addItem(tr("off"));
  aaCombo->addItem(tr("2x"));
  aaCombo->addItem(tr("4x"));
  aaCombo->addItem(tr("8x"));
  aaCombo->addItem(tr("16x"));
  aaCombo->addItem(tr("32x"));
  aaCombo->addItem(tr("64x"));
  aaCombo->addItem(tr("128x"));
  cpugpu->addItem(tr("CPU"));
  cpugpu->addItem(tr("GPU"));
  cpuCores->addItem(tr("1"));
  cpuCores->addItem(tr("2"));
  cpuCores->addItem(tr("3"));
  cpuCores->addItem(tr("4"));
  cpuCores->addItem(tr("6"));
  cpuCores->addItem(tr("8"));
  cpuCores->addItem(tr("12"));
  cpuCores->addItem(tr("50"));
  cpuCores->addItem(tr("200"));
  thresh->addItem(tr("200"));
  thresh->addItem(tr("500"));
  thresh->addItem(tr("1000"));
  thresh->addItem(tr("2000"));
  thresh->addItem(tr("5000"));
  thresh->addItem(tr("10000"));
  smooth->addItem(tr("Sharp"));
  smooth->addItem(tr("Smooth"));
  zoomSlider->setRange(0,250);
  zoomSlider->setTickInterval(1);
  zoomSlider->setTickPosition(QSlider::NoTicks);
  zoomSlider->setValue(0);
  zoomChanged(0);
  cReal->setDecimals(4);
  cReal->setRange(-5., 5.);
  cReal->setValue(-0.8);
  cReal->setSingleStep(0.0005);
  cImg->setDecimals(4);
  cImg->setRange(-5., 5.);
  cImg->setValue(0.156);
  cImg->setSingleStep(0.0005);
  
  /*settingsBox->addWidget(zooml, 0, 0);
  settingsBox->addWidget(zoomMinus, 0, 1);
  settingsBox->addWidget(zoomSlider, 0, 2);
  settingsBox->addWidget(zoomPlus, 0, 3);
  settingsBox->addWidget(fit, 0, 4);
  settingsBox->addWidget(aal, 0, 5);
  settingsBox->addWidget(aaCombo, 0, 6);
  settingsBox->addWidget(cl1, 0, 7);
  settingsBox->addWidget(cReal, 0, 8);
  settingsBox->addWidget(cl2, 0, 9);
  settingsBox->addWidget(cImg, 0, 10);
  settingsBox->addWidget(trs, 0, 11);
  settingsBox->addWidget(thresh, 0, 12);
  settingsBox->addWidget(smooth, 0, 13);
  settingsBox->addWidget(cpugpu, 0, 14);
  settingsBox->addWidget(core, 0, 15);
  settingsBox->addWidget(cpuCores, 0, 16);
  settings->setLayout(settingsBox);*/
  
  setCentralWidget(julia);
//   statusBar()->addPermanentWidget(settings);
  statusBar()->addPermanentWidget(zooml);
  statusBar()->addPermanentWidget(zoomMinus);
  statusBar()->addPermanentWidget(zoomSlider, 1);
  statusBar()->addPermanentWidget(zoomPlus);
  statusBar()->addPermanentWidget(fit);
  statusBar()->addPermanentWidget(aal);
  statusBar()->addPermanentWidget(aaCombo);
  statusBar()->addPermanentWidget(cl1);
  statusBar()->addPermanentWidget(cReal);
  statusBar()->addPermanentWidget(cl2);
  statusBar()->addPermanentWidget(cImg);
  statusBar()->addPermanentWidget(trs);
  statusBar()->addPermanentWidget(thresh);
  statusBar()->addPermanentWidget(smooth);
  statusBar()->addPermanentWidget(cpugpu);
  statusBar()->addPermanentWidget(core);
  statusBar()->addPermanentWidget(cpuCores);
  
  setWindowTitle(tr("Julia"));  
}

void JuliaFrame::zoomChanged(int zoom) {
  double zoomValue = exp(-0.1 * (zoom+50));
  julia->setZoom(zoomValue);
}

void JuliaFrame::changeC() {
//   printf("changing c... (%f, %f)\n", cReal->value(), cImg->value());
  julia->setC(cReal->value(), cImg->value());
}

void JuliaFrame::cChanged(double r, double i) {
//   printf("cChanged: (%f, %f)\n", r, i);
  cReal->setValue(r);
  cImg->setValue(i);
}

void JuliaFrame::incrementZoom() {
  zoomSlider->setValue(zoomSlider->value() + 1);
}

void JuliaFrame::decrementZoom() {
  zoomSlider->setValue(zoomSlider->value() - 1);
}

