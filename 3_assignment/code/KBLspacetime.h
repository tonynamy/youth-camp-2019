#ifndef KBLspacetime_h
#define KBLspacetime_h

/* Kerr spacetime in Boyer-Lindquist coordinate */
/* All quantities are unitless by scaling with M, a mass of black hole, in the geometrized unit. */

#include <stdbool.h>
#include <math.h>

/* Spin prameter */
#define a  0.9999
#define a2 (a*a)
#define a3 (a*a*a)
#define a4 (a*a*a*a)
#define a5 (a*a*a*a*a)
#define a6 (a*a*a*a*a*a)

/* Quantities           */
enum _FUNCTIONS {X_r, X_theta, X_phi, U_t, U_r, U_theta, U_phi, C_NUMBER_OF_FUNCTIONS};

/* coordinate */
#define r(v)  v[X_r    ]
#define r2(v) (r(v)*r(v))
#define r3(v) (r(v)*r(v)*r(v))
#define r4(v) (r(v)*r(v)*r(v)*r(v))
#define r5(v) (r(v)*r(v)*r(v)*r(v)*r(v))
#define r6(v) (r(v)*r(v)*r(v)*r(v)*r(v)*r(v))
#define r7(v) (r(v)*r(v)*r(v)*r(v)*r(v)*r(v)*r(v))
#define h(v)  v[X_theta]
#define p(v)  v[X_phi  ]
#define s(v)  sin(h(v))
#define s2(v) (s(v)*s(v))
#define s3(v) (s(v)*s(v)*s(v))
#define s4(v) (s(v)*s(v)*s(v)*s(v))
#define s5(v) (s(v)*s(v)*s(v)*s(v)*s(v))
#define c(v)  cos(h(v))
#define c2(v) (c(v)*c(v))
#define c3(v) (c(v)*c(v)*c(v))
#define c4(v) (c(v)*c(v)*c(v)*c(v))
#define c5(v) (c(v)*c(v)*c(v)*c(v)*c(v))
#define c6(v) (c(v)*c(v)*c(v)*c(v)*c(v)*c(v))

/* auxiliary quantities */
#define delta(v) (r2(v) - 2.*r(v) + a2)
#define sigma(v) (r2(v) + a2*c2(v))

/* metric */
#define g_tt(v) (-1. + 2.*r(v) / sigma(v))
#define g_tr(v) 0.
#define g_th(v) 0.
#define g_tp(v) (-2.*a*r(v)*s2(v) / sigma(v))
#define g_rr(v) (sigma(v) / delta(v))
#define g_rh(v) 0.
#define g_rp(v) 0.
#define g_hh(v) (sigma(v))
#define g_hp(v) 0.
#define g_pp(v) ((2.*a2*r(v)*s2(v) / sigma(v) + a2 + r2(v))*s2(v))

#define g_rt(v) g_tr(v)
#define g_ht(v) g_th(v)
#define g_hr(v) g_rh(v)
#define g_pt(v) g_tp(v)
#define g_pr(v) g_rp(v)
#define g_ph(v) g_hp(v)

#define g(a,b,v) g_##a##b(v)

/* unit basis vectors */
#define U_unit(a,v) \
    (1. / sqrt(g(a,a,v)))

/* four velocity */
#define u_t(v) v[U_t    ]
#define u_r(v) v[U_r    ]
#define u_h(v) v[U_theta]
#define u_p(v) v[U_phi  ]

#define u(a,v) u_##a(v)

#define u__t(v) \
    (g(t,r,v)*u(r,v) + g(t,h,v)*u(h,v) + g(t,p,v)*u(p,v))

#define u_square(v) \
    ( \
        + g(r,r,v)*u(r,v)*u(r,v) \
        + g(r,h,v)*u(r,v)*u(h,v) \
        + g(r,p,v)*u(r,v)*u(p,v) \
        + g(h,r,v)*u(h,v)*u(r,v) \
        + g(h,h,v)*u(h,v)*u(h,v) \
        + g(h,p,v)*u(h,v)*u(p,v) \
        + g(p,r,v)*u(p,v)*u(r,v) \
        + g(p,h,v)*u(p,v)*u(h,v) \
        + g(p,p,v)*u(p,v)*u(p,v) \
    )

/* public macros */

#define U_t(v) \
    (-(u__t(v) + sqrt(u__t(v)*u__t(v) - g(t,t,v)*u_square(v))) / g(t,t,v))

#define X(v) (r(v)*s(v)*cos(p(v)))
#define Y(v) (r(v)*s(v)*sin(p(v)))
#define Z(v) (r(v)*c(v))

/* derivative */

