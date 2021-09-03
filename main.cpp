#include "Analizador/Interprete.h"
#include <cstdlib>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    Interprete* interprete = new Interprete(false);
    stringstream comando;
    for (int i = 1; i < argc; ++i) {
        comando << argv[i] << " ";
    }
    interprete->SepararComando(comando.str());
    //interprete->SepararComando("exec -path=/home/daniel/Escritorio/exec/ajustes2.sh");
    return 0;
}



