// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++20 -fenable-matrix -triple i686-pc-win32

enum class N {};

using B1 = int;
using X1 = B1;
using Y1 = B1;

using B2 = void;
using X2 = B2;
using Y2 = B2;

using A3 = char __attribute__((vector_size(4)));
using B3 = A3;
using X3 = B3;
using Y3 = B3;

using A4 = float;
using B4 = A4 __attribute__((matrix_type(4, 4)));
using X4 = B4;
using Y4 = B4;

using X5 = A4 __attribute__((matrix_type(3, 4)));
using Y5 = A4 __attribute__((matrix_type(4, 3)));

N t1 = 0 ? X1() : Y1(); // expected-error {{rvalue of type 'B1'}}
N t2 = 0 ? X2() : Y2(); // expected-error {{rvalue of type 'B2'}}

const X1 &xt3 = 0;
const Y1 &yt3 = 0;
N t3 = 0 ? xt3 : yt3; // expected-error {{lvalue of type 'const B1'}}

N t4 = X3() + Y3();   // expected-error {{rvalue of type 'B3'}}

N t5 = A3() ? X3() : Y3(); // expected-error {{rvalue of type 'B3'}}
N t6 = A3() ? X1() : Y1(); // expected-error {{vector condition type 'A3' (vector of 4 'char' values) and result type '__attribute__((__vector_size__(4 * sizeof(B1)))) B1' (vector of 4 'B1' values) do not have elements of the same size}}

N t7 = X4() + Y4(); // expected-error {{rvalue of type 'B4'}}
N t8 = X4() * Y4(); // expected-error {{rvalue of type 'B4'}}
N t9 = X5() * Y5(); // expected-error {{rvalue of type 'A4 __attribute__((matrix_type(3, 3)))'}}

template <class T> struct S1 {
  template <class U> struct S2 {};
};

N t10 = 0 ? S1<X1>() : S1<Y1>(); // expected-error {{from 'S1<B1>' (aka 'S1<int>')}}
N t11 = 0 ? S1<X1>::S2<X2>() : S1<Y1>::S2<Y2>(); // expected-error {{from 'S1<int>::S2<B2>' (aka 'S2<void>')}}

template <class T> using Al = S1<T>;

N t12 = 0 ? Al<X1>() : Al<Y1>(); // expected-error {{from 'Al<B1>' (aka 'S1<int>')}}

#define AS1 __attribute__((address_space(1)))
#define AS2 __attribute__((address_space(1)))
using AS1X1 = AS1 B1;
using AS1Y1 = AS1 B1;
using AS2Y1 = AS2 B1;
N t13 = 0 ? (AS1X1){} : (AS1Y1){}; // expected-error {{rvalue of type 'AS1 B1' (aka '__attribute__((address_space(1))) int')}}
N t14 = 0 ? (AS1X1){} : (AS2Y1){}; // expected-error {{rvalue of type '__attribute__((address_space(1))) B1' (aka '__attribute__((address_space(1))) int')}}

using FX1 = X1 ();
using FY1 = Y1 ();
N t15 = 0 ? (FX1*){} : (FY1*){}; // expected-error {{rvalue of type 'B1 (*)()' (aka 'int (*)()')}}

struct SS1 {};
using SB1 = SS1;
using SX1 = SB1;
using SY1 = SB1;

using MFX1 = X1 SX1::*();
using MFY1 = Y1 SY1::*();

N t16 = 0 ? (MFX1*){} : (MFY1*){}; // expected-error {{rvalue of type 'B1 SB1::*(*)()'}}

N t17 = 0 ? (FX1 SX1::*){} : (FY1 SY1::*){}; // expected-error {{rvalue of type 'B1 (SB1::*)() __attribute__((thiscall))'}}

N t18 = 0 ? (__typeof(X1*)){} : (__typeof(Y1*)){}; // expected-error {{rvalue of type 'typeof(B1 *)' (aka 'int *')}}

struct Enums {
  enum X : B1;
  enum Y : ::B1;
};
using EnumsB = Enums;
using EnumsX = EnumsB;
using EnumsY = EnumsB;

N t19 = 0 ? (__underlying_type(EnumsX::X)){} : (__underlying_type(EnumsY::Y)){};
// expected-error@-1 {{rvalue of type 'B1' (aka 'int')}}

N t20 = 0 ? (__underlying_type(EnumsX::X)){} : (__underlying_type(EnumsY::X)){};
// expected-error@-1 {{rvalue of type '__underlying_type(Enums::X)' (aka 'int')}}

using SBTF1 = SS1 [[clang::btf_type_tag("1")]];
using SBTF2 = ::SS1 [[clang::btf_type_tag("1")]];
using SBTF3 = ::SS1 [[clang::btf_type_tag("2")]];

N t21 = 0 ? (SBTF1){} : (SBTF3){}; // expected-error {{from 'SS1'}}
N t22 = 0 ? (SBTF1){} : (SBTF2){}; // expected-error {{from 'SS1 btf_type_tag(1)' (aka 'SS1')}}

using QX = const SB1 *;
using QY = const ::SB1 *;
N t23 = 0 ? (QX){} : (QY){}; // expected-error {{rvalue of type 'const SB1 *' (aka 'const SS1 *')}}

template <class T> using Alias = short;
N t24 = 0 ? (Alias<X1>){} : (Alias<Y1>){}; // expected-error {{rvalue of type 'Alias<B1>' (aka 'short')}}
N t25 = 0 ? (Alias<X1>){} : (Alias<X2>){}; // expected-error {{rvalue of type 'short'}}

template <class T, class U> concept C1 = true;
template <class T, class U> concept C2 = true;
C1<X1> auto t26_1 = (SB1){};
C1<X2> auto t26_2 = (::SB1){};
C2<X2> auto t26_3 = (::SB1){};
N t26 = 0 ? t26_1 : t26_2; // expected-error {{from 'SB1' (aka 'SS1')}}
N t27 = 0 ? t26_1 : t26_3; // expected-error {{from 'SB1' (aka 'SS1')}}
