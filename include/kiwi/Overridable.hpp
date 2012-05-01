/*
 *******************************************************************************
 *   Copyright (C) 2012 Vasiliy Sheredeko
 *   MIT license. All Rights Reserved.
 *******************************************************************************
 */
#ifndef KIWI_OVERRIDABLE_INCLUDED
#define KIWI_OVERRIDABLE_INCLUDED

#include "kiwi/config.hpp"
#include <set>
#include <algorithm>

namespace kiwi {
    //==--------------------------------------------------------------------==//
    /// The class Overridable is mixin class for inhrited and overridable members,
    /// e.g. fieldss and methods
    template<typename T>
    class Overridable {
    public:
        /// set of overrides
        typedef std::set<T*>                            override_set;

        /// iterator for set of overrides
        typedef typename override_set::iterator         override_iterator;

        /// const iterator for set of overrides
        typedef typename override_set::const_iterator   override_const_iterator;

        /// Is override member from parent class?
        bool isDeclared() const {
            return m_isDeclared || !isOverride();
        }

        /// member is overrided member from base type?
        bool isOverride() const {
            return !m_overrides.empty();
        }

        /// Is override member from parent class?
        bool isOverride(T* member) const;
    protected:
        /// list of merged parent members
        override_set m_overrides;

        /// Field is declared in owner type?
        bool m_isDeclared;

        /// constructor
        Overridable(bool isDeclared);

        /// Override member in parent class with this fiels
        void override(T* member);

        /// Override member in parent class with this fiels
        void unoverride(T* member);
    };

    //==--------------------------------------------------------------------==//
    // constructor
    template<typename T>
    Overridable<T>::Overridable(bool isDeclared) : m_isDeclared(isDeclared) {
    }

    // Override member in parent class with this fiels
    template<typename T>
    void Overridable<T>::override(T* member) {
        m_overrides.insert(member);
    }

    // Remove override member in parent class with this fiels
    template<typename T>
    void Overridable<T>::unoverride(T* member) {
        override_iterator pos = std::find(m_overrides.begin(), m_overrides.end(), member);
        m_overrides.erase(pos);
    }

    // Override member in parent class with this fiels
    template<typename T>
    bool Overridable<T>::isOverride(T* member) const{
        return std::find(m_overrides.begin(), m_overrides.end(), member) != m_overrides.end();
    }
}

#endif
