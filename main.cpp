#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#define EXPECTED_ARGC(argc) ((argc) == 2)
// #define EXPECTED_ARGC(argc) ((argc) == 3)

#define ERR_INVALID_ARGC        1
#define ERR_COULD_NOT_OPEN_FILE 2
#define ERR_READ_FAIL           3
#define SUCCESS                 0 

std::vector <float> separaLinha(std::string linha){
    std::istringstream stream(linha);
    std::string palavra;
    std::vector <float> numeros;

    while (stream >> palavra) {
        char* end;
        float numero = strtof(palavra.c_str(), &end);

        if (*end == '\0') {
            numeros.push_back(numero);
        } else {
            std::cerr << "Erro ao converter a string em float: " << palavra << std::endl;
        }
    }

    return numeros;
}

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

    std::string linha;

    float n = 0;
    float h = 0;
    float d = 0;
    float t_max = 0;
    std::vector <float> t_d;
    std::vector <float> start_hotel;
    std::vector <float> end_hotel;
    std::vector <std::vector <float>> extra_hotel;
    std::vector <std::vector <float>> vertices;

    int ultimo_hotel_extra = 6 + h - 1;

    int i = -1;
    std::vector <float> numeros;

    while (std::getline(arqEntrada, linha)) {
        if (linha == "---------------------------------------------------") {
            continue; 
        }

        i += 1;
        numeros = separaLinha(linha);

        if (i == 0){
            n = numeros.at(0);
            h = numeros.at(1);
            d = numeros.at(2);
            continue;
        }
        if (i == 1){
            t_max = numeros.at(0);
            continue;
        }
        if (i == 2){
            t_d = numeros;
            continue;
        }
        if (i == 3){
            continue;
        }
        if (i == 4){
            start_hotel = numeros;
            continue;
        }
        if (i == 5){
            end_hotel = numeros;
            continue;
        }
        if (i >= 6 && i <= 6 + h - 1){
            extra_hotel.push_back(numeros);
            continue;
        }

        vertices.push_back(numeros);
    }

    std::cout << "N: " << n << " H: " << h << " D: "<< d << " T_Max: " << t_max << std::endl;

    std::cout << "T_d: " << std::endl;

    for (int i = 0; i < t_d.size(); i++) {
        std::cout << t_d[i] << " ";
    }

    std::cout << std::endl << "Starting_Hotel: " << std::endl;

    for (int i = 0; i < start_hotel.size(); i++) {
        std::cout << start_hotel[i] << " ";
    }

   std::cout << std::endl << "Ending_Hotel: " << std::endl;

    for (int i = 0; i < end_hotel.size(); i++) {
        std::cout << end_hotel[i] << " ";
    }

    std::cout << std::endl <<  "Extra_Hotel: " << std::endl;

    for (const std::vector<float>& linha : extra_hotel) {
        for (float elemento : linha) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Vertices: " << std::endl;

    for (const std::vector<float>& linha : vertices) {
        for (float elemento : linha) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }
    
    arqEntrada.close();

    return SUCCESS;  
}
