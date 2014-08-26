#ifndef __PYDATR_H__
#define __PYDATR_H__

extern "C" { void* pydatr_address_(void); }
/**
@class PyDatr

@brief       Class definition for PyDatr, which is used
      to modify the Pythia PYDATR common.
*/

class PyDatr {
public:
    PyDatr();
    ~PyDatr();
    
    int&        mrpy    (int i);
    double&     rrpy    (int i);

    void	init	(void);

    // return common array lengths
    inline int	lenI1() const {return _lenI1;}
    inline int	lenI2() const {return _lenI2;}

private: 

    // Lengths of array in PyDatr common
    static const int _lenI1	= 6; 
    static const int _lenI2	= 100; 

    struct PYDATR;
    friend struct PYDATR;

    struct PYDATR
    {
      int mrpy[_lenI1];
      double rrpy[_lenI2];
    };

    int  _dummy;
    double  _realdummy;

    static PYDATR* _PyDatr;
};

// set pointer to zero at start
PyDatr::PYDATR* PyDatr::_PyDatr = 0;

inline void
PyDatr::init(void)
{ if (!_PyDatr) _PyDatr = static_cast<PYDATR*>(pydatr_address_());}

// Constructor
inline
PyDatr::PyDatr()
       : _dummy		(-999),
       _realdummy	(-999.)
{}

// Destructor
inline
PyDatr::~PyDatr()
{}

inline int&
PyDatr::mrpy	(int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI1()) return _dummy;

    return _PyDatr->mrpy[i-1];
}

inline double&
PyDatr::rrpy	(int i)
{
    init(); // check COMMON is initialized
    if( i < 1 || i > lenI2()) return _realdummy;

    return _PyDatr->rrpy[i-1];
}

#endif /* __PYDATR_H__ */
