#include <coroutine>
#include <type_traits>

struct Nec {};

static_assert(std::is_same_v<std::coroutine_handle<>, std::coroutine_handle<void>>);

// typed -> void dönüşümü olmalı:
static_assert(std::is_convertible_v<std::coroutine_handle<Nec>, std::coroutine_handle<>>);


//static_assert(std::is_convertible_v<std::coroutine_handle<>, std::coroutine_handle<Nec>>); // fails

template <typename Handle>
concept has_promise_member = requires (Handle h) {
	h.promise();
};

static_assert(has_promise_member<std::coroutine_handle<Nec>>);
static_assert(has_promise_member<std::coroutine_handle<>>);
	
