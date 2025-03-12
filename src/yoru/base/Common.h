#pragma once
constexpr double M_PI = 3.1415926535897932385;
constexpr double M_INVPI = .31830988618379067154;
inline double Radian(double Angle) {
    return M_PI * Angle / 180.;
}