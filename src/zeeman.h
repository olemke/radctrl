#ifndef zeeman_h
#define zeeman_h

#include <Eigen/Core>
#include <cmath>

#include "constants.h"
#include "rational.h"
#include "wigner.h"

namespace Absorption {
struct Zeeman {
  /** Zeeman polarization selection */
  enum class Polarization : char { SigmaMinus, Pi, SigmaPlus, None };

  double gu, gl;
  constexpr Zeeman(double u = 0, double l = 0) noexcept : gu(u), gl(l) {}
  friend std::ostream& operator<<(std::ostream& os, Zeeman z) {
    return os << z.gu << ' ' << z.gl;
  }
  friend std::istream& operator>>(std::istream& os, Zeeman& z) {
    return os >> z.gu >> z.gl;
  }

  /** Gives the change of M given a polarization type
   *
   * @param[in] type The polarization type
   *
   * @return The change in M
   */
  static constexpr int dM(Polarization type) noexcept {
    switch (type) {
      case Polarization::SigmaMinus:
        return -1;
      case Polarization::None:
      case Polarization::Pi:
        return 0;
      case Polarization::SigmaPlus:
        return 1;
    }
    return std::numeric_limits<int>::max();
  }

  /** Gives the lowest M for a polarization type of this transition
   *
   * Since the polarization determines the change in M, this
   * function gives the first M of interest in the range of M
   * possible for a given transition
   *
   * The user has to ensure that Ju and Jl is a valid transition
   *
   * @param[in] Ju J of the upper state
   * @param[in] Jl J of the upper state
   * @param[in] type The polarization type
   *
   * @return The lowest M value
   */
  static constexpr Rational start(Rational Ju, Rational Jl,
                                  Polarization type) noexcept {
    switch (type) {
      case Polarization::SigmaMinus:
        if (Ju < Jl)
          return -Ju;
        else if (Ju == Jl)
          return -Ju + 1;
        else
          return -Ju + 2;
      case Polarization::Pi:
        return -std::min(Ju, Jl);
      case Polarization::SigmaPlus:
        return -Ju;
      case Polarization::None:
        return 0;
    }
    return Rational(std::numeric_limits<int>::max());
  }

  /** Gives the largest M for a polarization type of this transition
   *
   * Since the polarization determines the change in M, this
   * function gives the last M of interest in the range of M
   * possible for a given transition
   *
   * The user has to ensure that Ju and Jl is a valid transition
   *
   * @param[in] Ju J of the upper state
   * @param[in] Jl J of the upper state
   * @param[in] type The polarization type
   *
   * @return The largest M value
   */
  static constexpr Rational end(Rational Ju, Rational Jl,
                                Polarization type) noexcept {
    switch (type) {
      case Polarization::SigmaMinus:
        return Ju + 1;
      case Polarization::Pi:
        return std::min(Ju, Jl);
      case Polarization::SigmaPlus:
        if (Ju < Jl)
          return Ju + 1;
        else if (Ju == Jl)
          return Ju;
        else
          return Jl;
      case Polarization::None:
        return 0;
    }
    return Rational(std::numeric_limits<int>::max());
  }

  /** Gives the upper state M value at an index
   *
   * The user has to ensure that Ju and Jl is a valid transition
   *
   * The user has to ensure n is less than the number of elements
   *
   * @param[in] Ju J of the upper state
   * @param[in] Jl J of the upper state
   * @param[in] type The polarization type
   * @param[in] n The position
   *
   * @return The upper state M
   */
  static constexpr Rational Mu(Rational Ju, Rational Jl, Polarization type,
                               int n) noexcept {
    return start(Ju, Jl, type) + n;
  }

  /** Gives the lower state M value at an index
   *
   * The user has to ensure that Ju and Jl is a valid transition
   *
   * The user has to ensure n is less than the number of elements
   *
   * @param[in] Ju J of the upper state
   * @param[in] Jl J of the upper state
   * @param[in] type The polarization type
   * @param[in] n The position
   *
   * @return The lower state M
   */
  static constexpr Rational Ml(Rational Ju, Rational Jl, Polarization type,
                               int n) noexcept {
    return Mu(Ju, Jl, type, n) + dM(type);
  }

