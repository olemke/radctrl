#ifndef forward_h
#define forward_h

#include "grids.h"
#include "jacobian.h"
#include "rtehelpers.h"
#include "xsec.h"

namespace RTE {
namespace ForwardCalculations {
template <size_t N>
struct Results {
  Grid<RadVec<N>, 2> x;
  Derivative::Jacobian dx;

  Results(const std::vector<RadVec<N>> &rad0,
          const std::vector<Derivative::Target> &targets,
          const std::vector<Path::Point> &path,
          const std::vector<double> &freqs,
          const std::vector<Derivative::Polarization> &polarization) noexcept
      : x({}, path.size(), freqs.size()),
        dx(targets, path, freqs, polarization) {
    for (size_t i = 0; i < freqs.size(); i++) {
      x(path.size() - 1, i) = rad0[i];
    }
  }

  [[nodiscard]] std::vector<RadVec<N>> sensor_results() const {
    std::vector<RadVec<N>> out(x.sizes()[1]);
    for (std::size_t i = 0; i < x.sizes()[1]; i++) {
      out[i] = x(0, i);
    }
    return out;
  }
};

Results<1> compute(const std::vector<RadVec<1>> &rad0,
                   const std::vector<Path::Point> &path,
                   const std::vector<Absorption::Band> &bands,
                   const Frequency<FrequencyType::Freq> flow,
                   const Frequency<FrequencyType::Freq> fupp,
                   const size_t size);

Results<2> compute(const std::vector<RadVec<2>> &rad0,
                   const std::vector<Path::Point> &path,
                   const std::vector<Absorption::Band> &bands,
                   const Frequency<FrequencyType::Freq> flow,
                   const Frequency<FrequencyType::Freq> fupp,
                   const size_t size);

Results<3> compute(const std::vector<RadVec<3>> &rad0,
                   const std::vector<Path::Point> &path,
                   const std::vector<Absorption::Band> &bands,
                   const Frequency<FrequencyType::Freq> flow,
                   const Frequency<FrequencyType::Freq> fupp,
                   const size_t size);

Results<4> compute(const std::vector<RadVec<4>> &rad0,
                   const std::vector<Path::Point> &path,
                   const std::vector<Absorption::Band> &bands,
                   const Frequency<FrequencyType::Freq> flow,
                   const Frequency<FrequencyType::Freq> fupp,
                   const size_t size);
}  // namespace ForwardCalculations
}  // namespace RTE

#endif  // forward_h
