#include <iostream>
#include "source.h"
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
#include <memory>
#include <exception>
using namespace std;

template <typename T>
void Matrix<T>::initialMatrix(Matrix<T> *matrix, const size_t row, const size_t col)
{ // 一个一维矩阵开辟空间P
  // try{
    if (row <= 0 || col <= 0)
    {
        throw "invalid matrix, please put in again! \n";
    }
    // }

    else
    {
        matrix->row = row;
        matrix->column = col;
        matrix->data = (T *)malloc(sizeof(T) * row * col); // 申请行空间
        if (matrix->data == NULL)
        { // 申请空间失败
            throw "The memory is failed to allocated";
        }
        // for (size_t j = 0; j < (unsigned long long) row * col; j++) {
        //     matrix->data[j] = 0;
        // }
    }
}

template <typename T>
void Matrix<T>::createRamMatrix(Matrix *matrix, const size_t row, const size_t col, const size_t databound)
{ // 检查合法性
    srand((unsigned)time(NULL));
    if (row <= 0 || col <= 0)
    {
        throw "row or/and col is smaller than 0";
    }
    else
    {
        matrix->row = row;
        matrix->column = col;
        matrix->data = (T *)malloc(sizeof(T) * row * col); // 申请行空间
        // Matrix(row, column);
        if (matrix->data == NULL)
        { // 申请空间失败
            throw "The memory is failed to allocated";
            free(matrix);
        }
        for (size_t j = 0; j < (unsigned long long)row * col; j++)
        {
            T ram = (T)rand() / RAND_MAX * databound;
            matrix->data[j] = ram;
        }
    }
}

// template <typename T>
// void Matrix<T>::showMatrix(const Matrix *matrix)
// { // 打印矩阵
//     if (matrix == NULL || matrix->data == NULL)
//     {
//         cerr << "The matrix is null!" << endl;
//     }
//     else
//     {
//         for (size_t i = 0; i < matrix->row; i++)
//         {
//             for (size_t j = 0; j < matrix->column; j++)
//             {
//                 cout << std::left << setw(10);
//                 cout.fixed;
//                 cout << matrix->data[i * matrix->row + j] << "  ";
//             }
//             cout << endl;
//         }
//     }
//     cout << endl;
// }

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix &mat)
{
    Matrix<T> ans(this->row, this->column, this->channels);
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }
    else if (this->data == NULL || mat.data == NULL || ans.data == NULL)
    {
        throw "The matrix data is not valid!";
        //        return NULL;
    }
    else if (this->row != mat.row || this->row != ans.row || mat.column != ans.column)
    {
        throw "The matrixs ans the output doesn't match! \n";
        //        return NULL;
    }
    else
    {
        for (size_t i = 0; i < this->row; ++i)
        {
            for (size_t j = 0; j < this->column; ++j)
            {
                ans.data[i * this->row + j] = this->data[i * this->row + j] + mat.data[i * this->row + j];
            }
        }
    }
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const T addx)
{
    Matrix<T> ans(this->row, this->column, this->channels);
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }
    else if (this->data == NULL || ans.data == NULL)
    {
        throw "The matrix data is not valid!";
        //        return NULL;
    }
    else if (this->row != ans.row || this->column != ans.column)
    {
        throw "The matrixs ans the output doesn't match! \n";
        //        return NULL;
    }
    else
    {
        for (size_t i = 0; i < this->row; ++i)
        {
            for (size_t j = 0; j < this->column; ++j)
            {
                ans.data[i * this->row + j] = this->data[i * this->row + j] + addx;
            }
        }
    }
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix &mat)
{
    Matrix<T> ans(this->row, this->column, this->channels);
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }
    else if (this->data == NULL || mat.data == NULL || ans.data == NULL)
    {
        throw "The matrix data is not valid!";
        //        return NULL;
    }
    else if (this->row != mat.row || this->row != ans.row || mat.column != ans.column)
    {
        throw "The matrixs ans the output doesn't match! \n";
        //        return NULL;
    }
    else
    {
        for (size_t i = 0; i < this->row; ++i)
        {
            for (size_t j = 0; j < this->column; ++j)
            {
                ans.data[i * this->row + j] = this->data[i * this->row + j] - mat.data[i * this->row + j];
            }
        }
    }
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &mat)
{
    Matrix<T> ans(this->row, this->column, this->channels);
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }
    else if (this->data == NULL || mat.data == NULL || ans.data == NULL)
    {
        throw "The matrix data is not valid!";
        //        return NULL;
    }
    else if (this->row != mat.row || this->row != ans.row || mat.column != ans.column)
    {
        throw "The matrixs ans the output doesn't match! \n";
        //        return NULL;
    }
    else
    {
        for (size_t i = 0; i < this->row; ++i)
        {
            for (size_t j = 0; j < this->column; ++j)
            {
                ans.data[i * this->row + j] = this->data[i * this->row + j] - mat.data[i * this->row + j];
            }
        }
    }
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::operator=(const Matrix &mat) // soft copy
{
    if (this != &mat)
    {
        if (this == NULL)
        {
            throw "The matrixs are not valid! ";
        }
        else if (this->data == NULL)
        {
            throw "The matrix data is not valid!";
            //        return NULL;
        }
        else if (this->row != mat.row || this->column != mat.column)
        {
            throw "The matrixs ans the output doesn't match! \n";
            //        return NULL;
        }
        else
        {
            delete[] this->data;
            this->column = mat.column;
            this->row = mat.row;
            this->channels = mat.channels;
            if (mat.data)
            {
                this->data = mat.data;
                this->addrefCount();
            }
        }
    }
    // cout << data << endl;
    return *this;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix &mat)
{
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
        return false;
    }
    else if (this->data == NULL)
    {
        throw "The matrix data is not valid!";
        return false;
    }
    else if (this->row != mat.row || this->column != mat.column)
    {
        throw "The matrixs ans the output doesn't match! \n";
        return false;
    }
    else
    {
        for (size_t i = 0; i < this->row; ++i)
        {
            for (size_t j = 0; j < this->column; ++j)
            {
                if (this->data[i * this->row + j] != mat.data[i * this->row + j])
                {
                    return false;
                }
            }
        }
        return true;
    }
}

template class Matrix<int>;
int main()
{
    try
    {
        Matrix<float> m1(3, 4, 1);
        Matrix<float> m2(3, 4, 1);
        Matrix<float> m3(3, 4, 1);
        // std::shared_ptr<float> p1;
        m1.createRamMatrix(&m1, 3, 4, 5);
        sleep(1);
        m1.createRamMatrix(&m2, 3, 4, 5);
        cout << m1;
        cout << m2;
        m3 = m1 + m2;
        cout << m3;
        m3 = m1;
        cout << m3;
        cout << (m1 == m3) << endl;
        cout << (m1 == m2) << endl;
    }
    catch (const char *msg)
    {
        cerr << msg << endl;
    }
    return 0;
}
