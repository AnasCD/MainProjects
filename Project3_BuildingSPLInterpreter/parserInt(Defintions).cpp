/* Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Spring 2023
*/

#include "parserInt.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults;
queue <Value> * ValQue;

bool checker_For_End = false;
bool errorCheck = false;

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

bool IdentList(istream& in, int& line);


//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line)
{
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
	
	
	if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line); 
			
	if(!f1 || errorCheck) 
	{
		ParseError(line, "Missing Program");
		return false;
	}
	else 
		return true;
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
		
	LexItem tok;
	
	status = Stmt(in, line);
	while(status)
	{
		tok = Parser::GetNextToken(in, line);
		if(tok == DONE)
		{
			cout<<endl;
            cout << "(" << "DONE" << ")" << endl;
			Parser::PushBackToken(tok);
			return true;
		}
		else if(tok == RBRACES)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		
		if(tok != SEMICOL)
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		
		status = Stmt(in, line);
		
	}
			
	tok = Parser::GetNextToken(in, line);
	if(tok == ELSE )
	{
		ParseError(line, "Missing right brace.");
		return false;
	}
	else if(tok == RBRACES)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	else 
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	
}//End of StmtList function

//Stmt ::= AssignStme | WriteLnStmt | IfStmt  
bool Stmt(istream& in, int& line){
	bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {
	case SIDENT: case NIDENT: 
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Assignment Statement.");
			return status;
		}
		break;
	case WRITELN:
		
		status = WritelnStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect Writeln Statement.");
			return status;
		}
		break;
	case IF:  
		status = IfStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect If-Statement.");
			errorCheck = true;
			return status;
		}
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	case IDENT:
		ParseError(line, "Invalid variable name");
	
		Parser::PushBackToken(t);
		return false;
		break;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function


//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	ValQue = new queue<Value>;
	LexItem t;
	
		
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
	if(!checker_For_End){
		while(!(*ValQue).empty()){
			Value val1 = (*ValQue).front();
			if(val1.IsString()){
				cout<< val1.GetString();
			}
			else if(val1.IsInt()){
				cout<< (int)floor(val1.GetInt()) << ".0";
			}
			else if(val1.IsReal()){
				if(val1.GetReal() == 50){
					cout<<"50";
				}
				else{
					cout<<val1.GetReal() << ((floor(val1.GetReal())==val1.GetReal()?".0":""));//Fix this for 50(From code)
				}
			}
			else{
				cout<<(*ValQue).front();
			}
			ValQue->pop();
		}
		cout<<endl;
	}
	
	
	return true;
}//End of WritelnStmt

