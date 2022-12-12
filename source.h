#ifndef UNTITLED_SOURCE_H
#define UNTITLED_SOURCE_H

using namespace std;
#include <iostream>
#include "source.h"
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
#include <memory>
#include <exception>

template <typename T>
class Matrix
{
    size_t row;
    size_t column;
    size_t channels;

    // T *data;
    // template <typename T>
    // class shared_ptr;
    std::shared_ptr<T> *data = nullptr;

public:
    Matrix();
    Matrix(size_t row, size_t col) : row(row), column(col)
    {
        std::shared_ptr<T> *data = new shared_ptr<T>[row * col * channels * sizeof(T)] {};
    }
    Matrix operator+(const Matrix &mat);
    Matrix operator-(const Matrix &mat);
    Matrix operator=(const Matrix &mat);
    bool operator==(const Matrix &mat);
    friend std::ostream &operator<<(std::ostream &os, const Matrix &mat)
    {
        if (mat.data == NULL)
        {
            cerr << "The matrix data is not valid! \n"
                 << endl;
        }
        else
        {
            std::string str = "";
            for (size_t i = 0; i < mat.row; i++)
            {
                for (size_t j = 0; j < mat.column; j++)
                {
                    // str = str + std::to_string(mat.data[i * mat.row + j]) + "  ";
                    cout << std::left << setw(10);
                    cout.fixed;
                    cout << mat.data[i * mat.row + j] << "  ";
                }
                cout << endl;
                // str = str + "\n";
            }
            cout << endl;
            // os << str;
        }
        return os;
    }

    Matrix operator+(const T addx);

    void initialMatrix(Matrix *matrix, const size_t row, const size_t col);
    void createRamMatrix(Matrix *matrix, const size_t row, const size_t col, const size_t databound);
    // void showMatrix(const Matrix *matrix);
};
#endif // UNTITLED_SOURCE_HPP
