#include <stdexcept>
namespace blue
{
    // contiguous array of dynamic, but unchanging size
    template<typename T>
    class array
    {
    public:
        using self_type = array<T>;
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using reference = value_type&;
        using const_reference = const value_type&;

        using pointer = value_type*;
        using const_pointer = const value_type*;

        using iterator = pointer;
        using const_iterator = const_pointer;

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        T* data_;
        /*const*/ size_type length_;

        self_type* strip() const
        {
            return const_cast<self_type*>(this);
        }

    public:
        // allocate length without initialising
        array(size_type length) : length_(length)
        {
            data_ = new value_type[length_];
        }

        // initialise each element with a copy of the given value
        array(size_type length, const_reference value) : array(length)
        {
            for (auto& element : *this)
                element = value;
        }

        // initialise with the given values
        array(std::initializer_list<T> values) : array(values.size())
        {
            size_type i = 0;
            for (const auto& element : values)
                data_[i++] = element;
        }

        // copy constructor
        array(const self_type& other) : array(other.size())
        {
            size_type i = 0;
            for (const auto& element : other)
                data_[i++] = element;
        }

        // move constructor
        array(self_type&& other)
        {
            length_ = other.length_;
            data_ = other.data_;
            other.data_ = nullptr; // ensure other's destructor doesnt free this
        }

        ~array()
        {
            if (data_) delete[] data_;
        }

        self_type& operator=(const self_type& other)
        {
            if (length_ != other.length_)
            {
                delete[] data_;
                data_ = new value_type[other.length_];
            }

            length_ = other.length_;
            size_type i = 0;
            for (const auto& element : other)
                data_[i++] = element;
            return *this;
        }

        self_type& operator=(self_type&& other)
        {
            delete[] data_;
            length_ = other.length_;
            data_ = other.data_;
            other.data_ = nullptr; // ensure other's destructor doesnt free this
            return *this;
        }


        pointer data()
        {
            return data_;
        }

        const_pointer data() const
        {
            return data_;
        }


        size_type size() const
        {
            return length_;
        }

        size_type max_size() const
        {
            return size();
        }

        bool empty() const
        {
            return size() == 0;
        }


        reference operator[](size_type index)
        {
            return data_[index];
        }

        const_reference operator[](size_type index) const
        {
            return strip()->operator[](index);
        }

        reference at(size_type index)
        {
            if (index >= size())
                throw std::out_of_range("index out of bounds");
            return data_[index];
        }

        const_reference at(size_type index) const
        {
            return strip()->operator[](index);
        }


        reference front()
        {
            return data_[0];
        }

        const_reference front() const
        {
            return strip()->front();
        }

        reference back()
        {
            return data_[size() - 1];
        }

        const_reference back() const
        {
            return strip()->back();
        }


        void fill(const_reference value)
        {
            for (auto& element : *this)
                element = value;
        }

        void swap(self_type& other) noexcept
        {
            pointer tmpdata = data_;
            data_ = other.data_;
            other.data_ = tmpdata;

            size_type tmpsize = length_;
            length_ = other.length_;
            other.length_ = tmpsize;
        }


        iterator begin()
        {
            return data_;
        }

        const_iterator begin() const
        {
            return strip()->begin();
        }

        const_iterator cbegin() const
        {
            return begin();
        }

        reverse_iterator rbegin()
        {
            return std::reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
            return std::reverse_iterator(strip()->rbegin());
        }

        const_reverse_iterator crbegin() const
        {
            return rbegin();
        }

        iterator end()
        {
            if (size() == 0)
                return begin(); // return the start if zero so loop instantly ends
            return &data_[size()]; // 1 past the end
        }

        const_iterator end() const
        {
            return strip()->end();
        }

        const_iterator cend() const
        {
            return end();
        }

        reverse_iterator rend()
        {
            return std::reverse_iterator(begin());
        }

        const_reverse_iterator rend() const
        {
            return std::reverse_iterator(strip()->rend());
        }

        const_reverse_iterator crend() const
        {
            return rend();
        }
    };


    template<typename T>
    bool operator==(const blue::array<T>& lhs, const blue::array<T>& rhs)
    {
        if (lhs.size() != rhs.size()) return false;
        for (typename blue::array<T>::size_type i = 0, l = lhs.size(); i < l; i++)
        {
            if (lhs[i] != rhs[i]) return false;
        }
        return true;
    }

    template<typename T>
    bool operator!=(const blue::array<T>& lhs, const blue::array<T>& rhs)
    {
        return !(lhs == rhs);
    }

    // would add lexicographical comparison here if i ever used it
}

namespace std
{
    // allow tuple-like usage - maybe not appropriate since the length is dynamic

    template<std::size_t I, typename T>
    T& get(blue::array<T>& a) noexcept
    {
        return a[I];
    }

    template<std::size_t I, typename T>
    T&& get(blue::array<T>&& a) noexcept
    {
        return a[I];
    }

    template<std::size_t I, typename T>
    const T& get(const blue::array<T>& a) noexcept
    {
        return a[I];
    }

    template<std::size_t I, typename T>
    const T&& get(const blue::array<T>&& a) noexcept
    {
        return a[I];
    }

    template <std::size_t I, class T>
    struct tuple_element;

    template <std::size_t I, class T>
    struct tuple_element<I, blue::array<T>>
    {
        using type = T;
    };


    template<typename T>
    void swap(blue::array<T>& lhs, blue::array<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }
}