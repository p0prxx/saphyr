/* Saphyr, a C++ style compiler using LLVM
 * Copyright (C) 2009-2014, Justin Madru (justin.jdm64@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __CODE_CONTEXT_H__
#define __CODE_CONTEXT_H__

#include <stack>
#include <iostream>
#include <boost/program_options.hpp>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "Token.h"
#include "Value.h"
#include "Function.h"

using namespace boost::program_options;

struct LabelBlock
{
	BasicBlock* block;
	Token token;
	bool isPlaceholder;

	LabelBlock(BasicBlock* block, Token* token, bool isPlaceholder)
	: block(block), token(*token), isPlaceholder(isPlaceholder) {}
};

typedef unique_ptr<LabelBlock> LabelBlockPtr;
#define smart_label(block, token, placeholder) unique_ptr<LabelBlock>(new LabelBlock(block, token, placeholder))

class ScopeTable
{
	map<string, LValue> table;

public:
	void storeSymbol(LValue var, const string& name)
	{
		table[name] = var;
	}

	LValue loadSymbol(const string& name) const
	{
		auto varData = table.find(name);
		return varData != table.end()? varData->second : LValue();
	}
};

class SymbolTable
{
	ScopeTable globalTable;
	vector<ScopeTable> localTable;

public:
	void storeGlobalSymbol(LValue var, const string& name)
	{
		globalTable.storeSymbol(var, name);
	}

	void storeLocalSymbol(LValue var, const string& name)
	{
		localTable.back().storeSymbol(var, name);
	}

	void pushLocalTable()
	{
		localTable.push_back(ScopeTable());
	}

	void popLocalTable()
	{
		localTable.pop_back();
	}

	void clearLocalTable()
	{
		localTable.clear();
	}

	LValue loadSymbol(const string& name) const
	{
		for (auto it = localTable.rbegin(); it != localTable.rend(); it++) {
			auto var = it->loadSymbol(name);
			if (var)
				return var;
		}
		return globalTable.loadSymbol(name);
	}

	LValue loadSymbolCurr(const string& name) const
	{
		return localTable.empty()? globalTable.loadSymbol(name) : localTable.back().loadSymbol(name);
	}
};

class CodeContext : public SymbolTable
{
	friend class SType;
	friend class SFunction;
	friend class SUserType;
	friend class SStructType;
	friend class SUnionType;
	friend class SEnumType;

	vector<BasicBlock*> funcBlocks;
	vector<BasicBlock*> continueBlocks;
	vector<BasicBlock*> breakBlocks;
	vector<BasicBlock*> redoBlocks;
	map<string, LabelBlockPtr> labelBlocks;

	vector<pair<Token,string>> errors;

	Module* module;
	TypeManager typeManager;
	SFunction currFunc;

	void validateFunction()
	{
		for (auto& item : labelBlocks) {
			if (item.second->isPlaceholder)
				addError("label " + item.first + " not defined", &item.second->token);
		}
	}

	BasicBlock* loopBranchLevel(const vector<BasicBlock*>& branchBlocks, size_t level) const
	{
		auto idx = level > 0? branchBlocks.size() - level : abs(level) - 1;
		return (idx >= 0 && idx < branchBlocks.size())? branchBlocks[idx] : nullptr;
	}

public:
	explicit CodeContext(Module* module)
	: module(module), typeManager(module)
	{
	}

	operator LLVMContext&()
	{
		return module->getContext();
	}

	operator BasicBlock*() const
	{
		return currBlock();
	}

	Module* getModule() const
	{
		return module;
	}

	SFunction currFunction() const
	{
		return currFunc;
	}

	void addError(string error, Token* token)
	{
		if (token)
			errors.push_back({*token, error});
		else
			errors.push_back({Token(), error});
	}

	BasicBlock* currBlock() const
	{
		return funcBlocks.back();
	}

	void startFuncBlock(SFunction function)
	{
		pushLocalTable();
		funcBlocks.clear();
		funcBlocks.push_back(BasicBlock::Create(module->getContext(), "", function));
		currFunc = function;
	}

	void endFuncBlock()
	{
		validateFunction();

		clearLocalTable();
		funcBlocks.clear();
		continueBlocks.clear();
		breakBlocks.clear();
		redoBlocks.clear();
		labelBlocks.clear();

		currFunc = SFunction();
	}

	void pushBlock(BasicBlock* block)
	{
		block->moveAfter(currBlock());
		funcBlocks.push_back(block);
	}

	BasicBlock* createContinueBlock()
	{
		auto block = createBlock();
		continueBlocks.push_back(block);
		return block;
	}
	void popContinueBlock()
	{
		continueBlocks.pop_back();
	}
	BasicBlock* getContinueBlock(int level = 1) const
	{
		return loopBranchLevel(continueBlocks, level);
	}

	BasicBlock* createBreakBlock()
	{
		auto block = createBlock();
		breakBlocks.push_back(block);
		return block;
	}
	void popBreakBlock()
	{
		breakBlocks.pop_back();
	}
	BasicBlock* getBreakBlock(int level = 1) const
	{
		return loopBranchLevel(breakBlocks, level);
	}

	BasicBlock* createRedoBlock()
	{
		auto block = createBlock();
		redoBlocks.push_back(block);
		return block;
	}
	void popRedoBlock()
	{
		redoBlocks.pop_back();
	}
	BasicBlock* getRedoBlock(int level = 1) const
	{
		return loopBranchLevel(redoBlocks, level);
	}

	void popLoopBranchBlocks()
	{
		popBreakBlock();
		popContinueBlock();
		popRedoBlock();
	}

	LabelBlock* getLabelBlock(const string& name)
	{
		return labelBlocks[name].get();
	}
	LabelBlock* createLabelBlock(Token* name, bool isPlaceholder)
	{
		LabelBlockPtr &item = labelBlocks[name->str];
		if (!item.get()) {
			item = smart_label(createBlock(), name, isPlaceholder);
			item.get()->block->setName(name->str);
		}
		return item.get();
	}

	// NOTE: can only be used inside a function to add a new block
	BasicBlock* createBlock() const
	{
		return BasicBlock::Create(module->getContext(), "", currBlock()->getParent());
	}

	/*
	 * Returns true on errors
	 */
	bool handleErrors(string filename) const
	{
		if (errors.empty())
			return false;

		filename = filename.substr(filename.find_last_of('/') + 1);
		for (auto& error : errors) {
			if (error.first.line) {
				cout << error.first.filename << ":"
					<< error.first.line << ":";
			} else {
				cout << filename << ":";
			}
			cout << " " << error.second << endl;
		}
		cout << "found " << errors.size() << " errors" << endl;
		return true;
	}
};

#endif
