/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    math.hpp: Generic overridable matrix implementation for GE implementatiom

    This is single precision becuase 
*/

#include <Motion.hpp>

namespace Motion
{
    #define MATRIX_MINIMUM_WARNING_X                1024    
    #define MATRIX_MINIMUM_WARNING_Y                1024    

    /// @brief a matrix
    /// @tparam T the typename of the matrix
    /// @tparam X the X-size of the matrix
    /// @tparam Y the Y-size of the matrix
    template <typename T, int32_t X, int32_t Y>
    class Matrix
    {
    public:
        Matrix()
        {
            MOTION_ASSERT_FATAL(X <= 0 || Y <= 0, "Tried to create a matrix with a size of zero ?????");
            MOTION_ASSERT_WARNING(X >= MATRIX_MINIMUM_WARNING_X || Y >= MATRIX_MINIMUM_WARNING_Y, std::format("That matrix will use {} bytes of memory, are you sure you intended this?", MATRIX_MINIMUM_WARNING_X * MATRIX_MINIMUM_WARNING_Y));
            return; 
        }

        T& operator[](std::size_t r, std::size_t c) 
        {
            return data[(r * Y) + c];
        }

    private: 
        std::array<T, (X * Y)> data{};
    };
}