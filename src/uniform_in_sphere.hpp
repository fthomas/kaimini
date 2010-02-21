#ifndef BOOST_RANDOM_UNIFORM_IN_SPHERE_HPP
#define BOOST_RANDOM_UNIFORM_IN_SPHERE_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
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

private:
  normal_distribution<RealType> _normal;
  result_type _container;
  int _dim;
  RealType _radius;
};

} // namespace boost

#endif // BOOST_RANDOM_UNIFORM_IN_SPHERE_HPP
