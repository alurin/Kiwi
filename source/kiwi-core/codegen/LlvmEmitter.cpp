#include "LlvmEmitter.hpp"
#include "kiwi/DerivedTypes.hpp"
#include <llvm/InstrTypes.h>
#include <llvm/Instructions.h>
#include <llvm/Constants.h>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/ADT/ArrayRef.h>

using namespace kiwi;
using namespace kiwi::codegen;

// constructor
LlvmZeroUnaryOperator::LlvmZeroUnaryOperator(llvm::Instruction::BinaryOps opcode, TypeRef type)
: m_opcode(opcode), m_type(type) { }

// constructor
LlvmBinaryOperator::LlvmBinaryOperator(llvm::Instruction::BinaryOps opcode, TypeRef type)
: m_opcode(opcode), m_type(type) { }

// constructor
LlvmIntegerCompareOperator::LlvmIntegerCompareOperator(llvm::CmpInst::Predicate predicate, ContextRef context)
: m_predicate(predicate), m_context(context) { }

LlvmStringCompareOperator::LlvmStringCompareOperator(llvm::CmpInst::Predicate predicate, ContextRef context)
: m_predicate(predicate), m_context(context) { }

// emit instruction for interger unary operator
ExpressionGen LlvmZeroUnaryOperator::emit(const StatementGen& gen, const ExpressionGen& value)
{
    llvm::APInt cst(32, 0, false);
    llvm::ConstantInt* zero = llvm::ConstantInt::get(gen.getContext(), cst);

    llvm::Value* result = llvm::BinaryOperator::Create(m_opcode, zero, value.getValue(), "", gen.getBlock());
    return ExpressionGen(gen, m_type, result);
}

// emit instruction for binary operator
ExpressionGen LlvmBinaryOperator::emit(const StatementGen& gen, const ExpressionGen& left, const ExpressionGen& right)
{
    llvm::Value* result = llvm::BinaryOperator::Create(m_opcode, left.getValue(), right.getValue(), "", gen.getBlock());
    return ExpressionGen(gen, m_type, result);
}

// emit instruction for binary operator
ExpressionGen LlvmIntegerCompareOperator::emit(const StatementGen& gen, const ExpressionGen& left, const ExpressionGen& right)
{
    llvm::Value* result = new llvm::ICmpInst(*(gen.getBlock()), m_predicate, left.getValue(), right.getValue(), "");
    return ExpressionGen(gen, BoolType::get(m_context), result);
}
#include <iostream>
// emit instruction for binary operator
ExpressionGen LlvmStringCompareOperator::emit(const StatementGen& gen, const ExpressionGen& left, const ExpressionGen& right)
{
    // create stub for u_strCompare
    llvm::Module*       module = gen.getModule();
    llvm::LLVMContext& context = gen.getContext();
    llvm::Type*     lengthType = llvm::IntegerType::get(context, 32);
    llvm::Type*     charType   = llvm::IntegerType::get(context, 16)->getPointerTo();
    llvm::Type*     boolType   = llvm::IntegerType::get(context, 8);

    /// u_strCompare(const UChar *s1, int32_t length1, const UChar *s2, int32_t length2, UBool codePointOrder);
    llvm::Function* compare  = llvm::dyn_cast<llvm::Function>(module->getOrInsertFunction("u_strCompare", lengthType, charType, lengthType, charType, lengthType, boolType, NULL));

    // create variable for compare
    llvm::APInt cst(32, 0, false);
    llvm::ConstantInt* zero = llvm::ConstantInt::get(gen.getContext(), cst);

    cst = llvm::APInt(32, 1, false);
    llvm::ConstantInt* one = llvm::ConstantInt::get(gen.getContext(), cst);

    cst = llvm::APInt(8, 0, false);
    llvm::ConstantInt* falseBool = llvm::ConstantInt::get(gen.getContext(), cst);

    // prepare GetElementPtrInst indexes
    std::vector<llvm::Value*> lengthIdx; // length
    lengthIdx.push_back(zero);
    lengthIdx.push_back(zero);

    std::vector<llvm::Value*> bufferIdx; // buffer
    bufferIdx.push_back(zero);
    bufferIdx.push_back(one);

    // find first string for method
    llvm::Value* string1   = llvm::GetElementPtrInst::CreateInBounds(left.getValue(), makeArrayRef(bufferIdx), "string1", gen.getBlock());
    llvm::Value* position1 = llvm::GetElementPtrInst::CreateInBounds(left.getValue(), makeArrayRef(lengthIdx), "", gen.getBlock());
    llvm::Value* length1   = new llvm::LoadInst(position1, "length1", gen.getBlock());
    string1                = new llvm::BitCastInst(string1, charType, "", gen.getBlock());

    // find first string for method
    llvm::Value* string2   = llvm::GetElementPtrInst::CreateInBounds(right.getValue(), makeArrayRef(bufferIdx), "string2", gen.getBlock());
    llvm::Value* position2 = llvm::GetElementPtrInst::CreateInBounds(right.getValue(), makeArrayRef(lengthIdx), "", gen.getBlock());
    llvm::Value* length2   = new llvm::LoadInst(position2, "length2", gen.getBlock());
    string2                = new llvm::BitCastInst(string2, charType, "", gen.getBlock());

    // collect arguments
    std::vector<llvm::Value*> args;
    args.push_back(string1);
    args.push_back(length1);
    args.push_back(string2);
    args.push_back(length2);
    args.push_back(falseBool);

    // compute string compare and return result
    llvm::Value* value  = llvm::CallInst::Create(compare, makeArrayRef(args), "", gen.getBlock());
    llvm::Value* result = new llvm::ICmpInst(*(gen.getBlock()), m_predicate, zero, value, "");
    return ExpressionGen(gen, BoolType::get(m_context), result);
}

