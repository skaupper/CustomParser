#ifndef SK_LIB_UTILS_H
#define SK_LIB_UTILS_H

#include <array>
#include <type_traits>


// ---------- Type traits ----------------------------------------------------------------------------------------------

template<typename T>
struct is_std_array : std::false_type {};

template<typename T, size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

template<typename... T>
inline constexpr bool is_std_array_v = is_std_array<T...>::value;


template<typename T>
struct is_std_vector : std::false_type {};

template<typename... T>
struct is_std_vector<std::vector<T...>> : std::true_type {};

template<typename... T>
inline constexpr bool is_std_vector_v = is_std_vector<T...>::value;


// ---------- Functions ------------------------------------------------------------------------------------------------

//
// recursive_for_each_array
//
template<typename T, typename Fn, typename... Args,
         typename std::enable_if_t<!is_std_array_v<std::decay_t<T>>, int> = 0>
void recursive_for_each_array(T &&n_arr, Fn &&f, Args &&... args) {
    f(std::forward<T>(n_arr), std::forward<Args>(args)...);
}

template<typename T, typename Fn, typename... Args, typename std::enable_if_t<is_std_array_v<std::decay_t<T>>, int> = 0>
void recursive_for_each_array(T &&arr, Fn &&f, Args &&... args) {
    for (auto &&e : arr) {
        recursive_for_each_array(std::forward<decltype(e)>(e), std::forward<Fn>(f), std::forward<Args>(args)...);
    }
}

//
// recursive_for_each_vector
//
template<typename T, typename Fn, typename... Args,
         typename std::enable_if_t<!is_std_vector_v<std::decay_t<T>>, int> = 0>
void recursive_for_each_vector(T &&n_vec, Fn &&f, Args &&... args) {
    f(std::forward<T>(n_vec), std::forward<Args>(args)...);
}

template<typename T, typename Fn, typename... Args,
         typename std::enable_if_t<is_std_vector_v<std::decay_t<T>>, int> = 0>
void recursive_for_each_vector(T &&vec, Fn &&f, Args &&... args) {
    for (auto &&e : vec) {
        recursive_for_each_vector(std::forward<decltype(e)>(e), std::forward<Fn>(f), std::forward<Args>(args)...);
    }
}


//
// concat for std::array
//
template<typename T, size_t N1, size_t N2>
constexpr auto concat(const std::array<T, N1> &a1, const std::array<T, N2> &a2) {
    std::array<T, N1 + N2> arr;

    for (size_t i {0}; i < N1; ++i) {
        arr[i] = a1[i];
    }
    for (size_t i {0}; i < N2; ++i) {
        arr[i + N1] = a2[i];
    }

    return arr;
}

#endif
