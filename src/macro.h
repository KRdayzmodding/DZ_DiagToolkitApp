#pragma once

/**
 * @brief Создаёт реактивную лямбду с автоматическим захватом и вызовом
 *
 * Использование:
 *   AUI_REACT_LAMBDA({
 *       return _new<ALabel>("Hello");
 *   })
 *
 * Раскрывается в:
 *   AUI_REACT([=]() -> _<AView> { return _new<ALabel>("Hello"); }())
 */
#define AUI_REACT_LAMBDA(...) AUI_REACT([=]() -> _<AView> __VA_ARGS__())

/**
 * @brief Создаёт Dynamic элемент с реактивным содержимым
 *
 * Использование:
 *   AUI_DYNAMIC({
 *       switch (*mState % 3) {
 *           case 0: return _new<AButton>("Button");
 *           case 1: return _new<ALabel>("Label");
 *           default: return nullptr;
 *       }
 *   })
 *
 * Раскрывается в:
 *   declarative::experimental::Dynamic {
 *       .content = AUI_REACT([=]() -> _<AView> { ... }())
 *   }
 */
#define AUI_DYNAMIC(...) \
    declarative::experimental::Dynamic { .content = AUI_REACT_LAMBDA(__VA_ARGS__) }
