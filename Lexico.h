#include <stdio.h>
#include <string>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <cstring>


//NOME TOKENS
#define RELOP 259;
#define ID 260;
#define NUM 261;
#define STRING 268;


//ATRIBUTOS
#define LT 262;
#define LE 263;
#define EQ 264;
#define NE 265;
#define GT 266;
#define GE 267;

std::string reserved [21] = {"and", "break","do", "else", "elseif","end", "false", "for", "function", "if",
					"in","local","nil","not","or","repeat","return","then","true","until","while"};

// Declaracoes de variaveis globais
const int MAX_IDENTIFICADORES = 100;  // Tamanho maximo da tabela de simbolos

struct EntradaTabela {
    std::string identificador;
    int numero;
};

EntradaTabela tabela_simbolos[MAX_IDENTIFICADORES];
int proximo_id = 1;  // Proximo ID a ser atribui­do

int buscarNaTabelaDeSimbolos(const std::string& identificador) {
    for (int i = 1; i < proximo_id; i++) {
        if (tabela_simbolos[i].identificador == identificador) {
            return i;  // O identificador ja existe, retorna o ID existente
        }
    }
    return -1;  // O identificador nao foi encontrado na tabela
}

int adicionarNaTabelaDeSimbolos(const std::string& identificador) {
    int posicaoExistente = buscarNaTabelaDeSimbolos(identificador);
    if (posicaoExistente != -1) {
        return posicaoExistente;  // O identificador ja existe, retorna o ID existente
    }

    // Se o identificador nao foi encontrado na tabela, adicione-o
    if (proximo_id < MAX_IDENTIFICADORES) {
        tabela_simbolos[proximo_id].identificador = identificador;
        return proximo_id++;  // Retorna o novo ID e incrementa proximo_id
    } else {
        printf("Tabela de simbolos esta cheia.");
        return -1;  // Erro, a tabela de si­mbolos esta cheia
    }
}


bool eOperador(char c){ // funçao para identificar caracteres depois de um numero que nao seja ' ' e \n
    char operadores[13][2] = {"(", ")", "{", "}", "[", "]", ";", ":", ",", "=", "~","<", ">"};
    for(int i=0; i<13; i++){
        if(c == operadores[i][0]){
            return true;
        }
    }
    return false;
}



struct Token{
	int nome_token;
	int atributo;
};



int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;


