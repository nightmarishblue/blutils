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
        using array_type = blue::array<T>;

        using value_type = typename array_type::value_type;
        using size_type = typename array_type::size_type;

        using pointer = typename array_type::pointer;
        using const_pointer = typename array_type::const_pointer;

        using reference = typename array_type::reference;
        using const_reference = typename array_type::const_reference;

        using iterator = typename array_type::iterator;
        using const_iterator = typename array_type::const_iterator;

        using reverse_iterator = typename array_type::reverse_iterator;
        using const_reverse_iterator = typename array_type::const_reverse_iterator;

    private:
        static constexpr auto INDEX_OOB_MSG = "blue::matrix: index out of bounds";

        size_type rows_, columns_;
        array_type data_; // wastes a *teensy* bit of memory with an extra size_t but is a good backing

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

        size_type rows() const
        {
            return rows_;
        }

        size_type columns() const
        {
            return columns_;
        }

        size_type size() const
        {
            // the size of a Container is how many things it's storing total
            return data_.size();
        }

        // TODO add iterators
    };
}