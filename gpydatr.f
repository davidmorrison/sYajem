      INTEGER*8 FUNCTION PYDATR_ADDRESS() 

      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      COMMON/PYDATR/MRPY(6),RRPY(100)
      SAVE  /PYDATR/

      PYDATR_ADDRESS = LOC(MRPY(1))

      RETURN
      END