#define Gamma_ttt(v) 0.
#define Gamma_ttr(v) ((a2*r2(v) + r4(v) - (a4 + a2*r2(v))*c2(v)) / (a2*r4(v) - 2.*r5(v) + r6(v) + (a6 - 2.*a4*r(v) + a4*r2(v))*c4(v) + 2.*(a4*r2(v) - 2.*a2*r3(v) + a2*r4(v))*c2(v)))
#define Gamma_tth(v) (-(2.*a2*r(v)*c(v)*s(v)) / (a4*c4(v) + 2.*a2*r2(v)*c2(v) + r4(v)))
#define Gamma_ttp(v) 0.
#define Gamma_trr(v) 0.
#define Gamma_trh(v) 0.
#define Gamma_trp(v) (-((a3*r2(v) + 3.*a*r4(v) - (a5 - a3*r2(v))*c2(v))*s2(v)) / (a2*r4(v) - 2.*r5(v) + r6(v) + (a6 - 2.*a4*r(v) + a4*r2(v))*c4(v) + 2.*(a4*r2(v) - 2.*r3(v) + a2*r4(v))*c2(v)))
#define Gamma_thh(v) 0.
#define Gamma_thp(v) (-2.*(a5*r(v)*c(v)*s5(v) - (a5*r(v) + a3*r3(v))*c(v)*s3(v)) / (a6*c6(v) + 3.*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))
#define Gamma_tpp(v) 0.

#define Gamma_trt(v) Gamma_ttr(v)
#define Gamma_tht(v) Gamma_tth(v)
#define Gamma_thr(v) Gamma_trh(v)
#define Gamma_tpt(v) Gamma_ttp(v)
#define Gamma_tpr(v) Gamma_trp(v)
#define Gamma_tph(v) Gamma_thp(v)

#define Gamma_rtt(v) ((a2*r2(v) - 2.*r3(v) + r4(v) - (a4 - 2.*a2*r(v) + a2*r2(v))*c2(v)) / (a6*c6(v) + 3.*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))
#define Gamma_rtr(v) 0.
#define Gamma_rth(v) 0.
#define Gamma_rtp(v) (-(a3*r2(v) - 2.*a*r3(v) + a*r4(v) - (a5 - 2.*a3*r(v) + a3*r2(v))*c2(v))*s2(v) / (a6*c6(v) + 3.*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))
#define Gamma_rrr(v) ((a2*r(v) - r2(v) + (a2 - a2*r(v))*c2(v)) / (a2*r2(v) - 2.*r3(v) + r4(v) + (a4 - 2.*a2*r(v) + a2*r2(v))*c2(v)))
#define Gamma_rrh(v) (-(a2*c(v)*s(v)) / (a2*c2(v) + r2(v)))
#define Gamma_rrp(v) 0.
#define Gamma_rhh(v) (- (a2*r(v) - 2.*r2(v) + r3(v)) / (a2*c2(v) + r2(v)))
#define Gamma_rhp(v) 0.
#define Gamma_rpp(v) (((a4*r2(v) - 2.*a2*r3(v) + a2*r4(v) - (a6 - 2.*a4*r(v) + a4*r2(v))*c2(v))*s4(v) - (a2*r5(v) - 2.*r6(v) + r7(v) + (a6*r(v) - 2.*a4*r2(v) + a4*r3(v))*c4(v) + 2.*(a4*r3(v) - 2.*a2*r4(v) + a2*r5(v))*c2(v))*s2(v)) / (a6*c6(v) + 3.*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))

#define Gamma_rrt(v) Gamma_rtr(v)
#define Gamma_rht(v) Gamma_rth(v)
#define Gamma_rhr(v) Gamma_rrh(v)
#define Gamma_rpt(v) Gamma_rtp(v)
#define Gamma_rpr(v) Gamma_rrp(v)
#define Gamma_rph(v) Gamma_rhp(v)

#define Gamma_htt(v) (-(2.*a2*r(v)*c(v)*s(v)) / (a6*c6(v) + 3.*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))
#define Gamma_htr(v) 0.
#define Gamma_hth(v) 0.
#define Gamma_htp(v) (2.*(a3*r(v) + a*r3(v))*c(v)*s(v) / (a6*c6(v) + 3*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))
#define Gamma_hrr(v) ((a2*c(v)*s(v)) / (a2*r2(v) - 2.*r3(v) + r4(v) + (a4 - 2.*a2*r(v) + a2*r2(v))*c2(v)))
#define Gamma_hrh(v) (r(v) / (a2*c2(v) + r2(v)))
#define Gamma_hrp(v) 0.
#define Gamma_hhh(v) (-(a2*c(v)*s(v)) / (a2*c2(v) + r2(v)))
#define Gamma_hhp(v) 0.
#define Gamma_hpp(v) (-((a6 - 2.*a4*r(v) + a4*r2(v))*c5(v) + 2.*(a4*r2(v) - 2.*a2*r3(v) + a2*r4(v))*c3(v) + (2.*a4*r(v) + 4.*a2*r3(v) + a2*r4(v) + r6(v))*c(v))*s(v) / (a6*c6(v) + 3.*a4*r2(v)*c4(v) + 3.*a2*r4(v)*c2(v) + r6(v)))

