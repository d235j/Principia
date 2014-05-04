﻿#pragma once

#include "Geometry/R3Element.hpp"
#include "Quantities/Dimensionless.hpp"
#include "Quantities/Quantities.hpp"

namespace principia {
namespace geometry {

// A multivector of rank Rank on a three-dimensional real inner product space
// bearing the dimensionality of Scalar.
// Do not use this type for Rank = 0 (scalar), use the Scalar type directly
// instead.
template<typename Scalar, typename Frame, unsigned int Rank>
struct Multivector;

template<typename Scalar, typename Frame>
struct Multivector<Scalar, Frame, 1> {
  explicit Multivector(R3Element<Scalar> const& coordinates);
  ~Multivector() = default;

  R3Element<Scalar> coordinates;
};

template<typename Scalar, typename Frame>
struct Multivector<Scalar, Frame, 2> {
  explicit Multivector(R3Element<Scalar> const& coordinates);
  ~Multivector() = default;

  R3Element<Scalar> coordinates;
};

template<typename Scalar, typename Frame>
struct Multivector<Scalar, Frame, 3> {
  explicit Multivector(Scalar const& coordinates);
  ~Multivector() = default;

  Scalar coordinates;
};

template<typename Scalar, typename Frame>
using Vector = Multivector<Scalar, Frame, 1>;
template<typename Scalar, typename Frame>
using Bivector = Multivector<Scalar, Frame, 2>;
template<typename Scalar, typename Frame>
using Trivector = Multivector<Scalar, Frame, 3>;

template<typename LeftScalar, typename RightScalar, typename Frame>
quantities::Product<LeftScalar, RightScalar> InnerProduct(
    Vector<LeftScalar, Frame> const& left,
    Vector<RightScalar, Frame> const& right);
template<typename LeftScalar, typename RightScalar, typename Frame>
quantities::Product<LeftScalar, RightScalar> InnerProduct(
    Bivector<LeftScalar, Frame> const& left,
    Bivector<RightScalar, Frame> const& right);
template<typename LeftScalar, typename RightScalar, typename Frame>
quantities::Product<LeftScalar, RightScalar> InnerProduct(
    Trivector<LeftScalar, Frame> const& left,
    Trivector<RightScalar, Frame> const& right);

template<typename LeftScalar, typename RightScalar, typename Frame>
Bivector<quantities::Product<LeftScalar, RightScalar>,
         Frame> Wedge(Vector<LeftScalar, Frame> const& left,
                      Vector<RightScalar, Frame> const& right);
template<typename LeftScalar, typename RightScalar, typename Frame>
Trivector<quantities::Product<LeftScalar, RightScalar>,
          Frame> Wedge(Bivector<LeftScalar, Frame> const& left,
                       Vector<RightScalar, Frame> const& right);
template<typename LeftScalar, typename RightScalar, typename Frame>
Trivector<quantities::Product<LeftScalar, RightScalar>,
          Frame> Wedge(Vector<LeftScalar, Frame> const& left,
                       Bivector<RightScalar, Frame> const& right);

// Lie bracket on V ^ V = so(V).
template<typename LeftScalar, typename RightScalar, typename Frame>
Bivector<quantities::Product<LeftScalar, RightScalar>,
         Frame> Commutator(Bivector<LeftScalar, Frame> const& left,
                           Bivector<RightScalar, Frame> const& right);

// Left action of V ^ V = so(V) on V.
template<typename LeftScalar, typename RightScalar, typename Frame>
Vector<quantities::Product<LeftScalar, RightScalar>,
       Frame> operator*(Bivector<LeftScalar, Frame> const& left,
                        Vector<RightScalar, Frame> const& right);

// Right action of V ^ V = so(V) on V* = V.
template<typename LeftScalar, typename RightScalar, typename Frame>
Vector<quantities::Product<LeftScalar, RightScalar>,
       Frame> operator*(Vector<LeftScalar, Frame> const& left,
                        Bivector<RightScalar, Frame> const& right);

template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator+(
    Multivector<Scalar, Frame, Rank> const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator-(
    Multivector<Scalar, Frame, Rank> const& right);

template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator+(
    Multivector<Scalar, Frame, Rank> const& left,
    Multivector<Scalar, Frame, Rank> const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator-(
    Multivector<Scalar, Frame, Rank> const& left,
    Multivector<Scalar, Frame, Rank> const& right);

template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator*(
    quantities::Dimensionless const& left,
    Multivector<Scalar, Frame, Rank> const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator*(
    Multivector<Scalar, Frame, Rank> const& left,
    quantities::Dimensionless const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
Multivector<Scalar, Frame, Rank> operator/(
    Multivector<Scalar, Frame, Rank> const& left,
    quantities::Dimensionless const& right);

template<typename DLeft, typename RightScalar, typename Frame,
         unsigned int Rank>
Multivector<quantities::Product<quantities::Quantity<DLeft>, RightScalar>,
            Frame, Rank>
operator*(quantities::Quantity<DLeft> const& left,
          Multivector<RightScalar, Frame, Rank> const& right);

template<typename LeftScalar, typename DRight, typename Frame,
         unsigned int Rank>
Multivector<quantities::Product<LeftScalar, quantities::Quantity<DRight>>,
            Frame, Rank>
operator*(Multivector<LeftScalar, Frame, Rank> const& left,
          quantities::Quantity<DRight> const& right);

template<typename LeftScalar, typename DRight, typename Frame,
         unsigned int Rank>
Multivector<quantities::Quotient<LeftScalar, quantities::Quantity<DRight>>,
            Frame, Rank>
operator/(Multivector<LeftScalar, Frame, Rank> const& left,
           quantities::Quantity<DRight> const& right);

template<typename Scalar, typename Frame, unsigned int Rank>
void operator+=(Multivector<Scalar, Frame, Rank>& left,
                Multivector<Scalar, Frame, Rank> const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
void operator-=(Multivector<Scalar, Frame, Rank>& left,
                Multivector<Scalar, Frame, Rank> const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
void operator*=(Multivector<Scalar, Frame, Rank>& left,
                quantities::Dimensionless const& right);
template<typename Scalar, typename Frame, unsigned int Rank>
void operator/=(Multivector<Scalar, Frame, Rank>& left,
                quantities::Dimensionless const& right);

}  // namespace geometry
}  // namespace principia

#include "Grassmann-body.hpp"
