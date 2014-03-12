﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Geometry {
  public struct R3Element {
    public readonly Scalar X, Y, Z;
    public R3Element(Scalar x, Scalar y, Scalar z) {
      X = x; Y = y; Z = z;
    }
    public Scalar this[int i] {
      get {
        switch (i % 3) {
          case 0: return X;
          case 1: return Y;
          case 2: return Z;
          default: return (Scalar)0; // Stupid language.
        }
      }
    }
    public static R3Element operator -(R3Element v) {
      return new R3Element(-v.X, -v.Y, -v.Z);
    }
    public static R3Element operator -(R3Element left, R3Element right) {
      return new R3Element(left.X - right.X,
                           left.Y - right.Y,
                           left.Z - right.Z);
    }
    public static R3Element operator *(Scalar left, R3Element right) {
      return new R3Element(left * right.X, left * right.Y, left * right.Z);
    }
    public static R3Element operator *(R3Element left, Scalar right) {
      return new R3Element(left.X * right, left.Y * right, left.Z * right);
    }
    public static R3Element operator /(R3Element left, Scalar right) {
      return new R3Element(left.X / right, left.Y / right, left.Z / right);
    }
    public static R3Element operator +(R3Element left, R3Element right) {
      return new R3Element(left.X + right.X,
                           left.Y + right.Y,
                           left.Z + right.Z);
    }
    public R3Element Cross(R3Element right) {
      return new R3Element(this.Y * right.Z - this.Z * right.Y,
                           this.Z * right.X - this.X * right.Z,
                           this.X * right.Y - this.Y * right.X);
    }
    public Scalar Dot(R3Element right) {
      return this.X * right.X + this.Y * right.Y + this.Z * right.Z;
    }
  }
}