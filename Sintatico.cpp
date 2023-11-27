#include <stdio.h>
#include "lexico.h"

// Estrutura de Token
Token token;
char *code;

bool eOperador(char c){ // funçao para identificar caracteres depois de um numero que nao seja ' ' e \n
    char operadores[9][6] = {"or", "and", "RELOP", "..", "+", "-", "*", "/", "^"};
    for(int i=0; i<9; i++){
        if(c == operadores[i][0]){
            return true;
        }
    }
    return false;
}

void parse() {
        parseBlock();
    }

//FUNCOES TERMINAIS

// FUNCOES NAO-TERMINAIS

// Função principal para análise sintática
void parseBlock() {
	//Stmt (; Stmt)^*
    while (true) {
        parseStmt();
        if (token.nome_token == ';') {
            token = proximo_token();
            if (token.atributo == -1){
                codigo_fim();
            }
            else{
                parseBlock();
            }
        } 
        else {
            printf("Erro, se espera ';'\n");
            parseBlock();
        }
    }
}

void parseStmt() {
	// Stmt -> Vars = Exps
    if (token.nome_token == 'ID') {
		parseVars();
		if (token.nome_token == '=') {
			printf("=\n");
			token = proximo_token();
			parseExps();
			codigo_fim();
		}
	}
	// Stmt -> Function 
    else if (token.nome_token == 'function') {  
		printf("function\n"); 
		// Function -> FunctionBody Function'
		parseFunction();
	}
    else if (token.nome_token == 'do') {
		// Stmt -> do Block end
		printf("do\n");
		parseBlock();
		if (token.nome_token == 'end'){
			printf("end\n");
			token = proximo_token();
			codigo_fim();
		}
	} 
    else if (token.nome_token == 'while') {
		// Stmt -> while Exp do Block end
		printf(" while\n");
		parseExp();
		if (token.nome_token == 'do') {
			printf("do\n");
			parseBlock();
			if (token.nome_token == 'end'){
				printf("end\n");
				token = proximo_token();
				codigo_fim();
			}
			// else (fazer ter erro)
		}
		// else (fazer ter erro)
	} 
    else if (token.nome_token == 'if') {
		// Stmt -> if Exp then Block ElseIfList OptElse end
		printf("if\n");
		parseExp();
		if (token.nome_token == 'then'){
			printf("then\n");
			parseBlock();
			parseElseIfList();
			parseOptElse();
			if (token.nome_token == 'end'){
				printf("end\n");
				token = proximo_token();
				codigo_fim();
			}
			// else (fazer ter erro)
		}
		// else (fazer ter erro)
	} 
    else if (token.nome_token == 'return') {
		// Stmt -> return (Exps)^opt
		printf("return\n");
		parseExps();
		codigo_fim();
	} 
    else if (token.nome_token == 'break') {
		// Stmt -> break
		printf("break\n");
		token = proximo_token();
		codigo_fim();
	} 
    else if (token.nome_token == 'for') { 
		// Stmt -> for ForStmt
		printf("for\n");
		parseForStmt();
		codigo_fim();
	} 
    else if (token.nome_token == 'local') { 
		// Stmt -> local LocalStmt
		printf("local\n");
		parseLocalStmt();
		codigo_fim();
    }
    else if (token.atributo == -1) {
        printf("Erro, se espera algum não-terminal\n");
        codigo_fim();
        parseBlock();
    }
}

void parseForStmt() {
	//for Name = Exp, Exp OptExpList do Block end
	token = proximo_token();
	if (token.nome_token == 'ID') {
		printf("id\n");
		token = proximo_token();
		if (token.nome_token == '=') {
			printf("=\n");
			parseExp();
			token = proximo_token();
			if (token.nome_token == ','){
				printf(",\n");
				parseExp();
				parseOptExpList();
				token = proximo_token();
				if (token.nome_token == 'do') {
					printf("do\n");
					parseBlock();
					if (token.nome_token == 'end'){
						printf("end\n");
						token = proximo_token();
						codigo_fim();
					}
				}
			}
		}
		//for Names in Exps do Block end
		else if (token.nome_token == 'in'){
			printf("in\n");
			parseExps();
			token = proximo_token();
			if (token.nome_token == 'do') {
				printf("do\n");
				parseBlock();
				if (token.nome_token == 'end'){
					printf("end\n");
					token = proximo_token();
					codigo_fim();
				}
			}
		}
		// else (fazer ter erro)
	}
}

