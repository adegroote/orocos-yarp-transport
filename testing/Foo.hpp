#include <vector>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

class Foo {
public:
	int i;
	double d;
	std::string s;
	std::vector<double> v;
};

namespace boost {
namespace serialization {
	
template <class Archive>
void serialize(Archive& ar, Foo& foo, const unsigned int version) {
	ar & make_nvp("i", foo.i);
	ar & make_nvp("d", foo.d);
	ar & make_nvp("s", foo.s);
	ar & make_nvp("v", foo.v);
};

} // namespace serialization
} // namespace boost
