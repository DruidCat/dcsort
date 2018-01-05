#include "dcsort.h"
#include <QApplication>

int main(int argc, char* argv[]){
    QApplication a(argc, argv);
    DCSort w;
    w.show();

    return a.exec();
}
