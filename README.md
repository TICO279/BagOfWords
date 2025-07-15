# Parallel Word Count (Bag of Words)

This project implements a **parallel bag-of-words vectorization system** in C++ using **MPI**, and compares its performance with a serial version. It includes tools for **text preprocessing**, **matrix generation**, **speedup verification**, and **result comparison**.

## Features

- **Parallel Bag-of-Words in C++**:
  - Uses **MPI** to distribute the workload of word counting across multiple processes.
  - Each process handles one input document and sends local word counts to the root.
  - Produces a consolidated matrix of word frequencies saved as `matriz_paralela.csv`.

- **Serial Version in C++**:
  - A baseline implementation that processes all input documents sequentially.
  - Outputs word count matrix as `matriz_serial.csv` for later comparison.

- **Text Preprocessing in Python**:
  - `preprocesamiento.py`: Uses `CountVectorizer` from `sklearn` to tokenize and extract vocabulary from raw documents.
  - Saves the vocabulary to `vocab.txt` and builds the base matrix for testing.

- **Debugging Tool**:
  - `debug_conteo.cpp`: Tests vocabulary matching logic on the first 50 words of a book and reports any unmatched tokens.

- **Performance Verification and Visualization**:
  - `verificar_graficar.py`: Compares output matrices from serial and parallel implementations, measures execution time, and plots a speedup bar chart.
  - Produces `grafica_speedup.png` showing performance gain.

## Setup Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/YourUsername/Parallel-BagOfWords.git
   cd Parallel-BagOfWords
   
2. **Compile Serial and Parallel Versions**:
   Make sure you have `mpic++` installed.
   ```bash
   mpic++ main_paralelo.cpp -o paralelo
   g++ main_serial.cpp -o serial
   
3. **Generate Vocabulary (Python)**:
   ```bash
   python preprocesamiento.py

4. **Run Serial Version**:
   ```bash
   ./serial libro1.txt libro2.txt libro3.txt libro4.txt libro5.txt libro6.txt

5. **Run Parallel Version (6 processes)**:
   ```bash
   mpirun -np 6 ./paralelo libro1.txt libro2.txt libro3.txt libro4.txt libro5.txt libro6.txt

6. **Verify Output and Plot Speed-Up**:
   ```bash
   python verificar_graficar.py

## Notes and Limitations

- The parallel implementation assumes the **number of input documents matches the number of processes**.
- Input files must be plain `.txt` files, pre-cleaned and UTF-8 encoded.
- Vocabulary must be generated first using `preprocesamiento.py` before running the C++ programs.
- The generated matrices are saved as CSVs and are expected to match if both versions are correct.
- `debug_conteo.cpp` is useful to manually inspect early-stage token errors when matching vocabulary.
- Execution time must be input manually into `verificar_graficar.py`, unless automated timing is added.
- This project is intended for **academic benchmarking and experimentation** rather than production use.
