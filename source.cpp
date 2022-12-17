#include <iostream>
#include <fstream>
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
void Matrix<T>::createRamMatrix(const size_t databound)
{ // 检查合法性
    srand((unsigned)time(NULL));
    if (this->row <= 0 || this->column <= 0)
    {
        throw "row or/and col is smaller than 0";
    }
    else
    {
        if (this->data == NULL)
        { // 申请空间失败
            throw "The memory is failed to allocated";
        }
        for (size_t j = 0; j < (unsigned long long)this->row * this->column; j++)
        {
            T ram = (T)rand() / RAND_MAX * databound;
            this->data[j] = ram;
        }
        this->addrefCount();
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
    cout << "Sum of matrixs is: " << endl;
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
        throw "The matrixs and the output doesn't match! \n";
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
    cout << "Sum of matrix and the number is: " << endl;

    Matrix<T> ans(this->row, this->column, this->channels);

    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }
    else if (this->data == NULL || ans.data == NULL)
    {
        throw "The matrix data is not valid!";
    }
    else if (this->row != ans.row || this->column != ans.column)
    {
        throw "The matrixs and the output doesn't match! \n";
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
    cout << "The difference of matrixs is: " << endl;

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
        throw "The matrixs and the output doesn't match! \n";
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
    cout << "Product of matrixs is: " << endl;

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
                    ans.data[i * this->column + j] = temp;
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
        }
        else
        {
            if (*(this->ref_count) == 0 && this->data != nullptr)
            {
                delete[] this->data;
                delete this->ref_count;
            }
            this->column = mat.column;
            this->row = mat.row;
            this->channels = mat.channels;
            if (mat.data)
            {
                this->data = mat.data;
                *this->ref_count = *mat.ref_count;
                this->addrefCount();
            }
        }
    }
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
        throw "The two matrix doesn't match! \n";
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

size_t getFileRow(string fileName)
{
    ifstream ReadFile;
    size_t n = 0;
    string tmp;
    ReadFile.open(fileName, ios::in);
    if (ReadFile.fail())
    {
        throw "Not valid file!";
        return 0;
    }
    else // 文件存在
    {
        while (getline(ReadFile, tmp, '\n'))
        {
            n++;
        }
        ReadFile.close();
        return n;
    }
}
size_t getFileSize(string fileName)
{
    ifstream ReadFile;
    size_t n = 0;
    string tmp;
    string data1;
    ReadFile.open(fileName, ios::in);
    if (ReadFile.fail())
    {
        throw "Not valid file!";
        return 0;
    }
    else // 文件存在
    {
        while (ReadFile >> data1)
        {
            n++;
        }
        return n;
    }
}

template <typename T>
Matrix<T> Matrix<T>::ROI(size_t startR, size_t startC, size_t endR, size_t endC, string fileName) // 传入文件修改矩阵指定区域
{
    if (this == NULL)
    {
        throw "The matrixs are not valid! ";
    }

    else if (startR > endR || startC > endC || endR > this->row || endC > this->column)
    {
        throw "The ROI is not valid! 1";
    }
    else
    {
        size_t row = endR - startR + 1;
        size_t col = endC - startC + 1;

        size_t file_row = getFileRow(fileName);
        size_t file_col = getFileSize(fileName) / file_row;
        if (col * row != getFileSize(fileName))
        {
            throw "The ROI is not valid! 2";
        }
        else
        {
            // cout << col << endl
            //      << row << endl
            //      << getFileSize(fileName) << endl;

            std::ifstream fin(fileName);
            Matrix roi_data = Matrix(file_row, file_col, 1); // 将文件中的数据存入Matrix类的对象中
            fin >> roi_data;
            fin.close();
            T *roi_ptr = this->data; // 指向存储数据的首地址
            int copy_index = 0;
            for (int i = (startR - 1) * this->column + startC - 1;
                 i <= (endR - 1) * this->column + endC - 1; i++)
            {
                int ptr_row = i / this->column + 1;
                int ptr_col = i % this->column + 1;
                if (ptr_row >= startR && ptr_row <= endR && ptr_col >= startC && ptr_col <= endC)
                {
                    *(roi_ptr + i) = roi_data.data[copy_index];
                    copy_index++;
                }
            }
            this->addrefCount();
        }
        return *this;
    }
}

template class Matrix<float>;
int main()
{
    try
    {
        Matrix<float> m1(6, 7, 1);
        Matrix<float> m2(3, 3, 1);
        Matrix<float> m3(3, 3, 1);
        Matrix<float> m4(2, 3, 1);

        m1.createRamMatrix(5);
        sleep(1);
        m2.createRamMatrix(5);
        cout << m1;
        // cout << m2;
        // ifstream fin("mat1.txt");
        m1.ROI(2, 3, 3, 5, "mat1.txt");
        cout << m1;
        // m3 = m1 + m2; // wrong case
        // cout << m3;
        // m3 = m1 * m2;
        // cout << m3;
        // // cout << m3;
        // // cout << (m1 == m3) << endl;
        // m3 = m1;
        // cout << (m1 == m3) << endl;
    }
    catch (const char *msg)
    {
        cerr << msg << endl;
    }
    return 0;
}