char *readFile(char *fileName)
{
	FILE *file = fopen(fileName, "r");
	char *code;
	int n = 0;
	int c;

	if(file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	code = new char [f_size]; // tive que mudar de (f_size) para [f_size] porque estava dando problema

	while ((c = fgetc(file))!= EOF)
	{
		code[n++] = (char) c;
	}
	code[n] = '\000';
	return code;
}




int falhar()
{
	switch(estado)
	{
	case 0: partida = 13; break;

	case 13: partida = 16; break;

	case 16: partida = 24; break;

	case 24: partida = 30; break;

	case 30: partida = 40; break;

	case 40: partida = 43; break;

	case 43: partida = 46; break;

	case 46: partida = 47; break;

	case 47: partida = 52; break;

	case 52:
		//retornar msg de erro
		printf("Erro encontrado no codigo\n");
		cont_sim_lido++;
		break;

	default: printf("Erro do compilador\n");
	}
	return(partida);
}

Token proximo_token()
{
	Token token;
	char c;
	std::string identificador; //armazena cada caractere que passa no automato do identificador
	std::string number; // armazena cada caractere que passa no automato dos numeros 
	bool isReserved = false; //bool para as palvras reservadas
	int nomeToken = 0;// usado para identificar qual e a palavra reservada
	int pos_str = 0; //usado para gravar a posicao da string no buffer
	char string_buffer[256]; // Um buffer para armazenar a string
	int aninhado = 0; // usado para gravar a quantidade de colchetes aninhados
	while(code[cont_sim_lido] != EOF) 
	{
		switch(estado)
		{

//OPERADORES RELACIONAIS E ATRIBUICAO

			case 0:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 0;
					cont_sim_lido++;
				}
				else if(c == '<'){
					estado = 1;
					cont_sim_lido++;
				} 
				else if(c == '>'){
					estado = 5;
					cont_sim_lido++;
				}
				else if(c == '='){
					estado = 8;
					cont_sim_lido++;				
				}
				else if(c == '~'){
					estado = 11;
					cont_sim_lido++;
				} 
				else if (c == '\000'){
						token.nome_token = EOF;
						token.atributo = -1;
						return(token);
				}
				else{
					estado = falhar();
				}
				break;

			case 1:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){	
					estado = 4;
					cont_sim_lido++;
				}
				else if(c == '='){
					estado = 2;
					cont_sim_lido++;
				}
				else if(c == '>'){
					estado = 3;
					cont_sim_lido++;
				}
				else{
					printf("Erro lexico: < deve ser sucedido de algum operador relacional ou ' '\n");
					token.nome_token = EOF;
					token.atributo = -1;
					return (token); 
				}
				break;

			case 2:
				printf("<relop, LE>\n");
				token.nome_token = RELOP;
				token.atributo = LE;
				estado = 0;
				return(token);
				break;

			case 3:
				printf("<relop, NE>\n");
				token.nome_token = RELOP;
				token.atributo = NE;
				estado = 0;
				return(token);
				break;

			case 4:
				printf("<relop, LT>\n");
				token.nome_token = RELOP;
				token.atributo = LT;
				estado = 0;
				return(token);
				break;

			case 5:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){	
					estado = 7;
					cont_sim_lido++;
					}
				else if(c == '='){
					estado = 6;
					cont_sim_lido++;
				}
				else{
					printf("Erro lexico: > deve ser sucedido de algum operador relacional ou ' '\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000'; 
				}
				break;

			case 6:
				printf("<relop, GE>\n");
				token.nome_token = RELOP;
				token.atributo = GE;
				estado = 0;
				return(token);
				break;

			case 7:
				printf("<relop, GT>\n");
				token.nome_token = RELOP;
				token.atributo = GT;
				estado = 0;
				return(token);
				break;

			case 8:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n')){
					estado = 10;
					cont_sim_lido++;
				}
				else if(c == '='){
					estado = 9;
					cont_sim_lido++;
				}
				else if (c == '\000'){
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\0'; 
				}
				else{
					printf("Erro lexico: = deve ser sucedido de '=' ou ' '\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000'; 
				}
				break;

			case 9:
				printf("<relop, EQ>\n");
				token.nome_token = RELOP;
				token.atributo = EQ;
				estado = 0;
				return(token);
				break;
			
			case 10:
				printf("<=, >\n");
				token.nome_token = '=';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

			case 11:
				c=code[cont_sim_lido];
				if (c == '\000'){
					printf("Erro lexico: ~ deve ser sucedido do operador relacional '='\n");
					token.nome_token = EOF;
					token.atributo = -1;
					return(token);
				}
				else if ((c == ' ')||(c == '\n'))
				{
					printf("Erro lexico: ~ deve ser sucedido do operador relacional '='\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000';
					return(token);
				}	
				else if ((c == '=')){
					estado = 12;
					cont_sim_lido++;
				}
				break;
			
			case 12:
				printf("<relop, NE>\n");
				token.nome_token = RELOP;
				token.atributo = NE;
				estado = 0;
				return(token);
				break;



//IDENTIFICADOR
			case 13:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 0;
					cont_sim_lido++;
				}
				else if (std::isalpha(c)){
					estado = 14;
					cont_sim_lido++;
					identificador += c;
				}
				else if ((c == '#')|| (c == '$') || (c == '%')|| (c == '&')){
					printf("Erro lexico: um caracter nao reconhecido pela linguagem foi inserido.\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000';
				}
				else if (c == '\000'){
					token.nome_token = EOF;
					token.atributo = -1;
					return (token);
				}
				else{
					estado = falhar();
				}
				break;

			case 14:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 15;
					cont_sim_lido++;
				}
				else if ((std::isalnum(c)) || (c == '_') || (c == '.')){
					estado = 14;
					cont_sim_lido++;
					identificador += c;
				}
				else{
					estado = 15;
				}
				break;

			case 15:
				for (int i = 0; i < 21; i++) {
					if (identificador == reserved[i]) {
						isReserved = true;
						nomeToken = i;
						break;
					}
				}

				if (isReserved == true) {
					// e uma palavra reservada, emite o token correspondente
					printf("<%s,  >\n", identificador.c_str());
					token.nome_token = nomeToken;  // Armazena o nome da palavra reservada como token
					token.atributo = ' ';  // Atributo vazio para palavras reservadas
				}
				else{
					// Nao e uma palavra reservada, verifica se ja existe na tabela de si­mbolos
					int posicaoExistente = buscarNaTabelaDeSimbolos(identificador);
					if (posicaoExistente != -1) {
						// O identificador ja existe, use o ID existente
						printf("<ID, %d>\n", posicaoExistente);
						token.nome_token = ID;
						token.atributo = posicaoExistente;
					} else {
						// O identificador nao existe na tabela de si­mbolos, adicione-o
						printf("<ID, %d>\n", proximo_id);
						tabela_simbolos[proximo_id].identificador = identificador;
						token.nome_token = ID;
						token.atributo = proximo_id;
						proximo_id++;
					}
				}
				estado = 0;
				return token;
				break;

//NUMEROS

			case 16:
				c = code[cont_sim_lido];
				if (std::isdigit(c)){
					estado = 17;
					cont_sim_lido++;
					number += c;
				}
				else{
					estado = falhar();
				}
				break;

			case 17:
				c = code[cont_sim_lido];
				if ((c == ' ') || (c == '\n')|| (c == ';')){
					estado = 23;
					cont_sim_lido++;
				}
				else if (std::isdigit(c)){
					cont_sim_lido++;
					c = code[cont_sim_lido];
					if (eOperador(c)){
						cont_sim_lido--;
						c = code[cont_sim_lido];
						estado = 23;
						number += c;
						cont_sim_lido++;
					}
					else {
						cont_sim_lido--;
						c = code[cont_sim_lido];
						estado = 17;
						cont_sim_lido++;
						number += c;
					}
				}
				else if (c == '.'){
					estado = 18;
					cont_sim_lido++;
					number += c;
				}
				else if(c == '^'){
					estado = 20;
					cont_sim_lido++;
					number += c;
				}
				else if (std::isalpha(c)){
					printf("Erro lexico: Ma formacao do numero\n");
					cont_sim_lido++;
					token.nome_token = EOF;
					token.atributo = -1;
					return (token);
				}
				else if (c == '\000'){
					estado = 23;
					code[cont_sim_lido] = '\000';
				}
				else if (eOperador(c)){
					if (c == ']'){
					printf("<numero, %s>\n", number.c_str());
					token.nome_token = NUM;
					token.atributo = std::stoi(number);
					number.clear(); 
					estado = 30;
					}
					else if (c == '(') {
						estado = 30;
					}
					else if (c == ')') {
						printf("<numero, %s>\n", number.c_str());
						token.nome_token = NUM;
						token.atributo = std::stoi(number);
						number.clear(); 
						estado = 30;
					}
					else {
						estado = 17;
						cont_sim_lido++;
						number += c;
					}
				}
				else{
					// estado = falhar();
					printf("Erro lexico: . deve ser sucedido de digito\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000';
				}
				break;

			case 18:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					printf("Erro lexico: . deve ser sucedido de digito\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000';
				}
				else if (std::isdigit(c)){
					estado = 19;
					cont_sim_lido++;
					number += c;
				}
				else{
					token.nome_token = EOF;
					token.atributo = -1;
					return(token);
					code[cont_sim_lido] = '\000'; 
				}
				break;

			case 19: //final - retornar token
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 23;
					cont_sim_lido++;
				}
				else if (eOperador(c)){
					estado = 23;
				}
				else{
					if (std::isdigit(c)){
						estado = 19;
						cont_sim_lido++;
						number += c;
					}	
					else if(c == '^'){
						estado = 20;
						cont_sim_lido++;
						number += c;
					}
					else{
						estado = 23;
					}	
				}
				break;

			case 20:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					printf("Erro lexico: ^ deve ser sucedido de digito ou + ou -\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000';
				}
				else if (std::isdigit(c)){
					estado = 22;
					cont_sim_lido++;
					number += c;
				}
				else if(c == '+' || c == '-'){
					estado = 21;
					cont_sim_lido++;
					number += c;
				}
				else{
					token.nome_token = EOF;
					token.atributo = -1;
					return(token);
					code[cont_sim_lido] = '\000'; 
				}
				break;

			case 21:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					printf("Erro lexico: + ou - deve ser sucedido de digito\n");
					token.nome_token = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\000';
				}
				else if (std::isdigit(c)){
						estado = 22;
						cont_sim_lido++;
						number += c;
				}
				else{
					token.nome_token = EOF;
					token.atributo = -1;
					return(token);
					code[cont_sim_lido] = '\000';
				}
				break;

			case 22:
				c = code[cont_sim_lido];
				if (std::isdigit(c)){
					estado = 22;
					cont_sim_lido++;
					number += c;
				}
				else if((c == ' ')||(c == '\n')){
					estado = 23;
					cont_sim_lido++;
				}
				else if (eOperador(c)){
					estado = 23;
				}
				else{
					estado = 23;
				}
				break;

			case 23:  //final - retornar token
				printf("<numero, %s>\n", number.c_str());
				token.nome_token = NUM;
				token.atributo = std::stoi(number);
				number.clear(); 
				estado = 0;
				return(token);
				break;

// OPERADORES ARITMETICOS

			case 24:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 0;
					cont_sim_lido++;
				}
				else if (c == '+')
				{
					estado = 25;
					cont_sim_lido++;
				}
				else if (c == '-')
				{
					estado = 26;
					cont_sim_lido++; 
					c = code[cont_sim_lido];
					if (c == '-'){
						estado = 47;
					}
					cont_sim_lido--;
					c = code[cont_sim_lido];
					cont_sim_lido++;
				}
				else if (c == '*')
				{
					estado = 27;
					cont_sim_lido++;
				}
				else if (c == '/')
				{
					estado = 28;
					cont_sim_lido++;
				}
				else if (c == '^')
				{
					estado = 29;
					cont_sim_lido++;
				}
				else {
					estado = falhar();
				}
				break;

			case 25:
				printf("<+, >\n");
				token.nome_token = '+';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

			case 26:
				printf("<-, >\n");
				token.nome_token = '-';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

			case 27:
				printf("<*, >\n");
				token.nome_token = '*';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

			case 28:
				printf("</, >\n");
				token.nome_token = '/';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

			case 29:
				printf("<^, >\n");
				token.nome_token = '^';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

