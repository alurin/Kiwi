#ifndef KIWI_LANG_COMPOUNDNODE_INTERNAL
#define KIWI_LANG_COMPOUNDNODE_INTERNAL

#include "Node.hpp"
#include <vector>

namespace kiwi {
    class Type;
namespace lang {
    class MemberNode;
    class Driver;

    //==--------------------------------------------------------------------==//
    /// Base for member nodes
    class CompoundNode : public Node {
    public:
        /// remove nodes
        virtual ~CompoundNode();

        /// append member
        void append(MemberNode* member);

        /// void
        void inherit(const Identifier& name) { }

        /// void
        void implement(const Identifier& name) { }

        /// returns type
        Type* getType() const {
            return m_type;
        }

        /// generate types
        virtual void generateType(Driver& driver);

        /// generate members
        virtual void generateMembers(Driver& driver);

        /// generate code
        virtual void generateCode(Driver& driver);
    protected:
        /// Generated type
        Type* m_type;

        /// remove members
        std::vector<MemberNode*> m_members;

        /// internal constructor
        CompoundNode();

        /// internal constructor
        CompoundNode(Type* type);
    }; // class CompoundNode

    //==--------------------------------------------------------------------==//
    /// Class definition
    class ClassNode : public CompoundNode {
    public:
        /// constructor
        ClassNode(const Identifier& name);

        /// Generate type, only create analogs
        virtual void generateType(Driver& driver);
    protected:
        /// Class name
        Identifier m_name;
    }; // class ClassNode

    //==--------------------------------------------------------------------==//
    /// Class definition node for existed classes
    class ConcreteClassType : public CompoundNode {
    public:
        /// constructor
        ConcreteClassType(Type* type);
    protected:
    }; // class ConcreteClassType
}} // namespace kiwi::lang

#endif
