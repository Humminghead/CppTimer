#pragma once

#include <tuple>
#include <utility>

#include "timerbase.h"

namespace CppTimer {

// Stores a tuple of indices.  Used by tuple and pair, and by bind() to
// extract the elements in a tuple.
template <size_t... Indexes> struct TupleIndexes {};

// Builds an TupleIndexes<0, 1, 2, ..., _Num-1>.
// Concatenates two _Index_tuples.
template <typename _Itup1, typename _Itup2> struct TupleConcat;

template <size_t... _Ind1, size_t... _Ind2>
struct TupleConcat<TupleIndexes<_Ind1...>, TupleIndexes<_Ind2...>> {
  using type = TupleIndexes<_Ind1..., (_Ind2 + sizeof...(_Ind1))...>;
};

// Builds an _Index_tuple<0, 1, 2, ..., _Num-1>.
template <size_t _Num>
struct BuildIndexTuple
    : TupleConcat<typename BuildIndexTuple<_Num / 2>::type,
                  typename BuildIndexTuple<_Num - _Num / 2>::type> {};

template <> struct BuildIndexTuple<size_t{1}> { typedef TupleIndexes<0> type; };

template <> struct BuildIndexTuple<size_t{0}> { typedef TupleIndexes<> type; };

template <typename Fn, typename... Args> class CppTimer : public TimerBase {
private:
  // A call wrapper that does INVOKE(forwarded tuple elements...)
  template <typename _Tuple> struct Invoker {
    _Tuple _M_t;

    template <typename> struct result;
    template <typename _Fn, typename... _Args>
    struct result<std::tuple<_Fn, _Args...>>
        : std::invoke_result<_Fn, _Args...> {};

    template <size_t... _Ind>
    typename result<_Tuple>::type InvokeMe(TupleIndexes<_Ind...>) {
      return std::invoke(std::get<_Ind>(std::move(_M_t))...);
    }

    typename result<_Tuple>::type operator()() {
      using Indices =
          typename BuildIndexTuple<std::tuple_size<_Tuple>::value>::type;
      return InvokeMe(Indices());
    }
  };

  template <typename _Callable> class CallHoder {
    _Callable function_;

  public:
    template <typename... _Args>
    CallHoder(_Args &&...args) : function_{{std::forward<_Args>(args)...}} {}

    void invoke() { function_(); }
  };

  template <typename... Tp>
  using CallWrapper = Invoker<std::tuple<typename std::decay<Tp>::type...>>;

private:
  CallHoder<CallWrapper<Fn, Args...>> m_Handler;

public:
  CppTimer(const size_t seconds, Fn &&f, Args &&...args)
      : TimerBase(seconds),
        m_Handler(std::forward<Fn>(f), std::forward<Args>(args)...) {
    static_assert(std::is_invocable<typename std::decay<Fn>::type,
                                    typename std::decay<Args>::type...>::value,
                  "Arguments must be invocable after conversion to rvalues");

    setCallback([&](const std::string_view) { if(!isStoped()) m_Handler.invoke(); });
  }
};
} // namespace CppTimer
