#ifndef MY_MAP_TRAITS_H
#define MY_MAP_TRAITS_H 1

namespace mySymbolTable {
    template<typename T, bool IsMap>
    struct get_map_key_t {
        using key_type = typename T::first_type;
    };

    template<typename T>
    struct get_map_key_t<T, false> {
        using key_type = T;
    };
}

#endif // !MY_MAP_TRAITS_H