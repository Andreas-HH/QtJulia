#include <JuliaMath.h>

int juliaValueSharp(myComplex *x, myComplex *c, int threshold) {
  int i;
  
  for (i = 0; i < threshold; i++) {
    multiplyMyC(x, x);
    addMyC(x, c);
    
    if (norm2(x) > 1000.) {
      return 0;
    }
  }
  
  return 1;
}

double juliaValueSmooth(myComplex *x, myComplex *c, int threshold) {
  int i;
  
//   return 0.5;
  
  for (i = 0; i < threshold; i++) {
    multiplyMyC(x, x);
    addMyC(x, c);
    
    if (norm2(x) > 1000.) {
      break;
    }
  }
  
//   printf("returning 1!\n");
  return ((double) i)/((double) threshold);
}

myComplex* createMyComplex(double r, double i) {
  myComplex *a = (myComplex*) malloc(sizeof(myComplex));
  
  a->real = r;
  a->img = i;
  
  return a;
}

double norm2(myComplex *x) { 
  return x->real * x->real + x->img * x->img; 
}

void multiplyMyC(myComplex *x, myComplex *a) {
  double xReal = x->real;
  double xImg  = x->img;
  double aReal = a->real;
  double aImg  = a->img;
  
  x->real = xReal*aReal - xImg*aImg;
  x->img  = xImg*aReal + xReal*aImg;
}

void addMyC(myComplex *x, myComplex *a) {
  x->real = x->real + a->real;
  x->img  = x->img + a->img;
}

/*myComplex operator*(myComplex a, myComplex b) {
  return *createMyComplex(a.real*b.real - a.img*b.img, a.img*b.real + a.real*b.img);
}
  
myComplex operator+(myComplex a, myComplex b) {
  return *createMyComplex(a.real+b.real, a.img+b.img);
}*/

