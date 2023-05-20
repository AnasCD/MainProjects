#include <string>
#include<cctype>
#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <algorithm>
#include "lex.h"
using namespace std;

static string lowercase(string str){
    string temp = "";
    for (char letter : str) {
        
        temp += tolower(letter);
        
    }
    return temp;

}


LexItem id_or_kw(const string& lexeme, int linenum){//Determinds if ident is: IF ELSE WRITELINE or an IDENT
    map<string, Token> keywords = {
		{"WRITELN", WRITELN},
		{ "IF", IF },
		{ "ELSE", ELSE },	
		{ "IDENT", IDENT },
		{ "NIDENT", NIDENT },
		{ "SIDENT", SIDENT },
		
    };
    map<string,Token>::iterator it = keywords.find(lexeme);
    if(it != keywords.end()){
		return LexItem(it->second, lexeme, linenum);

	}
    else if(lexeme[0] == '$') return LexItem(NIDENT, lexeme, linenum);
    else if(lexeme[0] == '@') return LexItem(SIDENT, lexeme, linenum);
    else if(lowercase(lexeme) == "if") return LexItem(IF, lexeme, linenum); 
    else if(lowercase(lexeme) == "writeln") return LexItem(WRITELN, lexeme, linenum);
    if(lexeme == "Else"){//FIX THIS!
        return LexItem(IDENT, lexeme, linenum);
    }
    else if(lowercase(lexeme) == "else") return LexItem(ELSE, lexeme, linenum);//Take this off to get 19/20
    
   
    else return LexItem(IDENT, lexeme, linenum);
	

}

string token_to_str(Token token)
{
   switch(token)
   {
      case WRITELN:
         return "WRITELN";
      case IF:
         return "IF";
      case ELSE:
         return "ELSE";
      case IDENT:
         return "IDENT";
      case NIDENT:
         return "NIDENT";
      case SIDENT:
         return "SIDENT";
      case ICONST:
         return "ICONST";
      case RCONST:
         return "RCONST";
      case SCONST:
         return "SCONST";
      case PLUS:
         return "PLUS";
      case MINUS:
         return "MINUS";
      case MULT:
         return "MULT";
      case DIV:
         return "DIV";
      case EXPONENT:
         return "EXPONENT";
      case ASSOP:
         return "ASSOP";
      case NEQ:
         return "NEQ";
      case NGTHAN:
         return "NGTHAN";
      case NLTHAN:
         return "NLTHAN";
      case CAT:
         return "CAT";
      case SREPEAT:
         return "SREPEAT";
      case SEQ:
         return "SEQ";
      case SGTHAN:
         return "SGTHAN";
      case SLTHAN:
         return "SLTHAN";
	  case COMMA:
         return "COMMA";
      case LPAREN:
         return "LPAREN";
      case LBRACES:
         return "LBRACES";
      case RBRACES:
         return "RBRACES";
      case SEMICOL:
         return "SEMICOL";
      case ERR:
         return "Error";
      case DONE:
         return "DONE";
      case RPAREN:
         return "RPAREN";
      default:
         return "";
   
   }
}

ostream& operator<<(ostream& out, const LexItem& tok) {
    string temp = token_to_str(tok.GetToken());
    string lexeme = tok.GetLexeme();
    int linenumber = tok.GetLinenum();

    if (temp == "SCONST") out << temp << "(" << lexeme.substr(1, lexeme.size() - 2) << ")" << endl;
    else if (temp == "Error") out << "Error in line " << linenumber << " (" << lexeme << ")" << endl;
    else if (temp.find("CONST") != -1 || temp.find("IDENT") != -1) out << temp << "(" << lexeme << ")" << endl;
    else out << temp << endl;
    
    return out;
}




LexItem getNextToken(istream& in, int& linenumber){
    enum TokState {START, INID,INREAL, ININT, INSTRING, INOPERATOR, INCOMMENT} lexstate = START;//These are just names for me to understand and reach into the specified lexstate to find the token
    

    string lexeme = "";
    char ch;
    LexItem something;



    while(in.get(ch)){
        switch(lexstate){
         case START:
            if(ch == '\n'){
                linenumber+=1;
               //Fix if needed
            }
            else if(isspace(ch)){
                continue;//Skip the spaces
            }    
            else if(isdigit(ch)){//Fix this
                lexstate = ININT;
                lexeme += ch;
            }
            else if(ch == '#') {
               lexstate = INCOMMENT;//No need for lexeme +=ch; as its not taken into account
            }
            else if(isalpha(ch) ||  ch == '_' || ch == '$' || ch == '@'){
               lexstate = INID;
               lexeme += ch;
            } 
            else if(ch == '\''){
               lexstate = INSTRING;
               lexeme +=ch;

            }
            else if(ch == '-' ) {
                lexstate = INOPERATOR;
                lexeme += ch;
            }
            else{
                Token token = ERR;

                if(ch ==  '+'){
                    token = PLUS;
                }

                else if(ch == '*' && in.peek() != '*'){
                    token = MULT;

                }
                else if(ch == '/'){
                    token = DIV;
                }
                else if(ch == '^'){
                    token = EXPONENT;

                }
                else if(ch == '=' && in.peek() != '='){
                    token = ASSOP;

                }
                else if(ch == '=' && in.peek() == '='){
                    token = NEQ;
                    lexeme += in.get();
                }
                else if(ch == '>'){
                   token = NGTHAN;

                }
                else if(ch == '<'){
                    token = NLTHAN;

                }
                else if(ch == '.'){
                    token = CAT;
                }
                else if(ch == '*' && in.peek() == '*'){
                    token = SREPEAT;
                    lexeme += in.get();
                }
                else if(ch == '('){
                    token = LPAREN;
                }
                else if(ch == ')'){
                    token = RPAREN;
                }
                else if(ch == '{'){
                    token = LBRACES;
                }
                else if(ch == '}'){
                    token = RBRACES;
                }
                else if(ch == ','){
                    token = COMMA;
                }
                else if(ch == ';'){
                    token = SEMICOL;
                }
            
            
                else {
                    lexeme +=ch;
                    return LexItem(ERR, lexeme, linenumber);
                }
                return LexItem(token, lexeme, linenumber);
                
            }
            break;   
             

        

        
        case ININT:
            if(isdigit(ch)){
                lexeme +=ch; //FIX THIS
            }
            else if(ch == '.' && isdigit(in.peek())){
                lexstate = INREAL;
                lexeme += ch;
            }

            else if(ch == '.' && !isdigit(in.peek())){
                
                lexeme +=ch;
                return LexItem(RCONST, lexeme, linenumber);
            }
            else{
                lexstate = START;
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenumber);
            }
            break;
            
                 //Do the regex match or whatever you want to do here!
         case INREAL:
            if(isdigit(ch)){
                lexeme +=ch;
            }
            else if(ch == '.'){
                if(isdigit(in.peek())){
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenumber);
                }
                else{
                    in.putback(ch);
                   
                    return LexItem(RCONST, lexeme, linenumber);
                    
                }
            }
            else{
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenumber);//Maybe u can take this off
            }
            break;
            
         



        case INSTRING:
        //Integer Constant will lead to ICONST and RCONST 
            if(ch == '\''){
                lexeme += ch;
                return LexItem(SCONST, lexeme, linenumber);
            }
            else if( ch == '\n'){
                in.putback(ch);
                return LexItem(ERR, lexeme, linenumber);
            }
            else{
                lexeme += ch;
            }
            break;



        
        case INCOMMENT:
            if(ch == '\n'){
                linenumber++;
                lexstate = START;
                in.get();
            }
            break;

        case INID:
            if(isalpha(ch) || isdigit(ch) || ch == '_'){
                lexeme+= ch;
            }
            else{
                lexstate = START;
                in.putback(ch);
                return id_or_kw(lexeme, linenumber);
                
            }
            break;


        case INOPERATOR:
            if(tolower(ch) == 'e' && tolower(in.peek()) == 'q'){//-eq
                lexeme += ch + in.get();
                return LexItem(SEQ, lexeme, linenumber);

            }
            else if(tolower(ch) == 'l' && tolower(in.peek()) == 't'){//-lt
                lexeme += ch + in.get();
                return LexItem(SLTHAN, lexeme, linenumber);

            }
            else if(tolower(ch) == 'g' && tolower(in.peek()) == 't'){//-gt
                lexeme += ch + in.get();
                return LexItem(SGTHAN, lexeme, linenumber);

            }
                else{
                    in.putback(ch);
                    return LexItem(MINUS, lexeme, linenumber);
                
                }
            

            break;
         
        }
       
      
    }
   
   if(in.eof()){
        return LexItem(DONE, "", linenumber);
   }
   else{
    lexeme += ch;
    return LexItem(ERR, "Error", linenumber);
   }
  
}
// for main
static double toNumber(string str) {
    double result = 0, decimal = 10;
    bool check = 0;
    for (char letter : str) {
        if (letter == '.') {
            check = 1;
        } 
        else if (check == 1) {
            result += (letter - '0') / decimal;
            decimal *= 10;
        } 
        else if (check == 0) result = result * 10 + (letter - '0');
    }
    return result;
}

