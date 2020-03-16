#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <cctype>
#include <map>
#include <vector>
#include <utility>


using namespace std;

string keyword[20] = { "int", "float", "bool", "if", "else", "then",
"do", "while", "whileend", "do", "doend", "for", "and", "or" , "function" };

char sym_list[21] =
{
	'*','+','-','=','/','>','<','%','\'','(',')','{','}','[',']',',','.',':',';','!',' '
};

enum s {

	Q, S, T, R, F, E,

	IDENT, AST, BACKSLASH, PLUS, MINUS, RPAREN, LPAREN, DOLLAR, EQUAL, EPSILON
};

int statemachine[6][7] = {
		1,2,4,0,0,0,0,
		1,1,4,0,0,0,0,
		5,2,4,0,3,0,0,
		5,3,4,0,3,0,0,
		4,4,0,4,4,4,4,
		0,0,0,0,0,0,0
};


string out = "output.txt";
vector<pair<string, string> > output;


int check(char ch, int state)
{
	int C = state;

	if (isalpha(ch))
	{
		C = statemachine[C][0];
		return C;
	}
	else if (isdigit(int(ch)) != 0 || ch == '$')
	{
		C = statemachine[C][1];
		return C;
	}
	else if (ch == sym_list[19]) 
	{
		C = statemachine[C][2];
		return C;
	}
	else if (ch == sym_list[16])
	{
		C = statemachine[C][4];
		return C;
	}
	else if (isspace(ch))
	{
		C = statemachine[C][6];
		return C;
	}
	else
	{
		int i = 0;
		for (i = 0; i < 8; i++)
		{
			if (ch == sym_list[i] || isspace(ch))
			{
				C = statemachine[C][5];
				return C;
			}
		}
		for (i = 8; i < 20; i++)
		{
			if (ch == sym_list[i] || isspace(ch))
			{
				C = statemachine[C][3];
				return C;
			}
		}
	}
}



void check2(string r)
{

	
	if (r != "")
	{
		bool check_keyword = 0;
		
		for (int i = 0; i < 20; i++) {

			if (keyword[i] == r)
			{
				
				output.push_back(pair<string, string>("Keyword:\t", r));
				return;
			}
		}
		check_keyword = 1;
		
		if (isalpha(r[0]) && check_keyword == 1)
		{
			
			output.push_back(pair<string, string>("IDENT:\t", r));
			return;
		}
		
		int f = 0;
		int d = 0;
		for (int i = 0; i < r.length(); i++)
		{
			if (r[i] == '.')
			{
				f = 1;
				d++;

			}
			if (r[i] == '$')
			{
				f = 2;
			}
		}
		if (d > 1)
		{
			f = 2;
		}
		if (f == 0)		
		{
			
			output.push_back(pair<string, string>("Integer:\t", r));
			return;
		}
		else if (f == 1)	
		{
			
			output.push_back(pair<string, string>("Float:\t\t", r));
			return;
		}
		
		output.push_back(pair<string, string>("Invalid:\t", r));
		return;
	}
}


void check3(char ch)
{
	

	string r = string(1, ch);
	for (int i = 0; i < 8; i++)
	{
		if (ch == sym_list[i])
		{
			
			output.push_back(pair<string, string>("Operator:\t", r));
		}
	}
	for (int i = 8; i < 22; i++)
	{
		if (ch == sym_list[i] && ch != '!' && !isspace(ch))
		{
			
			output.push_back(pair<string, string>("Separator:\t", r));
		}
	}

}



