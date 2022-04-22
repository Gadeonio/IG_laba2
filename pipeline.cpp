/*

    Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pipeline.h"

void Pipeline::InitScaleTransform(glm::mat4& m) const
{
    m[0][0] = m_scale.x; m[0][1] = 0.0f     ; m[0][2] = 0.0f     ; m[0][3] = 0.0f;
    m[1][0] = 0.0f     ; m[1][1] = m_scale.y; m[1][2] = 0.0f     ; m[1][3] = 0.0f;
    m[2][0] = 0.0f     ; m[2][1] = 0.0f     ; m[2][2] = m_scale.z; m[2][3] = 0.0f;
    m[3][0] = 0.0f     ; m[3][1] = 0.0f     ; m[3][2] = 0.0f     ; m[3][3] = 1.0f;
}

void Pipeline::InitRotateTransform(glm::mat4& m) const
{
    glm::mat4 rx, ry, rz;

    const float x = ToRadian(m_rotateInfo.x);
    const float y = ToRadian(m_rotateInfo.y);
    const float z = ToRadian(m_rotateInfo.z);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f   ; rx[0][2] = 0.0f    ; rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x) ; rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f   ; rx[3][2] = 0.0f    ; rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f   ; ry[1][1] = 1.0f; ry[1][2] = 0.0f    ; ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y) ; ry[2][3] = 0.0f;
    ry[3][0] = 0.0f   ; ry[3][1] = 0.0f; ry[3][2] = 0.0f    ; ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z) ; rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f   ; rz[2][1] = 0.0f    ; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f   ; rz[3][1] = 0.0f    ; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    m = rz * ry * rx;
}

void Pipeline::InitTranslationTransform(glm::mat4& m) const
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = m_worldPos.x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = m_worldPos.y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = m_worldPos.z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Pipeline::InitPerspectiveProj(glm::mat4& m) const
{
    const float ar = m_persProj.Width / m_persProj.Height;
    const float zNear = m_persProj.zNear;
    const float zFar = m_persProj.zFar;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0f));

    m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;                   m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;                   m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-zNear - zFar) / zRange; m[2][3] = 2.0f * zFar * zNear / zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;                   m[3][3] = 0.0;
}


const glm::mat4* Pipeline::GetTrans()
{
    glm::mat4 ScaleTrans, RotateTrans, TranslationTrans, PersProjTrans;

    InitScaleTransform(ScaleTrans);
    InitRotateTransform(RotateTrans);
    InitTranslationTransform(TranslationTrans);
    InitPerspectiveProj(PersProjTrans);

    m_transformation = PersProjTrans * TranslationTrans * RotateTrans * ScaleTrans;
    return &m_transformation;
}


