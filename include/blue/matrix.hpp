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

    // with the number of columns in a 2D matrix and the desired row and column, flatten it into a 1D index
    static std::size_t normalize(/* std::size_t rows, (unneeded) */ std::size_t cols, std::size_t row, std::size_t col)
    {
        return rowIndex(cols, row) + col;
    }

    // fixed-size 2D container with a single block of heap memory
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
        blue::array<value_type> data_; // wastes a *teensy* bit of memory with an extra size_t but is a good backing

        this_type* strip() const
        {
            return const_cast<this_type*>(this);
        }

    public:
        // construct without initialising
        matrix(size_type rows, size_type columns) : rows_(rows), columns_(columns) , data_(rows * columns) {}
        // i would disallow it, but mathematically there is a matrix with a 0-dimension - the empty matrix

        // construct with the elements of a 2D array
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

        const_pointer operator[](size_type index) const
        {
            return strip()->operator[](index);
        }

        reference at(size_type row, size_type column)
        {
            if (row >= rows_ || column >= columns_)
                throw std::out_of_range(INDEX_OOB_MSG);
            return data_[normalize(columns_, row, column)];
        }

        const_reference at(size_type row, size_type column) const
        {
            return strip()->at(row, column);
        }
    };
}