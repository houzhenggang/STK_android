//  $Id: vec3.cpp 1954 2008-05-20 10:01:26Z scifly $
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2008 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "utils/vec3.hpp"

#include "utils/constants.hpp"


void Vec3::setHPR(const btQuaternion& q)
{
    float W = q.getW();
    float X = q.getX();
    float Y = q.getY();
    float Z = q.getZ();
    float WSquared = W * W;
    float XSquared = X * X;
    float YSquared = Y * Y;
    float ZSquared = Z * Z;

    setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
    setY(asinf(-2.0f * (X * Z - Y * W)));
    setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
}   // setHPR(btQuaternion)

// ----------------------------------------------------------------------------
void Vec3::degreeToRad()
{
    m_x=DEGREE_TO_RAD*m_x;
    m_y=DEGREE_TO_RAD*m_y;      
    m_z=DEGREE_TO_RAD*m_z;
}   // degreeToRad

// ----------------------------------------------------------------------------
/** Sets the pitch and the roll of this vector to follow the normal given. The
 *  heading is taken from this vector.
 *  \param normal The normal vector to which pitch and roll should be aligned.
 */
void Vec3::setPitchRoll(const Vec3 &normal)
{
    const float X = sin(getHeading());
    const float Z = cos(getHeading());
    // Compute the angle between the normal of the plane and the line to
    // (x,0,z).  (x,0,z) is normalised, so are the coordinates of the plane,
    // which simplifies the computation of the scalar product.
    float pitch = ( normal.getX()*X + normal.getZ()*Z );  // use ( x,0,z)
    float roll  = (-normal.getX()*Z + normal.getZ()*X );  // use (-z,0,x)

    // The actual angle computed above is between the normal and the (x,y,0)
    // line, so to compute the actual angles 90 degrees must be subtracted.
    setPitch(-acosf(pitch) + NINETY_DEGREE_RAD);
    setRoll (-acosf(roll)  + NINETY_DEGREE_RAD);
}   // setPitchRoll

// ----------------------------------------------------------------------------
/** Converts a bullet HPR value into an irrlicht HPR value.
 *  FIXME: this function should be inlined, but while debugging it's
 *  easier (compile-time wise) to modify it here :)
 */
const core::vector3df Vec3::toIrrHPR() const
{
    core::vector3df r(RAD_TO_DEGREE*(getX()),     // pitch
                      RAD_TO_DEGREE*(getY()),     // heading
                      RAD_TO_DEGREE*(getZ()) );   // roll
    return r;

}  // toIrrHPR
// ----------------------------------------------------------------------------
/** Converts a vec3 into an irrlicht vector (which is a simple type cast).
 */
const core::vector3df& Vec3::toIrrVector() const
{
    return (const core::vector3df&)*this;
}   // toIrrVector

// ----------------------------------------------------------------------------
/** Returns the X and Y component as an irrlicht 2d vector. */
const core::vector2df Vec3::toIrrVector2d() const
{
    core::vector2df v(m_x, m_z);
    return v;
}   // toIrrVector2d
