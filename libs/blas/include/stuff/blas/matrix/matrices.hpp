#include <stuff/blas/matrix.hpp>
#include <stuff/blas/vector.hpp>

namespace stf::blas::matrices {

/// https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
template<typename Float = float>
constexpr auto look_at_lh(vector<Float, 3> eye, vector<Float, 3> at, vector<Float, 3> up) -> matrix<Float, 4, 4> {
    const auto z_axis = normalize(at - eye);
    const auto x_axis = normalize(cross(up, z_axis));
    const auto y_axis = cross(z_axis, x_axis);

    // clang-format off
    return matrix<Float, 4, 4> {
        x_axis[0], y_axis[0], z_axis[0], 0.f,
        x_axis[1], y_axis[1], z_axis[1], 0.f,
        x_axis[2], y_axis[2], z_axis[2], 0.f,
        -dot(x_axis, eye), -dot(y_axis, eye), -dot(x_axis, eye), 1.f,
    };
    // clang-format on
}

template<typename Float = float>
constexpr auto rotate_around_x(Float radians) -> matrix<Float, 3, 3> {
    using std::cos;
    using std::sin;

    const auto α = radians;

    // clang-format off
    return matrix<Float, 3, 3> {
        1, 0     , 0      ,
        0, cos(α), -sin(α),
        0, sin(α),  cos(α),
    };
    // clang-format on
}

template<typename Float = float>
constexpr auto rotate_around_y(Float radians) -> matrix<Float, 3, 3> {
    using std::cos;
    using std::sin;

    const auto α = radians;

    // clang-format off
    return matrix<Float, 3, 3> {
         cos(α), 0, sin(α),
        0      , 1, 0     ,
        -sin(α), 0, cos(α),
    };
    // clang-format on
}

template<typename Float = float>
constexpr auto rotate_around_z(Float radians) -> matrix<Float, 3, 3> {
    using std::cos;
    using std::sin;

    const auto α = radians;

    // clang-format off
    return matrix<Float, 3, 3> {
        cos(α), -sin(α), 0,
        sin(α),  cos(α), 0,
        0     , 0      , 1,
    };
    // clang-format on
}

template<typename Float = float>
constexpr auto rotate(Float rad_pitch, Float rad_yaw, Float rad_roll) -> matrix<Float, 3, 3> {
    const auto rot_matrix = rotate_around_z<float>(rad_roll) *  //
                            rotate_around_y<float>(rad_yaw) *   //
                            rotate_around_x<float>(rad_pitch);
    return rot_matrix;
}

/// https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
template<typename Float = float>
constexpr auto perspective_lh(Float fov_y_radians, Float aspect, Float near, Float far) -> matrix<Float, 4, 4> {
    const auto y_scale = 1 / std::tan(fov_y_radians / 2);
    const auto x_scale = y_scale / aspect;
    // clang-format off
    return matrix<Float, 4, 4> {
        x_scale, 0      , 0                         , 0,
        0      , y_scale, 0                         , 0,
        0      , 0      , far / (far - near)        , 1,
        0      , 0      , -near * far / (far - near), 0,
    };
    // clang-format on
}

}  // namespace stf::blas::matrices
