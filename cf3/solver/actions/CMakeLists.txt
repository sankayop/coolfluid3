coolfluid_find_orphan_files()

list( APPEND coolfluid_solver_actions_files
  AdvanceTime.hpp
  AdvanceTime.cpp
  DirectionalAverage.hpp
  DirectionalAverage.cpp
  Iterate.hpp
  Iterate.cpp
  LoopOperation.hpp
  LoopOperation.cpp
  Probe.hpp
  Probe.cpp
  ProbePoints.hpp
  ProbePoints.cpp
  ProbePostProcFunction.hpp
  ProbePostProcFunction.cpp
  ProbePostProcHistory.hpp
  ProbePostProcHistory.cpp
  FieldTimeAverage.hpp
  FieldTimeAverage.cpp
  ForAllCells.hpp
  ForAllCells.cpp
  ForAllElements.hpp
  ForAllElements.cpp
  ForAllElementsT.hpp
  ForAllNodes2.hpp
  ForAllNodes2.cpp
  ForAllFaces.hpp
  ForAllFaces.cpp
  Loop.hpp
  Loop.cpp
  PrintIterationSummary.hpp
  PrintIterationSummary.cpp
  ReadRestartFile.hpp
  ReadRestartFile.cpp
  SynchronizeFields.hpp
  SynchronizeFields.cpp
  ComputeArea.hpp
  ComputeArea.cpp
  ComputeVolume.hpp
  ComputeVolume.cpp
  ParallelDataToFields.hpp
  ParallelDataToFields.cpp
  PeriodicWriteMesh.hpp
  PeriodicWriteMesh.cpp
  RandomizeField.hpp
  RandomizeField.cpp
  LibActions.hpp
  LibActions.cpp
  Conditional.hpp
  Conditional.cpp
  TimeSeriesWriter.hpp
  TimeSeriesWriter.cpp
  TurbulenceStatistics.hpp
  TurbulenceStatistics.cpp
  TwoPointCorrelation.hpp
  TwoPointCorrelation.cpp
  WriteRestartFile.hpp
  WriteRestartFile.cpp
  YPlus.hpp
  YPlus.cpp
)

list( APPEND coolfluid_solver_actions_proto_files
    # Core Proto files
    Proto/BlockAccumulator.hpp
    Proto/ComponentWrapper.hpp
    Proto/ConfigurableConstant.hpp
    Proto/FieldSync.hpp
    Proto/FieldSync.cpp
    Proto/ProtoAction.hpp
    Proto/ProtoAction.cpp
    Proto/DirichletBC.hpp
    Proto/EigenTransforms.hpp
    Proto/ElementData.hpp
    Proto/ElementExpressionWrapper.hpp
    Proto/ElementGradDiv.hpp
    Proto/ElementGrammar.hpp
    Proto/ElementIntegration.hpp
    Proto/ElementLooper.hpp
    Proto/ElementMatrix.hpp
    Proto/ElementOperations.hpp
    Proto/ElementTransforms.hpp
    Proto/Expression.hpp
    Proto/ExpressionGroup.hpp
    Proto/ForEachDimension.hpp
    Proto/Functions.hpp
    Proto/GaussPoints.hpp
    Proto/IndexLooping.hpp
    Proto/LSSWrapper.hpp
    Proto/NodalMatrixManipulation.hpp
    Proto/NodeData.hpp
    Proto/NodeGrammar.hpp
    Proto/NodeLooper.hpp
    Proto/Partial.hpp
    Proto/PhysicsConstant.hpp
    Proto/RestrictExpressionToElementType.hpp
    Proto/RHSVector.hpp
    Proto/SetRHS.hpp
    Proto/SetSolution.hpp
    Proto/SolutionVector.hpp
    Proto/SurfaceIntegration.hpp
    Proto/Terminals.hpp
    Proto/Transforms.hpp
    Proto/VolumeIntegration.hpp
    Proto/ZeroLSSRow.hpp

    # Actions using Proto
    CopyScalar.hpp
    CopyScalar.cpp
    NodeValence.hpp
    NodeValence.cpp
    FaceNormals.hpp
    FaceNormals.cpp
)

if( CF3_ENABLE_PROTO )
  list( APPEND coolfluid_solver_actions_files ${coolfluid_solver_actions_proto_files} )
else()
  coolfluid_mark_not_orphan( ${coolfluid_solver_actions_proto_files} )
endif()

coolfluid3_add_library( TARGET coolfluid_solver_actions
                        KERNEL
                        SOURCES ${coolfluid_solver_actions_files}
                        LIBS    coolfluid_solver coolfluid_math_lss coolfluid_mesh)