void parseLocalStmt() {
	//(local)^opt Function Name FunctionBody
	token = proximo_token();
	if (token.nome_token == 'function'){
		printf("function\n");
		parseFunction();
		token = proximo_token();
		if (token.nome_token == 'ID'){
			printf("id\n");
			parseFunctionBody();
		}
	}
	// Names = Exps
	else if (token.nome_token == 'ID') {
		parseNames();
		token = proximo_token();
		if (token.nome_token == '=') {
			printf("=\n");
			parseExps();
		}
	}
	// else (fazer ter erro)
}

void parseElseIfList(){
	//elseif Exp then Block ElseIfList | ε
	printf("elseif\n");
	parseExp();
	token = proximo_token();
	if (token.nome_token == 'then'){
		printf("then\n");
		parseBlock();
		parseElseIfList();
		token = proximo_token();
		codigo_fim();
	}
	else {
		//epsilon
	}

}

void parseOptElse(){
	// else Block | ε
	token = proximo_token();
	if (token.nome_token == 'else'){
	printf("else\n");
	parseBlock();
	token = proximo_token();
	codigo_fim();
	}
	else{
		//epsilon
	}
}


void parseOptExpList(){
	// , Exp OptExpList | ε
	token = proximo_token();
	if (token.nome_token = ','){
		printf(",\n");
		parseExp();
		parseOptExpList();
		token = proximo_token();
		codigo_fim();
	}
	else{
		//epsilon
	}

}

void parseExps(){
	// Exp Exps'
	parseExp();
	parseExpsPrime();
	token = proximo_token();
	codigo_fim();
}

void parseExpsPrime(){
	//, Exp Exps' | ε
	token = proximo_token();
	if (token.nome_token = ','){
		printf(",\n");
		parseExp();
		parseExpsPrime();
	}
	else{
		//epsilon
	}
}

// Função para análise sintática da expressão
void parseExp(){
    //PrefixExp Exp'
    parsePrefixExp();
    parseExpPrime();
    token = proximo_token();
    codigo_fim();
}

// Função para análise sintática da expressão'
void parseExpPrime() {
    //BinOp Exp Exp' | ε
    token = proximo_token();
    if (eOperador(token.nome_token)){
        parseBinOp();
        parseExp();
        parseExpPrime();
        token = proximo_token();
        codigo_fim();
    }
    else {
        //epsilon
    }
}

void parsePrefixExp(){ //Falta Completar
	//Var | Function | not Exp | { Field' } | nil | true | false | Number | String | ( Exp )
	token = proximo_token();
	if (token.nome_token == 'ID'){ //não sei como verificar Var (talvez cerebro derreteu)
		token = proximo_token();
		parseVars();
		token = proximo_token();
		codigo_fim();

	}
	else if (token.nome_token == 'ID'){ // não sei como verificar Function (talvez cerebro derreteu)
		parseFunction();
		token = proximo_token();
		codigo_fim();
	}
	else if (token.nome_token == 'not'){
		printf("not\n");
		parseExp();
		token = proximo_token();
		codigo_fim();
	}
	else if (token.nome_token == '{'){
		printf("{\n");
		parseFieldPrime();
		token = proximo_token();
		if (token.nome_token == '}'){
			printf("}\n");
			token = proximo_token();
			codigo_fim();
		}
	}
	else if (token.nome_token == 'nil'){
		printf("nil\n");
		token = proximo_token();
		codigo_fim();
	}
		else if (token.nome_token == 'true'){
		printf("true\n");
		token = proximo_token();
		codigo_fim();
	}
	else if (token.nome_token == 'false'){
		printf("false\n");
		token = proximo_token();
		codigo_fim();
	}
	else if (token.nome_token == 'NUM'){
		printf("Number\n");
		token = proximo_token();
		codigo_fim();
	}
	else if (token.nome_token == 'seila'){ // Como vou fazer para verificar se é string ???? 
		printf("String\n");            //pq tem o conteudo dela né, que é qualquer coisa.. penso dps, cerebro derreteu
		token = proximo_token();
		codigo_fim();
	}
	else if (token.nome_token == '('){
		printf("(\n");
		parseExp();
		token = proximo_token();
		if (token.nome_token == ')'){
			printf(")\n");
			token = proximo_token();
			codigo_fim();
		}
	}
	// else (algum erro)


}

