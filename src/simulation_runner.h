/*****************************************************************************************
*                                                                                        *
* This file is part of ALPACA                                                            *
*                                                                                        *
******************************************************************************************
*                                                                                        *
*  \\                                                                                    *
*  l '>                                                                                  *
*  | |                                                                                   *
*  | |                                                                                   *
*  | alpaca~                                                                             *
*  ||    ||                                                                              *
*  ''    ''                                                                              *
*                                                                                        *
* ALPACA is a MPI-parallelized C++ code framework to simulate compressible multiphase    *
* flow physics. It allows for advanced high-resolution sharp-interface modeling          *
* empowered with efficient multiresolution compression. The modular code structure       *
* offers a broad flexibility to select among many most-recent numerical methods covering *
* WENO/T-ENO, Riemann solvers (complete/incomplete), strong-stability preserving Runge-  *
* Kutta time integration schemes, level set methods and many more.                       *
*                                                                                        *
* This code is developed by the 'Nanoshock group' at the Chair of Aerodynamics and       *
* Fluid Mechanics, Technical University of Munich.                                       *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* LICENSE                                                                                *
*                                                                                        *
* ALPACA - Adaptive Level-set PArallel Code Alpaca                                       *
* Copyright (C) 2020 Nikolaus A. Adams and contributors (see AUTHORS list)               *
*                                                                                        *
* This program is free software: you can redistribute it and/or modify it under          *
* the terms of the GNU General Public License as published by the Free Software          *
* Foundation version 3.                                                                  *
*                                                                                        *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY        *
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A        *
* PARTICULAR PURPOSE. See the GNU General Public License for more details.               *
*                                                                                        *
* You should have received a copy of the GNU General Public License along with           *
* this program (gpl-3.0.txt).  If not, see <https://www.gnu.org/licenses/gpl-3.0.html>   *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* THIRD-PARTY tools                                                                      *
*                                                                                        *
* Please note, several third-party tools are used by ALPACA. These tools are not shipped *
* with ALPACA but available as git submodule (directing to their own repositories).      *
* All used third-party tools are released under open-source licences, see their own      *
* license agreement in 3rdParty/ for further details.                                    *
*                                                                                        *
* 1. tiny_xml           : See LICENSE_TINY_XML.txt for more information.                 *
* 2. expression_toolkit : See LICENSE_EXPRESSION_TOOLKIT.txt for more information.       *
* 3. FakeIt             : See LICENSE_FAKEIT.txt for more information                    *
* 4. Catch2             : See LICENSE_CATCH2.txt for more information                    *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* CONTACT                                                                                *
*                                                                                        *
* nanoshock@aer.mw.tum.de                                                                *
*                                                                                        *
******************************************************************************************
*                                                                                        *
* Munich, July 1st, 2020                                                                 *
*                                                                                        *
*****************************************************************************************/
#ifndef SIMULATION_RUNNER_H
#define SIMULATION_RUNNER_H

#include "log_writer.h"

#include "initialization/input_output/initialization_input_output_manager.h"
#include "initialization/input_output/initialization_output_writer.h"
#include "initialization/input_output/initialization_restart_manager.h"
#include "initialization/halo_manager/initialization_external_halo_manager.h"
#include "initialization/halo_manager/initialization_internal_halo_manager.h"
#include "initialization/halo_manager/initialization_halo_manager.h"
#include "initialization/materials/initialization_material_manager.h"
#include "initialization/topology/initialization_topology_manager.h"
#include "initialization/topology/initialization_tree.h"
#include "initialization/initialization_initial_condition.h"
#include "initialization/initialization_communication_manager.h"
#include "initialization/initialization_multiresolution.h"
#include "initialization/initialization_unit_handler.h"
#include "initialization/initialization_modular_algorithm_assembler.h"

namespace Simulation {

   /**
    * @brief Run simulation of ALPACA.
    * @param input_reader Reader that is used to provide user-information from an input file.
    */
   void Run( InputReader const& input_reader ) {
      LogWriter& logger = LogWriter::Instance();

      int number_of_ranks = -1;
      MPI_Comm_size( MPI_COMM_WORLD, &number_of_ranks );
      logger.LogMessage( "Number of MPI ranks : " + std::to_string( number_of_ranks ) );
#ifdef HILBERT
      logger.LogMessage( "Load balancing      : Hilbert-Curve" );
#else
      logger.LogMessage( "Load Balancing      : Z-Curve" );
#endif
      logger.AddBreakLine( true );

      // Instance for dimensionalization and non-dimensionalization of variables
      UnitHandler const unit_handler( Initialization::InitializeUnitHandler( input_reader ) );
      logger.AddBreakLine( true );
      // Instance for handling of material and material pairing data
      MaterialManager const material_manager( Initialization::InitializeMaterialManager( input_reader, unit_handler ) );
      logger.AddBreakLine( true );
      // Instance for handling global node data (cannot be const due to changes in loop)
      TopologyManager topology_manager( Initialization::InitializeTopologyManager( input_reader, material_manager ) );
      Tree tree( Initialization::InitializeTree( input_reader, topology_manager, unit_handler ) );
      logger.AddBreakLine( true );
      // Instance that provides multi-resolution information
      Multiresolution const multiresolution( Initialization::InitializeMultiresolution( input_reader, topology_manager ) );
      logger.AddBreakLine( true );
      // Instance to provide communication
      CommunicationManager communication_manager( Initialization::InitializeCommunicationManager( topology_manager ) );
      // Instances for handling boundary conditions (internal and external). The external and internal halo managera are not initialized inside the halo manager
      // due to delete move constructors of both classes. A creation of the external halo manager inside not suitable due to the inclusion of the input reader.
      // The internal cannot be created inside due to constness of some functions.
      ExternalHaloManager const external_halo_manager( Initialization::InitializeExternalHaloManager( input_reader, unit_handler, material_manager ) );
      InternalHaloManager internal_halo_manager( Initialization::InitializeInternalHaloManager( topology_manager, tree, communication_manager, material_manager ) );
      HaloManager halo_manager( Initialization::InitializeHaloManager( topology_manager, tree, external_halo_manager, internal_halo_manager, communication_manager ) );
      logger.AddBreakLine( true );
      // Instance to restart simulation from snapshot and write output files (cannot be const due to vector eraseing inside)
      OutputWriter const output_writer( Initialization::InitializeOutputWriter( topology_manager, tree, material_manager, unit_handler ) );
      RestartManager const restart_manager( Initialization::InitializeRestartManager( topology_manager, tree, unit_handler ) );
      InputOutputManager input_output_manager( Initialization::InitializeInputOutputManager( input_reader, output_writer, restart_manager, unit_handler ) );
      logger.AddBreakLine( true );
      // Instance for handling the initial conditions of the simulation
      InitialCondition const initial_condition( Initialization::InitializeInitialCondition( input_reader, topology_manager, tree, material_manager, unit_handler ) );

      // Instance for the whole computation loop
      ModularAlgorithmAssembler mr_based_algorithm( Initialization::InitializeModularAlgorithmAssembler( input_reader, topology_manager, tree,
                                                                                                         communication_manager, halo_manager, multiresolution,
                                                                                                         material_manager, input_output_manager, initial_condition,
                                                                                                         unit_handler ) );
      logger.AddBreakLine( true );

      logger.FlushWelcomeMessage();
      // Flush first here to ensure that the log file is set
      logger.Flush();

      // Calling the functions for simulation execution
      mr_based_algorithm.Initialization();

      mr_based_algorithm.ComputeLoop();

      logger.Flush();
   }

}// namespace Simulation

#endif// SIMULATION_RUNNER_H
