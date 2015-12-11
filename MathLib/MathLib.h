#ifndef MATHLIB_H_INCLUDED
#define MATHLIB_H_INCLUDED

#include <float.h>
#include "MathLibCommon.h"
#include "MathLib_Vector4.h"
#include "MathLib_Matrix4x4.h"
#include "MathLib_MatrixVectorOps.h"
#include "MathLib_Quaternion.h"
#include "MathLib_QuaternionMatrixOps.h"
#include "MathLib_Ray.h"
#include "MathLib_Plane.h"
#include "MathLib_PlaneOps.h"
#include "MathLibUtilities.h"

namespace MathLib
{

/// <summary>
/// Initializes MathLib.
/// !! This function must be called before any MathLib functions are used. !!
/// </summary>
void initMathLib();


/// <summary>Retrieves a char array with information regarding the current MathLib configuration.</summary>
const char* getInfoString();

}

#endif // MATHLIB_H_INCLUDED
