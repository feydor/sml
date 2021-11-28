#include "expr.h"
#include "smol.h"
#include <stdexcept>
#include <iostream>

llvm::Value*
NumberExprAST::code_gen(SMOL &smol)
{
    return llvm::ConstantFP::get(*smol.TheContext, llvm::APFloat(val));
}

llvm::Value*
VariableExprAST::code_gen(SMOL &smol)
{
    llvm::Value *val = smol.NamedValues[name]; // look up this var
    if (!val)
        throw std::invalid_argument("Unknown variable name.");
    return val;
}

llvm::Value*
BinaryExprAST::code_gen(SMOL &smol)
{
    llvm::Value *L = LHS->code_gen(smol);
    llvm::Value *R = RHS->code_gen(smol);
    if (!L || !R) return nullptr;

    // TODO: multi-character (non-ascii) operators?
    // store and switch by tokentype, not raw string
    switch(op[0]) {
        case '+': return smol.Builder->CreateFAdd(L, R, "addtmp");
        case '-': return smol.Builder->CreateFSub(L, R, "subtmp");
        case '*': return smol.Builder->CreateFMul(L, R, "multmp");
        case '<': {
            L = smol.Builder->CreateFCmpULT(L, R, "cmptmp"); // returns i1/boolean

            // Convert bool 0/1 to double 0.0 or 1.0
            return smol.Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*smol.TheContext), "booltmp");
        }
        case '>': {
            L = smol.Builder->CreateFCmpUGT(L, R, "cmptmp");
            return smol.Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*smol.TheContext), "booltmp");
        }
        default: throw std::invalid_argument("Unknown binary operator.");
    }
}

llvm::Value*
CallExprAST::code_gen(SMOL &smol)
{
    // look up name in global module table
    llvm::Function *calleef = smol.get_function(callee);
    
    // std::cout << "CallExprAST::code_gen: " << callee << std::endl;

    if (!calleef)
        throw std::invalid_argument("Unknown function called.");

    if (calleef->arg_size() != args.size())
        throw std::invalid_argument("Incorrect # of arguments passed.");

    std::vector<llvm::Value *> args_v;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        args_v.push_back(args[i]->code_gen(smol));
        if (!args_v.back())
            return nullptr;
    }
    return smol.Builder->CreateCall(calleef, args_v, "calltmp");
}

/**
 * @brief 
 * 
 * @param smol SMOL reference containg LLVM globals
 * @return llvm::Value* 
 */
llvm::Value*
IfExprAST::code_gen(SMOL &smol)
{
    llvm::Value *cond_val = this->conditional->code_gen(smol);
    if (!cond_val)
        return nullptr;
    
    // convert conditional to bool (compare with 0.0)
    cond_val = smol.Builder->CreateFCmpONE(
        cond_val, llvm::ConstantFP::get(*smol.TheContext, llvm::APFloat(0.0)), "ifcond");
    
    llvm::Function *the_function = smol.Builder->GetInsertBlock()->getParent();

    // create blocks for 'then' and 'else' cases
    // insert 'then' block at end of function
    llvm::BasicBlock *then_block = llvm::BasicBlock::Create(*smol.TheContext, "then", the_function);
    llvm::BasicBlock *else_block = llvm::BasicBlock::Create(*smol.TheContext, "else");
    llvm::BasicBlock *continue_block = llvm::BasicBlock::Create(*smol.TheContext, "ifcont");
    
    smol.Builder->CreateCondBr(cond_val, then_block, else_block);

    // emit 'then' value
    smol.Builder->SetInsertPoint(then_block);

    llvm::Value *then_val = this->then->code_gen(smol);
    if (!then_val)
        return nullptr;
    
    smol.Builder->CreateBr(continue_block); // fallthrough to continue_block
    then_block = smol.Builder->GetInsertBlock(); // get current block; can change in then->code_gen()

    // emit 'else' block
    the_function->getBasicBlockList().push_back(else_block); // adds block to the_function
    smol.Builder->SetInsertPoint(else_block); // set code into else_block

    llvm::Value *else_val = this->else_->code_gen(smol);
    if (!else_val)
        return nullptr;
    
    smol.Builder->CreateBr(continue_block); // fallthrough to continue_block
    else_block = smol.Builder->GetInsertBlock();

    // emit continue block
    the_function->getBasicBlockList().push_back(continue_block);
    smol.Builder->SetInsertPoint(continue_block);
    llvm::PHINode *phi_node = smol.Builder->CreatePHI(
        llvm::Type::getDoubleTy(*smol.TheContext), 2, "iftmp");
    phi_node->addIncoming(then_val, then_block);
    phi_node->addIncoming(else_val, else_block);
    return phi_node;
}
