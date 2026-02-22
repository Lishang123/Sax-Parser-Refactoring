#ifndef CPP_TASK_FUSPEC_HPP
#define CPP_TASK_FUSPEC_HPP
#include <string_view>
#include <span>

template <class Spec>
concept FUSpec = requires(
                           std::string_view tagName,
                           const char* text,
                           typename Spec::Element element,
                           typename Spec::Element parentElement,
                           typename Spec::Result& out,
                           typename Spec::State& state,
                           std::string_view repo)
{
    typename Spec::Element;
    typename Spec::Result;
    typename Spec::State;

    requires std::is_enum_v<typename Spec::Element>; // Spec::Element should be an enum type

    { Spec::documentRoot() } -> std::same_as<typename Spec::Element>;
    { Spec::toElement(tagName) } -> std::same_as<typename Spec::Element>;

    { Spec::isKnown(element) } -> std::same_as<bool>;
    { Spec::elementName(element) } -> std::same_as<std::string_view>;

    { Spec::allowedChildren(parentElement)} -> std::same_as<std::span<typename Spec::Element>>;
    { Spec::onText(element, text, out, state, repo) } -> std::same_as<void>;

    //for sax parsers in general
    // { Spec::onStart(element, out, state, repo) } -> std::same_as<void>;
    // { Spec::onEnd(element, out, state, repo) } -> std::same_as<void>;
};

#endif