#include <array>
#include <cstddef>

#include "array.hpp"

template<typename T>
static constexpr bool same_size(std::initializer_list<std::initializer_list<T>> b)
{
    return true;
    // if (arrays.size() == 0) return true; // no discrepancy if there isnt a single sublist
    // auto it = arrays.begin();
    // auto size = it->size();
    // for (auto end = arrays.end(); it != end; ++it)
    //     if (it->size() != size) return false;
    // return true;
}

template<typename T>
static std::size_t columns(std::initializer_list<std::initializer_list<T>> arrays)
{
    if (arrays.size() == 0) return 0; // must be an empty matrix
    return arrays.begin()->size();
}

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

    // template<std::size_t R, std::size_t C>
    // template<class A>
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

        // matrix(std::initializer_list<std::initializer_list<value_type>> values) : matrix(values.size(), columns(values))
        // {
        //     // tomfuckery with initializer_list not just being constexpr :/
        //     static_assert(same_size(values), "elements of initialiser list are not all of same size");
        // }

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