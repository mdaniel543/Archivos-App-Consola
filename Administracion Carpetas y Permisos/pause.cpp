//
// Created by daniel on 12/08/21.
//

#include "pause.h"

pause::pause() {
    int flag;
    string sd;
    cout << "###############################################" << endl;
    cout << "Se encontro una pausa, para continuar pulse Enter" << endl;
    cout << "###############################################" << endl;
    flag = cin.get();
    cout << "Continuando .";
    cout << ".";
    cout << ".";
    //sleep_for(500ms);
    cout << ".";
    cout << endl;
    cout << "=================================" << endl << endl ;
}
