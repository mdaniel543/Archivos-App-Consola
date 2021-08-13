#include "Analizador /Interprete.h"

using namespace std;

int main() {
    cout << " BIENVENIDO " << endl;
    Interprete* interprete = new Interprete(false);
    interprete->interpretar();
    return 0;
}



