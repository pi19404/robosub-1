#ifndef __MACRO_TRICKS_H__
#define __MACRO_TRICKS_H__

#define DO_JOIN(x,y)        x##y
#define JOIN(x,y)           DO_JOIN(x,y)
#define DO_LOCAL_INCLUDE(x) #x
#define LOCAL_INCLUDE(x)    DO_LOCAL_INCLUDE(x)

#define DO_LIST_GO_TO(a)            a
#define DO_LIST_ELEM(a,b)           , a b
#define DO_LIST_FIRST_ELEM(a,b)     a b
#define DO_LIST_N_ELEMS(a,b)        (sizeof(a)/sizeof(b))
#define DO_LIST(a,b,c)              a b[] = { CSV( c##1 ) }

// Ignore Commas Macro:
#define DO_CSV(...)                 __VA_ARGS__
#define CSV(...)                    DO_CSV(__VA_ARGS__)

// List Macros:
#define LIST_END
#define LIST_GO_TO(next)            DO_LIST_GO_TO(CSV(next))
#define LIST_ELEM(name,next)        DO_LIST_ELEM(CSV(name),CSV(next))
#define LIST_FIRST_ELEM(name,next)  DO_LIST_FIRST_ELEM(CSV(name),CSV(next))
#define LIST_N_ELEMS(name,type)     DO_LIST_N_ELEMS(CSV(name),type)
#define LIST(type,name,elem)        DO_LIST(CSV(type),name,elem) 

#endif //__MACRO_TRICKS_H__  