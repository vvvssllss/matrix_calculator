#include "matrix.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <locale>
#include <stdexcept>
#include <new>

// Создание матрицы

/**
 * @brief Создаёт матрицу заданного размера и заполняет её нулями.
 *
 * @param rows Количество строк.
 * @param cols Количество столбцов.
 * @return Matrix Созданная матрица.
 * @throws std::invalid_argument Если размеры неположительные.
 * @throws std::bad_alloc Если не удалось выделить память.
 */
Matrix create_matrix(int rows, int cols) {
    // 1. Проверка корректности размеров
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive integers");
    }

    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;

    // 2. Выделение памяти для массива указателей
    mat.data = new double*[rows];

    if (mat.data == nullptr) {
        throw std::bad_alloc();
    }

    // 3. Выделение памяти для каждой строки
    for (int i = 0; i < rows; i++) {
        mat.data[i] = new double[cols];

        if (mat.data[i] == nullptr) {
            // Освобождение уже выделенной памяти при ошибке
            for (int j = 0; j < i; j++) {
                delete[] mat.data[j];
            }

            delete[] mat.data;
            throw std::bad_alloc();
        }

        // 4. Инициализация нулями
        for (int j = 0; j < cols; j++) {
            mat.data[i][j] = 0.0;
        }
    }

    return mat;
}


// Освобождение памяти матрицы

/**
 * @brief Освобождает динамическую память матрицы.
 *
 * @param m Матрица, память которой необходимо освободить.
 */
void free_matrix(Matrix m) {
    // 1. Проверка на nullptr - защита от двойного освобождения
    if (m.data == nullptr) {
        return;
    }

    // 2. Освобождение памяти каждой строки
    for (int i = 0; i < m.rows; i++) {
        if (m.data[i] != nullptr) {
            delete[] m.data[i];  // Освобождаем строку
            m.data[i] = nullptr; // Обнуляем указатель
        }
    }

    // 3. Освобождение массива указателей
    delete[] m.data;

    // 4. Обнуление структуры (защита от use-after-free)
    m.data = nullptr;
    m.rows = 0;
    m.cols = 0;
}


// Сложение матриц

/**
 * @brief Выполняет поэлементное сложение двух матриц.
 *
 * @param a Первая матрица.
 * @param b Вторая матрица.
 * @return Matrix Новая матрица, содержащая сумму элементов a и b.
 * @throws std::invalid_argument Если размеры матриц не совпадают.
 */
Matrix matrix_add(Matrix a, Matrix b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }

    Matrix result = create_matrix(a.rows, a.cols);

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }

    return result;
}


/**
 * @brief Выполняет умножение двух матриц.
 *
 * @param a Первая матрица.
 * @param b Вторая матрица.
 * @return Matrix Новая матрица, содержащая произведение a и b.
 * @throws std::invalid_argument Если размеры матриц несовместимы.
 */
Matrix matrix_multiply(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        throw std::invalid_argument("Matrix dimensions are incompatible for multiplication");
    }

    Matrix result = create_matrix(a.rows, b.cols);

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < b.cols; ++j) {
            double sum = 0.0;

            for (int k = 0; k < a.cols; ++k) {
                sum += a.data[i][k] * b.data[k][j];
            }

            result.data[i][j] = sum;
        }
    }

    return result;
}


#include <iostream>
#include <iomanip>
#include <algorithm>

/**
 * @brief Выводит матрицу в консоль.
 *
 * Элементы выводятся с точностью до двух знаков после запятой.
 * Для больших матриц вывод сокращается.
 *
 * @param m Матрица, которую необходимо вывести.
 */
