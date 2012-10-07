#include <JuliaFrame.h>
#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  
  JuliaFrame frame;
  
  frame.show();
  
  return app.exec();
}