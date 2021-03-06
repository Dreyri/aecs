#include <catch2/catch.hpp>

#include "aecs/component/type.hpp"

#include <deque>
#include <list>

struct nonempty_component
{
    int i, j, k;
};

struct tag_component
{};

struct custom_container_type
{
    using container_type = std::list<custom_container_type>;
};

struct custom_fn_container_type
{
    static auto make_container() noexcept(
        std::is_nothrow_constructible_v<std::deque<custom_fn_container_type>>)
    {
        return std::deque<custom_fn_container_type>{};
    }
};

TEST_CASE("make_container")
{
    using aecs::component_type;

    // creates vector
    auto nonempty_cont = component_type<nonempty_component>{}.make_container();
    static_assert(std::is_same_v<std::vector<nonempty_component>,
                                 decltype(nonempty_cont)>);

    static_assert(
        std::is_same_v<
            aecs::tag_container<tag_component>,
            decltype(aecs::component_type<tag_component>::make_container())>);

    // specified required type
    auto custom_cont = component_type<custom_container_type>::make_container();
    static_assert(std::is_same_v<std::list<custom_container_type>,
                                 decltype(custom_cont)>);

    // custom function to create container
    auto custom_fn_cont =
        component_type<custom_fn_container_type>::make_container();
    static_assert(std::is_same_v<std::deque<custom_fn_container_type>,
                                 decltype(custom_fn_cont)>);

    static_assert(
        std::is_same_v<decltype(custom_fn_cont),
                       decltype(component_type<custom_fn_container_type>{}
                                    .make_container())>);
}