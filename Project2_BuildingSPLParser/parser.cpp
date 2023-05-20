/* Implementation of Recursive-Descent Parser
 * parser.cpp
 * Programming Assignment 2
 * Spring 2023
*/

#include "parser.h"


map<string, bool> defVar; //The key of the defVar is a variable name, and the value is a Boolean that is set to true when the first time the variable has been initialized, otherwise it is false
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}



//DONE
//Prog ::= StmtList
bool Prog(istream& in, int& line){
	
	bool status = StmtList(in,line);

	if(!status){
		ParseError(line, "Missing Program");
		return false;
	}
	else{
		cout<< "(DONE)" <<endl;
		return true;
	}

	
	
	return status;
}


//DONE
//Something is wrong in my StmtList
//StmtList ::= Stmt ;{ Stmt; }
bool StmtList(istream& in, int& line){
	LexItem t;
	
	
	bool status = Stmt(in,line);

	if(!status){
		ParseError(line,"Error in program STMT");
		return false;
	}
	else if(status){
		t = Parser::GetNextToken(in,line);
		while(t == SEMICOL){
			status = Stmt(in,line);
			if(!status){
				break;
			}
			else t = Parser::GetNextToken(in,line);
		}
		if(status && t!= SEMICOL){
			ParseError(line, "Missing semicol end of stmt");
			return false;
		}
		else if(!status && ErrCount() > 0){
			ParseError(line, "Error in program stmt");
			return false;
		}

	}
	return true;
	

}

//DONE
//Stmt ::= AssignStme | WriteLnStmt | IfStmt
bool Stmt(istream& in, int& line){
	
	LexItem t;
	t = Parser::GetNextToken(in,line);
	

	if(t.GetToken() == SIDENT || t.GetToken() == NIDENT){//Maybe needs IDENT
		Parser::PushBackToken(t);
		if(AssignStmt(in,line) == false){
			ParseError(line, "Assigngment statement wrong");
			return false;
		}
		else return true;
	}
	else if(t == WRITELN){
		if(WritelnStmt(in,line) == false){
			ParseError(line,"Incorrect Writln");
			return false;
		}
		else return true;
	}
	else if(t == IF){
		if(IfStmt(in,line) == false){
			ParseError(line, "If-Statement Wrong");
			return false;
		}
		else return true;
	}
	else{
		Parser::PushBackToken(t);
		return false;

	}
	return true;
	//Can assign the assignstm, writlnstmt etc to variable and return here


}
//DONE MIGHT NEED FIXING 
//Might need to be fixed
//everything is [] os optional
//IfStmt ::= if (Expr) ‘{‘ StmtList ‘}’ [ else ‘{‘ StmtList ‘}’ ]
bool IfStmt(istream& in, int& line){
	LexItem t;
	t = Parser::GetNextToken(in,line);
	

	//if(t.GetToken() != IF){
		//ParseError(line)
	//}

	if(t != LPAREN){
		ParseError(line, "LPAREN midding in if statement");//Fix
		return false;
	}
	bool status = Expr(in,line);
	if(!status){
		ParseError(line, "Missing EXPR");//Fix
		return false;
	}

	t = Parser::GetNextToken(in,line);

	if(t != RPAREN){
		ParseError(line, "RPAREN Missing in if statement");//Fix
		return false;
	}

	t = Parser::GetNextToken(in,line);

	if(t != LBRACES){
		ParseError(line, "LBRACE missing in if statement");//Fix
		return false;
	}

	
	bool status_2 = StmtList(in,line);
	if(!status_2){
		ParseError(line, "Missing STMTLIST");//Fix
		return false;
	}

	t = Parser::GetNextToken(in,line);

	if(t != RBRACES){
		ParseError(line, "RBRACE missing in if statement");//Fix
		return false;
	}

	t = Parser::GetNextToken(in,line);

	if(t.GetToken() == ELSE){
		t= Parser::GetNextToken(in,line);
		if(t.GetToken() != LBRACES){
			ParseError(line, "LBRACES missing in else statement");
			return false;
		}
			status_2 = StmtList(in,line);
			if(!status_2){
				ParseError(line, "MISSING STMTELSE in else statement");
				return false;
			}
			t = Parser::GetNextToken(in,line);
			if(t.GetToken() !=RBRACES){
				
				ParseError(line, "MISSING RBRACES in else statement");
				return false;

				
			}
	
	}
		
	else{
		Parser::PushBackToken(t);
	}

	
	return true;
	
	
	
}
//DONE
//AssignStmt ::= Var = Expr
bool AssignStmt(istream& in, int& line){
	LexItem t;
	bool status = false;
	bool Varstatus = Var(in,line);


	if(Varstatus){
		t = Parser::GetNextToken(in,line);

		if(t.GetToken() == ASSOP){
			status = Expr(in,line);
			if(!status){
				ParseError(line,"MISSING EXPR in ASignStmt");
				return false;
			}
			else defVar[t.GetLexeme()] = true; 

		}
		else if(t.GetToken() == ERR){
			ParseError(line, "UNRECOGNIZED INPUT PATTER");
			return false;
		}
		else{
			ParseError(line, "MISSING ASSIGMENT OPERATOR");
			return false;
		}
	}
	else{
		ParseError(line, "MISSING left-hand side VAR in AssignStmt");
		return false;
	}
	return status;

	
	
	

}
//DONE
//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line){
	LexItem t;
	t = Parser::GetNextToken(in, line);
	if(t== NIDENT || t== SIDENT){
		defVar[t.GetLexeme()] = true;
		return true;
		//if(!(defVar.find(t.GetLexeme()) -> second)){
			//ParseError(line, "VARIABLE ERROR");
			//return false;

		//}
		//return true;
	}
	else{
		ParseError(line, "UNRECOGNIZED IMPUT PATTERN");
		return false;
	}
	return true;
}

