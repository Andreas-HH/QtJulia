#ifndef JULIAMATH_H
#define JULIAMATHE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct myComplex {
  double real;
  double img;
} myComplex;

myComplex* createMyComplex(double r, double i);
// myComplex* createMyComplexOnGPU(double r, double i);
double norm2(myComplex *x);
void multiplyMyC(myComplex *x, myComplex *a); // x = x * a
void addMyC(myComplex *x, myComplex *a); // x = x + a
// myComplex operator*(myComplex a, myComplex b);
// myComplex operator+(myComplex a, myComplex b);

int juliaValueSharp(myComplex *x, myComplex *c, int threshold);
double juliaValueSmooth(myComplex *x, myComplex *c, int threshold);

extern "C" {
  void juliaValueGPU(myComplex* coords, double* data, myComplex* c, int width, int height, int threshold);
  void downloadData(double *data_gpu, double *data_cpu, int N);
  void uploadC(myComplex* c_gpu, myComplex* c_cpu);
  
//   myComplex* createEmptyMyComplexOnGPU();
  myComplex* createMyComplexOnGPU(myComplex *value_host);
  
  double* initDataGPU(int width, int height);
  myComplex* initCoordsGPU(int width, int height, myComplex* screenMid_host, double pixelW);
  
  void clearData(double *data);
  void clearCoords(myComplex* coords);
}

#endif