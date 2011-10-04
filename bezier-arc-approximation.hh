/*
 * Copyright © 2011  Google, Inc.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod, Maysum Panju
 */

#include "geometry.hh"

#include <vector>

#include <assert.h>

#ifndef BEZIER_ARC_APPROXIMATION_HH
#define BEZIER_ARC_APPROXIMATION_HH

namespace BezierArcApproximation {

using namespace Geometry;

template <typename T> const T min (const T &a, const T &b) { return a <= b ? a : b; }
template <typename T> const T max (const T &a, const T &b) { return a >= b ? a : b; }



class MaxDeviationApproximatorFast
{
  public:
  /* Returns upper bound for max(abs(d₀ t (1-t)² + d₁ t² (1-t)) for 0≤t≤1. */
  static double approximate_deviation (double d0, double d1)
  {
    d0 = fabs (d0);
    d1 = fabs (d1);
    double e0 = 3./4. * max (d0, d1);
    double e1 = 4./9. * (d0 + d1);
    return min (e0, e1);
  }
};

class MaxDeviationApproximatorExact
{
  public:
  /* Returns max(abs(d₀ t (1-t)² + d₁ t² (1-t)) for 0≤t≤1. */
  static double approximate_deviation (double d0, double d1)
  {
    double candidates[4] = {0,1};
    unsigned int num_candidates = 2;
    if (d0 == d1)
      candidates[num_candidates++] = .5;
    else {
      double delta = d0*d0 - d0*d1 + d1*d1;
      double t2 = 1. / (3 * (d0 - d1));
      double t0 = (2 * d0 - d1) * t2;
      if (delta == 0)
	candidates[num_candidates++] = t0;
      else if (delta > 0) {
	/* This code can be optimized to avoid the sqrt if the solution
	 * is not feasible (ie. lies outside (0,1)).  I have implemented
	 * that in cairo-spline.c:_cairo_spline_bound().  Can be reused
	 * here.
	 */
	double t1 = sqrt (delta) * t2;
	candidates[num_candidates++] = t0 - t1;
	candidates[num_candidates++] = t0 + t1;
      }
    }

    double e = 0;
    for (unsigned int i = 0; i < num_candidates; i++) {
      double t = candidates[i];
      double ee;
      if (t < 0. || t > 1.)
	continue;
      ee = fabs (3 * t * (1-t) * (d0 * (1 - t) + d1 * t));
      e = max (e, ee);
    }

    return e;
  }
};



template <class MaxDeviationApproximator>
class BezierBezierErrorApproximatorSimpleMagnitude
{
  public:
  static double approximate_bezier_bezier_error (const Bezier<Coord> &b0, const Bezier<Coord> &b1)
  {
    assert (b0.p0 == b1.p0);
    assert (b0.p3 == b1.p3);

    return MaxDeviationApproximator::approximate_deviation ((b1.p1 - b0.p1).len (),
							    (b1.p2 - b0.p2).len ());
  }
};

template <class MaxDeviationApproximator>
class BezierBezierErrorApproximatorSimpleMagnitudeDecomposed
{
  public:
  static double approximate_bezier_bezier_error (const Bezier<Coord> &b0, const Bezier<Coord> &b1)
  {
    assert (b0.p0 == b1.p0);
    assert (b0.p3 == b1.p3);

    return Vector<Coord> (MaxDeviationApproximator::approximate_deviation
			  (b1.p1.x - b0.p1.x, b1.p2.x - b0.p2.x),
			  MaxDeviationApproximator::approximate_deviation
			  (b1.p1.y - b0.p1.y, b1.p2.y - b0.p2.y)).len ();
  }
};



template <class BezierBezierErrorApproximator>
class BezierArcErrorApproximatorViaBezier
{
  public:
  static double approximate_bezier_arc_error (const Bezier<Coord> &b0, const Arc<Coord, Scalar> &a)
  {
    double ea;
    Bezier<Coord> b1 = a.approximate_bezier (&ea);
    double eb = BezierBezierErrorApproximator::approximate_bezier_bezier_error (b0, b1);
    return ea + eb;
  }
};

class BezierArcErrorApproximatorSampling
{
  public:
  static double approximate_bezier_arc_error (const Bezier<Coord> &b, const Arc<Coord, Scalar> &a,
					      double step = .001)
  {
    Circle<Coord, Scalar> c = a.circle ();
    double e = 0;
    for (double t = 0; t <= 1; t += step)
      e = max (e, fabs ((c.c - b.point (t)).len () - c.r));
    return e;
  }
};

template <class MaxDeviationApproximator>
class BezierArcErrorApproximatorBehdad
{
  public:
  static double approximate_bezier_arc_error (const Bezier<Coord> &b0, const Arc<Coord, Scalar> &a)
  {
    assert (b0.p0 == a.p0);
    assert (b0.p3 == a.p1);

    double ea;
    Bezier<Coord> b1 = a.approximate_bezier (&ea);

    assert (b0.p0 == b1.p0);
    assert (b0.p3 == b1.p3);

    Vector<Coord> v0 = b1.p1 - b0.p1;
    Vector<Coord> v1 = b1.p2 - b0.p2;

    Vector<Coord> b = (b0.p3 - b0.p0).normalized ();
    v0 = v0.rebase (b);
    v1 = v1.rebase (b);

    Vector<Coord> v (MaxDeviationApproximator::approximate_deviation (v0.dx, v1.dx),
		     MaxDeviationApproximator::approximate_deviation (v0.dy, v1.dy));

    double tan_half_alpha = 2 * fabs (a.d) / (1 - a.d*a.d);
    double tan_v = v.dx / v.dy;
    double eb;
    if (tan_half_alpha < 0 ||
	(-tan_half_alpha <= tan_v && tan_v <= tan_half_alpha)) {
      eb = v.len ();
    } else {
      Scalar c2 = (b1.p3 - b1.p0).len () / 2;
      double r = c2 * (a.d * a.d + 1) / (2 * fabs (a.d));
      eb = Vector<Coord> (c2/tan_half_alpha + v.dy, c2 + v.dx).len () - r;
    }

    return ea + eb;
  }
};



template <class BezierArcErrorApproximator>
class BezierArcApproximatorMidpointSimple
{
  public:
  static const Arc<Coord, Scalar> approximate_bezier_with_arc (const Bezier<Coord> &b, double *error)
  {
    Pair<Bezier<Coord> > pair = b.halve ();
    Point<Coord> m = pair.second.p0;

    Arc<Coord, Scalar> a (b.p0, b.p3, m, false);

    *error = BezierArcErrorApproximator::approximate_bezier_arc_error (b, a);

    return a;
  }
};

template <class BezierArcErrorApproximator>
class BezierArcApproximatorMidpointTwoPart
{
  public:
  static const Arc<Coord, Scalar> approximate_bezier_with_arc (const Bezier<Coord> &b, double *error)
  {
    Pair<Bezier<Coord> > pair = b.halve ();
    Point<Coord> m = pair.second.p0;

    Arc<Coord, Scalar> a0 (b.p0, m, b.p3, true);
    Arc<Coord, Scalar> a1 (m, b.p3, b.p0, true);

    double e0 = BezierArcErrorApproximator::approximate_bezier_arc_error (pair.first, a0);
    double e1 = BezierArcErrorApproximator::approximate_bezier_arc_error (pair.second, a1);
    *error = max (e0, e1);

    return Arc<Coord, Scalar> (b.p0, b.p3, m, false);
  }
};


#include <stdio.h>
template <class BezierArcApproximator>
class BezierArcsApproximatorSpring
{
  public:
  static std::vector<Arc<Coord, Scalar> > &approximate_bezier_with_arcs (const Bezier<Coord> &b,
									 double tolerance,
									 double *perror,
									 int max_segments = 1000,
									 int max_jiggle = 100)
  {
    std::vector<double> t;
    std::vector<double> e;
    std::vector<Arc<Coord, Scalar> > &arcs = * new std::vector<Arc<Coord, Scalar> >;
    double max_e, min_e;
    int n_jiggle = 0;
    for (unsigned int n = 1; n <= max_segments; n++) {
      t.resize (n + 1);
      e.resize (n);

      bool candidate = false;
      for (unsigned int i = 0; i <= n; i++)
        t[i] = double (i) / n;

      arcs.clear ();
      max_e = 0;
      min_e = INFINITY;
      for (unsigned int i = 0; i < n; i++)
      {
	Bezier<Coord> segment = b.segment (t[i], t[i + 1]);
	arcs.push_back (BezierArcApproximator::approximate_bezier_with_arc (segment, &e[i]));
	if (e[i] <= tolerance)
	  candidate = true;

	max_e = max (max_e, e[i]);
	min_e = min (min_e, e[i]);
      }

      if (candidate) {
	printf ("candidate n %d max_e %g min_e %g\n", n, max_e, min_e);
        for (unsigned int s = 0; s < max_jiggle; s++) {
	  std::vector<double> l;
	  std::vector<double> k_inv;

	  l.resize (n);
	  k_inv.resize (n);
	  double total = 0;
	  for (unsigned int i = 0; i < n; i++) {
	    l[i] = t[i + 1] - t[i];
	    k_inv[i] = l[i] / pow (e[i], .3);
	    total += k_inv[i];
	  }
	  for (unsigned int i = 0; i < n; i++) {
	    l[i] = k_inv[i] / total;
	    t[i + 1] = t[i] + l[i];
	  }

	  arcs.clear ();
	  max_e = 0;
	  min_e = INFINITY;
	  for (unsigned int i = 0; i < n; i++)
	  {
	    Bezier<Coord> segment = b.segment (t[i], t[i + 1]);
	    arcs.push_back (BezierArcApproximator::approximate_bezier_with_arc (segment, &e[i]));

	    max_e = max (max_e, e[i]);
	    min_e = min (min_e, e[i]);
	  }
	  printf ("n %d jiggle %d max_e %g min_e %g\n", n, s, max_e, min_e);

	  n_jiggle++;
	  if (max_e < tolerance || (2 * min_e - max_e > tolerance))
	    break;
	}
      }

      if (max_e <= tolerance)
        break;
    }
    if (perror)
      *perror = max_e;
    printf ("n_jiggle %d\n", n_jiggle);
    return arcs;
  }
};


} /* namespace BezierArcApproxmation */

#endif