ExpressionGen LlvmIntegerPrintOperator::emit(const StatementGen& gen, const ExpressionGen& value)
{
    llvm::Module*       module = gen.getModule();
    llvm::LLVMContext& context = gen.getContext();
    llvm::Type*       voidType = llvm::Type::getVoidTy(context);
    llvm::Type*           type = value.getType()->getVarType();

    llvm::Function* print   = llvm::dyn_cast<llvm::Function>(module->getOrInsertFunction("kiwi_print_integer", voidType, type, NULL));

    std::vector<llvm::Value*> args;
    args.push_back(value.getValue());

    llvm::CallInst::Create(print, makeArrayRef(args), "", gen.getBlock());

    return ExpressionGen(gen, value.getType(), value.getValue());
}

ExpressionGen LlvmBoolPrintOperator::emit(const StatementGen& gen, const ExpressionGen& value)
{
    llvm::Module*       module = gen.getModule();
    llvm::LLVMContext& context = gen.getContext();
    llvm::Type*       voidType = llvm::Type::getVoidTy(context);
    llvm::Type*           type = value.getType()->getVarType();

    llvm::Function* print   = llvm::dyn_cast<llvm::Function>(module->getOrInsertFunction("kiwi_print_bool", voidType, type, NULL));

    std::vector<llvm::Value*> args;
    args.push_back(value.getValue());

    llvm::CallInst::Create(print, makeArrayRef(args), "", gen.getBlock());

    return ExpressionGen(gen, value.getType(), value.getValue());
}

ExpressionGen LlvmCharPrintOperator::emit(const StatementGen& gen, const ExpressionGen& value)
{
    llvm::Module*       module = gen.getModule();
    llvm::LLVMContext& context = gen.getContext();
    llvm::Type*       voidType = llvm::Type::getVoidTy(context);
    llvm::Type*           type = value.getType()->getVarType();

    llvm::Function* print   = llvm::dyn_cast<llvm::Function>(module->getOrInsertFunction("kiwi_print_char", voidType, type, NULL));

    std::vector<llvm::Value*> args;
    args.push_back(value.getValue());

    llvm::CallInst::Create(print, makeArrayRef(args), "", gen.getBlock());

    return ExpressionGen(gen, value.getType(), value.getValue());
}

ExpressionGen LlvmStringPrintOperator::emit(const StatementGen& gen, const ExpressionGen& value)
{
    // create stub for u_strCompare
    llvm::Module*       module = gen.getModule();
    llvm::LLVMContext& context = gen.getContext();
    llvm::Type*     lengthType = llvm::IntegerType::get(context, 32);
    llvm::Type*     charType   = llvm::IntegerType::get(context, 16)->getPointerTo();
    llvm::Type*     voidType   = llvm::Type::getVoidTy(context);

    /// kiwi_print_string(const UChar *s, int32_t length);
    llvm::Function* print   = llvm::dyn_cast<llvm::Function>(module->getOrInsertFunction("kiwi_print_string", voidType, charType, lengthType, NULL));

    LlvmStringEmitter string;
    string.emit(gen, value);

    // collect arguments
    std::vector<llvm::Value*> args;
    args.push_back(string.getBufferValue());
    args.push_back(string.getLengthValue());

    // compute string compare and return result
    llvm::CallInst::Create(print, makeArrayRef(args), "", gen.getBlock());
    return ExpressionGen(gen, value.getType(), value.getValue());
}

/// emit reduce string as size and buffer
StatementGen LlvmStringEmitter::emit(const StatementGen& gen, const ExpressionGen& value)
{
    llvm::Module*       module = gen.getModule();
    llvm::LLVMContext& context = gen.getContext();
    llvm::Type*     lengthType = llvm::IntegerType::get(context, 32);
    llvm::Type*     charType   = llvm::IntegerType::get(context, 16)->getPointerTo();

    // create variable for compare
    llvm::APInt cst(32, 0, false);
    llvm::ConstantInt* zero = llvm::ConstantInt::get(gen.getContext(), cst);

    cst = llvm::APInt(32, 1, false);
    llvm::ConstantInt* one = llvm::ConstantInt::get(gen.getContext(), cst);

    // prepare GetElementPtrInst indexes
    std::vector<llvm::Value*> lengthIdx; // length
    lengthIdx.push_back(zero);
    lengthIdx.push_back(zero);

    std::vector<llvm::Value*> bufferIdx; // buffer
    bufferIdx.push_back(zero);
    bufferIdx.push_back(one);
    // bufferIdx.push_back(zero);

    // find first string for method
    llvm::Value* length = llvm::GetElementPtrInst::Create(value.getValue(), makeArrayRef(lengthIdx), "", gen.getBlock());
    length              = new llvm::LoadInst(length, "length", gen.getBlock());
    llvm::Value* buffer = llvm::GetElementPtrInst::Create(value.getValue(), makeArrayRef(bufferIdx), "", gen.getBlock());
    buffer              = new llvm::BitCastInst(buffer, charType, "buffer", gen.getBlock());

    // store and return
    m_lengthValue = length;
    m_bufferValue = buffer;
    return gen;
}