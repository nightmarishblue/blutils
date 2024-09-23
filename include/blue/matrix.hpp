#pragma once
#include <array>
#include <cstddef>

#include "array.hpp"

namespace blue
{
    static std::size_t rowIndex(std::size_t columns, std::size_t rowIndex)
    {
        return rowIndex * columns;
    }

    // with the number of columns in a 2D matrix and the desired row and column, get the index of
    static std::size_t normalize(/* std::size_t rows, (unneeded) */ std::size_t cols, std::size_t row, std::size_t col)
    {
        return rowIndex(cols, row) + col;
    }

    template<typename T>
    class row
    {
    public:
        using size_type = std::size_t;
        T* data_;
        T& operator[](size_type index)
        {
            return data_[index];
        }
    };

    template<typename T>
    class matrix
    {
    public:
        using this_type = matrix<T>;
        using value_type = T;
        using size_type = std::size_t;

        using pointer = value_type*;
        using const_pointer = const value_type*;

        using reference = value_type&;
        using const_reference = const value_type&;

    private:
        static constexpr auto INDEX_OOB_MSG = "blue::matrix: index out of bounds";

        size_type rows_, columns_;
        blue::array<T> data_; // wastes a *teensy* bit of memory with an extra size_t but is a good backing

    public:
        // i would disallow it, but mathematically there is a matrix with a 0-dimension - the empty matrix
        matrix(size_type rows, size_type columns) : rows_(rows), columns_(columns) , data_(rows * columns) {}

        template<size_type R, size_type C>
        matrix(const value_type(&array)[R][C]) : matrix(R, C)
        {
            for (size_type i = 0; i < R; i++)
            {
                for (size_type j = 0; j < C; j++)
                    (*this)[i][j] = array[i][j];
            }
        }

        pointer operator[](size_type index)
        {
            return &data_[rowIndex(columns_, index)];
        }

        reference at(size_type row, size_type column)
        {
            if (row >= rows_ || column >= columns_)
                throw std::out_of_range(INDEX_OOB_MSG);
            return data_[normalize(columns_, row, column)];
        }
    };
}