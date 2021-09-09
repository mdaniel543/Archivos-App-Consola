#include "Analizador/Interprete.h"
#include <cstdlib>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    Interprete* interprete = new Interprete(false);
    stringstream comando;
    char esc_char = 27;
    for (int i = 1; i < argc; ++i) {
        string as = argv[i];
        bool bandera = false;
        for (int j = 0; j < as.length(); ++j) {
            comando << as[j];
            if (as[j] == '=') {
                comando << '\"';
                bandera = true;
            }
            if (j == as.length() - 1) {
                if (bandera) comando << '\"';
                comando << " ";
            }
        }
    }
    cout << endl << esc_char << "[3m" << comando.str() << esc_char << "[0m" << endl;
    interprete->SepararComando(comando.str());
    return 0;
}



