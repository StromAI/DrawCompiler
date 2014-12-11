/*************************************************************************
parse.h
Copyright (C), Sliencer, 2014 - Orz
-------------------------------------------------------------------------
Description: 
	Syntax analysis
-------------------------------------------------------------------------
History:
- 12:10:2014		Created by Sliencer 

*************************************************************************/
#include "scanner.h"
#include <cstdarg>
#include <iostream>
#include <vector>

using namespace std;

typedef double (*FuncPtr)(double);

struct ExprNode
{
	enum Token_Type OpCode;
	union
	{
		struct {ExprNode *Left, *Right;} CaseOperator;				// 二元运算 
		struct {ExprNode *Child; FuncPtr MathFuncPtr;} CaseFunc;	//函数调用 
		double CaseConst;											// 常量 
		double *CaseParmPtr;										// T
	}Content;
};

class Parse
{
public:
	Parse();

	static Parse &Instance(){static Parse instance;return instance;}

//--------------------------------------------------------------------
	void Parser(char *SrcFilePtr);

//(UI_Interfaces)-----------------------------------------------------

	//For Statement
	string getStartFor();
	string getEndFor();
	string getStepFor();
	string getDrawXFor();
	string getDrawYFor();
	
	//Origin Statement
	string getXOrigin();
	string getYOrigin();

	//Rot Statement
	string getRot();

	//Scale Statement
	string getXScale();
	string getYScale();


private:
	void out(int speace);
	void PrintSyntaxTree(struct ExprNode * root, int indent);
	struct ExprNode * MakeExprNode(enum Token_Type opcode, ...);
	void FetchToken();
	void SyntaxError(int num);
	void MatchToken(enum Token_Type The_Token);

	//Atom --->
	//	CONST_ID | T | FUNC L_BRACKET Expression R_BRACKET 
	//	| L_BRACKET Eexpression R_BRACKET
	struct ExprNode *Atom();

	//Component --->
	// 		Atom [POWER Component]
	struct ExprNode * Component();

	//Factor --->
	//		PLUS Factor | MINUS Factor | Component
	struct ExprNode * Factor();

	// Term ---> 
	// 		Factor { ( MUL | DIV ) Factor }
	struct ExprNode * Term();

	//Expression --->
	//		Term {(PLUS | MINUS) Term}
 	struct ExprNode * Expression();


//----------------------------------------------------------
	
	//ForStatement --->
	//	FOR T FROM Expression TO Expression STEP Expression
	void ForStatement();

	//OriginStatment ---> 
	//	ORIGIN IS L_BRACKET Expression COMMA Expression R_BRACKET
	void OriginStatement();

	// RotStatment --->
	//		ROT IS Expression
	void RotStatement();

	// ScaleStatment ---> 
	//		SCALE IS L_BRACKET Expression COMMA Expression R_BRACKET
	void ScaleStatement();

	// Statement ---> 
	// 		OriginStatment | ScaleStatment | RotStatment | ForStatment
	void Statement();

	// Program  ---> 
	//		ε| Program Statement SEMICO
	void Program();

//----------------------------------------------------

	//check flag and add token into buffer
	void checkPoint();

private:
	Scanner scanner;
	struct Token token;
	double Parameter;	// T
	
//----------------------------------------------------
	//  [12/10/2014 Sliencer]
	//UI needed
	//Check point
	bool b_startFor;
	bool b_endFor;
	bool b_stepFor;
	bool b_drawXFor;
	bool b_drawYFor;

	bool b_xOrigin;
	bool b_yOrigin;

	bool b_Rot;

	bool b_xScale;
	bool b_yScale;

	//For Statement
	vector<string> startFor;
	vector<string> endFor;
	vector<string> stepFor;
	vector<string> drawXFor;
	vector<string> drawYFor;

	//Origin Statement
	vector<string> xOrigin;
	vector<string> yOrigin;

	//Rot Statement
	vector<string> rot;

	//Scale Statement
	vector<string> xScale;
	vector<string> yScale;
};