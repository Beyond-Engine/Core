#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "beyond/math/angle.hpp"
#include "beyond/math/constants.hpp"
#include "beyond/math/serial.hpp"

#include "../serial_test_util.hpp"

using namespace beyond;

using beyond::float_constants::pi;
using namespace beyond::literals;

using Catch::Approx;

TEST_CASE("TRadian", "[beyond.core.math.angle]")
{
  SECTION("Default construction")
  {
    TRadian<float> rad;
    REQUIRE(rad.value() == Approx(0));
  }

  GIVEN("A TRadian r1 = pi")
  {
    TRadian<float> r1{pi};
    REQUIRE(r1.value() == Approx(pi));

    THEN("-r1 = -pi")
    {
      REQUIRE((-r1).value() == Approx(-pi));
    }

    AND_GIVEN("Another TRadian r2 = pi / 2")
    {
      const TRadian<float> r2{pi / 2};

      WHEN("r1 += r2")
      {
        r1 += r2;
        THEN("r1 = pi * 3 / 2")
        {
          REQUIRE(r1.value() == Approx(pi * 3 / 2));
        }
      }
      WHEN("r1 -= r2")
      {
        r1 -= r2;
        THEN("r1 = pi * 1 / 2")
        {
          REQUIRE(r1.value() == Approx(pi * 1 / 2));
        }
      }

      THEN("r1 == r1")
      {
        REQUIRE(r1 == r1);
      }

      THEN("r1 != r2")
      {
        REQUIRE(r1 != r2);
      }

      THEN("r2 < r1")
      {
        REQUIRE(r2 < r1);
      }

      THEN("r2 <= r1")
      {
        REQUIRE(r2 <= r1);
      }

      THEN("r1 > r2")
      {
        REQUIRE(r1 > r2);
      }

      THEN("r1 >= r2")
      {
        REQUIRE(r1 >= r2);
      }

      THEN("r1 + r2 = pi * 3 / 2")
      {
        const auto result = r1 + r2;
        REQUIRE(result.value() == Approx(pi * 3 / 2));
      }

      THEN("r1 - r2 = pi * 1 / 2")
      {
        const auto result = r1 - r2;
        REQUIRE(result.value() == Approx(pi * 1 / 2));
      }

      THEN("r1 / r2 = 2")
      {
        REQUIRE((r1 / r2) == Approx(2));
      }
    }

    AND_GIVEN("A scalar s = 2")
    {
      const float s = 2;
      WHEN("r1 *= s")
      {
        r1 *= s;
        THEN("r1 = pi * s")
        {
          REQUIRE(r1.value() == Approx(pi * s));
        }
      }
      WHEN("r1 /= s")
      {
        r1 /= s;
        THEN("r1 = pi / s")
        {
          REQUIRE(r1.value() == Approx(pi / s));
        }
      }

      THEN("r1 * s = s * pi")
      {
        REQUIRE((r1 * s).value() == Approx(pi * s));
        REQUIRE((s * r1).value() == Approx(pi * s));
      }

      THEN("r1 / s = pi / s")
      {
        REQUIRE((r1 / s).value() == Approx(pi / s));
      }
    }
  }

  SECTION("Create TRadian by a literal")
  {
    REQUIRE((1._rad).value() == Approx(1));
  }

  SECTION("Converts from radian of another type")
  {
    const TRadian<double> r{static_cast<double>(pi)};
    REQUIRE(TRadian<float>{r}.value() == Approx(pi));
  }

  SECTION("Constants")
  {
    using namespace beyond::float_constants;
    REQUIRE(Radian::pi().value() == pi);
  }
}

TEST_CASE("TDegree", "[beyond.core.math.angle]")
{

  SECTION("Default construction")
  {
    TDegree<float> deg;
    REQUIRE(deg.value() == Approx(0));
  }

  GIVEN("A TDegree d1 = 90_deg")
  {
    TDegree<float> d1{90};
    REQUIRE(d1.value() == Approx(90));

    THEN("-d1 = -90")
    {
      REQUIRE((-d1).value() == Approx(-90));
    }

    AND_GIVEN("Another TDegree d2 = 45_deg")
    {
      const TDegree<float> d2{45};

      WHEN("d1 += d2")
      {
        d1 += d2;
        THEN("d1 = 135_deg")
        {
          REQUIRE(d1.value() == Approx(135));
        }
      }
      WHEN("d1 -= d2")
      {
        d1 -= d2;
        THEN("d1 = 45")
        {
          REQUIRE(d1.value() == Approx(45));
        }
      }

      THEN("d1 == d1")
      {
        REQUIRE(d1 == d1);
      }

      THEN("d1 != d2")
      {
        REQUIRE(d1 != d2);
      }

      THEN("d2 < d1")
      {
        REQUIRE(d2 < d1);
      }

      THEN("d2 <= d1")
      {
        REQUIRE(d2 <= d1);
      }

      THEN("d1 > d2")
      {
        REQUIRE(d1 > d2);
      }

      THEN("d1 >= d2")
      {
        REQUIRE(d1 >= d2);
      }

      THEN("d1 + d2 = 135_deg")
      {
        const auto result = d1 + d2;
        REQUIRE(result.value() == Approx(135));
      }

      THEN("d1 - d2 = 45_deg")
      {
        const auto result = d1 - d2;
        REQUIRE(result.value() == Approx(45));
      }

      THEN("d1 / d2 = 2")
      {
        REQUIRE((d1 / d2) == Approx(2));
      }
    }

    AND_GIVEN("A scalar s = 1.5")
    {
      const float s = 1.5f;
      WHEN("d1 *= s")
      {
        d1 *= s;
        THEN("d1 = s * 90")
        {
          REQUIRE(d1.value() == Approx(s * 90));
        }
      }
      WHEN("d1 /= s")
      {
        d1 /= s;
        THEN("d1 = 90 / s")
        {
          REQUIRE(d1.value() == Approx(90 / s));
        }
      }

      THEN("d1 * s = 90 * s")
      {
        REQUIRE((d1 * s).value() == Approx(90 * s));
        REQUIRE((s * d1).value() == Approx(90 * s));
      }

      THEN("d1 / s = 0.5 * s")
      {
        REQUIRE((d1 / s).value() == Approx(90 / s));
      }
    }
  }

  SECTION("Create TDegree by a literal")
  {
    REQUIRE((45._deg).value() == Approx(45));
  }

  SECTION("Converts from TDegree of another type")
  {
    const TDegree<double> r{60};
    REQUIRE(TDegree<float>{r}.value() == Approx(60));
  }
}

TEST_CASE("Conversion between TRadian and TDegree", "[beyond.core.math.angle]")
{
  GIVEN("A TRadian pi")
  {
    const TRadian<float> r{pi};
    THEN("Create a TDegree 180_deg from that radian")
    {
      REQUIRE(TDegree<float>{r}.value() == Approx(180));
    }
  }

  GIVEN("A TDegree 90")
  {
    const TDegree<float> d{90};
    THEN("Create a TRadian pi/2 from that degree")
    {
      REQUIRE(TRadian<float>{d}.value() == Approx(pi / 2));
    }
  }
}

TEST_CASE("Angle serialization test", "[beyond.core.math.angle]")
{
  SECTION("Output TRadian to a stream")
  {
    const TRadian<float> r{1};
    const auto expected = "1_radian";

    REQUIRE_TO_STRING_EQ(r, expected);
  }

  SECTION("Output TDegree to a stream")
  {
    const TDegree<float> d{90};
    const auto expected = "90_degree";

    REQUIRE_TO_STRING_EQ(d, expected);
  }
}
