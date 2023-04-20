#pragma once

#include "crow/types.hpp"
#include <tuple>
#include <type_traits>
#include <utility>

namespace Crow {

    // Typelist primitives

    template <typename... TS> struct Typelist {};
    using Nil = Typelist<>;

    // Typelist composition and decomposition metafunctions

    template <typename TL, typename T> struct AppendType;
    template <typename... TS, typename T> struct AppendType<Typelist<TS...>, T>
        { using type = Typelist<TS..., T>; };
    template <typename... TS> struct AppendType<Typelist<TS...>, Nil>
        { using type = Typelist<TS...>; };
    template <typename TL, typename T> using Append = typename AppendType<TL, T>::type;

    template <typename... TLS> struct ConcatType;
    template <typename TL, typename... TLS> struct ConcatType<TL, TLS...>:
        ConcatType<TL, typename ConcatType<TLS...>::type> {};
    template <typename T1, typename T2> struct ConcatType<T1, T2>
        { using type = Typelist<T1, T2>; };
    template <typename T, typename... TS> struct ConcatType<T, Typelist<TS...>>
        { using type = Typelist<T, TS...>; };
    template <typename... TS, typename T> struct ConcatType<Typelist<TS...>, T>
        { using type = Typelist<TS..., T>; };
    template <typename... Types1, typename... Types2>
        struct ConcatType<Typelist<Types1...>, Typelist<Types2...>>
        { using type = Typelist<Types1..., Types2...>; };
    template <typename T> struct ConcatType<T>
        { using type = Typelist<T>; };
    template <typename... TS> struct ConcatType<Typelist<TS...>>
        { using type = Typelist<TS...>; };
    template <> struct ConcatType<Nil>
        { using type = Nil; };
    template <> struct ConcatType<>
        { using type = Nil; };
    template <typename... TLS> using Concat = typename ConcatType<TLS...>::type;

    template <typename T, typename TL> struct PrefixType;
    template <typename T, typename... TS> struct PrefixType<T, Typelist<TS...>>
        { using type = Typelist<T, TS...>; };
    template <typename T, typename TL> using Prefix = typename PrefixType<T, TL>::type;

    template <typename TL, typename T, template <typename, typename> class CP> struct InsertType;
    template <typename T1, typename... TS, typename T2, template <typename, typename> class CP>
        struct InsertType<Typelist<T1, TS...>, T2, CP>:
        std::conditional<CP<T1, T2>::value, typename PrefixType<T1,
            typename InsertType<Typelist<TS...>, T2, CP>::type>::type, Typelist<T2, T1, TS...>> {};
    template <typename T, template <typename, typename> class CP> struct InsertType<Nil, T, CP>
        { using type = Typelist<T>; };
    template <typename TL, typename T, template <typename, typename> class CP>
        using Insert = typename InsertType<TL, T, CP>::type;

    template <typename T, int N> struct RepeatType:
        PrefixType<T, typename RepeatType<T, N - 1>::type> {};
    template <typename T> struct RepeatType<T, 0>
        { using type = Nil; };
    template <typename T, int N> using Repeat = typename RepeatType<T, N>::type;

    template <typename TL, int N> struct RepeatListType:
        ConcatType<TL, typename RepeatListType<TL, N - 1>::type> {};
    template <typename TL> struct RepeatListType<TL, 0>
        { using type = Nil; };
    template <typename TL, int N> using RepeatList = typename RepeatListType<TL, N>::type;

    template <typename TL, int N, typename T> struct ResizeType;
    template <typename T1, typename... TS, int N, typename T2> struct ResizeType<Typelist<T1, TS...>, N, T2>:
        ConcatType<T1, typename ResizeType<Typelist<TS...>, N - 1, T2>::type> {};
    template <typename T1, typename... TS, typename T2> struct ResizeType<Typelist<T1, TS...>, 0, T2>
        { using type = Nil; };
    template <int N, typename T> struct ResizeType<Nil, N, T>:
        RepeatType<T, N> {};
    template <typename T> struct ResizeType<Nil, 0, T>
        { using type = Nil; };
    template <typename TL, int N, typename T> using Resize = typename ResizeType<TL, N, T>::type;

