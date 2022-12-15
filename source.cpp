#include <iostream>
#include "source.h"
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
#include <memory>
#include <exception>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <immintrin.h>

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
void Matrix<T>::createRamMatrix(Matrix *matrix, const size_t databound)
{ // 检查合法性
    srand((unsigned)time(NULL));
    if (matrix->row <= 0 || matrix->column <= 0)
    {
        throw "row or/and col is smaller than 0";
    }
    else
    {
        if (matrix->data == NULL)
        { // 申请空间失败
            throw "The memory is failed to allocated";
            free(matrix);
        }
        for (size_t j = 0; j < (unsigned long long)matrix->row * matrix->column; j++)
        {
            T ram = (T)rand() / RAND_MAX * databound;
            // cout << j << " : " << ram << " ";

            matrix->data[j] = ram;
        }
        cout << endl;
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
                ans.data[i * this->column + j] = this->data[i * this->column + j] + mat.data[i * this->column + j];
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
                ans.data[i * this->column + j] = this->data[i * this->column + j] + addx;
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
                ans.data[i * this->column + j] = this->data[i * this->column + j] - mat.data[i * this->column + j];
            }
        }
    }
    return ans;
}
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &mat)
{ // 矩阵相乘,一维
    // #ifdef WITH_AVX2
    __m256 a, b;

    Matrix<T> ans(this->row, mat.column, this->channels);
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }
    else if (this->data == NULL || mat.data == NULL || ans.data == NULL)
    {
        throw "The matrix data is not valid!";
        //        return NULL;
    }
    else if (this->column != mat.row || this->row != ans.row || mat.column != ans.column)
    {
        throw "The matrixs and the output doesn't match! \n";
    }

    else
    {
        T *p2 = (T *)malloc(sizeof(T) * mat.column * this->column * this->channels);
        if (p2 == NULL)
        { // 申请空间失败
            throw "p2 is failed to allocated";
        }

        size_t o = 0;

        for (size_t j = 0; j < mat.column; ++j)
        { // reduce loops
            for (size_t k = 0; k < mat.column / 8 * 8; k += 8)
            {
                p2[o++] = mat.data[k * mat.column + j];
                p2[o++] = mat.data[(1 + k) * mat.column + j];
                p2[o++] = mat.data[(2 + k) * mat.column + j];
                p2[o++] = mat.data[(3 + k) * mat.column + j];
                p2[o++] = mat.data[(4 + k) * mat.column + j];
                p2[o++] = mat.data[(5 + k) * mat.column + j];
                p2[o++] = mat.data[(6 + k) * mat.column + j];
                p2[o++] = mat.data[(7 + k) * mat.column + j];
            }

            for (size_t k = mat.column / 8 * 8; k < mat.column; k++)
            {
                p2[o++] = mat.data[k * mat.column + j];
            }
        }

        if (this->row < 8)
        {
            float temp = 0;
            for (size_t i = 0; i < this->row; ++i)
            {
                for (size_t j = 0; j < mat.column; ++j)
                {
                    for (size_t k = 0; k < this->column; ++k)
                    {
                        temp += this->data[i * this->row + k] * mat.data[k * this->column + j];
                    }
                    ans.data[i * this->row + j] = temp;
                    temp = 0;
                }
            }
        }

        else
        {

#pragma omp parallel for
            for (size_t q = 0; q < 8; q++)
            {
                __m256 c = _mm256_setzero_ps();

                size_t m1row_8 = this->row / 8;
                T *this_8 = this->data + (this->row / 8 * q) * this->column;
                T *ans_8 = ans.data + (this->row / 8 * q) * mat.column;
                for (size_t i = 0; i < m1row_8; ++i)
                {
                    for (size_t j = 0; j < mat.column; ++j)
                    {
                        T temp = 0;
                        T sum[8] = {0};
                        c = _mm256_setzero_ps();
                        ans_8[i * this->row + j] = 0;

                        size_t i1 = i * this->column;
                        size_t j1 = j * mat.row;

                        for (size_t k = 0; k < this->column / 8 * 8; k += 8)
                        {
                            a = _mm256_loadu_ps(this_8 + i1 + k);
                            b = _mm256_loadu_ps(p2 + j1 + k);
                            c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
                        }
                        _mm256_storeu_ps(sum, c);

                        ans_8[i * mat.column + j] =
                            sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];

                        for (size_t k = this->column / 8 * 8; k < this->column; k += 1)
                        {
                            // Tail case
                            temp += this_8[i1 + k] * p2[j1 + k];
                        }
                        ans_8[i * mat.column + j] += temp;
                        temp = 0;
                    }
                    // printf("%d\n",i);
                }
            }

            T *this_else = this->data + (this->row / 8 * 8) * this->column;
            T *ans_else = ans.data + (this->row / 8 * 8) * mat.column;

            for (size_t i = 0; i < this->row - this->row / 8 * 8; ++i)
            { // tail
                for (size_t j = 0; j < mat.column; ++j)
                {
                    T temp = 0;
                    T sum[8] = {0};
                    __m256 c = _mm256_setzero_ps();
                    ans_else[i * this->row + j] = 0;

                    size_t i1 = i * this->column;
                    size_t j1 = j * mat.row;

                    for (size_t k = 0; k < this->column / 8 * 8; k += 8)
                    {
                        a = _mm256_loadu_ps(this_else + i1 + k);
                        b = _mm256_loadu_ps(p2 + j1 + k);
                        c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
                    }
                    _mm256_storeu_ps(sum, c);

                    ans_else[i * mat.column + j] =
                        sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];

                    for (size_t k = this->column / 8 * 8; k < this->column; k += 1)
                    {
                        // Tail case
                        temp += this_else[i1 + k] * p2[j1 + k];
                    }
                    ans_else[i * mat.column + j] += temp;
                    temp = 0;
                }
                // printf("%d\n",i);
            }

            free(p2); // 释放空间
        }
    }
    return ans;
    // #else
    //     printf("AVX2 is not supported");
    // #endif
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
            throw "The matrixs and the output doesn't match! \n";
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
                if (this->data[i * this->column + j] != mat.data[i * this->column + j])
                {
                    return false;
                }
            }
        }
        return true;
    }
}

template class Matrix<float>;
int main()
{
    try
    {
        Matrix<float> m1(2, 3, 1);
        Matrix<float> m2(2, 3, 1);
        Matrix<float> m3(2, 3, 1);
        // std::shared_ptr<float> p1;
        m1.createRamMatrix(&m1, 5);
        sleep(1);
        m2.createRamMatrix(&m2, 5);
        cout << m1;
        cout << m2;
        m3 = m1 + m2;
        cout << m3;
        // m3 = m1;
        // cout << m3;
        // cout << (m1 == m3) << endl;
        // cout << (m1 == m2) << endl;
    }
    catch (const char *msg)
    {
        cerr << msg << endl;
    }
    return 0;
}
