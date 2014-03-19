/*
Copyright 2014 Alex Dyachenko

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library.  If not, see http://www.gnu.org/licenses/.  
*/

#pragma once

using namespace System;

//defines a unary expression class
#define DEFINE_UNARY_EXPRESSION(name, type)                      \
public ref class Mpir##name##Expression : MpirExpression         \
{                                                                \
    internal:                                                    \
        type Operand;                                            \
        virtual void AssignTo(HugeInt^ destination) override;    \
                                                                 \
    public:                                                      \
        Mpir##name##Expression(type operand)                     \
        {                                                        \
            Operand = operand;                                   \
        }                                                        \
};

//defines a binary expression class
#define DEFINE_BINARY_EXPRESSION(name, leftType, rightType)      \
public ref class Mpir##name##Expression : MpirExpression         \
{                                                                \
    internal:                                                    \
        leftType Left;                                           \
        rightType Right;                                         \
        virtual void AssignTo(HugeInt^ destination) override;    \
                                                                 \
    public:                                                      \
        Mpir##name##Expression(leftType left, rightType right)   \
        {                                                        \
            Left = left;                                         \
            Right = right;                                       \
        }                                                        \
};

#define TYPE_FOR_ABBR_Int HugeInt^
#define TYPE_FOR_ABBR_Expr MpirExpression^
#define TYPE_FOR_ABBR_Si mpir_si
#define TYPE_FOR_ABBR_Ui mpir_ui
#define TYPE_FOR_ABBR_Bits mp_bitcnt_t