    template <typename TL, int N> struct SkipType;
    template <typename T, typename... TS, int N> struct SkipType<Typelist<T, TS...>, N>:
        SkipType<Typelist<TS...>, N - 1> {};
    template <typename T, typename... TS> struct SkipType<Typelist<T, TS...>, 0>
        { using type = Typelist<T, TS...>; };
    template <int N> struct SkipType<Nil, N>
        { using type = Nil; };
    template <> struct SkipType<Nil, 0>
        { using type = Nil; };
    template <typename TL, int N> using Skip = typename SkipType<TL, N>::type;

    template <typename TL, int N> struct TakeType;
    template <typename T, typename... TS, int N> struct TakeType<Typelist<T, TS...>, N>:
        ConcatType<T, typename TakeType<Typelist<TS...>, N - 1>::type> {};
    template <typename T, typename... TS> struct TakeType<Typelist<T, TS...>, 0>
        { using type = Nil; };
    template <int N> struct TakeType<Nil, N>
        { using type = Nil; };
    template <> struct TakeType<Nil, 0>
        { using type = Nil; };
    template <typename TL, int N> using Take = typename TakeType<TL, N>::type;

    template <typename TL, typename T, int N> struct InsertAtType:
        ConcatType<typename TakeType<TL, (N < 1 ? 0 : N)>::type, T,
            typename SkipType<TL, (N < 1 ? 0 : N)>::type> {};
    template <typename TL, typename T, int N> using InsertAt = typename InsertAtType<TL, T, N>::type;

    template <typename TL, int N1, int N2> struct SublistType:
        TakeType<typename SkipType<TL, N1>::type, N2> {};
    template <typename TL, int N1, int N2> using Sublist = typename SublistType<TL, N1, N2>::type;

    // Type selection metafunctions

    template <typename TL, int N> struct AtIndexType;
    template <typename T, typename... TS, int N> struct AtIndexType<Typelist<T, TS...>, N>:
        AtIndexType<Typelist<TS...>, (N > 0 ? N - 1 : -1)> {};
    template <typename T, typename... TS> struct AtIndexType<Typelist<T, TS...>, 0>
        { using type = T; };
    template <typename T, typename... TS> struct AtIndexType<Typelist<T, TS...>, -1>
        { using type = Nil; };
    template <int N> struct AtIndexType<Nil, N>
        { using type = Nil; };
    template <typename TL, int N> using AtIndex = typename AtIndexType<TL, N>::type;

    template <typename TL> struct HeadType;
    template <typename T, typename... TS> struct HeadType<Typelist<T, TS...>>
        { using type = T; };
    template <> struct HeadType<Nil>
        { using type = Nil; };
    template <typename TL> using Head = typename HeadType<TL>::type;

    template <typename TL> struct TailType;
    template <typename T, typename... TS> struct TailType<Typelist<T, TS...>>
        { using type = Typelist<TS...>; };
    template <> struct TailType<Nil>
        { using type = Nil; };
    template <typename TL> using Tail = typename TailType<TL>::type;

    template <typename TL> struct MostType;
    template <typename T, typename... TS> struct MostType<Typelist<T, TS...>>:
        PrefixType<T, typename MostType<Typelist<TS...>>::type> {};
    template <typename T> struct MostType<Typelist<T>>
        { using type = Nil; };
    template <> struct MostType<Nil>
        { using type = Nil; };
    template <typename TL> using Most = typename MostType<TL>::type;

    template <typename TL> struct LastType;
    template <typename T, typename... TS> struct LastType<Typelist<T, TS...>>:
        LastType<Typelist<TS...>> {};
    template <typename T> struct LastType<Typelist<T>>
        { using type = T; };
    template <> struct LastType<Nil>
        { using type = Nil; };
    template <typename TL> using Last = typename LastType<TL>::type;

    template <typename TL, template <typename, typename> class CP> struct MaxType;
    template <typename T, typename... TS, template <typename, typename> class CP>
        struct MaxType<Typelist<T, TS...>, CP>:
        MaxType<Typelist<T, typename MaxType<Typelist<TS...>, CP>::type>, CP> {};
    template <typename T1, typename T2, template <typename, typename> class CP>
        struct MaxType<Typelist<T1, T2>, CP>:
        std::conditional<CP<T1, T2>::value, T2, T1> {};
    template <typename T, template <typename, typename> class CP>
        struct MaxType<Typelist<T>, CP>
        { using type = T; };
    template <template <typename, typename> class CP> struct MaxType<Nil, CP>
        { using type = Nil; };
    template <typename TL, template <typename, typename> class CP>
        using Max = typename MaxType<TL, CP>::type;

