/* Saphyr, a C++ style compiler using LLVM
 * Copyright (C) 2009-2017, Justin Madru (justin.jdm64@gmail.com)
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
#ifndef __CGNIMPORTLIST_H__
#define __CGNIMPORTLIST_H__

class CGNImportList
{
	typedef void (CGNImportList::*classPtr)(NStatement*);

	static classPtr *vtable;

	void visitNImportStm(NImportStm* stm);

	void ignore(NStatement* stm) {}

	static classPtr* buildVTable();

public:
	void visit(NStatement* type);

	static void run(NStatementList* list)
	{
		CGNImportList runner;
		for (auto item : *list)
			runner.visit(item);
	}
};

#endif