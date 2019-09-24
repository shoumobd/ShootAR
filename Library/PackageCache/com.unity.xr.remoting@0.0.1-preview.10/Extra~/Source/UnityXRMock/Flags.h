#pragma once

template<typename T>
inline T AddFlag(T currentValue, T flag)
{
    return static_cast<T>(currentValue | flag);
}

template<typename T>
inline T RemoveFlag(T currentValue, T flag)
{
    return static_cast<T>(currentValue & ~flag);
}

template<typename T>
inline T SetFlag(T currentValue, T flag, bool isSet)
{
    return isSet ? AddFlag(currentValue, flag) : RemoveFlag(currentValue, flag);
}