#define DEFINE_BINARY_EXPRESSION_WITH_TWO(name, typeAbbr, type)                     \
    DEFINE_BINARY_EXPRESSION(name##typeAbbr##typeAbbr, type, type)                  \
    DEFINE_BINARY_EXPRESSION(name##typeAbbr##Expr, type, MpirExpression^)           \
    DEFINE_BINARY_EXPRESSION(name##Expr##typeAbbr, MpirExpression^, type)           \
    DEFINE_BINARY_EXPRESSION(name##ExprExpr, MpirExpression^, MpirExpression^)    

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION(name##leftTypeAbbr##rightTypeAbbr, leftType, rightType)                            \
    DEFINE_BINARY_EXPRESSION(name##Expr##rightTypeAbbr, MpirExpression^, rightType)                            

#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION(name##leftTypeAbbr##rightTypeAbbr, leftType, rightType)                           \
    DEFINE_BINARY_EXPRESSION(name##leftTypeAbbr##Expr, leftType, MpirExpression^)                            
    
#define DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)    \
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(name, leftTypeAbbr, rightTypeAbbr, leftType, rightType)                \
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT(name, rightTypeAbbr, leftTypeAbbr, rightType, leftType)

#define MAKE_BINARY_OPERATOR_MAKE(op, result, leftType, rightType, left, right)       \
    static MpirExpression^ operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b) { return gcnew Mpir##result##Expression(left, right); }

#define MAKE_BINARY_OPERATOR_DECLARE(op, result, leftType, rightType, left, right)     \
    static MpirExpression^ operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b);

#define MAKE_BINARY_OPERATOR_DEFINE(op, result, leftType, rightType, left, right)      \
    MpirExpression^ MpirExpression::operator op(TYPE_FOR_ABBR_##leftType a, TYPE_FOR_ABBR_##rightType b) { return gcnew Mpir##result##Expression(left, right); }

#define MAKE_BINARY_OPERATOR_STANDARD(action, kind, op, result, leftType, rightType)   \
    MAKE_BINARY_OPERATOR_STANDARD_##kind(action, op, result, leftType, rightType)

#define MAKE_BINARY_OPERATOR_SIMPLE(action, kind, op, result, mpType, limbType)     \
    MAKE_BINARY_OPERATOR_SIMPLE_##kind(action, op, result, mpType, limbType)

#define MAKE_BINARY_OPERATOR_RLIMB(action, kind, op, result, mpType, limbType)      \
    MAKE_BINARY_OPERATOR_RLIMB_##kind(action, op, result, mpType, limbType)

#define MAKE_BINARY_OPERATOR_LLIMB(action, kind, op, result, mpType, limbType)      \
    MAKE_BINARY_OPERATOR_LLIMB_##kind(action, op, result, limbType, mpType)

#define MAKE_BINARY_OPERATOR_LLIMB_R(action, kind, op, result, mpType, limbType)      \
    MAKE_BINARY_OPERATOR_LLIMB_R_##kind(action, op, result, limbType, mpType)

#define MAKE_BINARY_OPERATOR_STANDARD_Int(action, op, result, leftType, rightType) \
    MAKE_BINARY_OPERATOR_##action(op, result##leftType##rightType, leftType, rightType, a, b)

#define MAKE_BINARY_OPERATOR_SIMPLE_Int   MAKE_BINARY_OPERATOR_STANDARD_Int
#define MAKE_BINARY_OPERATOR_RLIMB_Int    MAKE_BINARY_OPERATOR_STANDARD_Int
#define MAKE_BINARY_OPERATOR_LLIMB_Int    MAKE_BINARY_OPERATOR_STANDARD_Int

#define MAKE_BINARY_OPERATOR_LLIMB_R_Int(action, op, result, limbType, mpType) \
    MAKE_BINARY_OPERATOR_##action(op, result##mpType##limbType, limbType, mpType, b, a)

#define MAKE_BINARY_OPERATOR_STANDARD_Expr(action, op, result, leftType, rightType)        \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##rightType, Expr, rightType, a, b)      \
    MAKE_BINARY_OPERATOR_##action(op, result##leftType##Expr, leftType, Expr, a, b)        \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##Expr, Expr, Expr, a, b)               

#define MAKE_BINARY_OPERATOR_RLIMB_Expr(action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##limbType, Expr, limbType, a, b)          

#define MAKE_BINARY_OPERATOR_LLIMB_Expr(action, op, result, limbType, mpType)  \
    MAKE_BINARY_OPERATOR_##action(op, result##limbType##Expr, limbType, Expr, a, b)           

#define MAKE_BINARY_OPERATOR_LLIMB_R_Expr(action, op, result, limbType, mpType) \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##limbType, limbType, Expr, b, a)

#define MAKE_BINARY_OPERATOR_SIMPLE_Expr(action, op, result, mpType, limbType)  \
    MAKE_BINARY_OPERATOR_##action(op, result##Expr##limbType, Expr, limbType, a, b)          

#define DEFINE_OPERATORS(action, kind)                                        \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, +, Add, Int, Int)           \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, +, Add, Int, Ui)            \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, +, Add, Int, Si)            \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, +, Add, Int, Si)            \
                                                                              \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, -, Subtract, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB     (action, kind, -, Subtract, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, -, Subtract, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB     (action, kind, -, Subtract, Int, Si)       \
                                                                              \
    MAKE_BINARY_OPERATOR_STANDARD  (action, kind, *, Multiply, Int, Int)      \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, *, Multiply, Int, Ui)       \
    MAKE_BINARY_OPERATOR_RLIMB     (action, kind, *, Multiply, Int, Si)       \
    MAKE_BINARY_OPERATOR_LLIMB_R   (action, kind, *, Multiply, Int, Si)       \
                                                                              \
    MAKE_BINARY_OPERATOR_SIMPLE    (action, kind, <<, ShiftLeft, Int, Bits)   \

namespace MPIR
{
    ref class HugeInt;

    public ref class MpirExpression abstract
    {
        internal:
            virtual void AssignTo(HugeInt^ destination) abstract;

        public:
            DEFINE_OPERATORS(DECLARE, Expr)
    };

    DEFINE_BINARY_EXPRESSION_WITH_TWO(Add, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Add, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Add, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_TWO(Subtract, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(Subtract, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_LEFT_OR_RIGHT(Subtract, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_TWO(Multiply, Int, HugeInt^)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Multiply, Int, Ui, HugeInt^, mpir_ui)
    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(Multiply, Int, Si, HugeInt^, mpir_si)

    DEFINE_BINARY_EXPRESSION_WITH_BUILT_IN_RIGHT(ShiftLeft, Int, Bits, HugeInt^, mp_bitcnt_t)
    
    DEFINE_UNARY_EXPRESSION(Negate, HugeInt^)
    DEFINE_UNARY_EXPRESSION(Abs, HugeInt^)

    DEFINE_OPERATORS(DEFINE, Expr)

    public ref class HugeInt sealed : MpirExpression
    {
        internal:
            //fields
            __mpz_struct* _value;

        private:
            //construction
            void AllocateStruct()
            {
                _value = (__mpz_struct*)CustomAllocate(sizeof(__mpz_struct));
            }
            void DeallocateStruct()
            {
                mpz_clear(_value);
                CustomFree(_value);
                _value = nullptr;
            }
            //HugeInt(mpz_srcptr src);
            void FromString(String^ value, int base);

        public:
            //construction
            HugeInt();
            HugeInt(mp_bitcnt_t bits);
            HugeInt(String^ value);
            HugeInt(String^ value, int base);
            static HugeInt^ FromLong(mpir_si value);
            static HugeInt^ FromUlong(mpir_ui value);

            //disposal
            ~HugeInt();
            !HugeInt();

            //object overrides
            virtual String^ ToString() override;
            String^ ToString(int base);

            //properties
            property MpirExpression^ Value
            {
                void set(MpirExpression^ expr);
            }

            virtual void AssignTo(HugeInt^ destination) override
            {
                if(this != destination)
                    mpz_set(destination->_value, _value);
            }

            //arithmetic
            DEFINE_OPERATORS(MAKE, Int)

    static MpirNegateExpression^ operator-(HugeInt^ a) { return gcnew MpirNegateExpression(a); }                                                                      \
            MpirAbsExpression^ Abs() { return gcnew MpirAbsExpression(this); }
    };
};