int main()
{
	map<s, map<s, int> >  table;

	
	table[S][IDENT] = 1;
	table[E][IDENT] = 2;
	table[Q][PLUS] = 3;
	table[Q][MINUS] = 4;
	table[Q][RPAREN] = 5;
	table[Q][DOLLAR] = 5;
	table[T][IDENT] = 6;
	table[T][RPAREN] = 6;
	table[R][PLUS] = 5;
	table[R][MINUS] = 5;
	table[R][BACKSLASH] = 7;
	table[R][RPAREN] = 5;
	table[R][DOLLAR] = 5;
	table[F][IDENT] = 8;
	table[F][LPAREN] = 9;
	string tokenString = "";
	char  chTest = 2;
	string input = ""; 
	string r;
	char ch;
	int C = 0;
	int lengthofString = 0;

	stack<s> stck;
	stck.push(DOLLAR);
	stck.push(S);

	

	string filename;
	cout << "Enter Filename: ";
	cin >> filename;
	ifstream fin(filename);

	
	if (!fin.is_open()) {
		cout << "Cannot open file" << endl;
		exit(0);
	}
	ofstream of(out);
	if (!of.is_open())
	{
		cout << "Cannot open file" << endl;
		exit(0);
	}

	
	while (getline(fin, input))
	{

		lengthofString = input.length(); 


		for (int i = 0; i < lengthofString; i++) 
		{
			ch = input[i];

			if (C != 4) 
			{
				C = check(ch, C);

				switch (C)
				{
				case 0:					 
					check2(r);
					check3(ch);
					r = "";
					break;

				case 1:		
					r += ch;
					break;
				case 2:
					if (ch == '.') 
					{
						r += ch;
						break;
					}
					r += int(ch);
					break;
				case 3:
					if (ch == '.')
					{
						r += ch;
						break;
					}
					r += int(ch);
					break;

				case 4:					
					check2(r);
					r = "";
					break;
				case 5:					
					check2(r);
					check3(ch);
					r = "";
					break;
				}

			}
			else if (ch == '!' && C == 4)	
			{
				C = statemachine[C][2];

			}

		}
		check2(r);						
		r = "";
	}
	fin.close();	

					
	vector <pair<s, string> > lex;
	for (int i = 0; i < output.size(); i++) {
		if (output[i].first == "IDENT:\t") {
			lex.push_back(pair<s, string>(IDENT, output[i].second));
		}
		else if (output[i].second == "+") {
			lex.push_back(pair<s, string>(PLUS, output[i].second));
		}
		else if (output[i].second == "-") {
			lex.push_back(pair<s, string>(MINUS, output[i].second));
		}
		else if (output[i].second == "*") {
			lex.push_back(pair<s, string>(AST, output[i].second));
		}
		else if (output[i].second == "/") {
			lex.push_back(pair<s, string>(BACKSLASH, output[i].second));
		}
		else if (output[i].second == "(") {
			lex.push_back(pair<s, string>(RPAREN, output[i].second));
		}
		else if (output[i].second == ")") {
			lex.push_back(pair<s, string>(LPAREN, output[i].second));
		}
		else if (output[i].second == "$") {
			lex.push_back(pair<s, string>(DOLLAR, output[i].second));
		}
		else if (output[i].second == "=") {
			lex.push_back(pair<s, string>(EQUAL, output[i].second));
		}
		else {
			cout << "Error: Character does not exist in this grammar" << endl;
		}
	}
	lex.push_back(pair<s, string>(DOLLAR, "$")); 
	
	while (stck.size() > 0) {
		for (int i = 0; i < lex.size(); i++) {
			for (int j = 0; j < sizeof(s); j++) {
				if (lex[i].first == IDENT) {
					tokenString = "IDENT";
				}
				else if (lex[i].first == PLUS || lex[i].first == MINUS ||
					lex[i].first == AST || lex[i].first == BACKSLASH
					|| lex[i].first == DOLLAR || lex[i].first == EQUAL) {
					tokenString = "Operator";
				}
				else if (lex[i].first == RPAREN || lex[i].first == LPAREN) {
					tokenString = "Seperator";
				}
				else if (lex[i].first == EPSILON) {
					tokenString = "e";
				}
				else {
					tokenString = "Invalid Syntax";
				}
			}


			if (stck.top() == lex[i].first) {
				cout << "Token: " << tokenString << "\t ";
				cout << "Lexeme: " << lex[i].second << endl;
				stck.pop();
			}
			else if (stck.top() == EQUAL) {
				cout << "Token: " << tokenString << "\t ";
				cout << "Lexeme: " << lex[i].second << endl;
				stck.pop();
			}
			else {
				cout << "Rule " << table[stck.top()][lex[i].first] << endl;
				switch (table[stck.top()][lex[i].first]) {
				case 1: 
					stck.pop();
					stck.push(E);        
					stck.push(EQUAL);
					stck.push(IDENT);
					cout << "<Statement> -> <IDENT> = <Expression>" << endl;
					i--;
					break;

				case 2:
					stck.pop();
					stck.push(Q);
					stck.push(T);
					cout << "<Expression> -> <Term><Expression Prime>" << endl;
					i--;
					break;

				case 3:
					stck.pop();
					stck.push(Q);
					stck.push(T);  
					stck.push(PLUS);  
					cout << "<Expression Prime> -> +<Term><Expression Prime>" << endl;
					i--;
					break;

				case 4:
					stck.pop();
					stck.push(Q);	 
					stck.push(T);	 
					stck.push(MINUS);
					cout << "<Expression Prime> -> -<Term><Expression Prime>" << endl;
					i--;
					break;

				case 5:
					stck.pop();
					cout << "<Expression Prime> or <Term Prime> -> <e>" << endl;
					i--;
					break;

				case 6:
					stck.pop();
					stck.push(R);
					stck.push(F);
					cout << "<Term> -> <Factor><Term Prime>" << endl;
					i--;
					break;

				case 7:
					table[R][BACKSLASH] = 7;
					stck.pop();
					stck.push(R);
					stck.push(F);
					cout << "<Term Prime> -> <Factor><Term Prime>" << endl;
					i--;
					break;

				case 8:
					stck.pop();
					stck.push(IDENT);
					cout << "<Factor> -> <IDENT>" << endl;
					i--;
					break;

				case 9:
					stck.pop();
					stck.push(RPAREN);
					stck.push(E);
					stck.push(LPAREN);
					cout << "<Factor> -> (Expression)" << endl;
					i--;
					break;

				default:
					cout << "default" << endl;
					break;
				}
			}
			cout << endl;
		}
		of.close();
	}

return 0;
}
	


