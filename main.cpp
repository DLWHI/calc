// #include "src/view/mainwindow.h"

// #include <QApplication>
#include <iostream>

#include "src/model/Translator.h"

int main(int argc, char *argv[])
{
    s21::Translator tr;
    std::cout << tr.Tokenize("sin()cos()") << std::endl;
    // std::cout << tr.Fix("2x") << std::endl;
    // std::cout << tr.Fix("x2") << std::endl;
    // std::cout << tr.Fix(")(") << std::endl;
    // std::cout << tr.Fix("x)cosx") << std::endl;
    // std::cout << tr.Fix("2(") << std::endl;
    // std::cout << tr.Fix("sin()(") << std::endl;
    // std::cout << tr.Fix(")cos()") << std::endl;
    // std::cout << tr.Fix("2sin") << std::endl;
    // std::cout << tr.Fix("xsin") << std::endl;
    // std::cout << tr.Fix("sin()x") << std::endl;
    // std::cout << tr.Fix("sin()2") << std::endl;
    // std::cout << tr.Fix("xx") << std::endl;
    // std::cout << tr.Fix("22") << std::endl;
    // std::cout << std::endl;
    // std::cout << tr.Fix("sin(2") << std::endl;
    // std::cout << tr.Fix("sin2x") << std::endl;
    // std::cout << tr.Fix("sinxcos2") << std::endl;
    // std::cout << tr.Fix("sin(2") << std::endl;
    // std::cout << tr.Fix("sincos8") << std::endl;
    // std::cout << tr.Fix(")cos()") << std::endl;
    // std::cout << tr.Fix("sin2xxcos5x") << std::endl;
    // std::cout << tr.Fix("sin(2xx(1+x))cos2x(1-x)") << std::endl;
    // std::cout << tr.Fix("(1+xsinx)sincos(1+x-cosx)") << std::endl;
    // std::cout << tr.Fix("cos(x-1)xcosx-sinxsin(1-xx)") << std::endl;

    // std::cout << tr.Fix("2sin") << std::endl;
    // std::cout << tr.Fix("2(") << std::endl;
    // std::cout << tr.Fix("2x") << std::endl;
    
    // std::cout << tr.Fix("++") << std::endl;
    
    // std::cout << tr.Fix("sin2") << std::endl;
    // std::cout << tr.Fix("sin+") << std::endl;
    // std::cout << tr.Fix("sincos") << std::endl;
    // std::cout << tr.Fix("sinx") << std::endl;
    // std::cout << tr.Fix("(sin()x)))0))00)0") << std::endl;
    // std::cout << tr.Fix("(sin)x") << std::endl;
    // std::cout << tr.Fix("(1+)x") << std::endl;
    // std::cout << tr.Fix("(sinx))") << std::endl;

    // std::cout << tr.Fix("()") << std::endl;

    // std::cout << tr.Fix(")2") << std::endl;
    // std::cout << tr.Fix(")sin") << std::endl;
    // std::cout << tr.Fix(")(") << std::endl;
    // std::cout << tr.Fix(")x") << std::endl;
    return 0;
}

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }
