/*******************************************************
JJ 
12-1-10
Compiler Code
*******************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <vector>
using namespace std;

ifstream ifile("test.txt");
ifstream reserveFile;
ofstream ofile;
ofstream tokenFile;
ofstream memFile;

// offset for the memory address file
int offset = 0;

// maximum sizes declarations
const int MAXID = 10;
const int MAXINT = 10;
const int MAXDIGIT = 5;
const int MAXDECIMAL = 5;
const int MAXEXPONENT = 2;

// reserved word list, symbol file
vector<string> reserves;
vector<string> symbols;
vector<int> tokens;
vector<int> attributes;

// declarations processing
bool procerror = false;
int deccounter = 0;
int paramcounter = 0;
vector<string> decs;
vector<int> tempdec;
vector<vector<int> > declist;

// store identifiers and their types
vector<string> identifier;
vector<int> idtype;
enum typelist {INT, REAL, AINT, AREAL, BOOL, ERROR, GREEN};
int  arraylength = 0;

// FORWARD DECLARATIONS
string getToken(char * front, char * back);
void generateError(int errCode, string token);
void init();
bool analyzer();
void parser();
void sign();
int factor();
int factor2(int temp);
int term();
int term2(int temp17);
int simple_expression();
int simple_expression2(int temp14);
int expression();
int expression2(int temp11);
void expression_list();
void expression_list2();
void procedure_statement();
void procedure_statement2();
int variable();
int variable2(int temp);
void statement();
void statement2();
void statement_list();
void statement_list2();
void optional_statements();
void compound_statement();
void compound_statement2();
void param_list();
void param_list2();
void arguments();
void subprogram_head();
void subprogram_head2();
void subprogram_declaration();
void subprogram_declaration2();
void subprogram_declarations();
void subprogram_declarations2();
int standard_type();
int typemethod();
void declarations();
void declarations2();
void id_list();
void id_list2();
void program();
void program2();
void match(string t);
void getReserves(string rF);
void synch(int a);
void checkID();

// GLOBAL VARIABLES
// token, type, attribute to be passed to the parser
string token;
int type;
int attribute;
// string for analyzer readLine
string input;
// pointers to the analyzer string
char * front;
char * back;
// line number for listing file
int line = 0;
// generic pointer starter
int point = 100;

// METHODS

// gets a string of the token
string getToken(char * front, char * back)
{
	string newToken(back, front - back + 1);
	return newToken;
}

// reads the reserved words from the reserved word file
void getReserves()
{
	string next;
	int convert = 0;
	if (reserveFile.is_open())
	{
		while (!reserveFile.eof())
		{
			reserveFile >> next;
			reserves.push_back(next);
			reserveFile >> next;
			convert = atoi(next.c_str());
			tokens.push_back(convert);
			reserveFile >> next;
			convert = atoi(next.c_str());
			attributes.push_back(convert);
		}
	}
}

// generates errors in the listing file
void generateError(int errCode)
{
	switch (errCode)
	{
	case 1: ofile << "LEXERR:\tLeading Zeroes in Exponent:\t" << token << endl;
			break;
	case 2: ofile << "LEXERR:\tTrailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 3: ofile << "LEXERR:\tTrailing Zeroes in Fraction and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 4: ofile << "LEXERR:\tLeading Zeroes in Integer:\t" << token << endl;
			break;
	case 5: ofile << "LEXERR:\tLeading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 6: ofile << "LEXERR:\tLeading Zeroes in Integer and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 7: ofile << "LEXERR:\tLeading Zeroes in Integer and Exponent and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 8: ofile << "LEXERR:\tExtra Long Exponent:\t" << token << endl;
			break;
	case 9: ofile << "LEXERR:\tExtra Long Exponent and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 10:ofile << "LEXERR:\tExtra Long Exponent and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 11:ofile << "LEXERR:\tExtra Long Exponent, Trailing Zeroes in Fraction and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 12:ofile << "LEXERR:\tExtra Long Exponent and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 13:ofile << "LEXERR:\tExtra Long Exponent and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 14:ofile << "LEXERR:\tExtra Long Exponent, Leading Zeroes in Integer and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 15:ofile << "LEXERR:\tExtra Long Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 16:ofile << "LEXERR:\tExtra Long Fraction:\t" << token << endl;
			break;
	case 17:ofile << "LEXERR:\tExtra Long Fraction Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 18:ofile << "LEXERR:\tExtra Long Fraction and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 19:ofile << "LEXERR:\tExtra Long Fraction, Trailing Zeroes in Fraction and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 20:ofile << "LEXERR:\tExtra Long Fraction and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 21:ofile << "LEXERR:\tExtra Long Fraction and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 22:ofile << "LEXERR:\tExtra Long Fraction, Leading Zeroes in Integer, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 23:ofile << "LEXERR:\tExtra Long Fraction, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 24:ofile << "LEXERR:\tExtra Long Fraction and Exponent:\t" << token << endl;
			break;
	case 25:ofile << "LEXERR:\tExtra Long Fraction and Exponent and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 26:ofile << "LEXERR:\tExtra Long Fraction and Exponent and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 27:ofile << "LEXERR:\tExtra Long Fraction and Exponent, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 28:ofile << "LEXERR:\tExtra Long Fraction and Exponent and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 29:ofile << "LEXERR:\tExtra Long Fraction and Exponent and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 30:ofile << "LEXERR:\tExtra Long Fraction and Exponent, Leading Zeroes in Integer, and Trailing Zeroes in Exponent:\t" << token << endl;
			break;
	case 31:ofile << "LEXERR:\tExtra Long Fraction and Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 32:ofile << "LEXERR:\tExtra Long Integer:\t" << token << endl;
			break;
	case 33:ofile << "LEXERR:\tExtra Long Integer and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 34:ofile << "LEXERR:\tExtra Long Integer and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 35:ofile << "LEXERR:\tExtra Long Integer, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 36:ofile << "LEXERR:\tExtra Long Integer and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 37:ofile << "LEXERR:\tExtra Long Integer and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 38:ofile << "LEXERR:\tExtra Long Integer, Leading Zeroes in Integer, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 39:ofile << "LEXERR:\tExtra Long Integer, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 40:ofile << "LEXERR:\tExtra Long Integer and Exponent:\t" << token << endl;
			break;
	case 41:ofile << "LEXERR:\tExtra Long Integer and Exponent and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 42:ofile << "LEXERR:\tExtra Long Integer and Exponent and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 43:ofile << "LEXERR:\tExtra Long Integer and Exponent, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 44:ofile << "LEXERR:\tExtra Long Integer and Exponent and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 45:ofile << "LEXERR:\tExtra Long Integer and Exponent and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 46:ofile << "LEXERR:\tExtra Long Integer and Exponent, Leading Zeroes in Integer, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 47:ofile << "LEXERR:\tExtra Long Integer and Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 48:ofile << "LEXERR:\tExtra Long Integer and Fraction:\t" << token << endl;
			break;
	case 49:ofile << "LEXERR:\tExtra Long Integer and Fraction and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 50:ofile << "LEXERR:\tExtra Long Integer and Fraction and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 51:ofile << "LEXERR:\tExtra Long Integer and Fraction, Trailing Zereos in Fraction, and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 52:ofile << "LEXERR:\tExtra Long Integer and Fraction and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 53:ofile << "LEXERR:\tExtra Long Integer and Fraction and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 54:ofile << "LEXERR:\tExtra Long Integer and Fraction, Leading Zeroes in Integer, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 55:ofile << "LEXERR:\tExtra Long Integer and Fraction, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 56:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent:\t" << token << endl;
			break;
	case 57:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 58:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 59:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent:\t" << token << endl;
			break;
	case 60:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent and Leading Zeroes in Integer:\t" << token << endl;
			break;
	case 61:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent and Leading Zeroes in Integer and Exponent:\t" << token << endl;
			break;
	case 62:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent, Leading Zeroes in Integer, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 63:ofile << "LEXERR:\tExtra Long Integer, Fraction and Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction:\t" << token << endl;
			break;
	case 64:ofile << "LEXERR:\tExtra Long Identifier:\t" << token << endl;
			break;
	case 65:ofile << "LEXERR:\tUnrecognized Symbol:\t" << token << endl;
			break;
	default:ofile << "LEXERR:\tOther:\t" << token << endl;
			break;
	}
}

// stores the tokens in the token file
void storeToken()
{
	tokenFile.setf(ios_base::left,ios_base::adjustfield);
	tokenFile << line << "\t\t\t" << setw(15) << setfill(' ') << token << " " << type << setw(14) << setfill(' ');
	switch (type)
	{
	case 10:tokenFile << " (PROG)";
			break;
	case 11:tokenFile << " (INT)";
			break;
	case 12:tokenFile << " (REAL)";
			break;
	case 13:tokenFile << " (VAR)";
			break;
	case 14:tokenFile << " (BEGIN)";
			break;
	case 15:tokenFile << " (END)";
			break;
	case 16:tokenFile << " (FUNCT)";
			break;
	case 17:tokenFile << " (PROCEDURE)";
			break;
	case 18:tokenFile << " (IF)";
			break;
	case 19:tokenFile << " (ELSE)";
			break;
	case 20:tokenFile << " (THEN)";
			break;
	case 21:tokenFile << " (WHILE)";
			break;
	case 22:tokenFile << " (DO)";
			break;
	case 23:tokenFile << " (ARRAY)";
			break;
	case 24:tokenFile << " (OF)";
			break;
	case 25:tokenFile << " (NOT)";
			break;
	case 26:tokenFile << " (ADDOP)";
			break;
	case 27:tokenFile << " (MULOP)";
			break;
	case 28:tokenFile << " (ASSIGNOP)";
			break;
	case 29:tokenFile << " (RELOP)";
			break;
	case 30:tokenFile << " (ID)";
			break;
	case 31:tokenFile << " (SEMICOLON)";
			break;
	case 32:tokenFile << " (COLON)";
			break;
	case 33:tokenFile << " (PERIOD)";
			break;
	case 34:tokenFile << " (COMMA)";
			break;
	case 35:tokenFile << " (OPEN PAREN)";
			break;
	case 36:tokenFile << " (CLOSE PAREN)";
			break;
	case 37:tokenFile << " (EOF)";
			break;
	case 38:tokenFile << " (ID)";
			break;
	case 39:tokenFile << " (CALL)";
			break;
	case 40:tokenFile << " (OPEN BRACK)";
			break;
	case 41:tokenFile << " (CLOSE BRACK)";
			break;
	case 99:tokenFile << " (LEXERR)";
			break;
	default:tokenFile << " (UNKOWN)";
			break;
	}
	// tokenFile << attribute << "\t";
	switch(attribute)
	{
	case 0: tokenFile << attribute << "\t" << "(NULL)" << endl;
			break;
	case 1: tokenFile << attribute << "\t" << "(Leading Zeroes in Exponent)" << endl;
			break;
	case 2: tokenFile << attribute << "\t" << "(Trailing Zeroes in Fraction)" << endl;
			break;
	case 3: tokenFile << attribute << "\t" << "(Trailing Zeroes in Fraction and Leading Zeroes in Exponent)" << endl;
			break;
	case 4: tokenFile << attribute << "\t" << "(Leading Zeroes in Integer)" << endl;
			break;
	case 5: tokenFile << attribute << "\t" << "(Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 6: tokenFile << attribute << "\t" << "(Leading Zeroes in Integer and Trailing Zeroes in Fraction)" << endl;
			break;
	case 7: tokenFile << attribute << "\t" << "(Leading Zeroes in Integer and Exponent and Trailing Zeroes in Fraction)" << endl;
			break;
	case 8: tokenFile << attribute << "\t" << "(Extra Long Exponent)" << endl;
			break;
	case 9: tokenFile << attribute << "\t" << "(Extra Long Exponent and Leading Zeroes in Exponent)" << endl;
			break;
	case 10:tokenFile << attribute << "\t" << "(Extra Long Exponent and Trailing Zeroes in Fraction)" << endl;
			break;
	case 11:tokenFile << attribute << "\t" << "(Extra Long Exponent, Trailing Zeroes in Fraction and Leading Zeroes in Exponent)"<< endl;
			break;
	case 12:tokenFile << attribute << "\t" << "(Extra Long Exponent and Leading Zeroes in Integer)" << endl;
			break;
	case 13:tokenFile << attribute << "\t" << "(Extra Long Exponent and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 14:tokenFile << attribute << "\t" << "(Extra Long Exponent, Leading Zeroes in Integer and Trailing Zeroes in Fraction)" << endl;
			break;
	case 15:tokenFile << attribute << "\t" << "(Extra Long Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 16:tokenFile << attribute << "\t" << "(Extra Long Fraction)" << endl;
			break;
	case 17:tokenFile << attribute << "\t" << "(Extra Long Fraction Leading Zeroes in Exponent)" << endl;
			break;
	case 18:tokenFile << attribute << "\t" << "(Extra Long Fraction and Trailing Zeroes in Fraction)" << endl;
			break;
	case 19:tokenFile << attribute << "\t" << "(Extra Long Fraction, Trailing Zeroes in Fraction and Leading Zeroes in Exponent)" << endl;
			break;
	case 20:tokenFile << attribute << "\t" << "(Extra Long Fraction and Leading Zeroes in Integer)" << endl;
			break;
	case 21:tokenFile << attribute << "\t" << "(Extra Long Fraction and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 22:tokenFile << attribute << "\t" << "(Extra Long Fraction, Leading Zeroes in Integer, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 23:tokenFile << attribute << "\t" << "(Extra Long Fraction, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 24:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent)" << endl;
			break;
	case 25:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent and Leading Zeroes in Exponent)" << endl;
			break;
	case 26:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent and Trailing Zeroes in Fraction)" << endl;
			break;
	case 27:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent)" << endl;
			break;
	case 28:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent and Leading Zeroes in Integer)" << endl;
			break;
	case 29:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 30:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent, Leading Zeroes in Integer, and Trailing Zeroes in Exponent)" << endl;
			break;
	case 31:tokenFile << attribute << "\t" << "(Extra Long Fraction and Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 32:tokenFile << attribute << "\t" << "(Extra Long Integer)" << endl;
			break;
	case 33:tokenFile << attribute << "\t" << "(Extra Long Integer and Leading Zeroes in Exponent)" << endl;
			break;
	case 34:tokenFile << attribute << "\t" << "(Extra Long Integer and Trailing Zeroes in Fraction)" << endl;
			break;
	case 35:tokenFile << attribute << "\t" << "(Extra Long Integer, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent)" << endl;
			break;
	case 36:tokenFile << attribute << "\t" << "(Extra Long Integer and Leading Zeroes in Integer)" << endl;
			break;
	case 37:tokenFile << attribute << "\t" << "(Extra Long Integer and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 38:tokenFile << attribute << "\t" << "(Extra Long Integer, Leading Zeroes in Integer, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 39:tokenFile << attribute << "\t" << "(Extra Long Integer, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 40:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent)" << endl;
			break;
	case 41:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent and Leading Zeroes in Exponent)" << endl;
			break;
	case 42:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent and Trailing Zeroes in Fraction)" << endl;
			break;
	case 43:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent)" << endl;
			break;
	case 44:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent and Leading Zeroes in Integer)" << endl;
			break;
	case 45:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 46:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent, Leading Zeroes in Integer, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 47:tokenFile << attribute << "\t" << "(Extra Long Integer and Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 48:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction)" << endl;
			break;
	case 49:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction and Leading Zeroes in Exponent)" << endl;
			break;
	case 50:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction and Trailing Zeroes in Fraction)" << endl;
			break;
	case 51:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction, Trailing Zereos in Fraction, and Leading Zeroes in Exponent)" << endl;
			break;
	case 52:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction and Leading Zeroes in Integer)" << endl;
			break;
	case 53:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 54:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction, Leading Zeroes in Integer, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 55:tokenFile << attribute << "\t" << "(Extra Long Integer and Fraction, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 56:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent)" << endl;
			break;
	case 57:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent and Leading Zeroes in Exponent)" << endl;
			break;
	case 58:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent and Trailing Zeroes in Fraction)" << endl;
			break;
	case 59:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent, Trailing Zeroes in Fraction, and Leading Zeroes in Exponent)" << endl;
			break;
	case 60:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent and Leading Zeroes in Integer)" << endl;
			break;
	case 61:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent and Leading Zeroes in Integer and Exponent)" << endl;
			break;
	case 62:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent, Leading Zeroes in Integer, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 63:tokenFile << attribute << "\t" << "(Extra Long Integer, Fraction and Exponent, Leading Zeroes in Integer and Exponent, and Trailing Zeroes in Fraction)" << endl;
			break;
	case 64:tokenFile << attribute << "\t" << "(Extra Long Identifier)" << endl;
			break;
	case 65:tokenFile << attribute << "\t" << "(Unrecognized Symbol)" << endl;
			break;
	case 66:tokenFile << attribute << "\t" << "(PLUS)" << endl;
			break;
	case 67:tokenFile << attribute << "\t" << "(MINUS)" << endl;
			break;
	case 68:tokenFile << attribute << "\t" << "(OR)" << endl;
			break;
	case 69:tokenFile << attribute << "\t" << "(MULT)" << endl;
			break;
	case 70:tokenFile << attribute << "\t" << "(SLASH)" << endl;
			break;
	case 71:tokenFile << attribute << "\t" << "(DIV)" << endl;
			break;
	case 72:tokenFile << attribute << "\t" << "(MOD)" << endl;
			break;
	case 73:tokenFile << attribute << "\t" << "(AND)" << endl;
			break;
	case 74:tokenFile << attribute << "\t" << "(ASSIGN)" << endl;
			break;
	case 75:tokenFile << attribute << "\t" << "(EQUAL)" << endl;
			break;
	case 76:tokenFile << attribute << "\t" << "(NOT EQUAL)" << endl;
			break;
	case 77:tokenFile << attribute << "\t" << "(LT)" << endl;
			break;
	case 78:tokenFile << attribute << "\t" << "(LE)" << endl;
			break;
	case 79:tokenFile << attribute << "\t" << "(GE)" << endl;
			break;
	case 80:tokenFile << attribute << "\t" << "(GT)" << endl;
			break;
	case 81:tokenFile << attribute << "\t" << "(SEMICOLON)" << endl;
			break;
	case 82:tokenFile << attribute << "\t" << "(COLON)" << endl;
			break;
	case 83:tokenFile << attribute << "\t" << "(PERIOD)" << endl;
			break;
	case 84:tokenFile << attribute << "\t" << "(COMMA)" << endl;
			break;
	case 85:tokenFile << attribute << "\t" << "(OPENPAR)" << endl;
			break;
	case 86:tokenFile << attribute << "\t" << "(CLOSEPAR)" << endl;
			break;
	case 87:// search symbol table
			bool found = false;
			for (int i = 0; i < symbols.size(); i++)
			{
				if (symbols.at(i).compare(token) == 0)
				{
					found = true;
					tokenFile << i + 100 << "\t(PTR)" << endl;
					break;
				}
			}
			if (!found)
			{
				tokenFile << point << "\t(PTR)" << endl;
				point++;
			}
			break;
	}
}


void init()
{
    // initialize file output
	ofile.open("output.txt");
	tokenFile.open("tokens.txt");
	memFile.open("memory.txt");
	memFile.setf(ios_base::left,ios_base::adjustfield);
	tokenFile << "Line No.\tLexeme\t\t\tTOKEN-TYPE\t\tATTRIBUTE" << endl;
    reserveFile.open("reserved.txt");
	getReserves();

    // get first line for analyzer
    if (ifile.is_open())
	{
	    do
	    {
            line++;
	        getline(ifile, input);
	        ofile << line << "\t" << input << endl;
	    } while(input.empty() && !ifile.eof());

        if (ifile.eof())
            cout << "File is empty" << endl;
        else
        {
            input = input + '\n';
            front = &input[0];
            back = &input[0];
        }
	}
	else
        cout << "Error: File not found." << endl;
}

void finish()
{
    ifile.close();
	tokenFile.close();
	ofile.close();
	memFile.close();
}

// returns true if there is another token
// false if not
// assigns new values to token, type, and attribute
bool analyzer()
{
    // for use in the switch statement
	int state = 0;

    // stores the amount of digits to check max
	int digits = 0;
	int decimals = 0;
	int exponents = 0;

    // error variable
    int error = 0;

    if (*front == '\n')
	{
        if (ifile.eof())
            return false;
	    line++;
		getline(ifile, input);
        ofile << line << "\t" << input << endl;
		while (input.empty() && !ifile.eof())
		{
            line++;
			getline(ifile, input);
            ofile << line << "\t" << input << endl;
		}
        input = input + '\n';
        front = &input[0];
        back = &input[0];
	}

	// continue until the end of the buffer
	while (true)
	{
		char current = *front;
        switch (state)
        {
        // first state; reverts here after every token
        case 0:	if (current == 0)
                {
                    return false;
                }
                else if (isspace(current))
                {
                    if (current == '\n')
                    {
                        if (ifile.eof())
                            return false;
                        line++;
                        getline(ifile, input);
                        ofile << line << "\t" << input << endl;
                        while (input.empty() && !ifile.eof())
                        {
                            line++;
                            getline(ifile, input);
                            ofile << line << "\t" << input << endl;
                        }
                        input = input + '\n';
                        front = &input[0];
                        back = &input[0];
                    }
                    front++;
                    back++;
                }
                else if (isalpha(current))
                {
                    state = 1;
                    front++;
                }
                else if (isdigit(current))
                {
                    if (current == '0')
                    {
                        state = 2;
                    }
                    else
                    {
                        state = 3;
                    }
                    digits++;
                    front++;
                }
                else if (ispunct(current))
                {
                    state = 5;
                }
                else
                {
                    // generate error
                    token = getToken(front, back);
                    generateError(65);
                    type = 99;
                    attribute = 65;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // id token state
        case 1: if (isalnum(current))
                {
                    front++;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    if (front - back > MAXID)
                    {
                        // generate error
                        generateError(64);
                    }
                    // search reserved word list
                    bool found = false;
                    for (int i = 0; i < reserves.size(); i++)
                    {
                        if (reserves.at(i).compare(token) == 0)
                        {
                            found = true;
                            type = tokens.at(i);
                            attribute = attributes.at(i);
                            storeToken();
                            break;
                        }
                    }
                    // identifier
                    if (!found)
                    {
                        symbols.push_back(token);
                        type = 30;
                        attribute = 87;
                        storeToken();
                    }
                    front++;
                    back = front;
                    state = 0;
                    return true;
                }
                break;
        // first num '0' state
        case 2: // leading zeroes error
                if (isdigit(current))
                {
                    state = 4;
                    error += 4;
                    digits++;
                    front++;
                }
                // 0.yyyy
                else if (current == '.')
                {
                    if (digits > MAXDIGIT)
                    {
                        error += 32;
                    }
                    state = 9;
                    front++;
                }
                // just a zero
                else
                {
                    front--;
                    token = getToken(front,back);
                    type = 11;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // num '1-9' state
        case 3: if (isdigit(current))
                {
                    digits++;
                    front++;
                }
                else if (current == '.')
                {
                    if (digits > MAXDIGIT)
                    {
                        error += 32;
                    }
                    state = 9;
                    front++;
                }
                else if (current == 'E')
                {
                    if (digits > MAXDIGIT)
                    {
                        error += 32;
                    }
                    state = 10;
                    front++;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    if (digits > MAXINT)
                    {
                        // generate error
                        error += 32;
                    }
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 11;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // leading zeroes state
        case 4: if (isdigit(current))
                {
                    digits++;
                    front++;
                }
                else if (current == '.')
                {
                    if (digits > MAXINT)
                    {
                        error += 32;
                    }
                    state = 9;
                    front++;
                }
                else if (current == 'E')
                {
                    if (digits > MAXINT)
                    {
                        error += 32;
                    }
                    state = 10;
                    front++;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    if (digits > MAXINT)
                    {
                        error += 32;
                    }
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 11;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // punctuation mark state (valid or invalid)
        case 5: if (current == ':')
                {
                    state = 6;
                    front++;
                }
                else if (current == '<')
                {
                    state = 7;
                    front++;
                }
                else if (current == '>')
                {
                    state = 8;
                    front++;
                }
                else if (current == '=')
                {
                    // store token
                    token = getToken(front, back);
                    type = 29;
                    attribute = 75;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '+')
                {
                    // store token
                    token = getToken(front, back);
                    type = 26;
                    attribute = 66;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '-')
                {
                    // store token
                    token = getToken(front, back);
                    type = 26;
                    attribute = 67;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '*')
                {
                    // store token
                    token = getToken(front, back);
                    type = 27;
                    attribute = 69;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '/')
                {
                    // store token
                    token = getToken(front, back);
                    type = 27;
                    attribute = 70;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == ';')
                {
                    // store token
                    token = getToken(front, back);
                    type = 31;
                    attribute = 81;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '.')
                {
                    // store token
                    token = getToken(front, back);
                    type = 33;
                    attribute = 83;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == ',')
                {
                    // store token
                    token = getToken(front, back);
                    type = 34;
                    attribute = 84;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '[')
                {
                    // store token
                    token = getToken(front, back);
                    type = 40;
                    attribute = 0;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == ']')
                {
                    // store token
                    token = getToken(front, back);
                    type = 41;
                    attribute = 0;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '(')
                {
                    // store token
                    token = getToken(front, back);
                    type = 35;
                    attribute = 85;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == ')')
                {
                    // store token
                    token = getToken(front, back);
                    type = 36;
                    attribute = 86;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else
                {
                    // generate error
                    token = getToken(front, back);
                    generateError(65);
                    type = 99;
                    attribute = 65;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // ':' state
        case 6: if (current == '=')
                {
                    // store token
                    token = getToken(front, back);
                    type = 28;
                    attribute = 74;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    type = 32;
                    attribute = 0;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // '<' state
        case 7: if (current == '>')
                {
                    // store token
                    token = getToken(front, back);
                    type = 29;
                    attribute = 76;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else if (current == '=')
                {
                    // store token
                    token = getToken(front, back);
                    type = 29;
                    attribute = 78;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else
                {
                    // store token
                    token = getToken(front, back);
                    type = 29;
                    attribute = 77;
                    storeToken();
                    back = front;
                    return true;
                }
                break;
        // '>' state
        case 8: if (current == '=')
                {
                    // store token
                    token = getToken(front, back);
                    type = 29;
                    attribute = 79;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                else
                {
                    // store token
                    token = getToken(front, back);
                    type = 29;
                    attribute = 80;
                    storeToken();
                    back = front;
                    return true;
                }
                break;
        // decimal num state
        case 9: if (isdigit(current))
                {
                    decimals++;
                    if (current == '0')
                    {
                        state = 13;
                        front++;
                    }
                    else
                    {
                        state = 14;
                        front++;
                    }
                }
                else
                {
                    // the period is not a decimal indicator; store both the number and the period
                    front -= 2;
                    token = getToken(front, back);
                    type = 33;
                    attribute = 83;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // exponent state
        case 10:if (isdigit(current))
                {
                    exponents++;
                    if (current == '0')
                    {
                        front++;
                        state = 11;
                    }
                    else
                    {
                        front++;
                        state = 12;
                    }
                }
                // xxx.yyyEf
                else
                {
                    front -= 2;
                    token = getToken(front, back);
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 12;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // exponent state: starting zero state
        case 11:if(isdigit(current))
                {
                    error += 1;
                    exponents++;
                    front++;
                    state = 12;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    if (exponents > MAXEXPONENT)
                    {
                        error += 8;
                    }
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 12;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // exponent state: no starting zero
        case 12:if (isdigit(current))
                {
                    exponents++;
                    front++;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    if (exponents > MAXEXPONENT)
                    {
                        error += 8;
                    }
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 12;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // decimal state with possible trailing zero
        case 13:if (isdigit(current))
                {
                    decimals++;
                    if (current == '0')
                    {
                        front++;
                    }
                    else
                    {
                        front++;
                        state = 14;
                    }
                }
                else if (current == 'E')
                {
                    // generate error
                    if (decimals > MAXDECIMAL)
                    {
                        error += 16;
                    }
                    error += 2;
                    state = 10;
                    front++;
                }
                else
                {
                    if (decimals > MAXDECIMAL)
                    {
                        error += 16;
                    }
                    front--;
                    error += 2;
                    token = getToken(front, back);
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 12;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        // decimal state with no currently trailing zero
        case 14:if (isdigit(current))
                {
                    decimals++;
                    if (current == '0')
                    {
                        front++;
                        state = 13;
                    }
                    else
                    {
                        front++;
                    }
                }
                else if (current == 'E')
                {
                    if (decimals > MAXDECIMAL)
                    {
                        error += 16;
                    }
                    state = 10;
                    front++;
                }
                else
                {
                    front--;
                    token = getToken(front, back);
                    if (decimals > MAXDECIMAL)
                    {
                        error += 16;
                    }
                    if (error > 0)
                    {
                        generateError(error);
                    }
                    type = 12;
                    attribute = error;
                    storeToken();
                    front++;
                    back = front;
                    return true;
                }
                break;
        } // end switch
	} // end while
} // end analyzer

void parser()
{
    if(analyzer())
    {
        program();
        if (!ifile.eof())
        {
            ofile << "SYNERR: Expected $ but received " + token + ".\n";
        }
    }
}

void match(string t)
{
	if (t == token && t == "$")
	{
		// done
	}
	else if (t == token && t != "$")
	{
		analyzer();
	}
	else
	{
		// print error
		ofile << "SYNERR: Expected " + t + " but received " + token + ".\n";
		analyzer();
	}
}

void program()
{
	switch(type)
	{
	case 10: match("program");
			 checkID();
			 match("(");
			 id_list();
			 match(")");
			 match(";");
			 program2();
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected program, got " + token + ". Synchronizing...\n";
	}
}

void program2()
{
	switch(type)
	{
	case 13: declarations();
			 subprogram_declarations();
			 compound_statement();
			 match(".");
			 break;
	case 14: compound_statement();
			 match(".");
			 break;
	case 17: subprogram_declarations();
			 compound_statement();
			 match(".");
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected var, procedure, or begin, got " + token + ". Synchronizing...\n";
	}
}

void id_list()
{
	switch(type)
	{
	case 30: checkID();
			 id_list2();
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected id, got " + token + ". Synchronizing...\n";
			 synch(1);
	}
}

void id_list2()
{
	switch(type)
	{
	case 34: match(",");
			 checkID();
			 id_list2();
			 break;
	default: // epsilon production
             if (token != ")")
             {
                 ofile << "SYNERR: Expected comma or ), got " + token + ". Synchronizing...\n";
                 synch(1);
             }
			 break;
	}
}

void declarations()
{
	switch(type)
	{
	case 13:
	{
             match("var");
             identifier.push_back(token);
             memFile << "" << token << setw(15) << setfill(' ') << "\t";
			 checkID();
			 match(":");
			 int temp = typemethod();
			 idtype.push_back(temp);
			 memFile << offset << "\n";
             if (temp == INT)
                 offset += 4;
             else if (temp == REAL)
                 offset += 8;
             else if (temp == AINT)
                 offset += (4 * arraylength);
             else if (temp == AREAL)
                 offset += (8 * arraylength);
			 match(";");
			 declarations2();
	}
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected var, got " + token + ". Synchronizing...\n";
			 synch(2);
			 break;
	}
}

void declarations2()
{
	switch(type)
	{
	case 13:
	{        match("var");
             identifier.push_back(token);
             memFile << "" << token << setw(15) << setfill(' ') << "\t";
			 checkID();
			 match(":");
			 int temp = typemethod();
			 idtype.push_back(temp);
			 memFile << offset << "\n";
             if (temp == INT)
                 offset += 4;
             else if (temp == REAL)
                 offset += 8;
             else if (temp == AINT)
                 offset += (4 * arraylength);
             else if (temp == AREAL)
                 offset += (8 * arraylength);
			 match(";");
			 declarations2();
	}
			 break;
    default: // do synch
             if ((!(token == "procedure" || token == "begin")))
             {
                 ofile << "SYNERR: Expected var, got " + token + ". Synchronizing...\n";
                 synch(2);
             }
             break;
	}
}

int typemethod()
{
	switch(type)
	{
	case 11:
	case 12: return standard_type();
	case 23:
	{        match("array");
			 match("[");
			 int start = atoi(token.c_str());
			 analyzer();
			 match(".");
			 match(".");
			 int end = atoi(token.c_str());
			 if (end > start)
                arraylength = end - start + 1;
             else
                ofile << "SEMERR: array index out of bounds.\n";
			 analyzer();
			 match("]");
			 match("of");
			 return 2 + standard_type();
	}
	default: // print error and do sync
			 ofile << "SYNERR: Expected integer, real, or array, got " + token + ". Synchronizing...\n";
			 synch(3);
	}
}

int standard_type()
{
	switch(type)
	{
	case 11: match("integer");
             return INT;
	case 12: match("real");
             return REAL;
	default: // print error and do sync
			 ofile << "SYNERR: Expected integer or real, got " + token + ". Synchronizing...\n";
			 synch(3);
	}
}

void subprogram_declarations()
{
	switch(type)
	{
	case 17: subprogram_declaration();
			 match(";");
             for (int i = idtype.size() - 1; i >= 0; i--)
             {
                if (idtype.at(i) == GREEN)
                {
                    identifier.pop_back();
                    idtype.pop_back();
                    break;
                }
                else
                {
                    identifier.pop_back();
                    idtype.pop_back();
                }
             }
			 subprogram_declarations2();
			 break;
    }
}

void subprogram_declarations2()
{
	switch(type)
	{
	case 17: subprogram_declaration();
			 match(";");
			 for (int i = idtype.size() - 1; i >= 0; i--)
             {
                if (idtype.at(i) == GREEN)
                {
                    identifier.pop_back();
                    idtype.pop_back();
                    break;
                }
                else
                {
                    identifier.pop_back();
                    idtype.pop_back();
                }
             }
			 subprogram_declarations2();
			 break;
	}
}

void subprogram_declaration()
{
	switch(type)
	{
	case 17: subprogram_head();
			 subprogram_declaration2();
			 break;
	}
}

void subprogram_declaration2()
{
	switch(type)
	{
	case 13: declarations();
			 subprogram_declarations();
			 compound_statement();
			 break;
	case 14: compound_statement();
			 break;
	case 17: subprogram_declarations();
			 compound_statement();
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected var,procedure, or begin, got " + token + ". Synchronizing...\n";
	}
}

void subprogram_head()
{
	switch(type)
	{
	case 17: match("procedure");
             identifier.push_back(token);
             decs.push_back(token);
             idtype.push_back(GREEN);
             offset = 0;
             memFile << token << setw(11) << setfill(' ') << "\t" << offset << endl;
			 checkID();
			 subprogram_head2();
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected procedure, got " + token + ". Synchronizing...\n";
			 synch(4);
	}
}

void subprogram_head2()
{
	switch(type)
	{
	case 31:
	{        tempdec.clear();
             tempdec.push_back(5);
             declist.push_back(tempdec);
             match(";");
			 break;
	}
	case 35: arguments();
             match(";");
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected ; or (, got " + token + ". Synchronizing...\n";
			 synch(4);
	}
}

void arguments()
{
	switch(type)
	{
	case 35: match("(");
             tempdec.clear();
			 param_list();
			 match(")");
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected (, got " + token + ". Synchronizing...\n";
	}
}

void param_list()
{
	switch(type)
	{
	case 30:
	{        identifier.push_back(token);
             checkID();
			 match(":");
			 int temp = typemethod();
             tempdec.push_back(temp);
			 idtype.push_back(temp);
			 param_list2();
	}
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected id, got " + token + ". Synchronizing...\n";
			 synch(5);
	}
}

void param_list2()
{
	switch(type)
	{
	case 31:
	{        match(";");
             identifier.push_back(token);
			 checkID();
			 match(":");
			 int temp = typemethod();
			 tempdec.push_back(temp);
			 idtype.push_back(temp);
			 param_list2();
	}
			 break;
    default: declist.push_back(tempdec);
             if (token != ")")
             {
                ofile << "SYNERR: Expected id, ), got " + token + ". Synchronizing...\n";
                synch(5);
             }
             break;
	}
}

void compound_statement()
{
	switch(type)
	{
	case 14: match("begin");
			 compound_statement2();
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected begin, got " + token + ". Synchronizing...\n";
			 synch(6);
	}
}

void compound_statement2()
{
	switch(type)
	{
	case 15: match("end");
			 break;
    case 14:
	case 18:
	case 21:
	case 30:
	case 39: optional_statements();
             match("end");
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected call, end, got " + token + ". Synchronizing...\n";
			 synch(14);
	}
}

void optional_statements()
{
    statement_list();
}

void statement_list()
{
	switch(type)
	{
	case 14:
	case 18:
	case 21:
	case 30:
	case 39: statement();
			 statement_list2();
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected call, if, while, begin, id, got " + token + ". Synchronizing...\n";
			 synch(7);
	}
}

void statement_list2()
{
	switch(type)
	{
	case 31: match(";");
			 statement();
			 statement_list2();
			 break;
    default: if (token != "end")
             {
                ofile << "SYNERR: Expected end, got " + token + ". Synchronizing...\n";
                 synch(7);
             }
	}
}

void statement()
{
	switch(type)
	{
	case 14: compound_statement();
			 break;
	case 18:
	{        match("if");
			 int temp5 = expression();
			 if (temp5 != BOOL)
                ofile << "SEMERR: Boolean expression required in if statement" << endl;
			 match("then");
			 statement();
			 statement2();
	}
			 break;
	case 21:
	{        match("while");
			 int temp6 = expression();
			 if (temp6 != BOOL)
                ofile << "SEMERR: Boolean expression required in while statement " << endl;
			 match("do");
			 statement();
	}
			 break;
	case 30:
	{        int temp7 = variable();
			 match(":=");
			 int temp8 = expression();
			 if (temp7 != temp8)
                ofile << "SEMERR: Invalid variable assignment " << endl;
	}
			 break;
	case 39: procedure_statement();
			 break;
    default: ofile << "SYNERR: Expected call, if, while, begin, id, got " + token + ". Synchronizing...\n";
             synch(8);
             break;
	}
}

void statement2()
{
	switch(type)
	{
	case 19: match("else");
			 statement();
			 break;
	}
}

int variable()
{
    int temp9 = ERROR;

	switch(type)
	{
	case 30:
	{        for (int i = identifier.size() - 1; i >= 0; i--)
             {
                if ((identifier.at(i)).compare(token) == 0)
                {
                    temp9 = idtype.at(i);
                    break;
                }
             }
             if (temp9 == ERROR)
             {
                 ofile << "SEMERR: variable not declared in this scope" << endl;
                 return temp9;
             }
             else
             {
                checkID();
                int temp = variable2(temp9);
                return temp;
             }
	}
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected id, got " + token + ". Synchronizing...\n";
			 synch(9);
	}
}

int variable2(int temp)
{
	switch(type)
	{
	case 40:
	{        match("[");
			 expression();
			 match("]");
			 return temp - 2;
	}
    default: return temp;
	}
}

void procedure_statement()
{
	switch(type)
	{
	case 39:
	{        match("call");
             bool found = false;
             for (int i = 0; i < decs.size(); i++)
             {
                if (decs.at(i) == token)
                {
                    found = true;
                    deccounter = i;
                    break;
                }
             }
             if (found == false)
             {
                ofile << "SEMERR: invalid procedure call" << endl;
             }
			 checkID();
			 procedure_statement2();
			 break;
	}
	default: // print error and do sync
			 ofile << "SYNERR: Expected call, got " + token + ". Synchronizing...\n";
			 synch(10);
	}
}

void procedure_statement2()
{
	switch(type)
	{
	case 35:
	{        procerror = false;
             tempdec = declist.at(deccounter);
             if (tempdec.front() == 5)
             {
                 ofile << "SEMERR: Invalid procedure parameters in procedure call" << endl;
                 procerror = true;
             }
             match("(");
			 expression_list();
			 match(")");
			 break;
	}
    default:
    {
            tempdec = declist.at(deccounter);
            if (tempdec.front() != 5)
                ofile << "SEMERR: Invalid procedure parameters in procedure call" << endl;
    }
	}
}

void expression_list()
{
    paramcounter = 0;
	int temp = expression();
	if (tempdec.front() != 5 && temp != tempdec.front())
	{
	         ofile << "SEMERR: Invalid procedure parameters in procedure call" << endl;
	         procerror = true;
	}
	paramcounter++;
	expression_list2();
}

void expression_list2()
{
	switch(type)
	{
	case 34:
	{        match(",");
			 int temp = expression();
			 if (procerror == false && tempdec.size() >= paramcounter + 1)
			 {
			     if (temp != tempdec.at(paramcounter))
			     {
                    ofile << "SEMERR: Invalid procedure parameters in procedure call" << endl;
                    procerror = true;
			     }
			 }
			 paramcounter++;
			 expression_list2();
			 break;
	}
	}
}

int expression()
{
	int temp10 = simple_expression();
	return expression2(temp10);
}

int expression2(int temp11)
{
	switch(type)
	{
	case 29:
	{        analyzer();
			 int temp12 = simple_expression();
			 if (temp11 != temp12)
			 {
                ofile << "SEMERR: Incompatible primitive types in expression" << endl;
                return ERROR;
			 }
             else
                return BOOL;
	}
			 break;
    default: return temp11;
	}
}

int simple_expression()
{
    int temp13 = ERROR;
	switch(type)
	{
	case 11:
	case 12:
	case 25:
	case 35:
	case 30:
	{
             temp13 = term();
			 return simple_expression2(temp13);
	}
			 break;
	case 26:
	{        sign();
			 temp13 = term();
			 return simple_expression2(temp13);
	}
			 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected id, num, (, +, -, got " + token + ". Synchronizing...\n";
			 synch(11);
	}
}

int simple_expression2(int temp14)
{
	switch(type)
	{
	case 26:
	{        analyzer();
			 int temp15 = term();
			 if (temp14 != temp15)
			 {
			     ofile << "SEMERR: Incompatible types " << endl;
                 return ERROR;
			 }
			 return simple_expression2(temp15);
	}
			 break;
    default: return temp14;
	}
}

int term()
{
	int temp16 = factor();
	return term2(temp16);
}

int term2(int temp17)
{
	switch(type)
	{
	case 27:
	{        analyzer();
			 int temp18 = factor();
			 if (temp17 != temp18)
			 {
                ofile << "SEMERR: Incompatible types " << endl;
                return ERROR;
			 }
			 return term2(temp17);
	}
			 break;
    default: return temp17;
	}
}

int factor()
{
    int temp19 = ERROR;

	switch(type)
	{
	case 11: analyzer();
             return INT;
             break;
	case 12: analyzer();
			 return REAL;
			 break;
	case 25: match("not");
			 return factor();
			 break;
	case 35:
	{        match("(");
             temp19 = expression();
			 match(")");
			 return temp19;
	}
             break;
	case 30:
	{        for (int i = identifier.size() - 1; i >= 0; i--)
             {
                if ((identifier.at(i)).compare(token) == 0)
                {
                    temp19 = idtype.at(i);
                    break;
                }
             }
             if (temp19 == ERROR)
             {
                 ofile << "SEMERR: variable not declared in this scope" << endl;
                 return temp19;
             }
             else
             {
                 analyzer();
                 int temp = factor2(temp19);
                 return temp;
             }

	}
             break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected id, num, (, not, got " + token + ". Synchronizing...\n";
			 synch(12);
	}
}

int factor2(int temp)
{
    switch(type)
    {
        case 35:
        {        match("(");
                 int temp2 = expression();
                 match(")");
                 if (temp2 != INT)
                 {
                    ofile << "SEMERR: Invalid array location " << endl;
                    return ERROR;
                 }
                 return temp - 2;
        }
        case 40:
        {        match("[");
                 int temp2 = expression();
                 match("]");
                 if (temp2 != INT)
                 {
                    ofile << "SEMERR: Invalid array location " << endl;
                    return ERROR;
                 }
                 return temp - 2;
        }
        default: return temp;
    }
}

void sign()
{
	switch(type)
	{
	case 26: if (token == "+" || token == "-")
				 break;
	default: // print error and do sync
			 ofile << "SYNERR: Expected + or -, got " + token + ". Synchronizing...\n";
			 synch(13);

	}
}

// searches for the follow set of a production that is called
void synch(int a)
{
    switch(a)
    {
        // id list
        case 1: while (token != ")")
                {
                    analyzer();
                }
                break;
        // declarations
        case 2: while (!(token == "procedure" || token == "begin"))
                {
                    analyzer();
                }
                break;
        // type and standard type
        case 3: while (!(token == ";" || token == ")"))
                {
                    analyzer();
                }
                break;
        // subprogram head
        case 4: while (!(token == "procedure" || token == "var" || token == "begin"))
                {
                    analyzer();
                }
                break;
        // param list
        case 5: while (token != ")")
                {
                    analyzer();
                }
                break;
        // compound statement
        case 6: while (!(token == ";" || token == "."))
                {
                    analyzer();
                }
                break;
        // statement list
        case 7: while (token != "end")
                {
                    analyzer();
                }
                break;
        // statement
        case 8: while (!(token == ";" || token == "end"))
                {
                    analyzer();
                }
                break;
        // variable
        case 9: while (token != ":=")
                {
                    analyzer();
                }
                break;
        // procedure statement
        case 10: while (!(token == ";" || token == "end"))
                 {
                     analyzer();
                 }
                 break;
        // simple expression
        case 11: analyzer();
                 if (type == 11 || type == 12 || type == 25 || type == 26 || type == 30 || type == 35)
                 {
                     simple_expression();
                     break;
                 }
                 while (!(token == "<" || token == ">" || token == "<>" || token == "=" || token == "<=" || token == ">=" ||
                          token == "," || token == ";" || token == "then" || token == "do" || token == "]"))
                 {
                     analyzer();
                 }
                 break;
        // factor
        case 12: analyzer();
                 if (type == 11 || type == 12 || type == 25 || type == 30 || type == 35)
                 {
                     factor();
                     break;
                 }
                 while (!(token == "<" || token == ">" || token == "<>" || token == "=" || token == "<=" || token == ">=" ||
                          token == "+" || token == "-" || token == "or" || token == "*" || token == "/" || token == "div" ||
                          token == "mod" || token == "and" || token == ";" || token == "then" || token == "," || token == "do" ||
                          token == "]" || token == ")" || token == "end" || token == "else"))
                 {
                     analyzer();
                 }
                 break;
        // sign
        case 13: while (!(token == "(" || token == "not" || type == 30 || type == 11 || type == 12))
                 {
                     analyzer();
                 }
                 break;
        // compound statement 2
        case 14: while(!(token == ";" || token == "end"))
                 {
                     analyzer();
                 }
                 if (token == ";")
                 {
                    analyzer();
                    optional_statements();
                    match("end");
                 }
                 break;
    }
}

void checkID()
{
    if (type != 30)
    {
        ofile << "SYNERR: Expected id, got " + token << endl;
    }
    analyzer();
}

// tree for declarations scoping
class tree
{

};

int main()
{
	init();
	parser();
	finish();
	// for (int i = 0; i < identifier.size(); i++)
    //    cout << identifier.at(i) << "\t" << idtype.at(i) << endl;
	return 0;
}
