#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <chrono>

// Función para limpiar y tokenizar el texto en palabras minúsculas
std::vector<std::string> tokenizar(const std::string& texto) {
    std::vector<std::string> tokens;
    std::string palabra;
    for (char c : texto) {
        if (std::isalpha(c)) {
            palabra += std::tolower(c);  // Solo letras, convierte a minúsculas
        } else if (!palabra.empty()) {
            tokens.push_back(palabra);  // Agrega palabra completa al vector
            palabra.clear();
        }
    }
    if (!palabra.empty()) {
        tokens.push_back(palabra);  // Agrega la última palabra si quedó algo pendiente
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    // Inicializa MPI
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // ID del proceso actual
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Número total de procesos

    auto inicio = std::chrono::high_resolution_clock::now();  // Inicio del cronómetro

    // Validar que el número de libros coincida con el número de procesos
    if (argc - 1 != size) {
        if (rank == 0) {
            std::cerr << "Usage: mpirun -np N ./program libro1.txt libro2.txt ... libroN.txt\n";
            std::cerr << "You provided " << argc - 1 << " books and started " << size << " processes.\n";
        }
        MPI_Finalize();
        return 1;
    }

    // === Paso 1: Leer vocabulario desde archivo ===
    std::ifstream vocabFile("vocabulario.txt");
    std::vector<std::string> vocabulario;
    std::unordered_map<std::string, int> palabra_a_columna;  // Mapa: palabra -> índice en la matriz
    std::string palabra;
    while (std::getline(vocabFile, palabra)) {
        vocabulario.push_back(palabra);
        palabra_a_columna[palabra] = vocabulario.size() - 1;
    }

    int V = vocabulario.size();  // Tamaño del vocabulario
    std::vector<int> conteo_local(V, 0);  // Vector local para contar palabras

    // === Paso 2: Leer y procesar el archivo asignado a este proceso ===
    std::ifstream file(argv[rank + 1]);  // argv[0] es el nombre del programa
    if (!file) {
        std::cerr << "Error opening file: " << argv[rank + 1] << "\n";
        MPI_Finalize();
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // Carga el contenido completo del archivo
    std::vector<std::string> tokens = tokenizar(buffer.str());  // Tokeniza el contenido

    // Cuenta ocurrencias de cada palabra del vocabulario
    for (const auto& token : tokens) {
        auto it = palabra_a_columna.find(token);
        if (it != palabra_a_columna.end()) {
            conteo_local[it->second]++;
        }
    }

    // === Paso 3: Enviar conteos al proceso 0 ===
    std::vector<int> matriz_total;
    if (rank == 0) {
        matriz_total.resize(V * size);  // Matriz final: libros x palabras
    }

    // Reunir conteos desde todos los procesos en el proceso 0
    MPI_Gather(conteo_local.data(), V, MPI_INT,
               matriz_total.data(), V, MPI_INT,
               0, MPI_COMM_WORLD);

    // === Paso 4: Proceso 0 escribe el CSV y mide tiempo ===
    if (rank == 0) {
        std::ofstream salida("matriz_paralela.csv");

        // Escribir encabezados del CSV
        salida << "libro";
        for (const auto& w : vocabulario) {
            salida << "," << w;
        }
        salida << "\n";

        // Escribir los conteos por libro
        for (int i = 0; i < size; ++i) {
            salida << argv[i + 1];  // Nombre del libro
            for (int j = 0; j < V; ++j) {
                salida << "," << matriz_total[i * V + j];
            }
            salida << "\n";
        }

        // Medir tiempo de ejecución
        auto fin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracion = fin - inicio;
        std::cout << "Archivo 'matriz_paralela.csv' generado correctamente.\n";
        std::cout << "Tiempo de ejecución (paralelo): " << duracion.count() << " segundos.\n";
    }

    // Finaliza MPI
    MPI_Finalize();
    return 0;
}
