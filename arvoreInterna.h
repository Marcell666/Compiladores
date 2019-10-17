
#define MAX_ID 81
#define MAX_STRING 500

void ARV_identa(int n);
char* tipoPrimitivoToString(TipoPrimitivo tp);

typedef enum noTag{
	TDefinicao,
	TBloco,
	TComando,
	TExpressao,
	TChamada
}NoTag;

struct arvore {
	NoTag tag;
	// A tag se refere para qual tipo de no esta apontando o atual
	Definicao* topo;
	// ponteiro generico porque sera um tipo diferente para cada no;
	void* atual;
};

typedef enum expression_tag {
	EBinaria,

	EString,
	EBoolean,
	EInteiro,
	EDouble,
	EChar,

	ENumeral,

	EVariavel,	
	EParenteses,
	EMenosUnario,
//TODO esta faltando o operador NOT aqui

	EChamada,
	EAs,
	ENewArray
} ETag;	

struct exp{
	ETag tag;	
	int lineNumber;
	union{
		struct {
			EBinaryTag tag;
			Exp* e1;
			Exp* e2;
		}bin; 

		
		Exp *e;
			//o que faco quando tem parenteses?
			//Basta colocar outra expressao dentro desta?
			//Como isso me ajuda?
			//Ta, e quanto ao menos unario?
			//Mesma coisa?
			
		/*struct{
			char id[MAX_ID];
			ChamadaArg* ac;
		}chamadaFuncao;*/
		Chamada* chamadaFuncao;

		struct{
			Exp* exp;
			TipoPrimitivo deTipo;// Acho que essa variavel nao faz muito sentido...
			TipoPrimitivo paraTipo; 
		}as;

		struct{
			//char id[MAX_ID];
			Exp* exp;
			TipoPrimitivo tipo; 
		}newArray;

		Numeral* numeral;

		Variavel* var;

		int boolean;
		int ival;
		double dval;
		char sval[MAX_STRING];
		char cval;
	}u;
}; 


struct numeral{
	TipoPrimitivo tag;
	int lineNumber;
	union {
		int ival;
		double dval;
	}u;
};
struct chamadaArg{
	int lineNumber;
	Exp *exp;
	ChamadaArg *prox;
	ChamadaArg *ult;
};

struct chamada{
	int lineNumber;
	char id[MAX_ID];
	ChamadaArg *args;
};

typedef enum ctag {
	CIf,
	CWhile,
	CAtribuicao,
	CReturn,
	CChamada,
	CArroba,
	CBloco
} CTag;

typedef enum dTag{
	EDefinicaoParametro,
	EDefinicaoVariavel,
	EDefinicaoFuncao
} DTag;


struct definicao{
	DTag tag;
	int lineNumber;
	Definicao *prox;
	Definicao *ult;
	union{
		struct definicaoVariavel{
			char id[MAX_ID];
			TipoPrimitivo tipo;
		}DefinicaoVariavel;

		struct definicaoFuncao{
			char id[MAX_ID];
			Definicao* parametro;
				//Sera que e justo fazer isso?
				//ou eu deveria fazer um tipo parametro?
			TipoPrimitivo tipoRetorno;
			Bloco* bloco;
		} DefinicaoFuncao;
	}u;
};
struct bloco{
	int lineNumber;
	Definicao* definicaoVariavel;
	//Bloco ~NAO~ tem definicoes de funcao dentro de si!!!
	//Estudar a possibilidade de separar definicao de variavel de definicao de funcao na union acima
	Comando* comando;	
};
struct comando{
	CTag tag;
	int lineNumber;
	Comando* prox;

	union {
		struct no_if{
			Exp *exp;
			Bloco *bloco_if;
			Bloco *bloco_else;
				//se o bloco else for null, entao nao tem else
		}no_If;

		struct no_while{
			Exp *exp;
			Bloco *bloco;
		}no_While;

		struct atribuicao{
			Variavel* var;
			Exp* exp;		
		} Atribuicao;

		struct no_return{
			Exp* exp;
		} no_Return;

		Exp* arrobaExp;

		Chamada* chamada;

		Bloco* bloco;
	}u;
};

typedef enum vTag{
	PRIMITIVO,
	ARRAY
}VTag;

struct var{
	VTag tag;
	int lineNumber;
	union{
		char id[MAX_ID];
		struct {
			//TODO consertar isso!!
			Exp* exp1;
			//char id[MAX_ID];
			Exp* exp2;
		}array;
	}u;
};
