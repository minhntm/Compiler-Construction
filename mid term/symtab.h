/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "token.h"

enum TypeClass {			// --- Listed types KPL ---
	TP_INT,
	TP_CHAR,
	TP_STRING,
	TP_ARRAY,
	TP_DOUBLE
};

enum ObjectKind {			// --- Listed symbol ---
	OBJ_CONSTANT,
	OBJ_VARIABLE,
	OBJ_TYPE,
	OBJ_FUNCTION,
	OBJ_PROCEDURE,
	OBJ_PARAMETER,
	OBJ_PROGRAM
};

enum ParamKind {
	PARAM_VALUE,
	PARAM_REFERENCE
};

struct Type_ {						// --- Declare Type struct ---
	enum TypeClass typeClass;		// --- Type of type ---
	int arraySize;					// --- != 0 when type is the type of the array ---
	struct Type_ *elementType;		// --- elementType of array ---
};

typedef struct Type_ Type;
typedef struct Type_ BasicType;


struct ConstantValue_ {
	enum TypeClass type;
	union {
		int intValue;
		char charValue;
		double doubleValue;
		char* stringValue;
	};
};

typedef struct ConstantValue_ ConstantValue;

struct Scope_;
struct ObjectNode_;
struct Object_;

// ***************************** Attributes of object ******************************

struct ConstantAttributes_ {
	ConstantValue* value;
};

struct VariableAttributes_ {
	Type *type;
	struct Scope_ *scope;
};

struct TypeAttributes_ {
	Type *actualType;
};

struct ProcedureAttributes_ {
	struct ObjectNode_ *paramList;
	struct Scope_* scope;
};

struct FunctionAttributes_ {
	struct ObjectNode_ *paramList;
	Type* returnType;
	struct Scope_ *scope;
};

struct ProgramAttributes_ {
	struct Scope_ *scope;
};

struct ParameterAttributes_ {
	enum ParamKind kind;
	Type* type;
	struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;

// *************************************************************************************

struct Object_ {						// --- Arrtributes of object ---
	char name[MAX_IDENT_LEN];			// --- Name of object ---
	enum ObjectKind kind;				//
	union {
		ConstantAttributes* constAttrs;
		VariableAttributes* varAttrs;
		TypeAttributes* typeAttrs;
		FunctionAttributes* funcAttrs;
		ProcedureAttributes* procAttrs;
		ProgramAttributes* progAttrs;
		ParameterAttributes* paramAttrs;
	};
};

typedef struct Object_ Object;

struct ObjectNode_ {
	Object *object;						// --- Data of objectNode ---
	struct ObjectNode_ *next;			// --- Next pointer of node ---
};

typedef struct ObjectNode_ ObjectNode;

struct Scope_ {						// --- Block 's scope ---
	ObjectNode *objList;			// --- Object list in block ---
	Object *owner;					// --- Func, procedure, program corresponding to block ---
	struct Scope_ *outer;			// --- Outer scope of block ---
};

typedef struct Scope_ Scope;

struct SymTab_ {					// --- Used to store information about the identifier of the program and its attributes ---
	Object* program;				// --- Main program ---
	Scope* currentScope;			// --- Current scope store block is being traversed , each time compile func or procedure we must update currentScope ---
	ObjectNode *globalObjectList;	// --- WRITEI, WRITEF, WRITEC, WRITELN, READC, READI, READF, READS ---
};

typedef struct SymTab_ SymTab;

Type* makeIntType(void);
Type* makeDoubleType(void);
Type* makeCharType(void);
Type* makeStringType(void);
Type* makeArrayType(int arraySize, Type* elementType);
Type* duplicateType(Type* type);
int compareType(Type* type1, Type* type2);
void freeType(Type* type);

ConstantValue* makeIntConstant(int i);
ConstantValue* makeDoubleConstant(double f);
ConstantValue* makeCharConstant(char ch);
ConstantValue* makeStringConstant(char* str);
ConstantValue* duplicateConstantValue(ConstantValue* v);

Scope* createScope(Object* owner, Scope* outer);

Object* createProgramObject(char *programName);
Object* createConstantObject(char *name);
Object* createTypeObject(char *name);
Object* createVariableObject(char *name);
Object* createFunctionObject(char *name);
Object* createProcedureObject(char *name);
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner);

Object* findObject(ObjectNode *objList, char *name);

void initSymTab(void);
void cleanSymTab(void);
void enterBlock(Scope* scope);
void exitBlock(void);
void declareObject(Object* obj);

#endif
