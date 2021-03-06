#ifndef C_VECTOR_H
#define	C_VECTOR_H

#include <stdint.h>

typedef struct VectorFloat
{
    float XAxis;
    float YAxis;
    float ZAxis;
} VectorFloat_t;

typedef struct VectorInt16
{
    int16_t XAxis;
    int16_t YAxis;
    int16_t ZAxis;
} VectorInt16_t;

typedef struct VectorUint16
{
    uint16_t XAxis;
    uint16_t YAxis;
    uint16_t ZAxis;
} VectorUint16_t;
#endif	/* C_VECTOR_H */

