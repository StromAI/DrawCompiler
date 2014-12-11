#include "parse.h"
#include <iostream>
#include <sstream>
using namespace std;

Parse::Parse()
	:	
	Parameter(0),
	b_startFor(false),
	b_endFor(false),
	b_stepFor(false),
	b_drawXFor(false),
	b_drawYFor(false),
	b_xOrigin(false),
	b_yOrigin(false),
	b_Rot(false),
	b_xScale(false),
	b_yScale(false)
{ 
	scanner = Scanner::Instance();
}

//--------------------------------------------------------------------------
void Parse::out(int space)
{
	while(space)
	{
		cout << "\t";
		space--;
	}
}

//--------------------------------------------------------------------------
void Parse::PrintSyntaxTree(struct ExprNode * root, int indent)
{
	
	out(indent);
	switch(root->OpCode)
	{
		case CONST_ID:
			cout << root->Content.CaseConst << endl;
			break;
		case T:
			cout << "T" << endl;
			break;
		case FUNC:
			cout << root->Content.CaseFunc.MathFuncPtr << endl;
			PrintSyntaxTree(root->Content.CaseFunc.Child, indent+1); 
			break;
		default:
			char tmp;
			switch(root->OpCode){
				case PLUS: tmp = '+'; break;
				case MINUS: tmp = '-'; break;
				case MUL: tmp = '*'; break;
				case DIV: tmp = '/'; break;
				case POWER: tmp = '**'; break;
			}
			cout << tmp << endl;
			PrintSyntaxTree(root->Content.CaseOperator.Left, indent+1);
			PrintSyntaxTree(root->Content.CaseOperator.Right, indent+1);
	}
}

//--------------------------------------------------------------------------
struct ExprNode * Parse::MakeExprNode(enum Token_Type opcode, ...)
{
	struct ExprNode * exprPtr = new ExprNode();
	exprPtr->OpCode = opcode;
	
	va_list argPtr;
	va_start(argPtr, opcode);
	
	switch(opcode)
	{
		case CONST_ID:	// 常量 
			exprPtr->Content.CaseConst = (double)va_arg(argPtr, double);
			break;
		case T:			// T
			exprPtr->Content.CaseParmPtr = &Parameter;
			break;
		case FUNC:		//函数调用 
			exprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(argPtr, FuncPtr);
			exprPtr->Content.CaseFunc.Child = 
							(struct ExprNode *)va_arg(argPtr, struct ExprNode *);
			break;
		default:		// 二元运算 
			exprPtr->Content.CaseOperator.Left = 
							(struct ExprNode *)va_arg(argPtr, struct ExprNode *);
			exprPtr->Content.CaseOperator.Right =
							(struct ExprNode *)va_arg(argPtr, struct ExprNode *);
			break;
	}
	
	va_end(argPtr);
	return exprPtr;
}

//--------------------------------------------------------------------------
void Parse::FetchToken()		 
{
	token = scanner.GetToken();
}

//--------------------------------------------------------------------------
void Parse::SyntaxError(int num)
{
	cout << "wrong token !" << endl;
}

//--------------------------------------------------------------------------
void Parse::MatchToken(enum Token_Type The_Token)
{
	if(token.type != The_Token) SyntaxError(2);
    cout << "match token		" << token.lexeme << endl;
	
	//check Token and add it to the buffer
	checkPoint();
	
	FetchToken();
}



// ----------------------------------------------------------------------
struct ExprNode * Parse::Atom()
{
	struct ExprNode *left = NULL, *right = NULL;
	FuncPtr funPtr;
		
	switch(token.type)
	{
		case CONST_ID:
			left = MakeExprNode(CONST_ID, token.value);
			MatchToken(CONST_ID);
			break;
		case T:
			MatchToken(T);
			left = MakeExprNode(T); 
			break;
		case FUNC:
			funPtr = token.FuncPtr;
			MatchToken(FUNC);
			MatchToken(L_BRACKET);
			right = Expression();
			MatchToken(R_BRACKET);
			left = MakeExprNode(FUNC, funPtr, right);
			break;
		case L_BRACKET:
			MatchToken(L_BRACKET);
			left = Expression();
			MatchToken(R_BRACKET);
	}
	return left;
}

