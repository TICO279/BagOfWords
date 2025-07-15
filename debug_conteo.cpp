// debug_conteo.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<string> leer_vocabulario(const string& filename) {
    vector<string> vocabulario;
    ifstream file(filename);
    string palabra;
    while (getline(file, palabra)) {
        vocabulario.push_back(palabra);
    }
    return vocabulario;
}

// Función de limpieza robusta: solo letras minúsculas
string limpiar(const string& palabra) {
    string limpia;
    for (char c : palabra) {
        if (isalpha(c)) limpia += tolower(c);
    }
    return limpia;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " vocabulario.txt libro.txt" << endl;
        return 1;
    }

    vector<string> vocabulario = leer_vocabulario(argv[1]);
    map<string, int> conteo;
    for (const auto& palabra : vocabulario) conteo[palabra] = 0;

    ifstream libro(argv[2]);
    if (!libro) {
        cerr << "Error abriendo libro: " << argv[2] << endl;
        return 1;
    }

    string palabra;
    int total = 0;
    int encontradas = 0;

    while (libro >> palabra) {
        palabra = limpiar(palabra);
        total++;
        if (conteo.find(palabra) != conteo.end()) {
            conteo[palabra]++;
            encontradas++;
        } else {
            cout << "NO encontrada: [" << palabra << "]\n";
        }
        if (total >= 50) break; // solo primeras 50 palabras para debug
    }

    cout << "Palabras procesadas: " << total << endl;
    cout << "Coincidencias con vocabulario: " << encontradas << endl;

    return 0;
}
