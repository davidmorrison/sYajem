      INTEGER*8 FUNCTION YADAT_ADDRESS() 

      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      COMMON /YADAT/YAPROFILET(500),YAPROFILEQ(500),YAPROFILEINT(500)
      SAVE  /YADAT/
      
      YADAT_ADDRESS = LOC(YAPROFILET(1))

      RETURN
      END