#define Gamma_hrt(v) Gamma_htr(v)
#define Gamma_hht(v) Gamma_hth(v)
#define Gamma_hhr(v) Gamma_hrh(v)
#define Gamma_hpt(v) Gamma_htp(v)
#define Gamma_hpr(v) Gamma_hrp(v)
#define Gamma_hph(v) Gamma_hhp(v)

#define Gamma_ptt(v) 0.
#define Gamma_ptr(v) (-(a3*c2(v) - a*r2(v)) / (a2*r4(v) - 2.*r5(v) + r6(v) + (a6 - 2.*a4*r(v) - a4*r2(v))*c4(v) + 2.*(a4*r2(v) - 2.*a2*r3(v) + a2*r4(v))*c2(v)))
#define Gamma_pth(v) (-(2.*a*r(v)*c(v)) / ((a4*c4(v) + 2.*a2*r2(v)*c2(v) + r4(v))*s(v)))
#define Gamma_ptp(v) 0.
#define Gamma_prr(v) 0.
#define Gamma_prh(v) 0.
#define Gamma_prp(v) (- (a2*r2(v) + 2.*r4(v) - r5(v) + (a4 - a4*r(v))*c4(v) - (a4 - a2*r2(v) + 2.*a2*r3(v))*c2(v)) / (a2*r4(v) - 2.*r5(v) + r6(v) + (a6 - 2.*a4*r(v) + a4*r2(v))*c4(v) + 2.*(a4*r2(v) - 2.*a2*r3(v) + a2*r4(v))*c2(v)))
#define Gamma_phh(v) 0.
#define Gamma_php(v) ((a4*c(v)*s4(v) - 2.*(a4 - a2*r(v) + a2*r2(v))*c(v)*s2(v) + (a4 + 2.*a2*r2(v) + r4(v))*c(v)) / ((a4*c4(v) + 2.*a2*r2(v)*c2(v) + r4(v))*s(v)))
#define Gamma_ppp(v) 0.

#define Gamma_prt(v) Gamma_ptr(v)
#define Gamma_pht(v) Gamma_pth(v)
#define Gamma_phr(v) Gamma_prh(v)
#define Gamma_ppt(v) Gamma_ptp(v)
#define Gamma_ppr(v) Gamma_prp(v)
#define Gamma_pph(v) Gamma_php(v)

#define Gamma(a,b,c,v) Gamma_##a##b##c(v)

#define Gamma_t(b,c,v) Gamma(t,b,c,v)
#define Gamma_r(b,c,v) Gamma(r,b,c,v)
#define Gamma_h(b,c,v) Gamma(h,b,c,v)
#define Gamma_p(b,c,v) Gamma(p,b,c,v)

#define cont(A, B, C, v) \
    ( \
        + A(t,t,v)*B(t,v)*C(t,v) \
        + A(t,r,v)*B(t,v)*C(r,v) \
        + A(t,h,v)*B(t,v)*C(h,v) \
        + A(t,p,v)*B(t,v)*C(p,v) \
        + A(r,t,v)*B(r,v)*C(t,v) \
        + A(r,r,v)*B(r,v)*C(r,v) \
        + A(r,h,v)*B(r,v)*C(h,v) \
        + A(r,p,v)*B(r,v)*C(p,v) \
        + A(h,t,v)*B(h,v)*C(t,v) \
        + A(h,r,v)*B(h,v)*C(r,v) \
        + A(h,h,v)*B(h,v)*C(h,v) \
        + A(h,p,v)*B(h,v)*C(p,v) \
        + A(p,t,v)*B(p,v)*C(t,v) \
        + A(p,r,v)*B(p,v)*C(r,v) \
        + A(p,h,v)*B(p,v)*C(h,v) \
        + A(p,p,v)*B(p,v)*C(p,v) \
    )

#define DERIVATIVE_X_r(v)       (u(r,v) / u(t,v))
#define DERIVATIVE_X_theta(v)   (u(h,v) / u(t,v))
#define DERIVATIVE_X_phi(v)     (u(p,v) / u(t,v))
#define DERIVATIVE_U_t(v)       (-cont(Gamma_t, u, u, v) / u(t,v))
#define DERIVATIVE_U_r(v)       (-cont(Gamma_r, u, u, v) / u(t,v))
#define DERIVATIVE_U_theta(v)   (-cont(Gamma_h, u, u, v) / u(t,v))
#define DERIVATIVE_U_phi(v)     (-cont(Gamma_p, u, u, v) / u(t,v))

#define BOOL_NEAR_HORIZON(v) (r(v) <= 1. + sqrt(1. - a*a*c(v)*c(v)))

#endif /* KBLspacetime_h */