    template <typename TL, template <typename, typename> class CP> struct MinType;
    template <typename T, typename... TS, template <typename, typename> class CP>
        struct MinType<Typelist<T, TS...>, CP>:
        MinType<Typelist<T, typename MinType<Typelist<TS...>, CP>::type>, CP> {};
    template <typename T1, typename T2, template <typename, typename> class CP>
        struct MinType<Typelist<T1, T2>, CP>:
        std::conditional<CP<T1, T2>::value, T1, T2> {};
    template <typename T, template <typename, typename> class CP>
        struct MinType<Typelist<T>, CP>
        { using type = T; };
    template <template <typename, typename> class CP> struct MinType<Nil, CP>
        { using type = Nil; };
    template <typename TL, template <typename, typename> class CP>
        using Min = typename MinType<TL, CP>::type;

    // Typelist transformation metafunctions

    template <typename TL, template <typename, typename> class BF, typename T> struct FoldLeftType;
    template <typename T1, typename... TS, template <typename, typename> class BF, typename T2>
        struct FoldLeftType<Typelist<T1, TS...>, BF, T2>:
        FoldLeftType<Typelist<TS...>, BF, BF<T2, T1>> {};
    template <template <typename, typename> class BF, typename T> struct FoldLeftType<Nil, BF, T>
        { using type = T; };
    template <typename TL, template <typename, typename> class BF, typename T>
        using FoldLeft = typename FoldLeftType<TL, BF, T>::type;

    template <typename TL, template <typename, typename> class BF, typename T> struct FoldRightType;
    template <typename T1, typename... TS, template <typename, typename> class BF, typename T2>
        struct FoldRightType<Typelist<T1, TS...>, BF, T2>:
        FoldRightType<typename MostType<Typelist<T1, TS...>>::type, BF,
            BF<typename LastType<Typelist<T1, TS...>>::type, T2>> {};
    template <template <typename, typename> class BF, typename T> struct FoldRightType<Nil, BF, T>
        { using type = T; };
    template <typename TL, template <typename, typename> class BF, typename T>
        using FoldRight = typename FoldRightType<TL, BF, T>::type;

    template <typename TL, template <typename> class UF> struct MapType;
    template <typename T, typename... TS, template <typename> class UF>
        struct MapType<Typelist<T, TS...>, UF>:
        PrefixType<UF<T>, typename MapType<Typelist<TS...>, UF>::type> {};
    template <template <typename> class UF> struct MapType<Nil, UF>
        { using type = Nil; };
    template <typename TL, template <typename> class UF> using Map = typename MapType<TL, UF>::type;

    template <typename TL, template <typename, typename> class BP,
            template <typename, typename> class BF>
        struct PartialReduceType;
    template <typename T1, typename T2, template <typename, typename> class BP,
            template <typename, typename> class BF, bool Match, typename... TS>
        struct PartialReduceHelperType:
        PartialReduceType<Typelist<BF<T1, T2>, TS...>, BP, BF> {};
    template <typename T1, typename T2, template <typename, typename> class BP,
            template <typename, typename> class BF, typename... TS>
        struct PartialReduceHelperType<T1, T2, BP, BF, false, TS...>:
        PrefixType<T1, typename PartialReduceType<Typelist<T2, TS...>, BP, BF>::type> {};
    template <typename T1, typename T2, typename... TS, template <typename, typename> class BP,
            template <typename, typename> class BF>
        struct PartialReduceType<Typelist<T1, T2, TS...>, BP, BF>:
        PartialReduceHelperType<T1, T2, BP, BF, BP<T1, T2>::value, TS...> {};
    template <typename T, template <typename, typename> class BP,
            template <typename, typename> class BF>
        struct PartialReduceType<Typelist<T>, BP, BF>
        { using type = Typelist<T>; };
    template <template <typename, typename> class BP, template <typename, typename> class BF>
        struct PartialReduceType<Nil, BP, BF>
        { using type = Nil; };
    template <typename TL, template <typename, typename> class BP, template <typename, typename> class BF>
        using PartialReduce = typename PartialReduceType<TL, BP, BF>::type;

