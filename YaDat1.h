#ifndef __YADAT1_H__
#define __YADAT1_H__

extern "C" { void* yadat1_address_(void); }
/**
@class YaDat1

@brief       Class definition for YaDat1, which is used
      to modify the Pythia YADAT1 common.
*/

class YaDat1 {
public:
    YaDat1();
    ~YaDat1();
    
    double&        yapars  (int i);
    double&        yaflags   (int i);

    void	init	(void);

    // return common array lengths
    inline int	lenI() const {return _lenI;}

private: 

    // Lengths of array in YaDat1 common
    static const int _lenI	= 20; 

    struct YADAT1;
    friend struct YADAT1;

    struct YADAT1
    {
      double yapars[_lenI];
      double yaflags[_lenI];
    };

    int  _dummy;
    double  _realdummy;

    static YADAT1* _YaDat1;
};

// set pointer to zero at start
YaDat1::YADAT1* YaDat1::_YaDat1 = 0;

inline void
YaDat1::init(void)
{ if (!_YaDat1) _YaDat1 = static_cast<YADAT1*>(yadat1_address_()); }

// Constructor
inline
YaDat1::YaDat1()
       : _dummy		(-999),
       _realdummy	(-999.)
{}

// Destructor
inline
YaDat1::~YaDat1()
{}

inline double&
YaDat1::yapars (int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI()) return _realdummy;

    return _YaDat1->yapars[i-1];
}

inline double&
YaDat1::yaflags (int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI()) return _realdummy;

    return _YaDat1->yaflags[i-1];
}

#endif /* __YADAT1_H__ */
