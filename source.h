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
    int *ref_count = new int(0);
    T *data = nullptr;
    size_t row;
    size_t column;
    size_t channels;

public:
    Matrix();
    Matrix(size_t row, size_t col, size_t channels) : row(row), column(col), channels(channels)
    {
        try
        {
            if (row <= 0 || col <= 0 || channels <= 0)
            {
                throw "The matrix is not valid!";
            }
            else
            {
                *ref_count = 1;
                data = new T[row * col * channels]{};
            }
        }
        catch (const bad_alloc &e)
        {
            cerr << "Memory is fail to allocate" << endl;
            delete ref_count;
            delete[] data;
            data = nullptr;
            throw "Memory is fail to allocate";
        }
        catch (const char *msg)
        {
            cerr << msg << endl;
            delete ref_count;
            delete[] data;
            data = nullptr;
            ref_count = nullptr;
            throw msg;
        }
    }
    ~Matrix()
    {
        *(this->ref_count)--;
        if (*(this->ref_count) == 0 && this->data == nullptr)
        {
            delete this->ref_count;
            delete[] this->data;
            this->data = NULL;
        }
    }
    void addrefCount()
    {
        this->ref_count++;
    }
    void minusrefCount()
    {
        this->ref_count--;
    }
    int *getrefCount() const // const很重要
    {
        return ref_count;
    }

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
                    cout << mat.data[i * mat.column + j] << "  ";
                }
                cout << endl;
                // str = str + "\n";
            }
            cout << endl;
            // os << str;
        }
        return os;
    }

    friend std::ifstream &operator>>(std::ifstream &ifs, const Matrix &mat)
    {
        for (size_t i = 0; i < mat.row; i++)
        {
            for (size_t j = 0; j < mat.column; j++)
            {
                ifs >> mat.data[i * mat.column + j];
            }
        }
        return ifs;
    }
    // Matrix ROI(int a, int b, string fileName);
    Matrix ROI(size_t startR, size_t endR, size_t startC, size_t endC, string fileName); // 传入文件修改矩阵指定区域

    Matrix
    operator+(const Matrix &mat);
    Matrix operator-(const Matrix &mat);
    Matrix operator=(const Matrix &mat);
    bool operator==(const Matrix &mat);
    Matrix operator+(const T addx);
    Matrix operator*(const Matrix &mat);

    void initialMatrix(Matrix *matrix, const size_t row, const size_t col);
    void createRamMatrix(Matrix *matrix, const size_t databound);
    // void showMatrix(const Matrix *matrix);
};

#endif // UNTITLED_SOURCE_HPP
