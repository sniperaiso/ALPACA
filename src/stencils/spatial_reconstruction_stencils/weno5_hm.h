/*****************************************************************************************
*                                                                                        *
* This file is part of ALPACA                                                            *
*                                                                                        *
******************************************************************************************
*                                                                                        *
*  \\\\                                                                                  *
*  l '>                                                                                  *
*  | |                                                                                   *
*  | |                                                                                   *
*  | alpaca~                                                                             *
*  ||    ||                                                                              *
*  ''    ''                                                                              *
*                                                                                        *
* ALPACA                                                                                 *
* Copyright (c) 2017-2018 Nikolaus A. Adams and contributors (see AUTHORS list)          *
* All rights reserved.                                                                   *
*                                                                                        *
* Chair of Aerodynamics and Fluid Mechanics                                              *
* Technical University of Munich                                                         *
*                                                                                        *
* This code is developed by the 'Nanoshock group' at the Chair of Aerodynamics and       *
* Fluid Mechanics, Technical University of Munich.                                       *
*                                                                                        *
* This project has received funding from the European Reseach Council (ERC)              *
* under the European Union's Horizon 2020 research and innovation programme              *
* (grant agreement No 667483).                                                           *
*                                                                                        *
* ERC Advanced Grant No 667483, Prof. Dr. Nikolaus A. Adams:                             *
* "NANOSHOCK - Manufacturing Shock Interactions for Innovative Nanoscale Processes"      *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* Redistribution and use in source and binary forms, with or without                     *
* modification, are permitted provided that the following conditions are met:            *
*                                                                                        *
* 1. Redistributions of source code must retain the above copyright notice,              *
*    this list of conditions and the following disclaimer.                               *
*                                                                                        *
* 2. Redistributions in binary form must reproduce the above copyright notice            *
*    this list of conditions and the following disclaimer in the documentation           *
*    and/or other materials provided with the distribution.                              *
*                                                                                        *
* 3. Neither the name of the copyright holder nor the names of its                       *
*    contributors may be used to endorse or promote products derived from this           *
*    software without specific prior written permission.                                 *
*                                                                                        *
* 4. Any redistribution of substantial fractions of the code as a                        *
*    different project should preserve the word ALPACA in the name                       *
*    of the code                                                                         *
*                                                                                        *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"            *
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE              *
* IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE            *
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE              *
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR                    *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF                   *
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS               *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN                *
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)                *
* ARISING IN ANY WAY OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            *
* POSSIBILITY OF SUCH DAMAGE.                                                            *
*                                                                                        *
* Please note, several third-party tools are used within the ALPACA code under           *
* their own license agreement.                                                           *
*                                                                                        *
* 1. tiny_xml           : TinyXML-2 is released under the zlib license.                  *
*                         This software is provided 'as-is', without any express or      *
*                         implied warranty. In no event will the authors be held liable  *
*                         for any damages arising from the use of this software.         *
*                         https://opensource.org/licenses/Zlib                           *
*                         See COPYING_TINY_XML.txt for more information.                 *
*                                                                                        *
* 2. expression_toolkit : Free use of the C++ Mathematical Expression Library is         *
*                         permitted under the guidelines and in accordance with the MIT  *
*                         License.                                                       *
*                         https://opensource.org/licenses/MIT                            *
*                         See COPYING_EXPRESSION_TOOLKIT.txt for more information.       *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* AUTHORS                                                                                *
*                                                                                        *
*   Prof. Dr. Nikolaus A. Adams                                                          *
*                                                                                        *
*   Dr. Stefan Adami                                                                     *
*   Vladimir Bogdanov                                                                    *
*   Aaron Buhendwa                                                                       *
*   Nico Fleischmann                                                                     *
*   Nils Hoppe                                                                           *
*   Naeimeh Hosseini                                                                     *
*   Jakob Kaiser                                                                         *
*   Aleksandr Lunkov                                                                     *
*   Thomas Paula                                                                         *
*   Josef Winter                                                                         *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* CONTACT                                                                                *
*                                                                                        *
*   nanoshock@aer.mw.tum.de                                                              *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* Munich, June 13th 2019                                                                 *
*                                                                                        *
*****************************************************************************************/
#ifndef WENOHM_H
#define WENOHM_H

#include "stencils/stencil.h"

/**
 * @brief Discretization of the SpatialReconstructionStencil class to compute fluxes according to \cite Zeng2019
 */
class WENO5HM : public Stencil<WENO5HM> {

    friend Stencil;

    static constexpr StencilType stencil_type_ = StencilType::Reconstruction;

    // Coefficients for WENOHM scheme
    static constexpr double coef_smoothness_1_  = 13.0/12.0;
    static constexpr double coef_smoothness_2_  = 0.25;

    static constexpr double coef_smoothness_11_ =  1.0;
    static constexpr double coef_smoothness_12_ = -2.0;
    static constexpr double coef_smoothness_13_ =  1.0;
    static constexpr double coef_smoothness_14_ =  1.0;
    static constexpr double coef_smoothness_15_ = -4.0;
    static constexpr double coef_smoothness_16_ =  3.0;

    static constexpr double coef_smoothness_21_ =  1.0;
    static constexpr double coef_smoothness_22_ = -2.0;
    static constexpr double coef_smoothness_23_ =  1.0;
    static constexpr double coef_smoothness_24_ =  1.0;
    static constexpr double coef_smoothness_25_ = -1.0;

    static constexpr double coef_smoothness_31_ =  1.0;
    static constexpr double coef_smoothness_32_ = -2.0;
    static constexpr double coef_smoothness_33_ =  1.0;
    static constexpr double coef_smoothness_34_ =  3.0;
    static constexpr double coef_smoothness_35_ = -4.0;
    static constexpr double coef_smoothness_36_ =  1.0;

    static constexpr double coef_weights_1_ = 0.1;
    static constexpr double coef_weights_2_ = 0.6;
    static constexpr double coef_weights_3_ = 0.3;

    static constexpr double coef_stencils_1_ =  2.0/6.0;
    static constexpr double coef_stencils_2_ = -7.0/6.0;
    static constexpr double coef_stencils_3_ = 11.0/6.0;
    static constexpr double coef_stencils_4_ = -1.0/6.0;
    static constexpr double coef_stencils_5_ =  5.0/6.0;
    static constexpr double coef_stencils_6_ =  2.0/6.0;
    static constexpr double coef_stencils_7_ =  2.0/6.0;
    static constexpr double coef_stencils_8_ =  5.0/6.0;
    static constexpr double coef_stencils_9_ = -1.0/6.0;

    //Number of cells required for upwind and downwind stencils, as well as number of cells downstream of the cell
    static constexpr unsigned int stencil_size_            = 6;
    static constexpr unsigned int downstream_stencil_size_ = 2;

    double ApplyImplementation( std::array<double, stencil_size_> const& array, std::array<int const, 2> const evaluation_properties, double const cell_size) const;

public:
    explicit WENO5HM() = default;
    ~WENO5HM() = default;
    WENO5HM( WENO5HM const& ) = delete;
    WENO5HM& operator=( WENO5HM const& ) = delete;
    WENO5HM( WENO5HM&& ) = delete;
    WENO5HM& operator=( WENO5HM&& ) = delete;
};

#endif // STENCIL_WENOHM_H
