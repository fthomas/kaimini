#ifndef BOOST_RANDOM_UNIFORM_IN_SPHERE_HPP
#define BOOST_RANDOM_UNIFORM_IN_SPHERE_HPP

#include <algorithm>    // std::transform
#include <cassert>
#include <cmath>        // std::pow, std::sqrt
#include <functional>   // std::bind2nd, std::multiplies
#include <vector>
#include <boost/random/detail/config.hpp>
#include <boost/random/normal_distribution.hpp>

namespace boost {

template<class RealType = double, class Cont = std::vector<RealType> >
class uniform_in_sphere
{
public:
  typedef RealType input_type;
  typedef Cont result_type;

  explicit uniform_in_sphere(int dim = 2, RealType radius = 1.)
    : _container(dim), _dim(dim), _radius(radius)
  {
    assert(radius > 0.);
  }

  // compiler-generated copy ctor and assignment operator are fine

  void reset() { _normal.reset(); }

  template<class Engine>
  const result_type& operator()(Engine& eng)
  {
    RealType length = 0.;
    for (typename Cont::iterator it = _container.begin();
         it != _container.end(); ++it)
    {
      const RealType val = _normal(eng);
      *it = val;
      length += val * val;
    }
#ifndef BOOST_NO_STDC_NAMESPACE
    using std::pow;
    using std::sqrt;
#endif
    length = sqrt(length);
    const RealType scale = pow(pow(_radius, _dim) * eng(), 1./_dim) / length;

    // for all i: _container[i] *= scale
    std::transform(_container.begin(), _container.end(), _container.begin(),
                   std::bind2nd(std::multiplies<RealType>(), scale));
    return _container;
  }

#ifndef BOOST_RANDOM_NO_STREAM_OPERATORS
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const uniform_in_sphere& sd)
  {
    os << sd._dim << " " << sd._radius;
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, uniform_in_sphere& sd)
  {
    is >> std::ws >> sd._dim >> std::ws >> sd._radius;
    sd._container.resize(sd._dim);
    return is;
  }
#endif

private:
  normal_distribution<RealType> _normal;
  result_type _container;
  int _dim;
  RealType _radius;
};

} // namespace boost

#endif // BOOST_RANDOM_UNIFORM_IN_SPHERE_HPP
