/* 
GPiler - types.cpp
Copyright (C) 2013 Jon Pry and Charles Cooper

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "node.h"
#include "codegen.h"
#include "parser.hpp"

using namespace std;


GType GetType(NBlock *pb, NExpression *exp){
	map<std::string, GType> locals;

	Node *parent = exp->parent;
	while(1){
		NFunctionDeclaration *funcp = dynamic_cast<NFunctionDeclaration*>(parent);
		if(funcp)
			break;
		parent = parent->parent;
	}
	NFunctionDeclaration *func = (NFunctionDeclaration *)parent;

	//Find return type for all functions
	{
		NodeList::iterator it;
		for(it=pb->children.begin(); it!= pb->children.end(); it++){
			if(*it != parent){
				NFunctionDeclaration *func = dynamic_cast<NFunctionDeclaration*>(parent);
				if(!func)
					continue;
				locals[func->id->name] = func->type->GetType(locals); 
			}
		}
	}

	//Add the functions arguments
	{
		VariableList::iterator it;
		for(it = func->arguments->begin(); it != func->arguments->end(); it++){
			locals[(*it)->id->name] = (*it)->type->GetType(locals);
		}
	}

	GType exptype;
	int found=0;
	func->block->GetType(locals,&exptype,&found,exp);

	return exptype;			
}

void printType(GType type){
	cout << type.type << ", " << type.length << "\n";
}
