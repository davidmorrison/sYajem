#ifndef __PYDAT1_H__
#define __PYDAT1_H__

extern "C" { void* pydat1_address_(void); }
/**
@class PyDat1

@brief       Class definition for PyDat1, which is used
      to modify the Pythia PYDAT1 common.
*/

class PyDat1 {
public:
    PyDat1();
    ~PyDat1();
    
    int&        mstu    (int i);
    double&     paru    (int i);
    int&        mstj   (int i);
    double&     parj    (int i);

    void	init	(void);

    // return common array lengths
    inline int	lenI1() const {return _lenI1;}

private: 

    // Lengths of array in PyDat1 common
    static const int _lenI1	= 200; 

    struct PYDAT1;
    friend struct PYDAT1;

    struct PYDAT1
    {
      int mstu[_lenI1];
      double paru[_lenI1];
      int mstj[_lenI1];
      double parj[_lenI1];
    };

    int  _dummy;
    double  _realdummy;

    static PYDAT1* _PyDat1;
};

// set pointer to zero at start
PyDat1::PYDAT1* PyDat1::_PyDat1 = 0;

inline void
PyDat1::init(void)
{ if (!_PyDat1) _PyDat1 = static_cast<PYDAT1*>(pydat1_address_());}

// Constructor
inline
PyDat1::PyDat1()
       : _dummy		(-999),
       _realdummy	(-999.)
{}

// Destructor
inline
PyDat1::~PyDat1()
{}

inline int&
PyDat1::mstu	(int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI1()) return _dummy;

    return _PyDat1->mstu[i-1];
}

inline double&
PyDat1::paru	(int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI1()) return _realdummy;

    return _PyDat1->paru[i-1];
}

inline int&
PyDat1::mstj	(int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI1()) return _dummy;

    return _PyDat1->mstj[i-1];
}

inline double&
PyDat1::parj	(int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI1()) return _realdummy;

    return _PyDat1->parj[i-1];
}

#endif /* __PYDAT1_H__ */