//DONE
//Expr ::= RelExpr [(-eq|==) RelExpr ]
bool Expr(istream& in, int& line){
	LexItem t;
	


	bool status = RelExpr(in,line);
	if(!status){
		ParseError(line, "MISSING RELEXPR");
		return false;
	}

	t = Parser::GetNextToken(in, line);

	if(t.GetToken() == SEQ || t.GetToken() == NEQ){
		status = RelExpr(in,line);//Fix
		if(!status){
			ParseError(line, "MISSING RELEXPR AFTER equal");
			return false;
		}
		else return true;
	}
	else{
		Parser::PushBackToken(t);
		
	}
	return true;
	


}

//DONE
//After addexpr is optional
//RelExpr ::= AddExpr [ ( -lt | -gt | < | > ) AddExpr ]
bool RelExpr(istream& in, int& line){
	LexItem t;
	

	bool status = AddExpr(in,line);
	if(!status){
		ParseError(line, "MISSING ADDEXPR in rel expr.");
		return false;
	}

	t = Parser::GetNextToken(in, line);

	if(t == SLTHAN || t == SGTHAN || 
		t == NLTHAN || t == NGTHAN){
			status = AddExpr(in,line);
			if(!status){
				ParseError(line, "MISSING ADDEXPR AFTER operator");
				return false;
			}
			else return true;
		}
		else{
			Parser::PushBackToken(t);
			
		}
		return true;
}

//DONE might need fixing
//Change this a bit
//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line){
	bool status = MultExpr(in,line);
	LexItem t;
	
	if(!status){
		ParseError(line, "MISSING MULTEXPR in AddExpr.");
		return false;
	}
	
	t = Parser::GetNextToken(in,line);
	if(t.GetToken() == PLUS || t.GetToken() == MINUS || t.GetToken() == CAT){
		status = MultExpr(in,line);
		if(!status){
			ParseError(line, "Missing multExpr after operator");
			return false;
		}


	}

	else{
		Parser::PushBackToken(t);
		
	}
	return true;
	

	

}
//Done might need fixing
//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line){
	LexItem t;
	
	bool status = ExponExpr(in,line);

	if(!status){
		ParseError(line, "Missing EXPONEXPR in MultExpr.");
		return false;

	}
	t = Parser::GetNextToken(in,line);
	if(t.GetToken() == MULT || t.GetToken() == DIV || t.GetToken() == SREPEAT){
		status = ExponExpr(in,line);
		if(!status){
			ParseError(line, "Missing ExponExpr after operator");
			return false;
		}

	}

	else{
		Parser::PushBackToken(t);
		
	}
	return true;


	
}
//Done Might need fixing 
//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
bool ExponExpr(istream& in, int& line){
	bool status = UnaryExpr(in, line);
	if(!status){
		ParseError(line, "Missing UNARYEXPR in ExponExpr.");
		return false;
	}
	
	LexItem t = Parser::GetNextToken(in, line);
	
	if (t.GetToken() == EXPONENT) {
		
		status = UnaryExpr(in, line);
		if(!status){
			ParseError(line, "Missing UnaryExpr after Exponent");
			return false;
		}

		
	}

	else{
		Parser::PushBackToken(t);
		
	}
	return true;
}


//Done
//UnaryExpr ::= [( - | + )] PrimaryExpr
bool UnaryExpr(istream& in, int& line){
	bool status;
	LexItem t;
	int sign = 0;
	t = Parser::GetNextToken(in,line);

	if(t == MINUS){
		sign = -1;
	}
	else if(t == PLUS){
		sign = 1;
	}
	else{
		Parser::PushBackToken(t);
	}
	status = PrimaryExpr(in,line,sign);
	return status;

	//if(t.GetToken() == MINUS || t.GetToken() == PLUS){
		//status = PrimaryExpr(in,line,0);

	//}
	//else if(t.GetToken() == ERR){
		//ParseError(line, "ERR");
		//return false;
	//}
	//else{
		//Parser::PushBackToken(t);
		//status = PrimaryExpr(in,line,0);
	//}
	//return status;
	

}
//Done
//PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign){
	LexItem t;
	t = Parser::GetNextToken(in,line);
	if(t != IDENT && t != SIDENT && 
	t != NIDENT && t != ICONST && 
	t != RCONST && t != SCONST ){
		if(t.GetToken() != LPAREN){
			ParseError(line, "MISSING LPAREN");
			return false;
		}
		bool status = Expr(in,line);
		if(!status){
			ParseError(line, "MISSING EXPR");
			return false;
		}
		t = Parser::GetNextToken(in,line);
		if(t.GetToken() != RPAREN){
			ParseError(line, "MISSING RPAREN");
			return false;
		}
		

	}
	else if (t == IDENT || t == SIDENT || t == NIDENT) {
		if (!(defVar.find(t.GetLexeme())->second)) {
            ParseError(line, "Undefined Variable");
            return false;
        }
	}
    return true;
}
	
	

//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in Writeln Stmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return true;
}//End of WritelnStmt




//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList


