#ifndef __NEW_IMPL_H__
#define __NEW_IMPL_H__

#ifndef __NEW_H__
    #error Only include this file from new.h
#endif

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
} 

// begin add by J.S.S.
void* operator new[]( size_t size )
{
    return operator new( size );
}

void operator delete[]( void* ptr )
{
    operator delete( ptr );
}
// end add

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {}; 

void __cxa_pure_virtual(void) {};

#endif //__NEW_IMPL_H__