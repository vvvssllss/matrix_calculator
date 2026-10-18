
#ifndef MATRIX_H
#define MATRIX_H

/**
 * @file matrix.h
 * @brief Библиотека для работы с матрицами.
 * @author Платонова В. Р.
 * @version 0.1
 */

// Структура матрицы
/**
 * @brief Структура матрицы.
 *
 * Хранит двумерный динамический массив и его размеры.
 * @warning Память освобождается через free_matrix().
 */
typedef struct {
    double** data; ///< Указатель на двумерный массив.
    int rows;      ///< Количество строк.
    int cols;      ///< Количество столбцов.
} Matrix;

// Создание и освобождение памяти
/**
 * @brief Создание матрицы заданного размера.
 *
 * @param rows Количество строк (> 0).
 * @param cols Количество столбцов (> 0).
 * @return Matrix Инициализированная нулями матрица.
 *
 * @exception std::invalid_argument Если rows <= 0 или cols <= 0.
 * @exception std::bad_alloc Если не удалось выделить память.
 *
 * @note Время выполнения: O(rows * cols).
 * @see free_matrix()
 *
 * @code
 * Matrix A = create_matrix(3, 3);
 * @endcode
 */
Matrix create_matrix(int rows, int cols);

void free_matrix(Matrix m);

// Базовые операции
Matrix matrix_add(Matrix a, Matrix b);

/**
 * @brief Умножение двух матриц.
 *
 * @param a Матрица M×N.
 * @param b Матрица N×K.
 * @return Matrix Результат M×K.
 *
 * @exception std::invalid_argument Если cols(a) != rows(b).
 *
 * @note Время выполнения: O(M*N*K).
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_multiply(Matrix a, Matrix b);

// Вспомогательные функции
void print_matrix(Matrix m);
Matrix matrix_from_array(double* data, int rows, int cols);

// Индивидуальное задание №23: экспорт матрицы в CSV
void matrix_to_csv(Matrix m, const char* filename);

#endif
