// 
// This module implements a numerically stable ordinary least squares linear
// regressor.
// 
// The Regressor uses a fixed size circular queue, and updates various moving
// statistics as sample points enter and leave the queue.
//
// After at least 2 samples have been pushed into the regressor, it is then
// ready to provide estimates of the best line of fit. These can be obtained by
// calling reg_slope and reg_intercept.
//
// The slope and intercept can then be used to predict samples on the line of
// fit.
//
// E.g., to predict y given x:
//
//     m = reg_slope();
//     c = reg_intercept();
//     y = m * x + c;
//
// Currently, the regressor is used within chopper3 to estimate the line of
// best fit between the cpu clock and the audio playback clock.
//
// This estimate is used to help the game thread position synth sample points
// with respect to the audio thread. This helps to prevent jitter and skew from
// creating audio glitches such as drops and skips.

#pragma once

#include <stdbool.h>
#include <assert.h>

struct RegPoint
{
    double x;
    double y;
};

typedef struct RegPoint RegPoint;

struct StableRegressor
{
    RegPoint q[1024];

    int w;
    double c;
    double mx;
    double my;
    double mu;
    double m2;

    int front;
    int back;
};

typedef struct StableRegressor StableRegressor;

static void reg_init(StableRegressor *r, int n)
{
    assert(n < (sizeof(r->q) / sizeof(r->q[0])));
    memset(r, 0, sizeof(*r));
    r->w = n;
}

static int reg_qsize(StableRegressor *r)
{
    return r->back - r->front;
}

static int reg_n(StableRegressor *r) { return reg_qsize(r); }

// It would be quicker if we could use power of two masking here, but the
// optimal size for lag/accuracy is currently 400
// samples at the moment.
static int reg_mask(StableRegressor *r, int i) { return (i % r->w); }

static void reg_push(StableRegressor *r, double x, double y)
{
    if (reg_n(r) < r->w)
    {
        // initial window

        RegPoint p;
        p.x = x;
        p.y = y;

        r->q[reg_mask(r, r->back++)] = p;

        double dxN = x - r->mx;
        r->mx += dxN / reg_n(r);
        r->my += ( y-r->my ) / reg_n(r);
        r->c += dxN * ( y-r->my );

        double delta = x - r->mu;
        r->mu += delta / reg_n(r);
        r->m2 += delta * (x - r->mu);
    }
    else
    {
        RegPoint old = r->q[reg_mask(r, r->front++)];

        // cov part
        
        double dx0 = old.x - r->mx;
        double dy0 = old.y - r->my;
        double dxN = x - r->mx;
        double dyN = y - r->my;
        r->c += (dxN*dyN) - (dx0*dy0) - ( (dxN-dx0)*(dyN-dy0)/r->w );
        r->mx += ( dxN-dx0 ) / r->w;
        r->my += ( dyN-dy0 ) / r->w;

        // var part
        
        double delta = x - old.x;
        double d1 = old.x - r->mu;
        r->mu += delta / r->w;
        double d2 = x - r->mu;
        r->m2 += delta * (d1 + d2);

        RegPoint p;
        p.x = x;
        p.y = y;

        r->q[reg_mask(r, r->back++)] = p;
    }
}

static double reg_mu(StableRegressor *r) { return r->mu; }
static double reg_mean_x(StableRegressor *r) { return r->mx; }
static double reg_mean_y(StableRegressor *r) { return r->my; }

static double reg_covxy(StableRegressor *r)
{
    if (reg_n(r) == 1)
        return 0.0;
    else if (reg_n(r) < r->w)
        return r->c / (reg_n(r)-1);
    else
        return r->c / reg_n(r);
}

static double reg_varx(StableRegressor *r)
{
    if ( reg_n(r) == 1 )
        return 0.0;
    else if (reg_n(r) < r->w)
        return r->m2 / (reg_n(r)-1);
    else
        return r->m2 / reg_n(r);
}

static bool reg_ready(StableRegressor *r)
{
    return reg_n(r) >= 2;
}

static double reg_slope(StableRegressor *r) { return reg_covxy(r) / reg_varx(r); }
static double reg_intercept(StableRegressor *r) { return reg_mean_y(r) - reg_slope(r) * reg_mean_x(r); }


