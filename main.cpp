// #include "src/view/mainwindow.h"

// #include <QApplication>
#include <iostream>

#include "src/model/Translator.h"

int main(int argc, char *argv[])
{
    s21::Translator tr;
    // std::cout << tr.fix("sin()cos()") << std::endl;
    // std::cout << tr.fix("2x") << std::endl;
    // std::cout << tr.fix("x2") << std::endl;
    // std::cout << tr.fix(")(") << std::endl;
    // std::cout << tr.fix("2(") << std::endl;
    // std::cout << tr.fix("sin()(") << std::endl;
    // std::cout << tr.fix(")cos()") << std::endl;
    // std::cout << tr.fix("2sin") << std::endl;
    // std::cout << tr.fix("xsin") << std::endl;
    // std::cout << tr.fix("sin()x") << std::endl;
    // std::cout << tr.fix("sin()2") << std::endl;
    // std::cout << tr.fix("xx") << std::endl;
    // std::cout << tr.fix("22") << std::endl;
    // std::cout << std::endl;
    // std::cout << tr.fix("sin(2") << std::endl;
    // std::cout << tr.fix("sin2x") << std::endl;
    // std::cout << tr.fix("sinxcos2") << std::endl;
    // std::cout << tr.fix("sin)cos(0") << std::endl;
    // std::cout << tr.fix("sin(2") << std::endl;
    // std::cout << tr.fix("sincos8") << std::endl;
    // std::cout << tr.fix(")cos()") << std::endl;

    std::cout << tr.fix("2sin") << std::endl; // add *
    std::cout << tr.fix("2(") << std::endl; // add *
    std::cout << tr.fix("2x") << std::endl; // add *
    
    std::cout << tr.fix("++") << std::endl; // check for unary
    std::cout << tr.fix("+)") << std::endl; // throw exception maybe
    
    std::cout << tr.fix("sin2") << std::endl; // add brackets 
    std::cout << tr.fix("sin+") << std::endl; // add brackets
    std::cout << tr.fix("sincos") << std::endl; // add brackets
    std::cout << tr.fix("sin)") << std::endl; // fix brackets
    std::cout << tr.fix("sinx") << std::endl; // add brackets

    std::cout << tr.fix("(+") << std::endl; // make unary
    std::cout << tr.fix("()") << std::endl; // x throw exception or fix

    std::cout << tr.fix(")2") << std::endl; // add *
    std::cout << tr.fix(")sin") << std::endl; // add *
    std::cout << tr.fix(")(") << std::endl; // add *
    std::cout << tr.fix(")x") << std::endl; // add *
    return 0;
}

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }
