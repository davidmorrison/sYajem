#ifndef __YADAT_H__
#define __YADAT_H__

extern "C" { void* yadat_address_(void); }
/**
@class YaDat

@brief       Class definition for YaDat, which is used
      to modify the Pythia YADAT common.
*/

class YaDat {
public:
    YaDat();
    ~YaDat();
    
    double&        yaprofilet    (int i);
    double&        yaprofileq    (int i);
    double&        yaprofileint   (int i);

    void	init	(void);

    // return common array lengths
    inline int	lenI() const {return _lenI;}

private: 

    // Lengths of array in YaDat common
    static const int _lenI	= 500; 

    struct YADAT;
    friend struct YADAT;

    struct YADAT
    {
      double yaprofilet[_lenI];
      double yaprofileq[_lenI];
      double yaprofileint[_lenI];
    };

    int  _dummy;
    double  _realdummy;

    static YADAT* _YaDat;
};

// set pointer to zero at start
YaDat::YADAT* YaDat::_YaDat = 0;

inline void
YaDat::init(void)
{ if (!_YaDat) _YaDat = static_cast<YADAT*>(yadat_address_()); }

// Constructor
inline
YaDat::YaDat()
       : _dummy		(-999),
       _realdummy	(-999.)
{}

// Destructor
inline
YaDat::~YaDat()
{}

inline double&
YaDat::yaprofilet (int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI()) return _realdummy;

    return _YaDat->yaprofilet[i-1];
}

inline double&
YaDat::yaprofileq (int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI()) return _realdummy;

    return _YaDat->yaprofileq[i-1];
}

inline double&
YaDat::yaprofileint (int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI()) return _realdummy;

    return _YaDat->yaprofileint[i-1];
}


#endif /* __YADAT_H__ */
