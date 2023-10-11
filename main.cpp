#include <iostream>
#include <fstream>

#define EXPECTED_ARGC(argc) ((argc) == 2)
// #define EXPECTED_ARGC(argc) ((argc) == 3)

#define ERR_INVALID_ARGC        1
#define ERR_COULD_NOT_OPEN_FILE 2
#define ERR_READ_FAIL           3
#define SUCCESS                 0 

int main(int argc, char **argv)
{
    // srand(time(NULL));

    if (!EXPECTED_ARGC(argc)) {
        std::cerr << "Uso: " << argv[0]
            << " ARQUIVO_ENTRADA ARQUIVO_SAIDA\n";
        return ERR_INVALID_ARGC;
    }

    std::ifstream arqEntrada;
    // std::ofstream arqSaida;

    arqEntrada.open(argv[1]);                                              
                                                                       
    if (!arqEntrada.is_open()) {                                              
        std::cerr << argv[0]                                           
            << ": não foi possível abrir o arquivo `"                      
            << argv[1] << "`\n";                                          
        return ERR_COULD_NOT_OPEN_FILE;                                    
    }

    

    return SUCCESS;  
}