  /** The renormalization factor of a polarization type
   *
   * The polarization comes from some geometry.  This function
   * returns the factor we need to compute that geometry and to
   * turn it into something that normalizes every possible M
   * for this type into some strength that sums to unity
   *
   * @param[in] type The polarization type
   *
   * @return Rescale factor
   */
  static constexpr double PolarizationFactor(Polarization type) noexcept {
    switch (type) {
      case Polarization::SigmaMinus:
        return .75;
      case Polarization::Pi:
        return 1.5;
      case Polarization::SigmaPlus:
        return .75;
      case Polarization::None:
        return 1.0;
    }
    return std::numeric_limits<double>::max();
  }

  /** Gives the strength of one subline of a given polarization
   *
   * The user has to ensure that Ju and Jl is a valid transition
   *
   * The user has to ensure n is less than the number of elements
   *
   * @param[in] Ju J of the upper state
   * @param[in] Jl J of the upper state
   * @param[in] type The polarization type
   * @param[in] n The position
   *
   * @return The relative strength of the Zeeman subline
   */
  double Strength(Rational Ju, Rational Jl, Polarization type, int n) const {
    using Constant::pow2;

    auto ml = Ml(Ju, Jl, type, n);
    auto mu = Mu(Ju, Jl, type, n);
    auto dm = Rational(dM(type));
    return PolarizationFactor(type) *
           pow2(wigner3j(Jl, Rational(1), Ju, ml, -dm, -mu));
  }

  /** Gives the splitting of one subline of a given polarization
   *
   * The user has to ensure that Ju and Jl is a valid transition
   *
   * The user has to ensure n is less than the number of elements
   *
   * @param[in] Ju J of the upper state
   * @param[in] Jl J of the upper state
   * @param[in] type The polarization type
   * @param[in] n The position
   *
   * @return The splitting of the Zeeman subline
   */
  constexpr double Splitting(Rational Ju, Rational Jl, Polarization type,
                             int n) const noexcept {
    using Constant::bohr_magneton;
    using Constant::h;
    constexpr double C = bohr_magneton / h;

    return C * (double(Ml(Ju, Jl, type, n)) * gl -
                double(Mu(Ju, Jl, type, n)) * gu);
  }

  static inline Eigen::Vector3d los_xyz_by_uvw_local(double u, double v,
                                                     double w) {
    return Eigen::Vector3d(v, u, w).normalized();
  }

  static inline Eigen::Vector3d los_xyz_by_za_local(double z, double a) {
    using Conversion::cosd;
    using Conversion::sind;
    return Eigen::Vector3d(cosd(a) * sind(z), sind(a) * sind(z), cosd(z));
  }

  static inline Eigen::Vector3d ev_xyz_by_za_local(double z, double a) {
    using Conversion::cosd;
    using Conversion::sind;
    return Eigen::Vector3d(cosd(a) * cosd(z), sind(a) * cosd(z), -sind(z));
  }

  struct Angles {
    double theta;
    double eta;
  };

  static inline Angles angles(double u, double v, double w, double z,
                              double a) noexcept {
    // XYZ vectors normalized
    const Eigen::Vector3d n = los_xyz_by_za_local(z, a);
    const Eigen::Vector3d ev = ev_xyz_by_za_local(z, a);
    const Eigen::Vector3d nH = los_xyz_by_uvw_local(u, v, w);
    const Eigen::Vector3d inplane = nH - nH.dot(n) * n;

    // If there is no magnetic field, bailout quickly
    double H = std::hypot(u, v, w);
    if (H == 0)
      return {0, 0};
    else
      return {Conversion::acosd(n.dot(nH)),
              Conversion::atan2d(ev.dot(inplane), ev.cross(inplane).dot(n))};
  }
};
}  // namespace Absorption

#endif  // zeeman_h
