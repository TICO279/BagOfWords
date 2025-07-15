// === SERIAL VERSION ===
// Archivo: serial_bolsa.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <chrono>

// Función que toma un texto y lo divide en palabras minúsculas
std::vector<std::string> tokenizar(const std::string& texto) {
    std::vector<std::string> tokens;
    std::string palabra;
    for (char c : texto) {
        if (std::isalpha(c)) palabra += std::tolower(c);  // Solo se consideran letras
        else if (!palabra.empty()) {
            tokens.push_back(palabra);  // Agrega palabra formada
            palabra.clear();
        }
    }
    if (!palabra.empty()) tokens.push_back(palabra);  // Agrega la última palabra si aplica
    return tokens;
}

int main(int argc, char* argv[]) {
    // Validar que se pasen libros como argumentos
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " libro1.txt libro2.txt ... libroN.txt\n";
        return 1;
    }

    // Inicia cronómetro
    auto start = std::chrono::high_resolution_clock::now();

    // === Paso 1: Leer vocabulario ===
    std::ifstream vocabFile("vocabulario.txt");
    std::vector<std::string> vocabulario;
    std::unordered_map<std::string, int> idx_map;  // Mapa palabra -> índice de columna
    std::string palabra;
    while (std::getline(vocabFile, palabra)) {
        idx_map[palabra] = vocabulario.size();  // Asigna índice
        vocabulario.push_back(palabra);
    }

    // === Paso 2: Leer libros desde argumentos de consola ===
    std::vector<std::string> libros;
    for (int i = 1; i < argc; ++i) {
        libros.push_back(argv[i]);
    }

    // Inicializa matriz de conteo (filas = libros, columnas = palabras)
    std::vector<std::vector<int>> matriz(libros.size(), std::vector<int>(vocabulario.size(), 0));

    // Procesar cada libro
    for (size_t i = 0; i < libros.size(); ++i) {
        std::ifstream file(libros[i]);
        if (!file) {
            std::cerr << "Error opening file: " << libros[i] << "\n";
            continue;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();  // Cargar contenido del archivo completo
        auto tokens = tokenizar(buffer.str());  // Tokenizar texto

        // Contar ocurrencias por palabra
        for (const auto& tok : tokens) {
            auto it = idx_map.find(tok);
            if (it != idx_map.end()) matriz[i][it->second]++;
        }
    }

    // === Paso 3: Guardar resultados en CSV ===
    std::ofstream out("matriz_serial.csv");

    // Escribir encabezados
    out << "libro";
    for (const auto& w : vocabulario) out << "," << w;
    out << "\n";

    // Escribir filas con nombre del libro y conteos
    for (size_t i = 0; i < libros.size(); ++i) {
        out << libros[i];
        for (int val : matriz[i]) out << "," << val;
        out << "\n";
    }

    // Calcular y mostrar tiempo de ejecución
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo de ejecución (serial): " << duration.count() << " segundos.\n";

    return 0;
}