    template <typename TL, template <typename, typename> class BF> struct ReduceType;
    template <typename T1, typename T2, typename... TS, template <typename, typename> class BF>
        struct ReduceType<Typelist<T1, T2, TS...>, BF>:
        ReduceType<Typelist<BF<T1, T2>, TS...>, BF> {};
    template <typename T, template <typename, typename> class BF> struct ReduceType<Typelist<T>, BF>
        { using type = T; };
    template <template <typename, typename> class BF> struct ReduceType<Nil, BF>
        { using type = Nil; };
    template <typename TL, template <typename, typename> class BF>
        using Reduce = typename ReduceType<TL, BF>::type;

    template <typename TL, typename T> struct RemoveType;
    template <typename T1, typename... TS, typename T2> struct RemoveType<Typelist<T1, TS...>, T2>:
        PrefixType<T1, typename RemoveType<Typelist<TS...>, T2>::type> {};
    template <typename T, typename... TS> struct RemoveType<Typelist<T, TS...>, T>:
        RemoveType<Typelist<TS...>, T> {};
    template <typename T> struct RemoveType<Nil, T>
        { using type = Nil; };
    template <typename TL, typename T> using Remove = typename RemoveType<TL, T>::type;

    template <typename TL, template <typename> class UP> struct RemoveIfType;
    template <typename T, typename... TS, template <typename> class UP>
        struct RemoveIfType<Typelist<T, TS...>, UP>:
        std::conditional<UP<T>::value, typename RemoveIfType<Typelist<TS...>, UP>::type,
            typename PrefixType<T, typename RemoveIfType<Typelist<TS...>, UP>::type>::type> {};
    template <template <typename> class UP> struct RemoveIfType<Nil, UP>
        { using type = Nil; };
    template <typename TL, template <typename> class UP>
        using RemoveIf = typename RemoveIfType<TL, UP>::type;

    template <typename TL> struct ReverseType;
    template <typename T, typename... TS> struct ReverseType<Typelist<T, TS...>>:
        AppendType<typename ReverseType<Typelist<TS...>>::type, T> {};
    template <typename T> struct ReverseType<Typelist<T, Nil>>
        { using type = Typelist<T, Nil>; };
    template <> struct ReverseType<Nil>
        { using type = Nil; };
    template <typename TL> using Reverse = typename ReverseType<TL>::type;

    template <typename TL, template <typename> class UP> struct SelectType;
    template <typename T, typename... TS, template <typename> class UP>
        struct SelectType<Typelist<T, TS...>, UP>:
        std::conditional<UP<T>::value, typename PrefixType<T,
            typename SelectType<Typelist<TS...>, UP>::type>::type,
            typename SelectType<Typelist<TS...>, UP>::type> {};
    template <template <typename> class UP> struct SelectType<Nil, UP>
        { using type = Nil; };
    template <typename TL, template <typename> class UP>
        using Select = typename SelectType<TL, UP>::type;

    template <typename TL, template <typename, typename> class CP> struct SortType;
    template <typename T, typename... TS, template <typename, typename> class CP>
        struct SortType<Typelist<T, TS...>, CP>:
        InsertType<typename SortType<Typelist<TS...>, CP>::type, T, CP> {};
    template <template <typename, typename> class CP> struct SortType<Nil, CP>
        { using type = Nil; };
    template <typename TL, template <typename, typename> class CP>
        using Sort = typename SortType<TL, CP>::type;

    template <typename TL> struct UniqueType;
    template <typename T, typename... TS> struct UniqueType<Typelist<T, T, TS...>>:
        UniqueType<Typelist<T, TS...>> {};
    template <typename T, typename... TS> struct UniqueType<Typelist<T, TS...>>:
        PrefixType<T, typename UniqueType<Typelist<TS...>>::type> {};
    template <> struct UniqueType<Nil>
        { using type = Nil; };
    template <typename TL> using Unique = typename UniqueType<TL>::type;

    template <typename TL> struct FullyUniqueType;
    template <typename T, typename... TS> struct FullyUniqueType<Typelist<T, TS...>>
        { using type = typename PrefixType<T,
            typename FullyUniqueType<typename RemoveType<Typelist<TS...>, T>::type>::type>::type; };
    template <> struct FullyUniqueType<Nil>
        { using type = Nil; };
    template <typename TL> using FullyUnique = typename FullyUniqueType<TL>::type;