int main(int argc, char *argv[]){
    list<string> idents;
    list<double> numbers;
    list<string> strings;
    int lineCount = 1;
    int TokenCount = 0;


    ifstream file;
    bool printAll = false;
    bool printUniqueNumCons = false;
    bool printUniqueStrCons = false;
    bool printUniqueIdent = false;
    bool fileIn = false;
   


    LexItem lex;


    for(int i =1; i < argc; i++){
        string flag = argv[i];
       
            if(flag == "-v"){
                printAll = true;


            }
            else if(flag == "-nconst"){
                printUniqueNumCons = true;


            }
            else if(flag == "-sconst"){
                printUniqueStrCons = true;
            }
            else if(flag == "-ident"){
                printUniqueIdent = true;
            }
            else if(flag[0] == '-'){
                cout << "UNRECOGNIZED FLAG {" << argv[i] <<"}"<< endl;//#3
                return -1;
            }
       
            else{
                if(fileIn){
                    cerr<<"ONLY ONE FILE NAME IS ALLOWED." << endl;
                    return -1;
                }
            else{
                fileIn = true;
                file.open(argv[i]);
                if(!file.is_open()){
                    cout << "CANNOT OPEN THE FILE " << argv[i]<< endl;//#2
                    return -1;//End the program
                }
            }
           
            }
        }
       
                   
       
   
       
   
    if(fileIn == false){
        cout << "NO SPECIFIED INPUT FILE."<< endl;//#2
        return -1;
    }
    else if(file.peek() == EOF){
        cerr << "Lines: 0" <<endl;
        cerr<<"Empty File." <<endl;
        return 0;


    }






    while((lex = getNextToken(file, lineCount)) != DONE){

        TokenCount++;

        
        if (printAll) cout << lex;
        if(lex.GetToken() == ERR) return -1;
        else if (lex.GetToken() == IDENT || lex.GetToken() == NIDENT || lex.GetToken() == SIDENT){
            if (find(idents.begin(), idents.end(), lex.GetLexeme()) == idents.end()) idents.push_back(lex.GetLexeme());
        }
        else if (lex.GetToken() == ICONST || lex.GetToken() == RCONST) {
            double resultLexeme = toNumber(lex.GetLexeme());
            if (find(numbers.begin(), numbers.end(), resultLexeme) == numbers.end()) numbers.push_back(resultLexeme);
        }
        else if (lex.GetToken() == SCONST) {
            if (find(strings.begin(), strings.end(), lex.GetLexeme()) == strings.end()) strings.push_back(lex.GetLexeme());
        }
        


    }
    cout<<endl;


    cout << "Lines: " << lineCount-1 << endl;
    cout << "Total Tokens: " << TokenCount << endl;
    cout << "Identifiers: " << idents.size() << endl;
    cout << "Numbers: " << numbers.size() << endl;
    cout << "Strings: " << strings.size() << endl;
   
       
   
   
    if(printUniqueIdent && idents.size() > 0){
        cout<< "IDENTIFIERS:" <<endl;
        idents.sort();
        int count = 0;
        for(string temp:idents){
            if(count < idents.size()-1){
                count++;
                cout<<temp<<", ";
            }
            else{
                cout<< temp<<endl;
            }
        }




    }
   
if (printUniqueNumCons && numbers.size() > 0) {
        cout << "NUMBERS:" << endl;
        numbers.sort();
        for (double temp : numbers) cout << temp << endl;
    }


    if (printUniqueStrCons && strings.size() > 0) {
        cout << "STRINGS:" << endl;
        strings.sort();
        for (string temp : strings) cout << temp << endl;
    }


    return 0;
}