void print_matrix(Matrix m) {
    // 1. Проверка валидности матрицы
    if (m.data == nullptr || m.rows <= 0 || m.cols <= 0) {
        std::cout << "[Empty matrix]" << std::endl;
        return;
    }

    // 2. Настройка формата вывода
    std::cout << std::fixed << std::setprecision(2);

    // 3. Проверка необходимости сокращения вывода
    const int MAX_ROWS_TO_SHOW = 10;
    const int MAX_COLS_TO_SHOW = 8;

    bool show_summary = (m.rows > MAX_ROWS_TO_SHOW ||
                         m.cols > MAX_COLS_TO_SHOW);

    int rows_to_show = show_summary ? MAX_ROWS_TO_SHOW : m.rows;
    int cols_to_show = show_summary ? MAX_COLS_TO_SHOW : m.cols;

    // 4. Вывод матрицы
    for (int i = 0; i < rows_to_show; i++) {
        // Пропуск средних строк для больших матриц
        if (show_summary &&
            i == MAX_ROWS_TO_SHOW / 2 &&
            m.rows > MAX_ROWS_TO_SHOW) {

            std::cout << "[...";

            for (int j = 0; j < cols_to_show - 2; j++) {
                std::cout << " ";
            }

            std::cout << "...]" << std::endl;
            continue;
        }

        std::cout << "[";

        // Вывод элементов строки
        for (int j = 0; j < cols_to_show; j++) {
            // Пропуск средних столбцов для больших матриц
            if (show_summary &&
                j == MAX_COLS_TO_SHOW / 2 &&
                m.cols > MAX_COLS_TO_SHOW) {

                std::cout << " ... ";
                continue;
            }

            std::cout << std::setw(6) << m.data[i][j];

            if (j < cols_to_show - 1) {
                std::cout << " ";
            }
        }

        std::cout << "]";

        // Добавление многоточия, если пропущены столбцы
        if (show_summary && m.cols > MAX_COLS_TO_SHOW) {
            std::cout << " ...";
        }

        std::cout << std::endl;
    }

    // 5. Добавление многоточия, если пропущены строки
    if (show_summary && m.rows > MAX_ROWS_TO_SHOW) {
        std::cout << "[...";

        for (int j = 0; j < std::min(cols_to_show, m.cols); j++) {
            std::cout << " ... ";
        }

        std::cout << "...]" << std::endl;
    }

    // 6. Информация о размере, если матрица большая
    if (show_summary) {
        std::cout << "Matrix " << m.rows << "x" << m.cols
                  << " (showing first " << rows_to_show
                  << "x" << cols_to_show << ")"
                  << std::endl;
    }
}


/**
 * @brief Создаёт матрицу из одномерного массива.
 *
 * @param data Указатель на одномерный массив данных.
 * @param rows Количество строк матрицы.
 * @param cols Количество столбцов матрицы.
 * @return Matrix Новая матрица, заполненная элементами массива.
 * @throws std::invalid_argument Если указатель data равен nullptr
 *         или размеры матрицы некорректны.
 */
Matrix matrix_from_array(double* data, int rows, int cols) {
    // 1. Проверка корректности размеров
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument(
            "Matrix dimensions must be positive integers"
        );
    }

    // 2. Проверка указателя на массив
    if (data == nullptr) {
        throw std::invalid_argument(
            "Input array must not be null"
        );
    }

    // 3. Создание матрицы нужного размера
    Matrix result = create_matrix(rows, cols);

    // 4. Заполнение матрицы элементами массива
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.data[i][j] = data[i * cols + j];
        }
    }

    return result;
}


/**
 * @brief Экспортирует матрицу в CSV-файл.
 *
 * @param m Матрица для экспорта.
 * @param filename Имя файла для сохранения.
 * @return void
 * @throws std::invalid_argument Если имя файла некорректно.
 * @throws std::runtime_error Если не удалось открыть или записать файл.
 */
void matrix_to_csv(Matrix m, const char* filename) {
    if (filename == nullptr || filename[0] == '\0') {
        throw std::invalid_argument("Invalid filename");
    }

    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file");
    }

    file.imbue(std::locale::classic());
    file << std::setprecision(17);

    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            file << m.data[i][j];

            if (j < m.cols - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    if (!file) {
        throw std::runtime_error("Failed to write CSV file");
    }

    file.close();

    if (file.fail()) {
        throw std::runtime_error("Failed to close CSV file");
    }
}
