#ifndef __NO_COPY_H__
#define __NO_COPY_H__

// A non-copyable class
class NoCopy
{
    public:
        virtual ~NoCopy(){ }
        NoCopy(){ }
        
    private:
        NoCopy( const NoCopy& ) { }
};

#endif //__NO_COPY_H__
