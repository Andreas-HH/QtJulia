#ifndef JULIA_H
#define JULIA_H

#include <QtGui>
#include <JuliaMath.h>
// #include <complex.h>

class JuliaWidget : public QWidget {
  Q_OBJECT
  public:
    JuliaWidget(QWidget *parent = 0);
    virtual ~JuliaWidget();
    void setZoom(double z);
    void setC(double real, double img);
  protected:
    void paintEvent(QPaintEvent *event);
  private:
    QImage *cache;
    
    int maxWidth, maxHeight;
    int calcSmooth;
    int calcOnGPU;
    double pixelWidth;
    myComplex *screenMid;
    myComplex *screenMid_gpu;
    myComplex *c;
    myComplex *c_gpu;
    double *data;
    double *data_gpu;
    myComplex *coords_gpu;
    
    void calculate();
    void calculateOnCPU();
    void calculateOnGPU();
    void paintCache();
    void init(int width, int height);
    void clear();
  signals:
    void cChanged(double r, double i);
  public slots:
    void fit();
    void setCalcMode(int);
};

#endif