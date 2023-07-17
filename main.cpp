// #include "src/view/mainwindow.h"

// #include <QApplication>
#include <iostream>

#include "src/model/Translator.h"

int main(int argc, char *argv[])
{
    s21::Translator tr;
    std::cout << tr.Tokenize("(*1+3") << std::endl;
    std::cout << tr.Tokenize("1+sin-2") << std::endl;
    std::cout << tr.Tokenize("1+sin*2") << std::endl;
    std::cout << tr.Tokenize("sin()cos()") << std::endl;
    std::cout << tr.Tokenize("2x") << std::endl;
    std::cout << tr.Tokenize("x2") << std::endl;
    std::cout << tr.Tokenize(")(") << std::endl;
    std::cout << tr.Tokenize("x)cosx") << std::endl;
    std::cout << tr.Tokenize("2(") << std::endl;
    std::cout << tr.Tokenize("sin()(") << std::endl;
    std::cout << tr.Tokenize(")cos()") << std::endl;
    std::cout << tr.Tokenize("2sin") << std::endl;
    std::cout << tr.Tokenize("xsin") << std::endl;
    std::cout << tr.Tokenize("sin()x") << std::endl;
    std::cout << tr.Tokenize("sin()2") << std::endl;
    std::cout << tr.Tokenize("xx") << std::endl;
    std::cout << tr.Tokenize("22") << std::endl;
    std::cout << std::endl;
    std::cout << tr.Tokenize("sin(2") << std::endl;
    std::cout << tr.Tokenize("sin2x") << std::endl;
    std::cout << tr.Tokenize("sinxcos2") << std::endl;
    std::cout << tr.Tokenize("sin(2") << std::endl;
    std::cout << tr.Tokenize("sincos8") << std::endl;
    std::cout << tr.Tokenize(")cos()") << std::endl;
    std::cout << tr.Tokenize("sin2xxcos5x") << std::endl;
    std::cout << tr.Tokenize("sin(2xx(1+x))cos2x(1-x)") << std::endl;
    std::cout << tr.Tokenize("(1+xsinx)sincos(1+x-cosx)") << std::endl;
    std::cout << tr.Tokenize("cos(x-1)xcosx-sinxsin(1-xx)") << std::endl;
    std::cout << tr.Tokenize("cos(x-1)xcosx-sinxcossqrtsin(1-xx+sqrt(1+cossin2x))") << std::endl;
    std::cout << tr.Tokenize("cos(x-1)xcosx-sinsinxsin(1-xx)") << std::endl;
    std::cout << tr.Tokenize("cos(x-1)xcosx-tgcossinsin(xsin(1-xx))") << std::endl;

    std::cout << tr.Tokenize("2sin") << std::endl;
    std::cout << tr.Tokenize("2(") << std::endl;
    std::cout << tr.Tokenize("2x") << std::endl;
    
    std::cout << tr.Tokenize("sin2") << std::endl;
    std::cout << tr.Tokenize("sincos") << std::endl;
    std::cout << tr.Tokenize("sinx") << std::endl;
    std::cout << tr.Tokenize("(sin()x)))0))00)0") << std::endl;
    std::cout << tr.Tokenize("(sinx))") << std::endl;

    std::cout << tr.Tokenize("()") << std::endl;

    std::cout << tr.Tokenize(")2") << std::endl;
    std::cout << tr.Tokenize(")sin") << std::endl;
    std::cout << tr.Tokenize(")(") << std::endl;
    std::cout << tr.Tokenize(")x") << std::endl;

    try {
        std::cout << tr.Tokenize("++") << std::endl;
        std::cout << tr.Tokenize("(sin)x") << std::endl;
        std::cout << tr.Tokenize("(1+)x") << std::endl;
        std::cout << tr.Tokenize("sin+") << std::endl;
    } catch (s21::bad_expression& exp) {
        std::cout << "Caught expected: " << exp.what() << std::endl;
    } catch (...) {
        std::cout << "Caught unknown exception" << std::endl;
    }

    return 0;
}

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }
