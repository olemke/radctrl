#ifndef geom_h
#define geom_h

#include <array>
#include <cmath>

#include "constants.h"
#include "enums.h"
#include "file.h"
#include "timeclass.h"
#include "units.h"

namespace Geom {  
class Ellipsoid {
  std::array<double, 2> ell;
public:
  constexpr Ellipsoid() noexcept : ell({0, 0}) {}
  constexpr Ellipsoid(const Ellipsoid&) noexcept = default;
  constexpr Ellipsoid(Ellipsoid&&) noexcept = default;
  constexpr Ellipsoid(Length<LengthType::meter> a, double e) noexcept : ell({a.value(), e}) {}
  constexpr double a() const noexcept {return ell[0];}
  constexpr double e() const noexcept {return ell[1];}
  double b() const noexcept {return a() * std::sqrt(1 - Constant::pow2(e()));}
  double N(double lat) const noexcept {
    return a() / std::sqrt(1 - Conversion::pow2(e()*Conversion::sind(lat)));
  }
  
  friend std::ostream& operator<<(std::ostream& os, Ellipsoid e) {return os << e.ell[0] << ' ' << e.ell[1];}
  friend std::istream& operator>>(std::istream& is, Ellipsoid& e) {return is >> e.ell[0] >> e.ell[1];}
}; // Ellipsoid
  
ENUMCLASS(PosType, unsigned char,
    Xyz,
    Spherical,
    Ellipsoidal
)

template <PosType T>
class Pos {
  Time time;
  std::array<double, 3> pos;
public:
  double r() const noexcept {return pos[0];}
  double h() const noexcept {return pos[0];}
  double lat() const noexcept {return pos[1];}
  double lon() const noexcept {return pos[2];}
  double x() const noexcept {return pos[0];}
  double y() const noexcept {return pos[1];}
  double z() const noexcept {return pos[2];}
  Time t() const noexcept {return time;}
  std::array<double, 3> arr() const noexcept {return pos;}
  
  Pos() noexcept : time(), pos({0, 0, 0}) {}
  Pos(Pos&&) noexcept = default;
  Pos(const Pos&) noexcept = default;
  Pos& operator=(Pos&&) noexcept = default;
  Pos& operator=(const Pos&) noexcept = default;
  Pos(std::array<double, 3> p) noexcept : time(), pos(p) {}
  Pos(Time t, std::array<double, 3> p) noexcept : time(t), pos(p) {}
  
  template <PosType P>
  Pos(Pos<P> p, Ellipsoid ell) noexcept : time(p.t()), pos(p.arr()) {
    using Constant::pow2;
    using Constant::pow3;
    using Constant::pow4;
    using Conversion::sind;
    using Conversion::cosd;
    using Conversion::asind;
    using Conversion::atan2d;
    if (T == P) {}
    else if (T == PosType::Xyz and P == PosType::Spherical) {
      pos[0] = p.r() * cosd(p.lat()) * cosd(p.lon());
      pos[1] = p.r() * cosd(p.lat()) * sind(p.lon());
      pos[2] = p.r() * sind(p.lat());
    } else if (T == PosType::Xyz and P == PosType::Ellipsoidal) {
      const double N = ell.N(p.lat());
      pos[0] = (N + p.h()) * cosd(p.lon()) * cosd(p.lat());
      pos[1] = (N + p.h()) * sind(p.lon()) * cosd(p.lat());
      pos[2] = (N*(1-pow2(ell.e())) + p.h()) * sind(p.lat());
    } else if (T == PosType::Spherical and P == PosType::Xyz) {
      pos[0] = std::hypot(p.z(), p.y(), p.x());
      pos[1] = asind(p.z() / r());
      pos[2] = atan2d(p.y(), p.x());
    } else if (T == PosType::Ellipsoidal and P == PosType::Xyz) {
      // Following Zeng: "Explicitly computing geodetic coordinates from Cartesian coordinates", EPS 65, 291-296 (2013)
      const double X = p.x();
      const double Y = p.y();
      const double Z = p.z();
      const double a = ell.a();
      const double e = ell.e();
      const double b = ell.b();
      
      if (std::abs(X) > 1/a or std::abs(Y) > 1/a) {
        const double DZ = sqrt(1-pow2(e))*Z;
        const double r = hypot(X, Y);
        const double e2p = (pow2(a) - pow2(b)) / pow2(b);
        const double F = 54 * pow2(b * Z);
        const double G = pow2(r) + pow2(DZ) - pow2(e) * (pow2(a) - pow2(b));
        const double c = pow4(e) * F * pow2(r) / pow3(G);
        const double s = cbrt(1 + c + sqrt(pow2(c) + 2*c));
        const double fP = F / (3 * pow2(G * (s + 1/s + 1)));
        const double Q = sqrt(1 + 2 * pow4(e) * fP);
        const double r0 = (-fP*pow2(e)*r)/(1+Q) + sqrt(0.5*pow2(a)*(1+1/Q) - fP*pow2(DZ)/(Q*(1+Q)) - 0.5*fP*pow2(r));
        const double U = hypot(r - pow2(e)*r0, Z);
        const double V = hypot(r - pow2(e)*r0, DZ);
        const double z0 = pow2(b)*Z / (a*V);
        pos[0] = U * (1 - pow2(b) / (a*V));;
        pos[1] = atan2d(Z + e2p*z0, r);
        pos[2] = atan2d(Y, X);
      } else if (std::abs(Z) < 1/b) {
        pos[0] = -a;
        pos[1] = 0;
        pos[2] = 180;
      } else {
        pos[0] = std::abs(Z)-b;
        pos[1] = Z < 0 ? -90 : 90;;
        pos[2] = 0;
      }
    } else {
      *this = Pos{Pos<PosType::Xyz>(p, ell), ell};
    }
  }
  
  friend std::ostream& operator<<(std::ostream& os, Pos p) {
    return os << p.time << ' '
              << p.pos[0] << ' '
              << p.pos[1] << ' '
              << p.pos[2];
  }
  
  friend std::istream& operator>>(std::istream& is, Pos& p) {
    return is >> p.time >> p.pos[0] >> p.pos[1] >> p.pos[2];
  }
  
  Pos (Pos a, Pos b, Ellipsoid ell) noexcept : time(a.time), pos({a.x() + b.x(), a.y() + b.y(), a.z() + b.z()}) {
    if (T == PosType::Xyz) {}
    else {
      *this = Pos<T>(Pos<PosType::Xyz>(Pos<PosType::Xyz>(a, ell), Pos<PosType::Xyz>(b, ell), ell), ell);
    }
  }
  
  void add_time(double dist, double v=Constant::speed_of_light) noexcept {
    time += TimeStep(std::abs(dist/v));
  }
  
  Altitude<AltitudeType::meter> altitude(Ellipsoid ell) const noexcept {
    return Altitude<AltitudeType::meter>(Pos<PosType::Ellipsoidal>(*this, ell).h());
  }
};  // Pos

ENUMCLASS(LosType, unsigned char,
  Xyz,
  Spherical
)

template <LosType T>
class Los {
  std::array<double, 3> los;
public:
  constexpr double dx() const noexcept {return los[0];}
  constexpr double dy() const noexcept {return los[1];}
  constexpr double dz() const noexcept {return los[2];}
  constexpr double za() const noexcept {return los[0];}
  constexpr double aa() const noexcept {return los[1];}
  constexpr double dr() const noexcept {return los[2];}
  std::array<double, 3> arr() const noexcept {return los;}
  
  constexpr Los() noexcept : los({0, 0, 0}) {}
  constexpr Los(Los&&) noexcept = default;
  constexpr Los(const Los&) noexcept = default;
  Los& operator=(Los&&) noexcept = default;
  Los& operator=(const Los&) noexcept = default;
  constexpr Los(std::array<double, 3> l) noexcept : los(l) {}
  
  template <LosType L, PosType P>
  Los(Los<L> l, Pos<P> p, Ellipsoid ell) noexcept : los(l.arr()) {
    using Conversion::cosd;
    using Conversion::sind;
    using Conversion::acosd;
    using Conversion::asind;
    using std::abs;
    
    if (T == L) {}
    else if (T == LosType::Spherical and L == LosType::Xyz and P == PosType::Spherical) {
      const auto norm = l.norm();
      
      const auto r = p.r();
      const auto slat = sind(p.lat());
      const auto clat = cosd(p.lat());
      const auto slon = sind(p.lon());
      const auto clon = cosd(p.lon());
      
      const auto dr = (clat*clon*l.dx() + slat*l.dz() + clat*slon*l.dy())/norm;
      const auto dlat = (-slat*clon*l.dx() + clat*l.dz() - slat*slon*l.dy())/(norm*r);
      const auto dlon = (-slon/clat*l.dx() + clon/clat*l.dy())/norm/r;
      
      los[0] = acosd(dr);
      los[1] = acosd(r*dlat / sind(los[0]));
      los[2] = norm;
      if(std::isnan(los[1])) {
        if(dlat >= 0) {
          los[1] = 0;
        } else {
          los[1] = 180;
        }
      } else if(dlon < 0) {
        los[1] = -los[1];
      }
    } else if (T == LosType::Xyz and L == LosType::Spherical and P == PosType::Spherical) {
      const auto norm = l.norm();
      const auto sza = sind(l.za());
      const auto cza = cosd(l.za());
      const auto saa = sind(l.aa());
      const auto caa = cosd(l.aa());
      if(abs(p.lat()) > 90 - 1e-4) {
        los[0] = norm * sza * caa;
        los[1] = norm * sza * saa;
        los[2] = norm * (p.lat() > 0 ? cza : -cza);
      }
      else { 
        const auto slat = sind(p.lat());
        const auto clat = cosd(p.lat());
        const auto slon = sind(p.lon());
        const auto clon = cosd(p.lon());
        
        const auto dr = cza;
        const auto dlat = sza*caa;
        const auto dlon = sza*saa;
        
        los[0] = norm * (clat*clon*dr - slat*clon*dlat - slon*dlon);
        los[1] = norm * (clat*slon*dr - slat*slon*dlat + clon*dlon);
        los[2] = norm * (slat*dr + clat*dlat);
      }
    } else {
      *this = Los(l, Pos<PosType::Spherical>(p, ell), ell);
    }
  }
  
  constexpr double norm2() const noexcept {
    if (T == LosType::Spherical)
      return Constant::pow2(dr());
    else if (T == LosType::Xyz)
      return Constant::pow2(dx()) + Constant::pow2(dy()) + Constant::pow2(dz());
  }
  double norm() const noexcept {return std::sqrt(norm2());}
  
  friend std::ostream& operator<<(std::ostream& os, Los l) {
    return os << l.los[0] << ' '
              << l.los[1] << ' '
              << l.los[2];
  }
  
  friend std::istream& operator>>(std::istream& is, Los& l) {
    return is >> l.los[0] >> l.los[1] >> l.los[2];
  }
  
  friend constexpr Los operator*(double x, Los l) {
    if (T == LosType::Xyz) {
      return Los({l.dx() * x, l.dy() * x, l.dz() * x});
    } else if (T == LosType::Spherical) {
      return Los({l.za(), l.aa(), l.dr() * x});
    }
  }
  
  operator Pos<PosType::Xyz>() const {
    if (T == LosType::Xyz) return Pos<PosType::Xyz>{los};
    else if (T == LosType::Spherical) throw std::runtime_error("not yet supported");
  }
  
  Los operator-() const noexcept {
    if (T == LosType::Xyz)
      return Los({-dx(), -dy(), -dz()});
    else if (T == LosType::Spherical)
      return Los({180-za(), 360-aa(), dr()});
  }
};

class Nav {
  enum class HitTarget : char {
    PlusX,
    MinusX,
    PlusY,
    MinusY,
    PlusZ,
    MinusZ
  };
  
  Pos<PosType::Xyz> pos;
  Los<LosType::Xyz> los;
  Ellipsoid ell;
  
  double plane_pxangle() const noexcept {return Conversion::acosd(+los.dx() / los.norm());}
  double plane_mxangle() const noexcept {return Conversion::acosd(-los.dx() / los.norm());}
  double plane_pyangle() const noexcept {return Conversion::acosd(+los.dy() / los.norm());}
  double plane_myangle() const noexcept {return Conversion::acosd(-los.dy() / los.norm());}
  double plane_pzangle() const noexcept {return Conversion::acosd(+los.dz() / los.norm());}
  double plane_mzangle() const noexcept {return Conversion::acosd(-los.dz() / los.norm());}
  
  template<HitTarget X> double snells_next_angel(double n_this, double n_next) {
    if (X == HitTarget::PlusX) {
      return Conversion::asind(n_this/n_next * Conversion::sind(plane_pxangle()));
    } else if (X == HitTarget::MinusX) {
      return Conversion::asind(n_this/n_next * Conversion::sind(plane_mxangle()));
    } else if (X == HitTarget::PlusY) {
      return Conversion::asind(n_this/n_next * Conversion::sind(plane_pyangle()));
    } else if (X == HitTarget::MinusY) {
      return Conversion::asind(n_this/n_next * Conversion::sind(plane_myangle()));
    } else if (X == HitTarget::PlusY) {
      return Conversion::asind(n_this/n_next * Conversion::sind(plane_pzangle()));
    } else if (X == HitTarget::MinusY) {
      return Conversion::asind(n_this/n_next * Conversion::sind(plane_mzangle()));
    }
  }
  
  enum class MovingTarget {
    ForwardInside,
    ForwardOutside,
    ForwardMiss,
    BackwardInside,
    BackwardOutside,
    BackwardMiss,
    CompleteMiss
  };
  
  struct LineEllipseIntersect {
    MovingTarget type;
    double min_step;
    double max_step;
  };
  
  LineEllipseIntersect line_ellipsoid_intersect(double alt, bool forward) const noexcept {
    using Constant::pow2;
    using Constant::pow4;
    using Num = std::numeric_limits<double>;
    
    const double x0 = pos.x();
    const double y0 = pos.y();
    const double z0 = pos.z();
    const double dx = los.dx();
    const double dy = los.dy();
    const double dz = los.dz();
    const double a = ell.a() + alt;
    const double b = ell.b() + alt;
    const double sqr = std::sqrt(pow2(a)*pow2(a*dz) + pow2(a*b*dx) + pow2(a*b*dy) - pow2(a*dx*z0) + 2*pow2(a)*dx*dz*x0*z0 - pow2(a*dy*z0) + 2*pow2(a)*dy*dz*y0*z0 - pow2(a*dz*x0) - pow2(a*dz*y0) - pow2(b*dx*y0) + 2*pow2(b)*dx*dy*x0*y0 - pow2(b*dy*x0));
    if (std::isnan(sqr)) {
      return {MovingTarget::CompleteMiss, Num::quiet_NaN(), Num::quiet_NaN()};
    } else {
      const double term = -pow2(a)*dz*z0 - pow2(b)*dx*x0 - pow2(b)*dy*y0;
      const double invden = 1/(pow2(a*dz) + pow2(b*dx) + pow2(b*dy));
      
      /* If we move forward we want the smallest value, and
       * if we move backward we want the largest value
       * 
       * Forward:  Smallest value is first intersection if outside
       * Backward:  Largest value is first intersection if outside
       * Forward:   Largest value is first intersection if inside
       * Backward: Smallest value is first intersection if inside
      */
      
      const double t0 = (term + b*sqr) * invden;
      const double t1 = (term - b*sqr) * invden;
      if (forward) {
        if (t0 >= 0 and t1 >= 0)
          return {MovingTarget::ForwardOutside, std::min(t0, t1), std::max(t0, t1)};
        if (t0 < 0 and t1 < 0)
          return {MovingTarget::ForwardMiss, -Num::max(), Num::max()};
        else
          return {MovingTarget::ForwardInside, std::min(t0, t1), std::max(t0, t1)};
      } else {
        if (t0 < 0 and t1 < 0)
          return {MovingTarget::BackwardOutside, std::min(t0, t1), std::max(t0, t1)};
        else if (t0 >= 0 and t1 >= 0)
          return {MovingTarget::BackwardMiss, -Num::max(), Num::max()};
        else
          return {MovingTarget::BackwardInside, std::min(t0, t1), std::max(t0, t1)};
      }
    }
  }
public:
  Nav() noexcept {}
  
