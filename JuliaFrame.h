#ifndef JULIAFRAME_H
#define JULIAFRAME_H

#include <JuliaWidget.h>

#include <QtGui>

class JuliaFrame : public QMainWindow {
  Q_OBJECT
  public:
    JuliaFrame(QWidget *parent = 0);
  private:
//     QWidget *settings;
    JuliaWidget *julia;
    
    QLabel *zoomLabel;
    QSlider *zoomSlider;
    QComboBox *aaCombo;
    QComboBox *cpugpu;
    QComboBox *cpuCores;
    QComboBox *thresh;
    QComboBox *smooth;
    QDoubleSpinBox *cReal;
    QDoubleSpinBox *cImg;
    QPushButton *fit;
    QPushButton *zoomPlus;
    QPushButton *zoomMinus;
  public slots:
    void zoomChanged(int zoom);
    void cChanged(double r, double i);
  private slots:
    void changeC();
    void incrementZoom();
    void decrementZoom();
  signals:
//     void changeZoom(double zoom);
};

#endif