//--------------------------------------------------------------------------
struct ExprNode * Parse::Component()
{
	struct ExprNode *left, *right;
	Token_Type token_tmp;
	
	left = Atom();
	while(token.type == POWER)
	{
		MatchToken(POWER);
		right = Component();
		left = MakeExprNode(POWER, left, right);
	}
	return left;
}

//--------------------------------------------------------------------------
struct ExprNode * Parse::Factor()
{
	struct ExprNode *left, *right;
	
	if(token.type == PLUS)
	{
		MatchToken(PLUS);
		left = Factor();
	}
	else if(token.type == MINUS)
	{
		MatchToken(MINUS);
		left = MakeExprNode(CONST_ID, 0);
		right = Factor();
		left = MakeExprNode(MINUS, left, right);
	}
	else
		left = Component();
	return left;
}

//--------------------------------------------------------------------------
struct ExprNode * Parse::Term()
{
	struct ExprNode *left, *right;
	Token_Type token_tmp;
	left = Factor();
	
	while(token.type == MUL || token.type == DIV)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp, left, right); 
	}
	return left;
}

//--------------------------------------------------------------------------
struct ExprNode * Parse::Expression()
{
	cout << "enter Expression" << endl;
	struct ExprNode *left, *right;
	Token_Type token_tmp;
	left = Term();
	char tmp;
	
	while(token.type == PLUS || token.type == MINUS)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Term();
		
		left = MakeExprNode(token_tmp, left, right); 
	}
	cout << "exit from Expression" << endl;
	
	PrintSyntaxTree(left, 0);
	return left;
}

//---------------------------------------------------------------------
void Parse::ForStatement()
{
	cout << "enter ForStatement" << endl;
	MatchToken(FOR);
	MatchToken(T);
	MatchToken(FROM);

	b_startFor = true;
	Expression();
	b_startFor = false;

	MatchToken(TO);

	b_endFor = true;
	Expression();
	b_endFor = false;

	MatchToken(STEP);

	b_stepFor = true;
	Expression();
	b_stepFor = false;

	MatchToken(DRAW);
	MatchToken(L_BRACKET);

	b_drawXFor = true;
	Expression();
	b_drawXFor = false;
	MatchToken(COMMA);

	b_drawYFor = true;
	Expression();
	b_drawYFor = false;
	MatchToken(R_BRACKET); 
	cout << "exit from forStatement" << endl;
}

//--------------------------------------------------------------------------
void Parse::OriginStatement()
{
	cout << "enter OriginStatement" << endl;
	MatchToken(ORIGIN);
	MatchToken(IS);
	MatchToken(L_BRACKET);	

	b_xOrigin = true;
	Expression();
	b_xOrigin = false;

	MatchToken(COMMA);

	b_yOrigin = true;
	Expression();
	b_yOrigin = false;
	MatchToken(R_BRACKET);	
	cout << "exit from OriginStatement" << endl;
}

//--------------------------------------------------------------------------
void Parse::RotStatement()
{
	cout << "enter RotStatement" << endl;
	MatchToken(ROT);
	MatchToken(IS);
	b_Rot = true;
	Expression();
	b_Rot = false;
	cout << "exit from RotStatement" << endl;
}

//--------------------------------------------------------------------------
void Parse::ScaleStatement()
{
	cout << "enter ScaleStatement" << endl;
	MatchToken(SCALE); 
	MatchToken(IS);
	MatchToken(L_BRACKET);

	b_xScale = true;
	Expression();
	b_xScale = false;
	MatchToken(COMMA);

	b_yScale = true;
	Expression();
	b_yScale = false;
	MatchToken(R_BRACKET);		
	cout << "exit from ScaleStatement" << endl;
}

//--------------------------------------------------------------------------
void Parse::Statement()
{
	cout << "enter Statement" << endl;
	switch(token.type){
		case ORIGIN:
			OriginStatement(); break;
		case SCALE:
			ScaleStatement(); break;
		case ROT:
			RotStatement(); break;
		case FOR:
			ForStatement(); break;
	}
	cout << "exit from Statement" << endl;
	return;
}

//--------------------------------------------------------------------------
void Parse::Program()
{
	cout << "enter Program" << endl;
	while(token.type != NONTOKEN)
	{
		Statement();
		MatchToken(SEMICO);
	}
	cout << "exit from Program" << endl;
}

