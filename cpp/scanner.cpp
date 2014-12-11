#include "scanner.h"

Scanner::Scanner(char const *FileName){
	InitScanner(FileName);
}

bool Scanner::InitScanner(char const *FileName){

	char BufferTop = 0;

	InFile = fopen(FileName, "r");

	if (InFile){
		LineNo = 1;
		return true;
	}
	return false;	
}

void Scanner::CloseScanner()
{
	fclose(InFile);
}

Token Scanner::GetToken()
{
	EmptyTokenString();
	struct Token token = {ERRTOKRN, "", 0.0, NULL};    

	token.lexeme = TokenBuffer;
	char nch = GetChar();

	while (true) {

		if (isalpha(nch)) {
			AddCharTokenString(nch);
			nch = GetChar();

			while (isalpha(nch)) {
				AddCharTokenString(nch);
				nch = GetChar();
			}

			BackChar();

			AddCharTokenString('\0');


			for (int i = 0; i < 18; i++) {
				if (strcmp(TokenBuffer, TokenTab[i].lexeme) == 0) {
					return TokenTab[i];
				}
			}

			return token;

		}else if (isdigit(nch)){
			AddCharTokenString(nch);
			nch = GetChar();

			while (isdigit(nch)) {
				AddCharTokenString(nch);
				nch = GetChar();
			}

			if (nch == '.') {
				AddCharTokenString(nch);
				nch = GetChar();

				while (isdigit(nch)) {
					AddCharTokenString(nch);
					nch = GetChar();
				}
			}

			BackChar();
			AddCharTokenString('\0');

			token.type = CONST_ID;
			token.value = atof(TokenBuffer);
			return token;

		}else{
			switch (nch) {
			case ';': token.type = SEMICO;
				AddCharTokenString(nch);
				AddCharTokenString('\0'); return token;
			case '(': token.type = L_BRACKET;
				AddCharTokenString(nch);
				AddCharTokenString('\0'); return token;
			case ')': token.type = R_BRACKET;
				AddCharTokenString(nch);
				AddCharTokenString('\0'); return token;
			case ',': token.type = COMMA;
				AddCharTokenString(nch);
				AddCharTokenString('\0'); return token;
			case '+': token.type = PLUS;
				AddCharTokenString(nch);
				AddCharTokenString('\0'); return token;

			case '-': {
				token.type = MINUS;
				AddCharTokenString(nch);

				char nch = GetChar();
				if (nch == '-') {
					fgets(trash, 180, InFile);
					return GetToken();
				}
				BackChar();

				AddCharTokenString('\0'); return token;
					  }
			case '/': {
				token.type = DIV;
				AddCharTokenString(nch);

				char nch = GetChar();
				if (nch == '/') {
					fgets(trash, 180, InFile);
					return GetToken();
				}	
				BackChar();
				AddCharTokenString('\0'); return token;
					  }
			case '*': {
				AddCharTokenString(nch);
				char nch = GetChar();
				if  (nch == '*'){
					AddCharTokenString(nch);
					AddCharTokenString('\0');
					token.type = POWER;
					return token;
				}
				AddCharTokenString('\0');
				BackChar();
				token.type = MUL; return token;
					  }
			case EOF: {
				token.type = NONTOKEN;
				return token;
					  }
			default:
				nch = GetChar();
				continue;
			}
		}
	}

	return token;
}

char Scanner::GetChar()
{
	char ret = fgetc(InFile);
	if (isalpha(ret)) {
		if (ret >= 'a' && ret <= 'z') {
			ret = ret-'a'+'A';
		}
	}
	return ret;
}

void Scanner::BackChar()
{
	fseek(InFile, -static_cast<long>(sizeof(char)), SEEK_CUR);
}

void Scanner::AddCharTokenString(char ch)
{
	TokenBuffer[BufferTop++] = ch;
}

void Scanner::EmptyTokenString()
{
	BufferTop = 0;
}

Token Scanner::JudgeKeyToken(const char *IDString)
{
	Token ret = {ERRTOKRN, "", 0.0, NULL};

	return ret;
}