  template <PosType P, LosType L>
  Nav(Pos<P> p0, Los<L> l0, Ellipsoid e) noexcept : pos(p0, e), los(l0, p0, e), ell(e) {}
  
  Nav(const Nav& old, Distance<DistanceType::meter> d) noexcept : pos(old.pos), los(old.los), ell(old.ell) {
    double dist = d.value();
    const auto intersect = old.line_ellipsoid_intersect(0, dist >= 0);
    if (intersect.type == MovingTarget::BackwardInside and dist < intersect.min_step) {
      dist = intersect.min_step;
    } else if (intersect.type == MovingTarget::BackwardOutside and dist < intersect.max_step) {
      dist = intersect.max_step;
    } else if (intersect.type == MovingTarget::ForwardInside and dist > intersect.min_step) {
      dist = intersect.min_step;
    } else if (intersect.type == MovingTarget::ForwardOutside and dist > intersect.max_step) {
      dist = intersect.max_step;
    }
    pos = Pos<PosType::Xyz>(old.pos, Pos<PosType::Xyz>(dist * old.los), old.ell);
    pos.add_time(dist);
  }
  
  Nav(const Nav& old, Altitude<AltitudeType::meter> alt) noexcept : pos(old.pos), los(old.los), ell(old.ell) {
    const auto intersect = old.line_ellipsoid_intersect(alt.value(), true);
    if (intersect.type == MovingTarget::CompleteMiss) {}
    else {
      const double dist = (std::abs(intersect.min_step) < std::abs(intersect.max_step)) ? intersect.min_step : intersect.max_step;
      if (not std::isnan(dist)) {
        pos = Pos<PosType::Xyz>(old.pos, Pos<PosType::Xyz>(dist * old.los), old.ell);
        pos.add_time(dist);
      }
    }
  }
  
  friend std::ostream& operator<<(std::ostream& os, Nav n) {return os << n.pos << ' ' << n.los << ' ' << n.ell;}
  
  friend std::istream& operator>>(std::istream& is, Nav& n) {return is >> n.pos >> n.los >> n.ell;}
};  // Nav

void readNav(File::File<File::Operation::Read, File::Type::Xml>& in, Nav& n);
void readNav(File::File<File::Operation::ReadBinary, File::Type::Xml>& in, Nav& n);
void saveNav(File::File<File::Operation::Write, File::Type::Xml>& out, const Nav& n);
void saveNav(File::File<File::Operation::Append, File::Type::Xml>& out, const Nav& n);
void saveNav(File::File<File::Operation::WriteBinary, File::Type::Xml>& out, const Nav& n);
void saveNav(File::File<File::Operation::AppendBinary, File::Type::Xml>& out, const Nav& n);
}  // Geom
#endif  // geom_h