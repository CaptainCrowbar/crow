#pragma once

#include "crow/algorithm.hpp"
#include "crow/enum.hpp"
#include "crow/maths.hpp"
#include "crow/matrix.hpp"
#include "crow/root-finding.hpp"
#include "crow/transform.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <map>
#include <memory>
#include <numbers>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    // Forward declarations

    class MapProjection;
        template <typename T> class BasicMapProjection;
            template <typename T> class AzimuthalProjection;
                template <typename T> class AzimuthalEquidistantProjection;
                template <typename T> class GnomonicProjection;
                template <typename T> class LambertAzimuthalProjection;
                template <typename T> class OrthographicProjection;
                template <typename T> class StereographicProjection;
            template <typename T> class CylindricalProjection;
                template <typename T> class CylindricalEquidistantProjection;
                template <typename T> class LambertCylindricalProjection;
                template <typename T> class GallPetersProjection;
                template <typename T> class MercatorProjection;
            template <typename T> class PseudocylindricalProjection;
                template <typename T> class Eckert4Projection;
                template <typename T> class MollweideProjection;
                template <typename T> class SinusoidalProjection;
                template <typename T> class InterruptedProjectionBase;
                    template <typename Projection> class InterruptedProjection;

    // Constants

    enum class Maps: uint32_t {
        none               = 0,
        // Projection family
        azimuthal          = 1ul << 0,   // Azimuthal projections
        pseudoazimuthal    = 1ul << 1,   // Pseudo-Azimuthal projections
        conic              = 1ul << 2,   // Conic projections
        pseudoconic        = 1ul << 3,   // Pseudo-conic projections
        cylindrical        = 1ul << 4,   // Cylindrical projections
        pseudocylindrical  = 1ul << 5,   // Pseudo-cylindrical projections
        hybrid             = 1ul << 6,   // Hybrid projections
        // Coverage of the globe
        sub_hemisphere     = 1ul << 8,   // Less than one hemisphere can be represented
        hemisphere         = 1ul << 9,   // One hemisphere can be represented
        sub_sphere         = 1ul << 10,  // More than one hemisphere, but less than the globe, can be represented
        sphere             = 1ul << 11,  // The complete globe can be represented
        // Shape of the map
        circle             = 1ul << 16,  // The globe maps to a circle
        ellipse            = 1ul << 17,  // The globe maps to an ellipse
        plane              = 1ul << 18,  // The globe maps to an infinite plane
        rectangle          = 1ul << 19,  // The globe maps to a rectangle
        square             = 1ul << 20,  // The globe maps to a square
        other_shape        = 1ul << 21,  // The globe maps to some other shape
        // Other properties
        conformal          = 1ul << 24,  // Local shape is preserved
        equal_area         = 1ul << 25,  // Area is preserved
        hemisphere_circle  = 1ul << 26,  // Hemisphere around the origin maps to a circle
        interrupted        = 1ul << 27,  // Interrupted projections
        numerical          = 1ul << 28,  // No analytic solution for globe_to_map()
        // Combined masks
        family_mask        = 0xff,
        cover_mask         = 0xff00,
        shape_mask         = 0xff0000,
        other_mask         = 0xff000000,
    };

    CROW_BITMASK_OPERATORS(Maps)

    std::string to_string(Maps m);

    inline std::string to_string(Maps m) {

        static const std::vector<std::pair<Maps, std::string>> all_flags = {

            { Maps::azimuthal,          "azimuthal"          },
            { Maps::pseudoazimuthal,    "pseudoazimuthal"    },
            { Maps::conic,              "conic"              },
            { Maps::pseudoconic,        "pseudoconic"        },
            { Maps::cylindrical,        "cylindrical"        },
            { Maps::pseudocylindrical,  "pseudocylindrical"  },
            { Maps::hybrid,             "hybrid"             },
            { Maps::sub_hemisphere,     "sub-hemisphere"     },
            { Maps::hemisphere,         "hemisphere"         },
            { Maps::sub_sphere,         "sub-sphere"         },
            { Maps::sphere,             "sphere"             },
            { Maps::circle,             "circle"             },
            { Maps::ellipse,            "ellipse"            },
            { Maps::plane,              "plane"              },
            { Maps::rectangle,          "rectangle"          },
            { Maps::square,             "square"             },
            { Maps::other_shape,        "other-shape"        },
            { Maps::conformal,          "conformal"          },
            { Maps::equal_area,         "equal-area"         },
            { Maps::hemisphere_circle,  "hemisphere-circle"  },
            { Maps::interrupted,        "interrupted"        },
            { Maps::numerical,          "numerical"          },

        };

        std::string list;
        for (auto& [flag, str]: all_flags)
            if (!! (m & flag))
                list += str + ',';
        if (list.empty())
            list = "none";
        else
            list.pop_back();

        return list;

    }

    inline std::ostream& operator<<(std::ostream& out, Maps m) {
        return out << to_string(m);
    }

    namespace Detail {

        // Convert spherical surface (phi,theta) coordinates so the reference
        // point is at (0,pi/2) (= lat/long zero), and reverse this
        // transformation.

        template <typename T>
        class PolarReduce {
        private:
            using V2 = Vector<T, 2>;
            using V3 = Vector<T, 3>;
            using M3 = Matrix<T, 3>;
        public:
            PolarReduce() = default;
            explicit PolarReduce(V2 ref_point) noexcept {
                ref_[0] = euclidean_remainder(ref_point[0], 2 * SN::pi_v<T>);
                ref_[1] = std::clamp(ref_point[1], T(0), SN::pi_v<T>);
                M3 to_meridian = rotate3(- ref_[0], 2);
                M3 to_equator = rotate3(SN::pi_v<T> / 2 - ref_[1], 1);
                mat_ = to_equator * to_meridian;
                M3 from_equator = rotate3(ref_[1] - SN::pi_v<T> / 2, 1);
                M3 from_meridian = rotate3(ref_[0], 2);
                inv_ = from_meridian * from_equator;
            }
            V2 reduce_to_polar(V2 polar) const noexcept {
                V3 xyz = reduce_to_xyz(polar);
                V3 sph = cartesian_to_spherical(xyz);
                return {sph[1], sph[2]};
            }
            V2 reduce_to_xy(V2 polar) const noexcept {
                V3 xyz = reduce_to_xyz(polar);
                return {xyz.y(), xyz.z()};
            }
            V2 inverse_from_polar(V2 polar) const noexcept {
                V3 sph = {T(1), polar[0], polar[1]};
                V3 xyz = spherical_to_cartesian(sph);
                return inverse_from_xyz(xyz);
            }
            V2 inverse_from_xy(V2 xy) const noexcept {
                V3 xyz = {T(1), xy.z(), xy.y()};
                return inverse_from_xyz(xyz);
            }
            V2 reference() const noexcept { return ref_; }
        private:
            M3 mat_ = M3::identity();
            M3 inv_ = M3::identity();
            V2 ref_ = {0, SN::pi_v<T> / 2};
            V3 reduce_to_xyz(V2 polar) const noexcept {
                V3 sph = {T(1), polar[0], polar[1]};
                V3 xyz = spherical_to_cartesian(sph);
                return mat_ * xyz;
            }
            V2 inverse_from_xyz(V3 xyz) const noexcept {
                V3 xyz2 = inv_ * xyz;
                V3 sph = cartesian_to_spherical(xyz2);
                return {sph[1], sph[2]};
            }
        };

    }

    // Untyped abstract base class

    class MapProjection {
    public:
        virtual ~MapProjection() noexcept {}
        virtual std::string name() const = 0;
        virtual Maps properties() const noexcept = 0;
        Maps family() const noexcept { return properties() & Maps::family_mask; }
        Maps cover() const noexcept { return properties() & Maps::cover_mask; }
        Maps shape() const noexcept { return properties() & Maps::shape_mask; }
    protected:
        MapProjection() noexcept {}
    };

    // Typed abstract base class

    template <typename T>
    class BasicMapProjection:
    public MapProjection {
    public:
        static_assert(std::is_floating_point_v<T>);
        using scalar_type = T;
        using vector_type = Vector<T, 2>;
        static constexpr vector_type default_origin = {0, std::numbers::pi_v<T> / 2};
        virtual std::shared_ptr<BasicMapProjection> clone() const = 0;
        virtual bool has_min_x() const noexcept { return has_max_x(); }
        virtual bool has_max_x() const noexcept { return ! (properties() & Maps::plane); }
        virtual bool has_min_y() const noexcept { return has_max_y(); }
        virtual bool has_max_y() const noexcept { return ! (properties() & Maps::plane); }
        virtual T min_x() const noexcept { return - max_x(); }
        virtual T max_x() const noexcept { return 0; }
        virtual T min_y() const noexcept { return - max_y(); }
        virtual T max_y() const noexcept { return 0; }
        vector_type globe_to_map(vector_type polar) const noexcept;
        vector_type map_to_globe(vector_type xy) const noexcept;
        bool is_on_globe(vector_type polar) const noexcept;
        bool is_on_map(vector_type xy) const noexcept;
        vector_type origin() const noexcept { return offset_.reference(); }
    protected:
        explicit BasicMapProjection(vector_type origin) noexcept: offset_(origin) {}
        virtual bool canonical_on_globe(vector_type polar) const noexcept;
        virtual bool canonical_on_map(vector_type xy) const noexcept;
        virtual vector_type canonical_to_globe(vector_type xy) const noexcept = 0;
        virtual vector_type canonical_to_map(vector_type polar) const noexcept = 0;
        T angle_from_origin(vector_type polar) const noexcept;
    private:
        using polar_reduce = Detail::PolarReduce<T>;
        polar_reduce offset_;
    };

    template <typename T>
    Vector<T, 2> BasicMapProjection<T>::globe_to_map(Vector<T, 2> polar) const noexcept {
        auto rel_polar = offset_.reduce_to_polar(polar);
        return canonical_to_map(rel_polar);
    }

    template <typename T>
    Vector<T, 2> BasicMapProjection<T>::map_to_globe(Vector<T, 2> xy) const noexcept {
        auto rel_polar = canonical_to_globe(xy);
        return offset_.inverse_from_polar(rel_polar);
    }

    template <typename T>
    bool BasicMapProjection<T>::is_on_globe(Vector<T, 2> polar) const noexcept {
        auto rel_polar = offset_.reduce_to_polar(polar);
        return canonical_on_globe(rel_polar);
    }

    template <typename T>
    bool BasicMapProjection<T>::is_on_map(Vector<T, 2> xy) const noexcept {
        return canonical_on_map(xy);
    }

    template <typename T>
    bool BasicMapProjection<T>::canonical_on_globe(Vector<T, 2> polar) const noexcept{
        using namespace std::numbers;
        switch (cover()) {
            case Maps::sub_hemisphere:  return angle_from_origin(polar) < pi_v<T>;
            case Maps::hemisphere:      return angle_from_origin(polar) <= pi_v<T>;
            case Maps::sub_sphere:      return angle_from_origin(polar) < 2 * pi_v<T>;
            default:                   return true;
        }
    }

    template <typename T>
    bool BasicMapProjection<T>::canonical_on_map(Vector<T, 2> xy) const noexcept {
        T a, b;
        switch (properties() & Maps::shape_mask) {
            case Maps::none:
                return std::abs(xy.x()) <= max_x();
            case Maps::circle:
            case Maps::ellipse:
                a = xy.x() / max_x();
                b = xy.y() / max_y();
                return a * a + b * b <= 1;
            case Maps::rectangle:
                return std::abs(xy.x()) <= max_x() && std::abs(xy.y()) <= max_y();
            default:
                return true;
        }
    }

    template <typename T>
    T BasicMapProjection<T>::angle_from_origin(Vector<T, 2> polar) const noexcept {
        using V3 = Vector<T, 3>;
        V3 sph = {T(1), polar[0], polar[1]};
        auto xyz = spherical_to_cartesian(sph);
        return V3::unit(0).angle(xyz);
    }

    // Intermediate abstract base classes

    template <typename T>
    class AzimuthalProjection:
    public BasicMapProjection<T> {
    protected:
        explicit AzimuthalProjection(Vector<T, 2> origin) noexcept: BasicMapProjection<T>(origin) {}
    };

    template <typename T>
    class CylindricalProjection:
    public BasicMapProjection<T> {
    protected:
        explicit CylindricalProjection(Vector<T, 2> origin) noexcept: BasicMapProjection<T>(origin) {}
    };

    template <typename T>
    class PseudocylindricalProjection:
    public BasicMapProjection<T> {
    protected:
        template <typename Projection> friend class InterruptedProjection;
        explicit PseudocylindricalProjection(Vector<T, 2> origin) noexcept: BasicMapProjection<T>(origin) {}
    };

    // Azimuthal projection classes

    template <typename T>
    class AzimuthalEquidistantProjection:
    public AzimuthalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::azimuthal | Maps::sphere | Maps::circle | Maps::hemisphere_circle;
        AzimuthalEquidistantProjection() noexcept: AzimuthalEquidistantProjection(BasicMapProjection<T>::default_origin) {}
        explicit AzimuthalEquidistantProjection(Vector<T, 2> origin) noexcept: AzimuthalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<AzimuthalEquidistantProjection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return std::numbers::pi_v<T>; }
        virtual std::string name() const override { return "azimuthal equidistant projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };


    template <typename T>
    bool AzimuthalEquidistantProjection<T>::canonical_on_map(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        return pow(xy.x(), T(2)) + pow(xy.y(), T(2)) <= pi_v<T> * pi_v<T>;
    }

    template <typename T>
    Vector<T, 2> AzimuthalEquidistantProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        if (x == 0 && y == 0)
            return this->default_origin;
        T c = std::clamp(std::hypot(x, y), T(0), pi_v<T>);
        T sin_c = std::sin(c);
        T cos_c = std::cos(c);
        T theta = std::acos(y * sin_c / c);
        T phi = std::atan2(x * sin_c, c * cos_c);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> AzimuthalEquidistantProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T sin_phi = std::sin(phi);
        T cos_phi = std::cos(phi);
        T sin_theta = std::sin(theta);
        T cos_theta = std::cos(theta);
        T c = std::acos(sin_theta * cos_phi);
        T k = 0;
        if (c > 0)
            k = c / std::sin(c);
        T x = k * sin_theta * sin_phi;
        T y = k * cos_theta;
        return {x, y};
    }

    template <typename T>
    class GnomonicProjection:
    public AzimuthalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::azimuthal | Maps::sub_hemisphere | Maps::plane;
        GnomonicProjection() noexcept: GnomonicProjection(BasicMapProjection<T>::default_origin) {}
        explicit GnomonicProjection(Vector<T, 2> origin) noexcept: AzimuthalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<GnomonicProjection>(*this); }
        virtual std::string name() const override { return "gnomonic projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> polar) const noexcept override;
        virtual bool canonical_on_map(Vector<T, 2> /*xy*/) const noexcept override { return true; }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool GnomonicProjection<T>::canonical_on_globe(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        return this->angle_from_origin(polar) < pi_v<T>;
    }

    template <typename T>
    Vector<T, 2> GnomonicProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        if (x == 0 && y == 0)
            return this->default_origin;
        T rho = std::hypot(x, y);
        T c = std::atan(rho);
        T cos_c = std::cos(c);
        T sin_c = std::sin(c);
        T theta = std::acos(y * sin_c / rho);
        T phi = std::atan2(x * sin_c, rho * cos_c);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> GnomonicProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T sin_phi = std::sin(phi);
        T cos_phi = std::cos(phi);
        T sin_theta = std::sin(theta);
        T cos_theta = std::cos(theta);
        T cos_c = sin_theta * cos_phi;
        T x = sin_theta * sin_phi / cos_c;
        T y = cos_theta / cos_c;
        return {x, y};
    }

    template <typename T>
    class LambertAzimuthalProjection:
    public AzimuthalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::azimuthal | Maps::sphere | Maps::circle | Maps::equal_area | Maps::hemisphere_circle;
        LambertAzimuthalProjection() noexcept: LambertAzimuthalProjection(BasicMapProjection<T>::default_origin) {}
        explicit LambertAzimuthalProjection(Vector<T, 2> origin) noexcept: AzimuthalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<LambertAzimuthalProjection>(*this); }
        virtual T max_x() const noexcept override { return T(2); }
        virtual T max_y() const noexcept override { return T(2); }
        virtual std::string name() const override { return "Lambert azimuthal projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override { return pow(xy.x(), T(2)) + pow(xy.y(), T(2)) <= T(4); }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    Vector<T, 2> LambertAzimuthalProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        if (x == 0 && y == 0)
            return this->default_origin;
        T rho = std::clamp(std::hypot(x, y), T(0), T(2));
        T c = 2 * std::asin(rho / 2);
        T sin_c = std::sin(c);
        T cos_c = std::cos(c);
        T theta = std::acos(y * sin_c / rho);
        T phi = std::atan2(x * sin_c, rho * cos_c);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> LambertAzimuthalProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T sin_phi = std::sin(phi);
        T cos_phi = std::cos(phi);
        T sin_theta = std::sin(theta);
        T cos_theta = std::cos(theta);
        T divisor = 1 + sin_theta * cos_phi;
        if (divisor <= 0)
            return {0, 0};
        T k = 2 / std::sqrt(divisor);
        T x = k * sin_theta * sin_phi;
        T y = k * cos_theta;
        return {x, y};
    }

    template <typename T>
    class OrthographicProjection:
    public AzimuthalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::azimuthal | Maps::hemisphere | Maps::circle | Maps::hemisphere_circle;
        OrthographicProjection() noexcept: OrthographicProjection(BasicMapProjection<T>::default_origin) {}
        explicit OrthographicProjection(Vector<T, 2> origin) noexcept: AzimuthalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<OrthographicProjection>(*this); }
        virtual T max_x() const noexcept override { return T(1); }
        virtual T max_y() const noexcept override { return T(1); }
        virtual std::string name() const override { return "orthographic projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> polar) const noexcept override;
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override { return pow(xy.x(), T(2)) + pow(xy.y(), T(2)) <= 1; }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool OrthographicProjection<T>::canonical_on_globe(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        return this->angle_from_origin(polar) <= pi_v<T>;
    }

    template <typename T>
    Vector<T, 2> OrthographicProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        T cos_c = std::sqrt(std::clamp(1 - x * x - y * y, T(0), T(1)));
        T theta = std::acos(y);
        T phi = std::atan2(x, cos_c);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> OrthographicProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T sin_phi = std::sin(phi);
        T sin_theta = std::sin(theta);
        T cos_theta = std::cos(theta);
        T x = sin_theta * sin_phi;
        T y = cos_theta;
        return {x, y};
    }

    template <typename T>
    class StereographicProjection:
    public AzimuthalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::azimuthal | Maps::sub_sphere | Maps::plane | Maps::conformal | Maps::hemisphere_circle;
        StereographicProjection() noexcept: StereographicProjection(BasicMapProjection<T>::default_origin) {}
        explicit StereographicProjection(Vector<T, 2> origin) noexcept: AzimuthalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<StereographicProjection>(*this); }
        virtual std::string name() const override { return "stereographic projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> polar) const noexcept override;
        virtual bool canonical_on_map(Vector<T, 2> /*xy*/) const noexcept override { return true; }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool StereographicProjection<T>::canonical_on_globe(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        return this->angle_from_origin(polar) < 2 * pi_v<T>;
    }

    template <typename T>
    Vector<T, 2> StereographicProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        if (x == 0 && y == 0)
            return this->default_origin;
        T rho = std::hypot(x, y);
        T c = 2 * std::atan(rho / 2);
        T cos_c = std::cos(c);
        T sin_c = std::sin(c);
        T theta = std::acos(y * sin_c / rho);
        T phi = std::atan2(x * sin_c, rho * cos_c);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> StereographicProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T sin_phi = std::sin(phi);
        T cos_phi = std::cos(phi);
        T sin_theta = std::sin(theta);
        T cos_theta = std::cos(theta);
        T k = 2 / (1 + sin_theta * cos_phi);
        T x = k * sin_theta * sin_phi;
        T y = k * cos_theta;
        return {x, y};
    }

    // Cylindrical projection classes

    template <typename T>
    class CylindricalEquidistantProjection:
    public CylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::cylindrical | Maps::sphere | Maps::rectangle;
        CylindricalEquidistantProjection() noexcept: CylindricalEquidistantProjection(BasicMapProjection<T>::default_origin) {}
        explicit CylindricalEquidistantProjection(Vector<T, 2> origin) noexcept: CylindricalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<CylindricalEquidistantProjection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return std::numbers::pi_v<T> / 2; }
        virtual std::string name() const override { return "cylindrical equidistant projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override { return abs(xy.x()) <= max_x() && abs(xy.y()) <= max_y(); }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    Vector<T, 2> CylindricalEquidistantProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = std::clamp(xy.y(), - pi_v<T> / 2, pi_v<T> / 2);
        T phi = euclidean_remainder(x, 2 * pi_v<T>);
        T theta = pi_v<T> / 2 - y;
        return {phi, theta};
    }

    template <typename T>
    Vector<T, 2> CylindricalEquidistantProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T x = symmetric_remainder(phi, 2 * pi_v<T>);
        T y = pi_v<T> / 2 - theta;
        return {x, y};
    }

    template <typename T>
    class LambertCylindricalProjection:
    public CylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::cylindrical | Maps::sphere | Maps::rectangle | Maps::equal_area;
        LambertCylindricalProjection() noexcept: LambertCylindricalProjection(BasicMapProjection<T>::default_origin) {}
        explicit LambertCylindricalProjection(Vector<T, 2> origin) noexcept: CylindricalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<LambertCylindricalProjection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return T(1); }
        virtual std::string name() const override { return "Lambert cylindrical projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        friend class GallPetersProjection<T>;
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override { return abs(xy.x()) <= max_x() && abs(xy.y()) <= max_y(); }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    Vector<T, 2> LambertCylindricalProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = std::clamp(xy.y(), T(-1), T(1));
        T phi = euclidean_remainder(x, 2 * pi_v<T>);
        T theta = std::acos(y);
        return {phi, theta};
    }

    template <typename T>
    Vector<T, 2> LambertCylindricalProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T x = symmetric_remainder(phi, 2 * pi_v<T>);
        T y = std::cos(theta);
        return {x, y};
    }

    template <typename T>
    class GallPetersProjection:
    public CylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = LambertCylindricalProjection<T>::map_properties;
        GallPetersProjection() noexcept: GallPetersProjection(BasicMapProjection<T>::default_origin) {}
        explicit GallPetersProjection(Vector<T, 2> origin) noexcept: CylindricalProjection<T>(origin), lambert_(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<GallPetersProjection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return T(2); }
        virtual std::string name() const override { return "Gall-Peters projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override { return abs(xy.x()) <= max_x() && abs(xy.y()) <= max_y(); }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    private:
        LambertCylindricalProjection<T> lambert_;
    };

    template <typename T>
    Vector<T, 2> GallPetersProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        return lambert_.canonical_to_globe(Vector<T, 2>(xy.x(), xy.y() / 2));
    }

    template <typename T>
    Vector<T, 2> GallPetersProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        auto xy = lambert_.canonical_to_map(polar);
        xy.y() *= 2;
        return xy;
    }

    template <typename T>
    class MercatorProjection:
    public CylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::cylindrical | Maps::sub_sphere | Maps::other_shape | Maps::conformal;
        MercatorProjection() noexcept: MercatorProjection(BasicMapProjection<T>::default_origin) {}
        explicit MercatorProjection(Vector<T, 2> origin) noexcept: CylindricalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<MercatorProjection>(*this); }
        virtual bool has_max_x() const noexcept override { return true; }
        virtual bool has_max_y() const noexcept override { return false; }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual std::string name() const override { return "Mercator projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> polar) const noexcept override;
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override { return abs(xy.x()) <= max_x(); }
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool MercatorProjection<T>::canonical_on_globe(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        return polar[1] > 0 && polar[1] < pi_v<T>;
    }

    template <typename T>
    Vector<T, 2> MercatorProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        T phi = euclidean_remainder(x, 2 * pi_v<T>);
        T theta = pi_v<T> - 2 * std::atan(std::exp(y));
        return {phi, theta};
    }

    template <typename T>
    Vector<T, 2> MercatorProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T x = symmetric_remainder(phi, 2 * pi_v<T>);
        T y = std::log(std::tan((pi_v<T> - theta) / 2));
        return {x, y};
    }

    // Pseudo-cylindrical projection classes

    template <typename T>
    class Eckert4Projection:
    public PseudocylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area | Maps::numerical;
        Eckert4Projection() noexcept: Eckert4Projection(BasicMapProjection<T>::default_origin) {}
        explicit Eckert4Projection(Vector<T, 2> origin) noexcept: PseudocylindricalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<Eckert4Projection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return std::numbers::pi_v<T> / 2; }
        virtual std::string name() const override { return "Eckert IV projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool Eckert4Projection<T>::canonical_on_map(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        auto abs_x = std::abs(xy.x());
        if (abs_x <= pi_v<T> / 2)
            return std::abs(xy.y()) <= pi_v<T> / 2;
        else if (abs_x <= pi_v<T>)
            return std::pow(2 * abs_x / pi_v<T> - 1, T(2)) + std::pow(2 * xy.y() / pi_v<T>, T(2)) <= 1;
        else
            return false;
    }

    template <typename T>
    Vector<T, 2> Eckert4Projection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = std::clamp(xy.y(), - pi_v<T> / 2, pi_v<T> / 2);
        T sin_t = - 2 * y / pi_v<T>;
        T t = std::asin(sin_t);
        T cos_t = std::sqrt(1 - sin_t * sin_t);
        T theta = std::acos(- (t + sin_t * cos_t + 2 * sin_t) / (pi_v<T> / 2 + 2));
        T phi = 2 * x / (1 + cos_t);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> Eckert4Projection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T c = (2 + pi_v<T> / 2) * std::cos(theta);
        T t = newton_raphson<T>(
            [c] (T x) { return x + std::sin(x) * (2 + std::cos(x)) - c; },
            [] (T x) { return 2 * std::cos(x) * (1 + std::cos(x));
        })->solve();
        T x = symmetric_remainder(phi, 2 * pi_v<T>) * (1 + std::cos(t)) / 2;
        T y = pi_v<T> * std::sin(t) / 2;
        return {x, y};
    }

    template <typename T>
    class MollweideProjection:
    public PseudocylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::pseudocylindrical | Maps::sphere | Maps::ellipse | Maps::equal_area
            | Maps::hemisphere_circle | Maps::numerical;
        MollweideProjection() noexcept: MollweideProjection(BasicMapProjection<T>::default_origin) {}
        explicit MollweideProjection(Vector<T, 2> origin) noexcept: PseudocylindricalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<MollweideProjection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return std::numbers::pi_v<T> / 2; }
        virtual std::string name() const override { return "Mollweide projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool MollweideProjection<T>::canonical_on_map(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        return x * x + 4 * y * y <= pi_v<T> * pi_v<T>;
    }

    template <typename T>
    Vector<T, 2> MollweideProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        if (x == 0 && y == 0)
            return this->default_origin;
        y = std::clamp(y, - pi_v<T> / 2, pi_v<T> / 2);
        T t = - std::asin(2 * y / pi_v<T>);
        T theta = std::acos(- (2 * t + std::sin(2 * t)) / pi_v<T>);
        T phi = x / std::cos(t);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> MollweideProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T cos_theta = std::cos(theta);
        T t = newton_raphson<T>(
            [cos_theta] (T x) { return 2 * x + std::sin(2 * x) - pi_v<T> * cos_theta; },
            [] (T x) { return 2 + 2 * std::cos(2 * x);
        })->solve();
        T x = symmetric_remainder(phi, 2 * pi_v<T>) * std::cos(t);
        T y = pi_v<T> * std::sin(t) / 2;
        return {x, y};
    }

    template <typename T>
    class SinusoidalProjection:
    public PseudocylindricalProjection<T> {
    public:
        static constexpr Maps map_properties = Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area;
        SinusoidalProjection() noexcept: SinusoidalProjection(BasicMapProjection<T>::default_origin) {}
        explicit SinusoidalProjection(Vector<T, 2> origin) noexcept: PseudocylindricalProjection<T>(origin) {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<SinusoidalProjection>(*this); }
        virtual T max_x() const noexcept override { return std::numbers::pi_v<T>; }
        virtual T max_y() const noexcept override { return std::numbers::pi_v<T> / 2; }
        virtual std::string name() const override { return "sinusoidal projection"; }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(Vector<T, 2> /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_globe(Vector<T, 2> xy) const noexcept override;
        virtual Vector<T, 2> canonical_to_map(Vector<T, 2> polar) const noexcept override;
    };

    template <typename T>
    bool SinusoidalProjection<T>::canonical_on_map(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        return std::abs(xy.x()) <= pi_v<T> * std::cos(xy.y());
    }

    template <typename T>
    Vector<T, 2> SinusoidalProjection<T>::canonical_to_globe(Vector<T, 2> xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = xy.y();
        if (x == 0 && y == 0)
            return this->default_origin;
        y = std::clamp(y, - pi_v<T> / 2, pi_v<T> / 2);
        T theta = pi_v<T> / 2 - y;
        T phi = x / std::cos(y);
        return {euclidean_remainder(phi, 2 * pi_v<T>), theta};
    }

    template <typename T>
    Vector<T, 2> SinusoidalProjection<T>::canonical_to_map(Vector<T, 2> polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T x = symmetric_remainder(phi * std::sin(theta), 2 * pi_v<T>);
        T y = pi_v<T> / 2 - theta;
        return {x, y};
    }

    // Interrupted projection classes

    template <typename T>
    class InterruptedProjectionBase:
    public PseudocylindricalProjection<T> {
    public:
        template <typename Range> void interrupt(const Range& inter) { interrupt(inter, inter); }
        template <typename Range> void interrupt(const Range& inter_north, const Range& inter_south);
    protected:
        using coord_list = std::vector<T>;
        struct segment_type { T begin, centre, delta, end; };
        template <typename Range> InterruptedProjectionBase(Vector<T, 2> origin, const Range& inter_north, const Range& inter_south):
            PseudocylindricalProjection<T>(origin), segments_() { interrupt(inter_north, inter_south); }
        const segment_type& find_segment(T x, bool south) const noexcept;
    private:
        using segment_map = std::map<T, segment_type>;
        segment_map segments_[2];
    };

    template <typename T>
    template <typename Range>
    void InterruptedProjectionBase<T>::interrupt(const Range& inter_north, const Range& inter_south) {
        using namespace std::numbers;
        const Range* ptrs[] = {&inter_north, &inter_south};
        segment_map new_segments[2];
        for (int i = 0; i < 2; ++i) {
            new_segments[i].clear();
            coord_list inter(begin(*ptrs[i]), end(*ptrs[i]));
            for (auto& t: inter)
                t = symmetric_remainder(t, 2 * pi_v<T>);
            sort_unique_in(inter);
            std::erase_if(inter, [] (T t) { return t <= - pi_v<T> || t >= pi_v<T>; });
            inter.insert(inter.begin(), - pi_v<T>);
            inter.push_back(pi_v<T>);
            auto second = std::next(inter.begin());
            auto last = std::prev(inter.end());
            for (auto j = inter.begin(), k = second; j != last; ++j, ++k)
                new_segments[i][*j] = {*j, (*j + *k) / 2, (*k - *j) / 2, *k};
        }
        for (int i = 0; i < 2; ++i)
            segments_[i].swap(new_segments[i]);
    }

    template <typename T>
    const typename InterruptedProjectionBase<T>::segment_type&
    InterruptedProjectionBase<T>::find_segment(T x, bool south) const noexcept {
        auto& map = segments_[int(south)];
        auto s = map.upper_bound(x);
        if (s != map.begin())
            --s;
        return s->second;
    }

    template <typename Projection>
    class InterruptedProjection:
    public InterruptedProjectionBase<typename Projection::scalar_type> {
    private:
        static_assert(std::is_base_of_v<PseudocylindricalProjection<typename Projection::scalar_type>, Projection>);
        static_assert((Projection::map_properties & Maps::family_mask) == Maps::pseudocylindrical);
        using T = typename Projection::scalar_type;
        using base_type = InterruptedProjectionBase<T>;
        using coord_list = typename base_type::coord_list;
    public:
        using projection_type = Projection;
        using vector_type = Vector<T, 2>;
        static constexpr Maps map_properties =
            (Projection::map_properties & ~ Maps::shape_mask & ~ Maps::hemisphere_circle) | Maps::other_shape | Maps::interrupted;
        InterruptedProjection():
            base_type(BasicMapProjection<T>::default_origin, coord_list(), coord_list()), proj_() {}
        template <typename Range> InterruptedProjection(vector_type origin, const Range& inter):
            base_type(origin, inter, inter), proj_() {}
        template <typename Range> InterruptedProjection(vector_type origin, const Range& inter_north, const Range& inter_south):
            base_type(origin, inter_north, inter_south), proj_() {}
        virtual std::shared_ptr<BasicMapProjection<T>> clone() const override
            { return std::make_shared<InterruptedProjection>(*this); }
        virtual T max_x() const noexcept override { return proj_.max_x(); }
        virtual T max_y() const noexcept override { return proj_.max_y(); }
        virtual std::string name() const override { return "interrupted " + proj_.name(); }
        virtual Maps properties() const noexcept override { return map_properties; }
    protected:
        virtual bool canonical_on_globe(vector_type /*polar*/) const noexcept override { return true; }
        virtual bool canonical_on_map(vector_type xy) const noexcept override;
        virtual vector_type canonical_to_globe(vector_type xy) const noexcept override;
        virtual vector_type canonical_to_map(vector_type polar) const noexcept override;
    private:
        Projection proj_;
        const PseudocylindricalProjection<T>& pscyl() const noexcept { return proj_; }
    };

    template <typename Projection>
    bool InterruptedProjection<Projection>::canonical_on_map(vector_type xy) const noexcept {
        using namespace std::numbers;
        if (std::abs(xy.x()) > proj_.max_x() || std::abs(xy.y()) > proj_.max_y())
            return false;
        T x = xy.x();
        T y = std::clamp(xy.y(), - pi_v<T> / 2, pi_v<T> / 2);
        auto& seg = this->find_segment(x, y < 0);
        vector_type pxy = {x - seg.centre, y};
        if (! pscyl().canonical_on_map(pxy))
            return false;
        vector_type polar = pscyl().canonical_to_globe(pxy);
        T seg_phi = symmetric_remainder(polar[0], 2 * pi_v<T>);
        return std::abs(seg_phi) <= seg.delta;
    }

    template <typename Projection>
    typename InterruptedProjection<Projection>::vector_type
    InterruptedProjection<Projection>::canonical_to_globe(vector_type xy) const noexcept {
        using namespace std::numbers;
        T x = xy.x();
        T y = std::clamp(xy.y(), - pi_v<T> / 2, pi_v<T> / 2);
        auto& seg = this->find_segment(x, y < 0);
        T seg_x = symmetric_remainder(x - seg.centre, 2 * pi_v<T>);
        vector_type seg_xy = {seg_x, y};
        auto polar = pscyl().canonical_to_globe(seg_xy);
        polar[0] = euclidean_remainder(polar[0] + seg.centre, 2 * pi_v<T>);
        return polar;
    }

    template <typename Projection>
    typename InterruptedProjection<Projection>::vector_type
    InterruptedProjection<Projection>::canonical_to_map(vector_type polar) const noexcept {
        using namespace std::numbers;
        T phi = polar[0];
        T theta = std::clamp(polar[1], T(0), pi_v<T>);
        T map_phi = symmetric_remainder(phi, 2 * pi_v<T>);
        auto& seg = this->find_segment(map_phi, theta > pi_v<T> / 2);
        T seg_phi = symmetric_remainder(map_phi - seg.centre, 2 * pi_v<T>);
        vector_type seg_polar = {seg_phi, theta};
        auto xy = pscyl().canonical_to_map(seg_polar);
        xy.x() = symmetric_remainder(xy.x() + seg.centre, 2 * pi_v<T>);
        return xy;
    }

}