    template <typename TL1, typename TL2, template <typename, typename> class BF> struct ZipType;
    template <typename T1, typename... Types1, typename T2, typename... Types2,
            template <typename, typename> class BF>
        struct ZipType<Typelist<T1, Types1...>, Typelist<T2, Types2...>, BF>:
        PrefixType<BF<T1, T2>, typename ZipType<Typelist<Types1...>, Typelist<Types2...>, BF>::type> {};
    template <typename TL, template <typename, typename> class BF> struct ZipType<TL, Nil, BF>
        { using type = Nil; };
    template <typename TL, template <typename, typename> class BF> struct ZipType<Nil, TL, BF>
        { using type = Nil; };
    template <template <typename, typename> class BF> struct ZipType<Nil, Nil, BF>
        { using type = Nil; };
    template <typename TL1, typename TL2, template <typename, typename> class BF>
        using Zip = typename ZipType<TL1, TL2, BF>::type;

    template <typename TL, template <typename, typename> class CP> struct MakeSetType:
        UniqueType<typename SortType<TL, CP>::type> {};
    template <typename TL, template <typename, typename> class CP>
        using MakeSet = typename MakeSetType<TL, CP>::type;

    // Other typelist operations

    template <typename TL> struct InheritTypelistHelperType;
    template <typename T, typename... TS> struct InheritTypelistHelperType<Typelist<T, TS...>>:
        T, InheritTypelistHelperType<Typelist<TS...>> {};
    template <> struct InheritTypelistHelperType<Nil> {};
    template <typename TL> struct InheritTypelistType
        { using type = InheritTypelistHelperType<typename FullyUniqueType<TL>::type>; };
    template <typename TL> using InheritTypelist = typename InheritTypelistType<TL>::type;

    template <typename TL> struct TypelistToTupleType;
    template <typename... TS> struct TypelistToTupleType<Typelist<TS...>>
        { using type = std::tuple<TS...>; };
    template <typename TL> using TypelistToTuple = typename TypelistToTupleType<TL>::type;

    template <typename T> struct TupleToTypelistType;
    template <typename... TS> struct TupleToTypelistType<std::tuple<TS...>>
        { using type = Typelist<TS...>; };
    template <typename T1, typename T2> struct TupleToTypelistType<std::pair<T1, T2>>
        { using type = Typelist<T1, T2>; };
    template <typename T> using TupleToTypelist = typename TupleToTypelistType<T>::type;

    // Typelist integer metafunctions

    template <typename TL, typename T> struct Count;
    template <typename T1, typename... TS, typename T2>
        struct Count<Typelist<T1, TS...>, T2>
        { static constexpr int value = std::is_same<T1, T2>::value
            + Count<Typelist<TS...>, T2>::value; };
    template <typename T> struct Count<Nil, T>
        { static constexpr int value = 0; };
    template <typename TL, typename T> constexpr int count = Count<TL, T>::value;

    template <typename TL, template <typename> class UP> struct CountIf;
    template <typename T, typename... TS, template <typename> class UP>
        struct CountIf<Typelist<T, TS...>, UP>
        { static constexpr int value = UP<T>::value + CountIf<Typelist<TS...>, UP>::value; };
    template <template <typename> class UP> struct CountIf<Nil, UP>
        { static constexpr int value = 0; };
    template <typename TL, template <typename> class UP>
        constexpr int count_if = CountIf<TL, UP>::value;

    template <typename TL, template <typename> class UP> struct CountIfNot;
    template <typename T, typename... TS, template <typename> class UP>
        struct CountIfNot<Typelist<T, TS...>, UP>
        { static constexpr int value = ! UP<T>::value + CountIfNot<Typelist<TS...>, UP>::value; };
    template <template <typename> class UP> struct CountIfNot<Nil, UP>
        { static constexpr int value = 0; };
    template <typename TL, template <typename> class UP>
        constexpr int count_if_not = CountIfNot<TL, UP>::value;

    template <typename TL, typename T> struct Find;
    template <typename T1, typename... TS, typename T2>
        struct Find<Typelist<T1, TS...>, T2>
        { static constexpr int value = (Find<Typelist<TS...>, T2>::value >= 0) ?
            Find<Typelist<TS...>, T2>::value + 1 : -1; };
    template <typename T, typename... TS> struct Find<Typelist<T, TS...>, T>
        { static constexpr int value = 0; };
    template <typename T> struct Find<Nil, T>
        { static constexpr int value = -1; };
    template <typename TL, typename T> constexpr int find = Find<TL, T>::value;

