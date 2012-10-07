#include <JuliaWidget.h>
#include <stdio.h>
#include <QtGui/qimage.h>

JuliaWidget::JuliaWidget(QWidget *parent) : QWidget(parent) {
  c = createMyComplex(0., 0.);
  screenMid = createMyComplex(0., 0.);
  pixelWidth = 0.002;
  calcOnGPU = 0;
  
  c_gpu = createMyComplexOnGPU(c);
  screenMid_gpu = createMyComplexOnGPU(screenMid);
  
  init(1600, 900);
}

JuliaWidget::~JuliaWidget() {
  clear();
}

void JuliaWidget::init(int width, int height) {
  printf("init(%i, %i)\n", width, height);
  maxWidth  = width;
  maxHeight = height;
  
//   free(data);
  data = (double*) malloc(maxWidth * maxHeight * sizeof(double));
  cache = new QImage(maxWidth, maxHeight, QImage::Format_ARGB32_Premultiplied);
  
  data_gpu = initDataGPU(maxWidth, maxHeight);
  coords_gpu = initCoordsGPU(maxWidth, maxHeight, screenMid, pixelWidth);
}

void JuliaWidget::clear() {
  printf("clear()\n");
  free(data);
  clearData(data_gpu);
  clearCoords(coords_gpu);
}



void JuliaWidget::paintEvent(QPaintEvent *) {
//   int x, y;
  QPainter painter;
  painter.begin(this);

  painter.drawImage(0, 0, *cache);
  
  painter.end();
}

void JuliaWidget::paintCache() {
  int x, y;
//   QBrush *brush = new QBrush(Qt::lightGray);
  QPainter painter;
  QColor *bg = new QColor(0, 0, 0, 0);
  painter.begin(cache);

  painter.setOpacity(0.);
  cache->fill(bg->rgba());
//   painter.setBrush(*brush);
//   painter.fillRect(0, 0, maxWidth, maxHeight, *brush);
  painter.setPen(Qt::darkRed);
  
  for (x = 0; x < maxWidth; x++) {
    for (y = 0; y < maxHeight; y++) {
//       if (data[y*maxWidth + x] > 0.) {
	painter.setOpacity(data[y*maxWidth + x]);
	painter.drawPoint(x,y);
//       }
    }
  }

  painter.end();
  update();
}


void JuliaWidget::calculateOnCPU() {
  int x, y;
  double topLeftX = screenMid->real - (maxWidth/2.)*pixelWidth;
  double topLeftY = screenMid->img + (maxHeight/2.)*pixelWidth;
  myComplex *current_point = createMyComplex(topLeftX, topLeftY);
  myComplex *tmp_point = createMyComplex(1., 1.);
  
//   juliaValueSmoothGPU();
  
  for (x = 0; x < maxWidth; x++) {
    for (y = 0; y < maxHeight; y++) {
      tmp_point->real = current_point->real;
      tmp_point->img = current_point->img;
//       tmp_point = current_point;
//       data[y*maxWidth + x] = (double) juliaValueSharp(tmp_point, c, 200);
      data[y*maxWidth + x] = juliaValueSmooth(tmp_point, c, 1000);
      current_point->img -= pixelWidth;
    }
    current_point->img = topLeftY;
    current_point->real += pixelWidth;
  }
  free(current_point);
  free(tmp_point);
  paintCache();
//   update();
//   printf("end: (%f, %f), %f, c = (%f, %f)\n", current_point->real, current_point->img, pixelWidth, c->real, c->img);
}

void JuliaWidget::calculateOnGPU() {
//   printf("JuliaWidget: berechne auf GPU!\n");
  juliaValueGPU(coords_gpu, data_gpu, c_gpu, maxWidth, maxHeight, 1000);
  downloadData(data_gpu, data, maxWidth*maxHeight);
  paintCache();
}


void JuliaWidget::calculate() {
  if (calcOnGPU) {
    calculateOnGPU();
  } else {
    calculateOnCPU();
  }
}


void JuliaWidget::setC(double r, double i) {
//   printf("%f\n", c->real);
  c->real = r;
  c->img = i;
//   printf("c_cpu = (%f. %f) %i\n", c->real, c->img, sizeof(myComplex));
  uploadC(c_gpu, c);
  calculate();
//   paintCache();
//   update();
  cChanged(r, i);
}

void JuliaWidget::setZoom(double pixelW) {
  printf("new pixelWidth: %e\n", pixelW);
  pixelWidth = pixelW;
  calculate();
//   update();
}

void JuliaWidget::fit() {
  printf("fit\n");
  clear();
  init(width(), height());
  calculate();
//   update();
}

void JuliaWidget::setCalcMode(int mode) {
  calcSmooth = mode;
}

