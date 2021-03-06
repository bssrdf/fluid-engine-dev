// Copyright (c) 2016 Doyub Kim

#ifndef INCLUDE_JET_ENO_LEVEL_SET_SOLVER2_H_
#define INCLUDE_JET_ENO_LEVEL_SET_SOLVER2_H_

#include <jet/iterative_level_set_solver2.h>

namespace jet {

//! Two-dimensional third-order ENO-based iterative level set solver.
class EnoLevelSetSolver2 final : public IterativeLevelSetSolver2 {
 public:
    //! Default constructor.
    EnoLevelSetSolver2();

 protected:
    //! Computes the derivatives for given grid point.
    void getDerivatives(
        ConstArrayAccessor2<double> grid,
        const Vector2D& gridSpacing,
        size_t i,
        size_t j,
        std::array<double, 2>* dx,
        std::array<double, 2>* dy) const override;
};

}  // namespace jet

#endif  // INCLUDE_JET_ENO_LEVEL_SET_SOLVER2_H_