//-------------------------------------------------------------------
void Parse::Parser(char *SrcFilePtr)
{
	if(!scanner.InitScanner(SrcFilePtr))
	{
		cout << "Open Source File Error!" << endl;
		return;
	}
	cout << "Open Source File OK" << endl;
	FetchToken();
	Program();
	scanner.CloseScanner();
}

//-------------------------------------------------------------------
void Parse::checkPoint()
{
	if(b_startFor)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			startFor.push_back(token.lexeme);
	}
	if(b_endFor)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			endFor.push_back(token.lexeme);
	}
	if(b_stepFor)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			stepFor.push_back(token.lexeme);
	}
	if(b_drawXFor)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			drawXFor.push_back(token.lexeme);
	}
	if(b_drawYFor)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			drawYFor.push_back(token.lexeme);
	}
	if(b_xOrigin)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			xOrigin.push_back(token.lexeme);
	}
	if(b_yOrigin)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			yOrigin.push_back(token.lexeme);
	}
	if(b_Rot)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			rot.push_back(token.lexeme);
	}
	if(b_xScale)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			xScale.push_back(token.lexeme);
	}
	if(b_yScale)
	{
		if(token.type == FUNC || token.type == CONST_ID || token.type == T || token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV || token.type == POWER || token.type == L_BRACKET || token.type == R_BRACKET)
			yScale.push_back(token.lexeme);
	}
}

//For Statement
//-------------------------------------------------------------------
string Parse::getStartFor()
{
	vector<string>::iterator it;
	string sStartFor;
	for(it = startFor.begin();it != startFor.end(); ++it)
	{
		sStartFor = sStartFor + *it;
	}
	return sStartFor;
}

//-------------------------------------------------------------------
string Parse::getEndFor()
{
	vector<string>::iterator it;
	string sEndFor;
	for(it = endFor.begin();it != endFor.end(); ++it)
	{
		sEndFor = sEndFor + *it;
	}
	return sEndFor;
}

//-------------------------------------------------------------------
string Parse::getStepFor()
{
	vector<string>::iterator it;
	string sStepFor;
	for(it = stepFor.begin();it != stepFor.end(); ++it)
	{
		sStepFor = sStepFor + *it;
	}
	return sStepFor;
}

//-------------------------------------------------------------------
string Parse::getDrawXFor()
{
	vector<string>::iterator it;
	string sDrawXFor;
	for(it = drawXFor.begin();it != drawXFor.end(); ++it)
	{
		sDrawXFor = sDrawXFor + *it;
	}
	return sDrawXFor;
}

//-------------------------------------------------------------------
string Parse::getDrawYFor()
{
	vector<string>::iterator it;
	string sDrawYFor;
	for(it = drawYFor.begin();it != drawYFor.end(); ++it)
	{
		sDrawYFor = sDrawYFor + *it;
	}
	return sDrawYFor;
}

//Origin Statement
//-------------------------------------------------------------------
string Parse::getXOrigin()
{
	vector<string>::iterator it;
	string sXOrigin;
	for(it = xOrigin.begin();it != xOrigin.end(); ++it)
	{
		sXOrigin = sXOrigin + *it;
	}
	return sXOrigin;
}

//-------------------------------------------------------------------
string Parse::getYOrigin()
{
	vector<string>::iterator it;
	string sYOrigin;
	for(it = yOrigin.begin();it != yOrigin.end(); ++it)
	{
		sYOrigin = sYOrigin + *it;
	}
	return sYOrigin;
}

//Rot Statement
//-------------------------------------------------------------------
string Parse::getRot()
{
	vector<string>::iterator it;
	string sRot;
	for(it = rot.begin();it != rot.end(); ++it)
	{
		sRot = sRot + *it;
	}
	return sRot;
}

//Scale Statement
//-------------------------------------------------------------------
string Parse::getXScale()
{
	vector<string>::iterator it;
	string sXScale;
	for(it = xScale.begin();it != xScale.end(); ++it)
	{
		sXScale = sXScale + *it;
	}
	return sXScale;
}

//-------------------------------------------------------------------
string Parse::getYScale()
{
	vector<string>::iterator it;
	string sYScale;
	for(it = yScale.begin();it != yScale.end(); ++it)
	{
		sYScale = sYScale + *it;
	}
	return sYScale;
}