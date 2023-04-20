#include "crow/random-continuous-distributions.hpp"
#include "crow/random-discrete-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/random-other-distributions.hpp"
#include "crow/random-spatial-distributions.hpp"
#include "crow/unit-test.hpp"
#include "crow/uuid.hpp"
#include "crow/vector.hpp"
#include <random>
#include <string>

using namespace Crow;

void test_crow_random_concepts() {

    TEST(RandomEngineType<std::minstd_rand>);
    TEST(RandomEngineType<std::mt19937>);
    TEST(RandomEngineType<std::mt19937_64>);
    TEST(RandomEngineType<std::random_device>);
    TEST(RandomEngineType<Lcg32>);
    TEST(RandomEngineType<Lcg64>);
    TEST(RandomEngineType<Pcg64>);
    TEST(RandomEngineType<Xoshiro>);
    TEST(RandomEngineType<StdRng>);

    { using D = UniformInteger<int>;                          TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, int>));          }
    { using D = BernoulliDistribution;                        TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, bool>));         }
    { using D = DiscreteNormal<int>;                          TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, int>));          }
    { using D = PoissonDistribution<int>;                     TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, int>));          }
    { using D = UniformReal<float>;                           TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, float>));        }
    { using D = NormalDistribution<float>;                    TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, float>));        }
    { using D = LogUniform<float>;                            TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, float>));        }
    { using D = LogNormal<float>;                             TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, float>));        }
    { using D = ConstrainedDistribution<UniformReal<float>>;  TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, float>));        }
    { using D = RandomChoice<std::string>;                    TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, std::string>));  }
    { using D = WeightedChoice<std::string>;                  TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, std::string>));  }
    { using D = RandomUuid;                                   TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, Uuid>));         }
    { using D = RandomVector<float, 3>;                       TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, Float3>));       }
    { using D = SymmetricRandomVector<float, 3>;              TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, Float3>));       }
    { using D = RandomPointInSphere<float, 3>;                TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, Float3>));       }
    { using D = RandomDirection<float, 3>;                    TEST(RandomDistributionType<D>);  TEST((SpecificDistributionType<D, Float3>));       }

}