    template <typename TL, template <typename> class UP> struct FindIf;
    template <typename T, typename... TS, template <typename> class UP>
        struct FindIf<Typelist<T, TS...>, UP>
        { static constexpr int value = UP<T>::value ? 0 : (FindIf<Typelist<TS...>, UP>::value >= 0) ?
            FindIf<Typelist<TS...>, UP>::value + 1 : -1; };
    template <template <typename> class UP> struct FindIf<Nil, UP>
        { static constexpr int value = -1; };
    template <typename TL, template <typename> class UP>
        constexpr int find_if = FindIf<TL, UP>::value;

    template <typename TL, template <typename> class UP> struct FindIfNot;
    template <typename T, typename... TS, template <typename> class UP>
        struct FindIfNot<Typelist<T, TS...>, UP>
        { static constexpr int value = ! UP<T>::value ? 0 : (FindIfNot<Typelist<TS...>, UP>::value >= 0) ?
            FindIfNot<Typelist<TS...>, UP>::value + 1 : -1; };
    template <template <typename> class UP> struct FindIfNot<Nil, UP>
        { static constexpr int value = -1; };
    template <typename TL, template <typename> class UP>
        constexpr int find_if_not = FindIfNot<TL, UP>::value;

    template <typename TL> struct LengthOf;
    template <typename... TS> struct LengthOf<Typelist<TS...>>
        { static constexpr int value = sizeof...(TS); };
    template <typename TL> constexpr int length_of = LengthOf<TL>::value;

    // Typelist predicate metafunctions

    template <typename TL, template <typename> class UP> struct AllOf;
    template <typename T, typename... TS, template <typename> class UP>
        struct AllOf<Typelist<T, TS...>, UP>
        { static constexpr bool value = UP<T>::value && AllOf<Typelist<TS...>, UP>::value; };
    template <template <typename> class UP> struct AllOf<Nil, UP>:
        std::true_type {};
    template <typename TL, template <typename> class UP>
        constexpr bool all_of = AllOf<TL, UP>::value;

    template <typename TL, template <typename> class UP> struct AnyOf;
    template <typename T, typename... TS, template <typename> class UP>
        struct AnyOf<Typelist<T, TS...>, UP>
        { static constexpr bool value = UP<T>::value || AnyOf<Typelist<TS...>, UP>::value; };
    template <template <typename> class UP> struct AnyOf<Nil, UP>:
        std::false_type {};
    template <typename TL, template <typename> class UP>
        constexpr bool any_of = AnyOf<TL, UP>::value;

    template <typename TL, typename T> struct InList;
    template <typename T1, typename... TS, typename T2> struct InList<Typelist<T1, TS...>, T2>:
        InList<Typelist<TS...>, T2> {};
    template <typename T, typename... TS> struct InList<Typelist<T, TS...>, T>:
        std::true_type {};
    template <typename T> struct InList<Nil, T>:
        std::false_type {};
    template <typename TL, typename T> constexpr bool in_list = InList<TL, T>::value;

    template <typename TL> struct IsEmpty;
    template <typename... TS> struct IsEmpty<Typelist<TS...>>
        { static constexpr bool value = sizeof...(TS) == 0; };
    template <typename TL> constexpr bool is_empty = IsEmpty<TL>::value;

    template <typename TL> struct IsNotEmpty:
        std::false_type {};
    template <typename... TS> struct IsNotEmpty<Typelist<TS...>>:
        std::true_type {};
    template <> struct IsNotEmpty<Nil>:
        std::false_type {};
    template <typename TL> constexpr bool is_not_empty = IsNotEmpty<TL>::value;

    template <typename TL> struct IsUnique:
        std::true_type {};
    template <typename T, typename... TS> struct IsUnique<Typelist<T, TS...>>
        { static constexpr bool value = ! Count<Typelist<TS...>, T>::value
            && IsUnique<Typelist<TS...>>::value; };
    template <typename TL> constexpr bool is_unique = IsUnique<TL>::value;

    template <typename TL, template <typename> class UP> struct NoneOf;
    template <typename T, typename... TS, template <typename> class UP>
        struct NoneOf<Typelist<T, TS...>, UP>
        { static constexpr bool value = ! UP<T>::value && NoneOf<Typelist<TS...>, UP>::value; };
    template <template <typename> class UP> struct NoneOf<Nil, UP>:
        std::true_type {};
    template <typename TL, template <typename> class UP>
        constexpr bool none_of = NoneOf<TL, UP>::value;

}