void parseVar(){
    //Name | PrefixExp [ Exp ]
	token = proximo_token();
    if (token.nome_token == 'ID'){
        printf("id\n");
        token = proximo_token();
        codigo_fim();
    }
	else{
		parsePrefixExp();
		token = proximo_token();
		if (token.nome_token == '['){
			printf("[\n");
			parseExp();
			token = proximo_token();
			if (token.nome_token == ']'){
				printf("]\n");
				token = proximo_token();
				codigo_fim();
			}
		}
	}
}


void parseField(){ // Falta completar
	
}

void parseFieldPrime(){ //Falta completar

}

void parseBinOp(){
	//or | and | < | > | <= | >= | ^= | == | .. | + | - | * | / | ^
	token = proximo_token();
	if (eOperador(token.nome_token)){
		token = proximo_token();
		codigo_fim();
	}
}

// Função para análise sintática da declaração de variáveis
void parseVars(){
	parseVar();
	parseVarsPrime();	
}

// Função para análise sintática da declaração de variáveis'
void parseVarsPrime(){
	//, Var Vars’ |  ε
	token = proximo_token();
	if (token.nome_token = ','){
		printf(",\n");
		parseVar();
		parseVarsPrime();
	}
	else{
		//epsilon
	}
}

// Função para análise sintática da declaração de função
void parseFunction(){
	//FunctionBody Function'
	token = proximo_token();
	if (token.nome_token == 'ID') {
		printf("ID\n");
		token = proximo_token();
		// FunctionBody -> Name (Params)^opt Block end
		parseFunctionBody();
		parseFunctionPrime();
	}
	//else (fazer dar erro)
}

void parseFunctionPrime(){
	token = proximo_token();
	if (token.nome_token == 'ID') {
		printf("ID\n");
		token = proximo_token();
		// FunctionBody -> Name (Params)^opt Block end
		parseFunctionBody();
		parseFunctionPrime();
	}
	else{
		//epsilon
		//codigo_fim();
	}

}

// Função para análise sintática da declaração do corpo função
void parseFunctionBody(){
	//Name ( Params^opt ) Block end
	token = proximo_token();
	if (token.nome_token == '_'){
		printf("_\n");
		parseParams();
		parseBlock();
		token = proximo_token();
		if (token.nome_token == 'end'){
			printf("end\n");
			token = proximo_token();
			codigo_fim();
		}
	}
	else if (token.nome_token != '_'){
		parseBlock();
		token = proximo_token();
		if (token.nome_token == 'end'){
			printf("end\n");
			token = proximo_token();
			codigo_fim();
		}
	}
	//else (tratar erro)
}

// Função para análise sintática dos parâmetros
void parseParams(){
	//_ Names
	token = proximo_token();
	if (token.nome_token== 'ID') {
		printf("id\n");
		parseNames();
		token = proximo_token();
		codigo_fim();
	}
	// else (tratar erro)
}

// Função para análise sintática dos nomes
void parseNames(){
	// Name Names’
	parseNamesPrime();
}

// Função para análise sintática dos nomes'
void parseNamesPrime(){
	//, Name Names’ |  ε
	token = proximo_token();
	if (token.nome_token = ','){
		printf(",\n");
		token = proximo_token();
		if (token.nome_token = 'ID'){
			printf("id\n");
			parseNamesPrime();
		}
	}
	else{
		//epsilon
	}
}

// Função para tratamento de erro usando o modo pânico
void panicMode(){

}

void codigo_fim() {
	if (token.atributo == -1){ 
		printf("Fim do Arquivo\n");
		exit(1); 
	}
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
		printf("Como chamar o compilador: %s <nome do arquivo>\n", argv[0]);
		return 1;
	}

	char *filename = argv[1];
	code = readFile(filename);
	
	
	if (code == NULL) {
		printf("Arquivo não encontrado\n");
	} else {
		token = proximo_token();
		codigo_fim();	
		parse();
	}
	printf("Análise sintática realizada com sucesso\n");
			
	free(code);
	return 0;
}