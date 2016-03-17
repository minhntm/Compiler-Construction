/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type* dupType = (Type*) malloc(sizeof(Type));
  dupType->typeClass = type->typeClass;
  dupType->arraySize = type->arraySize;
  dupType->elementType = type->elementType;
  return dupType;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if (type1->typeClass != type2->typeClass){
    return 0;
  } else {
    if (type1->arraySize != type2->arraySize){
        return 0;
    } else {
        if (type1->arraySize != 0 &&
            type1->elementType->typeClass != type2->elementType->typeClass){
                return 0;
        }
    }
  }
  return 1;
}

void freeType(Type* type) {
  // TODO
  if (type->arraySize != 0){
    free(type->elementType);
  }
  free (type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue* constant = (ConstantValue*)malloc(sizeof(ConstantValue));
  constant->type = TP_INT;
  constant->intValue = i;
  return constant;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue* constant = (ConstantValue*)malloc(sizeof(ConstantValue));
  constant->type = TP_CHAR;
  constant->charValue = ch;
  return constant;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue* dupConstant = (ConstantValue*)malloc(sizeof(ConstantValue));
  dupConstant->type = v->type;
  if (v->type == TP_INT){
    dupConstant->intValue = v->intValue;
  } else {
    //v->type == TP_CHAR
    dupConstant->charValue = v->charValue;
  }
  return dupConstant;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  // TODO
  Object* constantObject = (Object*) malloc(sizeof(Object));
  strcpy(constantObject->name, name);
  constantObject->kind = OBJ_CONSTANT;
  constantObject->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  constantObject->constAttrs->value = (ConstantValue*)malloc(sizeof(ConstantValue));
  return constantObject;
}

Object* createTypeObject(char *name) {
  // TODO
  Object* typeObject = (Object*) malloc(sizeof(Object));
  strcpy(typeObject->name, name);
  typeObject->kind = OBJ_TYPE;
  typeObject->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  typeObject->typeAttrs->actualType = (Type*)malloc(sizeof(Type));
  return typeObject;
}

Object* createVariableObject(char *name) {
  // TODO
  Object* variableObject = (Object*) malloc(sizeof(Object));
  strcpy(variableObject->name, name);
  variableObject->kind = OBJ_VARIABLE;
  variableObject->varAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  variableObject->varAttrs->type = (Type*)malloc(sizeof(Type));
  variableObject->varAttrs->scope = createScope(variableObject, symtab->currentScope);
  return variableObject;
}

Object* createFunctionObject(char *name) {
  // TODO
  Object* funcObject = (Object*)malloc(sizeof(Object));
  strcpy(funcObject->name, name);
  funcObject->kind = OBJ_FUNCTION;
  funcObject->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  funcObject->funcAttrs->paramList = (ObjectNode*)malloc(sizeof(ObjectNode));
  funcObject->funcAttrs->returnType = (Type*)malloc(sizeof(Type));
  funcObject->funcAttrs->scope = createScope(funcObject, symtab->currentScope);
  return funcObject;
}

Object* createProcedureObject(char *name) {
  // TODO
  Object* procObject = (Object*)malloc(sizeof(Object));
  strcpy(procObject->name, name);
  procObject->kind = OBJ_PROCEDURE;
  procObject->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  procObject->procAttrs->paramList = (ObjectNode*)malloc(sizeof(ObjectNode));
  procObject->procAttrs->scope = createScope(procObject, symtab->currentScope);
  return procObject;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* paramObject = (Object*)malloc(sizeof(Object));
  strcpy(paramObject->name, name);
  paramObject->kind = OBJ_PARAMETER;
  paramObject->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  paramObject->paramAttrs->kind = kind;
  paramObject->paramAttrs->function = owner;
  return paramObject;
}

void freeObject(Object* obj) {
  // TODO
  if (obj != NULL){
      switch (obj->kind){
        case OBJ_CONSTANT:
            free(obj->constAttrs->value);
            free(obj->constAttrs);
            break;
        case OBJ_VARIABLE:
            free(obj->varAttrs->type);
            freeScope(obj->varAttrs->scope);
            free(obj->varAttrs);
            break;
        case OBJ_TYPE:
            free(obj->typeAttrs->actualType);
            free(obj->typeAttrs);
            break;
        case OBJ_FUNCTION:
            free(obj->funcAttrs->paramList);
            free(obj->funcAttrs->returnType);
            freeScope(obj->funcAttrs->scope);
            free(obj->funcAttrs);
            break;
        case OBJ_PROCEDURE:
            free(obj->procAttrs->paramList);
            freeScope(obj->procAttrs->scope);
            free(obj->procAttrs);
            break;
        case OBJ_PROGRAM:
            freeScope(obj->progAttrs->scope);
            free(obj->progAttrs);
            break;
        case OBJ_PARAMETER:
            free(obj->paramAttrs->type);
            free(obj->paramAttrs->function);
            free(obj->paramAttrs);
            break;
      }

      free(obj);
  }
}

void freeScope(Scope* scope) {
  // TODO
  if (scope != NULL){
    freeObjectList(scope->objList);
    freeScope(scope->outer);
  }
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if (objList != NULL){
      ObjectNode* temp;
      while (objList->next != NULL){
        temp = objList->next;
        objList->next = temp->next;
        free(temp);
      }
      free(objList);
  }
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  if (objList != NULL){
      ObjectNode* temp;
      while (objList->next != NULL){
        temp = objList->next;
        objList->next = temp->next;
        free(temp);
      }
      free(objList);
  }
}

//insert obj to last of objList
void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode* loop;
  for (loop = objList; loop != NULL; loop = loop->next){
    if (strcmp(loop->object->name, name) == 0){
        return loop;
    }
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  addObject(&(symtab->currentScope->objList), obj);
}


