
/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

#ifndef OBJDICT_H
#define OBJDICT_H

#include "data.h"
#include "objdictdef.h"


/* Prototypes of function provided by object dictionnary */
unsigned long ObjDict_valueRangeTest (unsigned char typeValue, void * value);
//const indextable * ObjDict_scanIndexOD (unsigned int wIndex, unsigned long * errorCode, ODCallback_t **callbacks);
const indextable * ObjDict_scanIndexOD (unsigned int wIndex, unsigned long * errorCode, ODCallback_t **callbacks);
/* Master node data struct */
extern CO_Data ObjDict_Data;

extern unsigned char ObjDict_obj2002[24];
extern unsigned char ObjDict_obj2003[];
extern unsigned char ObjDict_highestSubIndex_obj1016;
extern unsigned long ObjDict_obj1016[];


extern unsigned int ObjDict_ObjdictSize ;


#endif // OBJDICT_H