//DELIMITADORES

			case 30:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n') || (c == '\000'))
				{
					estado = 0;
					cont_sim_lido++;
				}
				else if(c == '('){
					estado = 31;
					cont_sim_lido++;
				}
				else if(c == ')'){
					estado = 32;
					cont_sim_lido++;
				}
				else if(c == '{'){
					estado = 33;
					cont_sim_lido++;
				}
				else if(c == '}'){
					estado = 34;
					cont_sim_lido++;
				}
				else if(c == '['){
					estado = 35;
					cont_sim_lido++;
				}
				else if(c == ']'){
					estado = 36;
					cont_sim_lido++;
				}
				else if(c == ';'){
					estado = 37;
					cont_sim_lido++;
				}
				else if(c == ':'){
					estado = 38;
					cont_sim_lido++;
				}
				else if(c == ','){
					estado = 39;
					cont_sim_lido++;
				}
				else if(c == '.'){
					estado = 40;
					cont_sim_lido++;
				}
				else{
					estado = falhar();
				}
				break;
			
			case 31:
				printf("<(, >\n");
				token.nome_token = '(';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 32:
				printf("<), >\n");
				token.nome_token = ')';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 33:
				printf("<{, >\n");
				token.nome_token = '{';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 34:
				printf("<}, >\n");
				token.nome_token = '}';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 35:
				printf("<[, >\n");
				token.nome_token = '[';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 36:
				printf("<], >\n");
				token.nome_token = ']';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 37:
				printf("<;, >\n");
				token.nome_token = ';';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 38:
				printf("<:, >\n");
				token.nome_token = ':';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 39:
				printf("<,, >\n");
				token.nome_token = ',';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 40:
				c = code[cont_sim_lido];
				if((c == ' ')||(c == '\n'))
				{
					estado = 41;
					cont_sim_lido++;
				}
				else if(c == '.'){
					estado = 42;
					cont_sim_lido++;
				}
				else{
					estado = falhar();
				}
				break;

			case 41:
				printf("<., >\n");
				token.nome_token = '.';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;
			
			case 42:
				printf("<.., >\n");
				token.nome_token = '..';
				token.atributo = ' ';
				estado = 0;
				return(token);
				break;

