# CS205 C/ C++ Programming - Lab Assignment5

name:李冰（Libing)

SID:12110141

## **My codes are provided on the github <font color=red> https://github.com/2069958859/CppProject5.git </font> ,please check it there.**

## Part1-Analysis

> This project is to ask us design a class for matrix with mainly 4 requirements:<br>1. Support different data types: I use class template to solve this problem.<br>2. Do not use memory hard copy: instead, I will use soft copy and define a counter **\*ref_count** for each data to avoid memory leaks and to release memory multiple times.<br>3. Implement some frequently used operators.<br>4. Implement region of interest (ROI) which avoid memory hard copy.

## Part2-Code

> Here is source.cpp one cpp file, and source.h one h file.<br>

### 2.1 Matrix class has four basic fields and a field named **\*ref_count** to avoid memory hard copy:

```C++
template <typename T>
class Matrix
{
    size_t row;
    size_t column;
    size_t channels;
    T *data = nullptr;
    int *ref_count = new int(0);

    ...
}
```

### 2.2 Constructors

For the constructor, I check the validation first, and using **try catch** to deal with it.

```C++
public:
    Matrix();
    Matrix(size_t row, size_t col, size_t channels): row(row), column(col), channels(channels)
    {
        try
        {
            if (row == 0 || col == 0 || channels == 0)
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
```

And for copy constructor, I increase **ref_count** by 1, and do soft copy.

```C++
    Matrix(const Matrix<T> &mat) // copy constructor
    {
        this->column = mat.column;
        this->row = mat.row;
        this->channels = mat.channels;
        this->ref_count = mat.ref_count;
        this->data = mat.data;
        *(this->ref_count) += 1;
    }
```

### 2.3 Implement of some useful operators

The followings are the operators I have overloaded.

```C++
    Matrix operator+(const Matrix &mat);
    Matrix operator-(const Matrix &mat);
    Matrix operator=(const Matrix &mat);
    bool operator==(const Matrix &mat);
    Matrix operator+(const T addx);
    Matrix operator*(const Matrix &mat);
    friend std::ostream &operator<<(std::ostream &os, const Matrix &mat);
    friend std::ifstream &operator>>(std::ifstream &ifs, const Matrix &mat); // 从文件中读入矩阵的数据

```

#### 2.3.1 Assignment method

Free the data if no other matrix assign to it.

```C++
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
```

```C++
    void addrefCount()
    {
        *this->ref_count += 1;
    }
    void minusrefCount()
    {
        *this->ref_count -= 1;
    }
```

#### 2.3.2 Some other detail code about operator overload

I will check its validation carefully first, and if it is invalid, I will throw the exception to the main function.<br>
**operator==**

```C++
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
```

**operator+**

```C++
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
    }
    else if (this->row != mat.row || this->row != ans.row || mat.column != ans.column)
    {
        throw "The matrixs and the output doesn't match! \n";
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
```
```C++
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
```
### 2.4 Implement of ROI using soft copy

> In my point of view, ROI is used for processing the pictures by change a small region of the picture, the implement is as follow:

```C++
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
```
### 2.5 Other functions used:
```C++
size_t getFileRow(string fileName);//用于ROI的文件中矩阵大小的计算
size_t getFileSize(string fileName);
template <typename T>
void Matrix<T>::createRamMatrix(const size_t databound)//生成随机矩阵
```
## 3.Result & Verify

### 3.1 Results of operators

```C++
        Matrix<float> m1(2, 3, 1);
        Matrix<float> m2(2, 3, 1);
        Matrix<float> m3(2, 3, 1);
        m1.createRamMatrix(5);
        sleep(1);
        m2.createRamMatrix(5);
        m3 = m1 + m2;
        cout << m3;
        m3 = m1 + 2;
        cout << m3;
        m3 = m1;
        cout << m3;
```

![+](./++.png)<br>

```C++
        m3 = m1 * m2;
        cout << m3;
        m3 = m1;
        cout << (m1 == m3) << endl;

```

![==](./===.png)<br>

### 3.1 Result of ROI

```C++
        Matrix<float> m1(6, 7, 1);
        m1.createRamMatrix(5);
        cout << m1;
        m1.ROI(2, 3, 3, 5, "mat1.txt");
        cout << m1;
```

![==](./roi1.png)<br>
![==](./mat11.png)<br>

## 4. Problems & Conclusion

Firstly, I felt confused about how to use soft copy, at the beginning, I decided to use the smart pointer, but I found that it will cause a lot of problems and difficult to use, so finally I implement soft copy by a field **\*ref_count** to deal with the memory.<br>
In this project, I learnt to design a class for matrix in C++ which support many data types, and overriding the operators.<br>
Besides, I learn to use soft copy and manage the memory. And learn a new concept ROI and its manipulations.
