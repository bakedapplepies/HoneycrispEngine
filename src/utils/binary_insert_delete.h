#pragma once

#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

// also works with T types that support overloaded operations (<, >)
template <typename T>
void binary_insert_shader_comparator(
    std::vector<T>& vec,
    std::vector<GLuint>& shader_list,
    const T& value,
    size_t comparator
) {
    static_assert(std::is_arithmetic_v<T>, "T is not a number.");

    if (vec.size() == 0)
    {
        vec.push_back(value);
        shader_list.push_back(comparator);
        return;
    }
    else if (vec.size() == 1)
    {
        size_t pos = static_cast<size_t>(comparator > shader_list[0]);
        vec.insert(vec.begin() + pos, value);
        shader_list.insert(shader_list.begin() + pos, comparator);
    }

    size_t left = 0, right = vec.size() - 1;
    size_t mid = (left + right) / 2;

    while (!(shader_list[mid] < comparator && comparator < shader_list[mid + 1]))
    {
        if (comparator < shader_list[mid]) right = mid;
        else left = mid + 1;

        mid = (left + right) / 2;

        if (mid == vec.size() - 1) break;
        else if (mid == 0) break;
    }

    vec.insert(vec.begin() + mid + (size_t)(mid != 0), value);
    shader_list.insert(shader_list.begin() + mid + (size_t)(mid != 0), comparator);
}

template <typename T>
void binary_delete_w_shader_list(
    std::vector<T>& vec,
    std::vector<GLuint>& shader_list,
    T value
) {
    static_assert(std::is_arithmetic_v<T>, "T is not a number.");

    if (vec.size() == 0)
    {
        HNCRSP_TERMINATE("Vector is empty");
    }

    size_t left = 0, right = vec.size() - 1;
    size_t mid = (left + right) / 2;

    while (value != vec[mid])
    {
        if (value < vec[mid]) right = mid - 1;
        else left = mid + 1;

        mid = (left + right) / 2;
    }

    vec.erase(vec.begin() + mid);
    shader_list.erase(shader_list.begin() + mid);
}

HNCRSP_NAMESPACE_END