//LITERAIS DE STRING

			case 43:
				c = code[cont_sim_lido];
				if (c == '"') {	
					estado = 44;
					cont_sim_lido++;
					string_buffer[pos_str] = c;
					pos_str++;
				}
				else{
					estado = falhar();
				}
				break;
			
			case 44:
				c = code[cont_sim_lido];

				while((c != '"') && (c != '\0')){
					string_buffer[pos_str] = c;
					cont_sim_lido++;
					pos_str++;
					c = code[cont_sim_lido];
				}
				if (c == '"') {
						estado = 45;
						cont_sim_lido++; // Avanca para alem da aspas de fechamento
						string_buffer[pos_str] = c;
						pos_str++;
				}
				else {
					printf("Erro lexico: String nao foi finalizada\n");
					token.nome_token = EOF;
					token.atributo = -1;
					return(token);
				}
				string_buffer[pos_str] = '\0'; // Termina a string no buffer
				break;

			case 45:
				printf("<%s, >\n", string_buffer);
				token.nome_token = STRING;
				token.atributo = ' ';
				estado = 0;
				return(token);

		//COMENTARIO LONGO E CURTO  // --coment --[[coment --[[coment]] --[[coment --[[coment[[coment]]oi]]]]]]
			case 46:
				c = code[cont_sim_lido];
				if (c == '-') {	
					estado = 47;
					cont_sim_lido++;
				}
				else{
					estado = falhar();
				}
				break;
			
			case 47:
				c = code[cont_sim_lido];
				if (c == '-') {	
					estado = 48;
					cont_sim_lido++;
				}
				else{
					estado = falhar();
				}
				break;
			
			case 48:
				c = code[cont_sim_lido];
				if (c == '[') {	
					estado = 49;
					cont_sim_lido++;
				}
				else{ //COMENTARIO CURTO
					while((c != '\n') && (c != '\0')){
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}
					if(c == '\n'){
						estado = 0;
					}
					else {
					token.nome_token = -1;
					token.atributo = -1;
					return(token);
					}
				}	
				break;
			
			case 49:
				c = code[cont_sim_lido];
				if (c == '[') {
					aninhado++;
					cont_sim_lido++;
					estado = 50;
				}
				else{
					estado = 48;
				}
				break;
				
			case 50:
				c = code[cont_sim_lido];
				if (aninhado == 0 ){
					estado = 0;
				}
				else {
					while((c != ']') && (c != '\0') && (c != '\n')){
						cont_sim_lido++;
						c = code[cont_sim_lido];
						if(c == '['){
							cont_sim_lido++;
							c = code[cont_sim_lido];
							if(c == '['){
								aninhado++; 
							}
						}
					}
					if(c == ']'){
						estado = 51;
						cont_sim_lido++;
					}
					else if (c == '\000'){
						estado = 51;
					}
					else if (c == '\n'){
						estado = 50;
						cont_sim_lido++;
					}
					else{
						token.nome_token = EOF;
						token.atributo = -1;
						return(token);
					}
				}
				break;
				
			case 51:
				c = code[cont_sim_lido];
				if ((c != ']') && (c != '\000')) {
					estado = 50;
				}
				else if(c == ']'){
					aninhado--;
					if(aninhado >= 0){
						estado = 50;
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}
					else if (aninhado == -1){
						printf("Erro lexico: Comentario longo nao foi finalizado\n");
						token.nome_token = -1;
						token.atributo = -1;
						return(token);
					}
				}
				else if (c == '\000'){
					printf("Erro lexico: Comentario longo nao foi finalizado\n");
					token.nome_token = -1;
					token.atributo = -1;
					return(token);
				}
				else{
					token.nome_token = -1;
					token.atributo = -1;
					return(token);
				}
				break;
		}
	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);
}

int main ()
{
	Token token;
    code = readFile("programa.txt");
	while (code[cont_sim_lido] != '\000')
		token = proximo_token();

	for (int i = 1; i < proximo_id; i++) {
    std::cout << "ID: " << i << ", Identificador: " << tabela_simbolos[i].identificador << std::endl;
	}

	free(code);
	return 0;

}