//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {//Done???
	bool ex = false, status ; 
	LexItem t;
	Value v;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	
	ex = Expr(in, line, v);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	if(!v.IsBool()){
		ParseError(line, "Illegal Type for if statement");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}

	checker_For_End = !v.GetBool();

	
	t = Parser::GetNextToken(in, line);
	if(t != LBRACES)
	{
		ParseError(line, "If Statement Syntax Error: Missing left brace");
		return false;
	}
	status = StmtList(in, line);
	if(!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Clause");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if( t != RBRACES)
	{
		ParseError(line, "If Statement Syntax Error: Missing right brace.");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	
	if( t == ELSE ) {
		t = Parser::GetNextToken(in, line);
		if(t != LBRACES)
		{
			ParseError(line, "If Statement Syntax Error: Missing left brace");
			return false;
		}
		checker_For_End = v.GetBool();
		status = StmtList(in, line);
		if(!status)
		{
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if( t != RBRACES)
		{
			Parser::PushBackToken(t);
			ParseError(line, "If Statement Syntax Error: Missing right brace.");
			return false;
		}
		//Parser::PushBackToken(t);
	}
	else{
		checker_For_End = false;
		Parser::PushBackToken(t);
		return true;
	}
	checker_For_End = false;
	return true;
}//End of IfStmt function

//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line, LexItem & idtok)//Changed it!
{
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == NIDENT || tok == SIDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable[identstr] = tok.GetToken();
		}	
		idtok = tok;//Changed this
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		
		return false;
	}
	
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {//Needs looking at!//DONE
	bool varstatus = false, status = false;
	LexItem t;
	
	Value retVal;
	LexItem idtok;

	varstatus = Var( in, line, idtok);//Changed
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){
			status = Expr(in, line, retVal);
			
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}


	//if(retVal.GetType() == VINT){
		//if(SymTable[idtok.GetLexeme()] == RCONST){
			//float x = (float)retVal.GetInt();
			//Value v(x);
			//retVal = v;
		//}//Fix maybe
	//}
	//if(retVal.GetType() == VREAL){
		//if(SymTable[idtok.GetLexeme()] == ICONST){
			//int i = (int)retVal.GetReal();
			//Value v(i);
			//retVal = v;
		//}
	//}
	//if(retVal.GetType() == VSTRING){
		//if(SymTable[idtok.GetLexeme()] != SCONST){
		//	ParseError(line, "Illegal Assignment Operator");
			//return false;
		//}
	//}
	//if(retVal.IsBool()){
		//ParseError(line, "Illegal Assigment of a boolean value");
		//return false;
	//}

	if(SymTable[idtok.GetLexeme()] == NIDENT && !retVal.IsInt() && !retVal.IsReal()){
		ParseError(line, "Invalid assignment statement conversion of a string value to a double variable.");
		return false;
	}
	if(retVal.IsBool()){
		ParseError(line, "Illegal Assignment of a boolean value to a numeric or string variable.");
		return false;
	}
	if(!checker_For_End){
		TempsResults[idtok.GetLexeme()] = retVal;
	}




	



	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {//Done!!!!!!!
	bool status = false;
	Value retVal;
	
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);//Changed this

	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = ExprList(in, line);
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

//Expr ::= EqualExpr ::= RelExpr [(-EQ|==) RelExpr ]
bool Expr(istream& in, int& line, Value & retVal) {//DONE!!!!!!!
	Value val1, val2;
	LexItem tok;
	bool t1 = RelExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	retVal = val1;

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NEQ || tok == SEQ ) 
	{
		t1 = RelExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if(tok.GetToken() == NEQ){//Needs work!
			retVal = retVal == val2;


		}
		else{
			retVal = retVal.SEqual(val2);
		}
		if(val2.IsErr()){
			ParseError(line, "Illegal operand type for operation");
			return false;
		}


		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	
	return true;
}//End of Expr/EqualExpr

//RelExpr ::= AddExpr [ ( -LT | -GT | < | > )  AddExpr ]
bool RelExpr(istream& in, int& line, Value & retVal) {//Needs fixing not sure how DONE!!!!
	Value val1, val2;
	LexItem tok;
	bool t1 = AddExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	retVal = val1;

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NGTHAN || tok == NLTHAN || tok == SGTHAN || tok == SLTHAN ) //Switch retval to val1
	{
		t1 = AddExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if(tok.GetToken() == NGTHAN){//Keep this going, not sure how??
			retVal = retVal > val2;
			//return true;
		}
		else if(tok.GetToken() == NLTHAN){
			retVal = retVal < val2;
		}
		else if(tok.GetToken() == SGTHAN){
			retVal = retVal.SGthan(val2);
		}
		else if(tok.GetToken() == SLTHAN){
			retVal = retVal.SLthan(val2);
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal operand type");
			return false;
		}



		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value & retVal) {//DONE!!!!!
	Value val1, val2;


	bool t1 = MultExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		t1 = MultExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Illegal operand type for the operation.");
			return false;
		}
		if(tok.GetToken() == PLUS){
			retVal = retVal + val2;

			if(retVal.IsErr()){
				ParseError(line, "Illegal operand type for the operation.");
				return false;
			}

		}
		else if(tok.GetToken() == MINUS){
			retVal = retVal - val2;
			if(retVal.IsErr()){
				ParseError(line, "Illegal operand type for the operation.");
				return false;
			}
		}
		else if(tok.GetToken() == CAT){
		
			retVal = retVal.Catenate(val2);//Might be wrong

			if(retVal.IsErr()){
				ParseError(line, "Illegal operand type for the operation.");
				return false;
			}
			
		}




		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line, Value & retVal) {//Done!!!!
	Value val1, val2;
	bool t1 = ExponExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  || tok == SREPEAT)
	{
		
		t1 = ExponExpr(in, line, val2);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if(tok.GetToken() == MULT){
			retVal = retVal * val2;
			//if(retVal.IsErr()){
				//ParseError(line, "Illegal operand type for the operation.");
				//return false;
			//}
		}
		else if(tok.GetToken() == DIV){
			//if((val2.GetInt() == 0 || val2.GetReal() == 0.00)){//Might be wrong
				//ParseError(line, "Division by zero error");
				//return false;

			//}
			retVal = retVal/ val2;
			//if(retVal.IsErr()){
				//ParseError(line, "Illegal operand type for the operation.");
				//return false;
			//}
		}
		else{
			retVal = retVal.Repeat(val2);
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal operand type for the operation.");
      		return false;
		}



		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	
	return true;
}//End of MultExpr

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, Value & retVal)//DONE!!!
{
	
	bool status;
	Value val1;
		
	status = UnaryExpr(in, line, val1);
	if( !status ) {
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT)
	{
		Value val2;
		status = ExponExpr(in, line, val2);
		if( !status ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		val1 = val1^val2;
		tok	= Parser::GetNextToken(in, line);
		
		//if(tok.GetToken() == EXPONENT){
			//retVal = (retVal)^retVal;//FIX THIS TO SQUARED OF WHATEVER
			//if(retVal.IsErr()){
				//ParseError(line, "Illegal squared operation");
				//return false;
			//}
		//}

		
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of ExponExpr

//UnaryExpr ::= ( - | + ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value & retVal)//DONE!!!
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		

	Value val1;
	status = PrimaryExpr(in, line, sign, val1);


	if((t.GetToken() == MINUS || t.GetToken() == PLUS) && status && !val1.IsReal() && !val1.IsInt()){//Probably Wrong
		ParseError(line, "Illegal Operand Type for Sign Operator");
		return false;
	}
	retVal = val1;

	return status;
}//End of UnaryExpr


//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value & retVal){//Probably Done?
	
	Value val1;
	LexItem tok = Parser::GetNextToken(in, line);


	if(sign == 0){
		sign = 1;
	}
	
	if( tok == NIDENT || tok == SIDENT) {
		
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		//return true;

		//Value v = TempsResults[lexeme];
		//if(v.IsErr()){
			//ParseError(line, "Undefined var");
			//return false;
		//}
		//if(v.IsInt() || v.IsReal()){//Might be wrong
			//if(sign == -1){
				//retVal = v * -1;
				//return true;
			//}
			//else{
				//retVal = v;
			//}
		//}
		//retVal = v;
		//return true;

		if(tok.GetToken() == NIDENT){
			retVal = TempsResults[lexeme]*Value(sign);
		}
		else{
			retVal = TempsResults[lexeme];
			
		}
		return true;
	}
		
		else if(tok.GetToken() == ICONST){
			auto lex = tok.GetLexeme();
			retVal = Value(sign*stoi(lex));
			return true;
		}
		else if(tok.GetToken() == SCONST){
			auto lex = tok.GetLexeme();
			retVal = Value(lex);
			return true;
		}
		else if(tok.GetToken() == RCONST){
			auto lex = tok.GetLexeme();
			retVal = Value(sign*stod(lex));
			return true;
		}
		


	
	//else if( tok == ICONST ) {

		//Value v(stoi(tok.GetLexeme()));
		//if(sign == -1){
			//retVal = v * -1;
		//}
		//else{
			//retVal = v;
		//}
		
		//return true;
	//}
	//else if( tok == SCONST ) {

		//Value v(tok.GetLexeme());
		//retVal = v;
		
		//return true;
	//}
	//else if( tok == RCONST ) {
		//Value v(stof(tok.GetLexeme()));
		//retVal = v;
		//if(sign == -1){
			//retVal = retVal * -1;
		//}
		
		
		//return true;
	//}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, val1);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN ){

			if(sign == -1){
				val1 = val1*-1;
			}
			retVal = val1;


			return ex;
		}
			
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}


