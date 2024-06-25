// Ceres Solver - A fast non-linear least squares minimizer
// Copyright 2023 Google Inc. All rights reserved.
// http://ceres-solver.org/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of Google Inc. nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Author: sameeragarwal@google.com (Sameer Agarwal)

#include <cmath>
#include <cstdlib>

#include "absl/log/check.h"
#include "absl/log/log.h"
#include "ceres/ceres.h"
#include "gtest/gtest.h"

namespace ceres::internal {

class QuadraticFirstOrderFunction : public ceres::FirstOrderFunction {
 public:
  bool Evaluate(const double* parameters,
                double* cost,
                double* gradient) const final {
    cost[0] = parameters[0] * parameters[0];
    if (gradient != nullptr) {
      gradient[0] = 2.0 * parameters[0];
    }
    return true;
  }

  int NumParameters() const final { return 1; }
};

TEST(LineSearchMinimizerTest, FinalCostIsZero) {
  double parameters[1] = {2.0};
  ceres::GradientProblem problem(new QuadraticFirstOrderFunction);
  ceres::GradientProblemSolver::Options options;
  ceres::GradientProblemSolver::Summary summary;
  ceres::Solve(options, problem, parameters, &summary);
  EXPECT_NEAR(summary.final_cost, 0.0, std::numeric_limits<double>::epsilon());
}

}  // namespace ceres::internal
