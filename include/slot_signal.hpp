//
// Created by LCBHSStudent on 2020/2/25.
//

#ifndef SLOT_SIGNAL_SLOT_SIGNAL_H
#define SLOT_SIGNAL_SLOT_SIGNAL_H

#include <base_function.hpp>
#include <base_observer.hpp>

namespace van
{

template <typename RT, typename MT_Policy = ST_Policy>
class Signal;
template <typename RT, typename MT_Policy, typename... Args>
class Signal<RT(Args...), MT_Policy> final: public Observer<MT_Policy> {
    using observer = Observer<MT_Policy>;
    using function = Function<RT(Args...)>;

    template <typename T>
    void insert_sfinae(
    		Delegate_Key const& key,
    		typename T::Observer* instance
    ) {
        observer::insert(key, instance);
        instance->insert(key, this);
    }
    template <typename T>
    void remove_sfinae(
    		Delegate_Key const& key,
    		typename T::Observer* instance
    ) {
        observer::remove(key);
        instance->remove(key);
    }
    template <typename T>
    void insert_sfinae(Delegate_Key const& key, ...) {
        observer::insert(key, this);
    }
    template <typename T>
    void remove_sfinae(Delegate_Key const& key, ...) {
        observer::remove(key);
    }

    public:
 
    //-------------------------------------------------------------------CONNECT

    template <typename L>
    void connect(L* instance) {
        observer::insert(function::template bind(instance), this);
    }
    template <typename L>
    void connect(L& instance) {
        connect(std::addressof(instance));
    }

    template <RT(*fun_ptr)(Args...)>
    void connect() {
        observer::insert(function::template bind<fun_ptr>(), this);
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void connect(T* instance) {
        insert_sfinae<T>(function::template bind<mem_ptr>(instance), instance);
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void connect(T* instance) {
        insert_sfinae<T>(function::template bind<mem_ptr>(instance), instance);
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void connect(T& instance) {
        connect<mem_ptr, T>(std::addressof(instance));
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void connect(T& instance) {
        connect<mem_ptr, T>(std::addressof(instance));
    }

    template <auto mem_ptr, typename T>
    void connect(T* instance) {
        insert_sfinae<T>(function::template bind<mem_ptr>(instance), instance);
    }
    template <auto mem_ptr, typename T>
    void connect(T& instance) {
        connect<mem_ptr, T>(std::addressof(instance));
    }

    //----------------------------------------------------------------DISCONNECT

    template <typename L>
    void disconnect(L* instance) {
        observer::remove(function::template bind(instance));
    }
    template <typename L>
    void disconnect(L& instance) {
        disconnect(std::addressof(instance));
    }

    template <RT(*fun_ptr)(Args...)>
    void disconnect() {
        observer::remove(function::template bind<fun_ptr>());
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void disconnect(T* instance) {
        remove_sfinae<T>(function::template bind<mem_ptr>(instance), instance);
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void disconnect(T* instance) {
        remove_sfinae<T>(function::template bind<mem_ptr>(instance), instance);
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void disconnect(T& instance) {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void disconnect(T& instance) {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }

    template <auto mem_ptr, typename T>
    void disconnect(T* instance) {
        remove_sfinae<T>(function::template bind<mem_ptr>(instance), instance);
    }
    template <auto mem_ptr, typename T>
    void disconnect(T& instance) {
        disconnect<mem_ptr, T>(std::addressof(instance));
    }

    //----------------------------------------------------FIRE / FIRE ACCUMULATE

    template <typename... Uref>
    void emit(Uref&&... args) {
        observer::template for_each<function>(std::forward<Uref>(args)...);
    }

    template <typename Accumulate, typename... Uref>
    void emit_accumulate(Accumulate&& accumulate, Uref&&... args) {
        observer::template for_each_accumulate<function, Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Uref>(args)...);
    }
};

}

#endif