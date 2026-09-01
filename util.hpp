#ifndef util_H
#define util_H
inline int coordinateTo1d(int x, int y, int width){
    return width * y + x;
}

inline int clamp(int min, int val, int max){
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

#endif
