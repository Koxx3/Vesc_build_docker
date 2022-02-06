/*
    Copyright 2019, 2021, 2022 Joel Svensson   svenssonjoel@yahoo.se
                          2022 Benjamin Vedder

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <lbm_types.h>
#include "symrepr.h"
#include "stack.h"
#include "heap.h"
#include "eval_cps.h"
#include "print.h"
#include <stdio.h>
#include <math.h>

static lbm_uint add2(lbm_uint a, lbm_uint b) {

  lbm_uint retval = lbm_enc_sym(SYM_TERROR);

  if (!(lbm_is_number(a) && lbm_is_number(b))) {
    return retval;
  }

  lbm_uint t = (lbm_type_of(a) < lbm_type_of(b)) ? lbm_type_of(b) : lbm_type_of(a);
  switch (t) {
  case LBM_VAL_TYPE_I: retval = lbm_enc_i(lbm_dec_as_i(a) + lbm_dec_as_i(b)); break;
  case LBM_VAL_TYPE_U: retval = lbm_enc_u(lbm_dec_as_u(a) + lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_U: retval = lbm_enc_U(lbm_dec_as_u(a) + lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_I: retval = lbm_enc_I(lbm_dec_as_i(a) + lbm_dec_as_i(b)); break;
  case LBM_PTR_TYPE_BOXED_F: retval = lbm_enc_F(lbm_dec_as_f(a) + lbm_dec_as_f(b)); break;
  }
  return retval;
}

static lbm_uint mul2(lbm_uint a, lbm_uint b) {

  lbm_uint retval = lbm_enc_sym(SYM_TERROR);

  if (!(lbm_is_number(a) && lbm_is_number(b))) {
    return retval;
  }

  lbm_uint t = (lbm_type_of(a) < lbm_type_of(b)) ? lbm_type_of(b) : lbm_type_of(a);
  switch (t) {
  case LBM_VAL_TYPE_I: retval = lbm_enc_i(lbm_dec_as_i(a) * lbm_dec_as_i(b)); break;
  case LBM_VAL_TYPE_U: retval = lbm_enc_u(lbm_dec_as_u(a) * lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_U: retval = lbm_enc_U(lbm_dec_as_u(a) * lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_I: retval = lbm_enc_I(lbm_dec_as_i(a) * lbm_dec_as_i(b)); break;
  case LBM_PTR_TYPE_BOXED_F: retval = lbm_enc_F(lbm_dec_as_f(a) * lbm_dec_as_f(b)); break;
  }
  return retval;
}

static lbm_uint div2(lbm_uint a, lbm_uint b) {

  lbm_uint retval = lbm_enc_sym(SYM_TERROR);

  if (!(lbm_is_number(a) && lbm_is_number(b))) {
    return retval;
  }

  lbm_uint t = (lbm_type_of(a) < lbm_type_of(b)) ? lbm_type_of(b) : lbm_type_of(a);
  switch (t) {
  case LBM_VAL_TYPE_I: if (lbm_dec_i(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_i(lbm_dec_as_i(a) / lbm_dec_as_i(b)); break;
  case LBM_VAL_TYPE_U: if (lbm_dec_as_u(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_u(lbm_dec_as_u(a) / lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_U: if (lbm_dec_as_u(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_U(lbm_dec_as_u(a) / lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_I: if (lbm_dec_as_i(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_I(lbm_dec_as_i(a) / lbm_dec_as_i(b)); break;
  case LBM_PTR_TYPE_BOXED_F: if (lbm_dec_as_f(b) == 0.0 || lbm_dec_as_f(b) == -0.0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_F(lbm_dec_as_f(a) / lbm_dec_as_f(b)); break;
  }
  return retval;
}

static lbm_uint mod2(lbm_uint a, lbm_uint b) {

  lbm_uint retval = lbm_enc_sym(SYM_TERROR);

  if (!(lbm_is_number(a) && lbm_is_number(b))) {
    return retval;
  }

  lbm_uint t = (lbm_type_of(a) < lbm_type_of(b)) ? lbm_type_of(b) : lbm_type_of(a);
  switch (t) {
  case LBM_VAL_TYPE_I: if (lbm_dec_i(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_i(lbm_dec_as_i(a) % lbm_dec_as_i(b)); break;
  case LBM_VAL_TYPE_U: if (lbm_dec_as_u(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_u(lbm_dec_as_u(a) % lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_U: if (lbm_dec_as_u(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_U(lbm_dec_as_u(a) % lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_I: if (lbm_dec_as_i(b) == 0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_I(lbm_dec_as_i(a) % lbm_dec_as_i(b)); break;
  case LBM_PTR_TYPE_BOXED_F: if (lbm_dec_as_f(b) == 0.0 || lbm_dec_as_f(b) == -0.0) {return lbm_enc_sym(SYM_DIVZERO);} retval = lbm_enc_F(fmodf(lbm_dec_as_f(a), lbm_dec_as_f(b))); break;
  }
  return retval;
}

static lbm_uint negate(lbm_uint a) {

  lbm_uint retval = lbm_enc_sym(SYM_TERROR);
  lbm_int i0;
  lbm_uint u0;
  lbm_float f0;

  if (lbm_type_of(a) > LBM_VAL_TYPE_CHAR) {
    switch (lbm_type_of(a)) {
    case LBM_VAL_TYPE_I:
      i0 = lbm_dec_i(a);
      retval = lbm_enc_i(-i0);
      break;
    case LBM_VAL_TYPE_U:
      u0 = lbm_dec_u(a);
      retval = lbm_enc_u(-u0);
      break;
    case LBM_PTR_TYPE_BOXED_U:
      u0 = lbm_dec_U(a);
      retval = lbm_enc_U(-u0); //cons(-u0, enc_sym(SYM_BOXED_U_TYPE));
      break;
    case LBM_PTR_TYPE_BOXED_I:
      i0 = lbm_dec_I(a);
      retval = lbm_enc_I(-i0); //cons(-i0, enc_sym(SYM_BOXED_I_TYPE));
      break;
    case LBM_PTR_TYPE_BOXED_F:
      f0 = lbm_dec_F(a);
      f0 = -f0;
      //memcpy(&retval, &f0, sizeof(FLOAT));
      retval = lbm_enc_F(f0); //cons(retval, enc_sym(SYM_BOXED_F_TYPE));
      break;
    }
  }
  return retval;
}

static lbm_uint sub2(lbm_uint a, lbm_uint b) {

  lbm_uint retval = lbm_enc_sym(SYM_TERROR);

  if (!(lbm_is_number(a) && lbm_is_number(b))) {
    return retval;
  }

  lbm_uint t = (lbm_type_of(a) < lbm_type_of(b)) ? lbm_type_of(b) : lbm_type_of(a);
  switch (t) {
  case LBM_VAL_TYPE_I: retval = lbm_enc_i(lbm_dec_as_i(a) - lbm_dec_as_i(b)); break;
  case LBM_VAL_TYPE_U: retval = lbm_enc_u(lbm_dec_as_u(a) - lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_U: retval = lbm_enc_U(lbm_dec_as_u(a) - lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_I: retval = lbm_enc_I(lbm_dec_as_i(a) - lbm_dec_as_i(b)); break;
  case LBM_PTR_TYPE_BOXED_F: retval = lbm_enc_F(lbm_dec_as_f(a) - lbm_dec_as_f(b)); break;
  }
  return retval;
}

static bool array_equality(lbm_value a, lbm_value b) {
  if (lbm_type_of(a) == LBM_PTR_TYPE_ARRAY &&
      lbm_type_of(a) == lbm_type_of(b)) {
    lbm_array_header_t *a_ = (lbm_array_header_t*)lbm_car(a);
    lbm_array_header_t *b_ = (lbm_array_header_t*)lbm_car(b);

    if (a_->elt_type == b_->elt_type &&
        a_->size == b_->size) {
      switch(a_->elt_type) {
      case LBM_VAL_TYPE_U:
      case LBM_PTR_TYPE_BOXED_U:
        if (memcmp((char*)a_->data, (char*)b_->data, a_->size * sizeof(lbm_uint)) == 0) return true;
        break;
      case LBM_VAL_TYPE_I:
      case LBM_PTR_TYPE_BOXED_I:
        if (memcmp((char*)a_->data, (char*)b_->data, a_->size * sizeof(lbm_int)) == 0) return true;
        break;
      case LBM_VAL_TYPE_CHAR:
        if (memcmp((char*)a_->data, (char*)b_->data, a_->size) == 0) return true;
        break;
      case LBM_PTR_TYPE_BOXED_F:
        if (memcmp((char*)a_->data, (char*)b_->data, a_->size * sizeof(lbm_float)) == 0) return true;
        break;
      default:
        break;
      }
    }
  }
  return false;
}

static bool struct_eq(lbm_value a, lbm_value b) {

  if (!lbm_is_ptr(a) && !lbm_is_ptr(b)) {
    if (lbm_type_of(a) == lbm_type_of(b)){
      switch (lbm_type_of(a)) {
      case LBM_VAL_TYPE_SYMBOL:
        return (lbm_dec_sym(a) == lbm_dec_sym(b));
      case LBM_VAL_TYPE_I:
        return (lbm_dec_i(a) == lbm_dec_i(b));
      case LBM_VAL_TYPE_U:
        return (lbm_dec_u(a) == lbm_dec_u(b));
      case LBM_VAL_TYPE_CHAR:
        return (lbm_dec_char(a) == lbm_dec_char(b));
      default:
        return false;
        break;
      }
    } else {
      return false;
    }
  }

  if (lbm_is_ptr(a) && lbm_is_ptr(b)) {
    if (lbm_type_of(a) == lbm_type_of(b)) {
      switch (lbm_type_of(a)) {
      case LBM_PTR_TYPE_CONS:
        return ( struct_eq(lbm_car(a),lbm_car(b)) &&
                 struct_eq(lbm_cdr(a),lbm_cdr(b)) );
      case LBM_PTR_TYPE_BOXED_I:
        return ((lbm_int)lbm_car(a) == (lbm_int)lbm_car(b));
      case LBM_PTR_TYPE_BOXED_U:
        return (lbm_car(a) == lbm_car(b));
      case LBM_PTR_TYPE_BOXED_F:
        return ((lbm_float)lbm_car(a) == (lbm_float)lbm_car(b));
      case LBM_PTR_TYPE_ARRAY:
        return array_equality(a, b);
      default:
        return false;
      }
    }
  }
  return false;
}

static int cmpi(lbm_int a, lbm_int b) {
  int res = (a > b) - (a < b);
  return res;
}

static int cmpu(lbm_uint a, lbm_uint b) {
  int res = (a > b) - (a < b);
  return res;
}

static int cmpf(lbm_float a, lbm_float b) {
  int res = (a > b) - (a < b);
  return  res;
}


/* returns -1 if a < b; 0 if a = b; 1 if a > b */
static int compare(lbm_uint a, lbm_uint b) {

  int retval = 0;

  if (!(lbm_is_number(a) && lbm_is_number(b))) {
    return retval;
  }

  lbm_uint t = (lbm_type_of(a) < lbm_type_of(b)) ? lbm_type_of(b) : lbm_type_of(a);
  switch (t) {
  case LBM_VAL_TYPE_I: retval = cmpi(lbm_dec_as_i(a), lbm_dec_as_i(b)); break;
  case LBM_VAL_TYPE_U: retval = cmpu(lbm_dec_as_u(a), lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_U: retval = cmpu(lbm_dec_as_u(a), lbm_dec_as_u(b)); break;
  case LBM_PTR_TYPE_BOXED_I: retval = cmpi(lbm_dec_as_i(a), lbm_dec_as_i(b)); break;
  case LBM_PTR_TYPE_BOXED_F: retval = cmpf(lbm_dec_as_f(a), lbm_dec_as_f(b)); break;
  }
  return retval;
}


void array_read(lbm_value *args, lbm_uint nargs, lbm_uint *result) {
  (void) nargs;
  if (nargs < 2) return;
  // Args are: array, index
  lbm_value arr = args[0];
  lbm_value index = args[1];
  lbm_value index_end = index;
  lbm_value acc = lbm_enc_sym(SYM_NIL);
  lbm_value curr = lbm_enc_sym(SYM_EERROR);
  bool read_many = false;

  if (nargs > 2) {
    index_end = args[2];
    read_many = true;
  }

  // Get array index
  lbm_uint ix;
  lbm_uint ix_end;

  if (lbm_is_number(index) && lbm_is_number(index_end)) {
    ix = lbm_dec_as_u(index);
    ix_end = lbm_dec_as_u(index_end);
  } else {
    return;
  }

  if (ix > ix_end) {
    lbm_uint tmp = ix;
    ix = ix_end;
    ix_end = tmp;
  }

  if (lbm_type_of(arr) == LBM_PTR_TYPE_ARRAY) {
    lbm_array_header_t *array = (lbm_array_header_t*)lbm_car(arr);
    uint32_t* data = array->data;

//    printf("ix: %d, ix_end: %d\n", ix, ix_end);
    for (lbm_int i = (lbm_int)ix_end; i >= (lbm_int)ix; i--) {
//      printf("%d\n", i);
      if ((lbm_uint)i >= array->size){
//        printf("hmm %d %d\n", i, array->size);
        *result = lbm_enc_sym(SYM_NIL);
        return;
      }

      switch(array->elt_type) {
      case LBM_VAL_TYPE_CHAR:
        curr = lbm_enc_char((lbm_uint) ((char*)data)[i]);
        break;
      case LBM_VAL_TYPE_U:
        curr = lbm_enc_u(((lbm_uint*)data)[i]);
        break;
      case LBM_VAL_TYPE_I:
         curr = lbm_enc_i(((lbm_int*)data)[i]);
        break;
      case LBM_PTR_TYPE_BOXED_U:
        curr = lbm_cons(((lbm_uint*)data)[i], lbm_enc_sym(SYM_BOXED_U_TYPE));
        if (lbm_type_of(curr) == LBM_VAL_TYPE_SYMBOL) return;
        curr = lbm_set_ptr_type(curr, LBM_PTR_TYPE_BOXED_U);
        break;
      case LBM_PTR_TYPE_BOXED_I:
        curr = lbm_cons(((lbm_uint*)data)[i], lbm_enc_sym(SYM_BOXED_I_TYPE));
        if (lbm_type_of(curr) == LBM_VAL_TYPE_SYMBOL) return;
        curr = lbm_set_ptr_type(curr, LBM_PTR_TYPE_BOXED_I);
        break;
      case LBM_PTR_TYPE_BOXED_F:
        curr = lbm_cons(((lbm_uint*)data)[i], lbm_enc_sym(SYM_BOXED_F_TYPE));
        if (lbm_type_of(*result) == LBM_VAL_TYPE_SYMBOL) return;
        curr = lbm_set_ptr_type(curr, LBM_PTR_TYPE_BOXED_F);
        break;
      default:
        curr = lbm_enc_sym(SYM_EERROR);
        break;
      }
      if (read_many) {
        acc = lbm_cons(curr, acc);
      }
    } /* for i */
  }
  if (read_many) {
    *result = acc;
  } else {
    *result = curr;
  }
}

void array_write(lbm_value *args, lbm_uint nargs, lbm_uint *result) {
  (void) nargs;
  lbm_value arr = args[0];
  lbm_value index = args[1];
  lbm_value val = args[2];
  lbm_uint ix;

  *result = lbm_enc_sym(SYM_EERROR);

  if (lbm_is_number(index)) {
    ix = lbm_dec_as_u(index);
  } else {
    return;
  }

  if (lbm_type_of(arr) == LBM_PTR_TYPE_ARRAY) {
    lbm_array_header_t *array = (lbm_array_header_t*)lbm_car(arr);


    if (lbm_type_of(val) != array->elt_type ||
        ix >= array->size) {
      *result =  lbm_enc_sym(SYM_NIL);
      return;
    }

    switch(array->elt_type) {
    case LBM_VAL_TYPE_CHAR: {
      char * data = (char *)array->data;
      data[ix] = lbm_dec_char(val);
      break;
    }
    case LBM_VAL_TYPE_U: {
      lbm_uint* data = (lbm_uint*)array->data;
      data[ix] = lbm_dec_u(val);
      break;
    }
    case LBM_VAL_TYPE_I: {
      lbm_int *data = (lbm_int*)array->data;
      data[ix] = lbm_dec_i(val);
      break;
    }
    case LBM_PTR_TYPE_BOXED_U: {
      lbm_uint *data = (lbm_uint*)array->data;
      data[ix] = lbm_dec_U(val);
      break;
    }
    case LBM_PTR_TYPE_BOXED_I: {
      lbm_int *data = (lbm_int*)array->data;
      data[ix] = lbm_dec_I(val);
      break;
    }
    case LBM_PTR_TYPE_BOXED_F: {
      //uv = car(val);
      //memcpy(&v, &uv, sizeof(FLOAT));
      lbm_uint *data = (lbm_uint*)array->data;
      data[ix] = lbm_car(val);
      break;
    }
    default:
      // Maybe result should be something else than arr here.
      break;
    }
    *result = arr;
    return;
  }
}


//void array_create(lbm_value *args, lbm_uint nargs, lbm_uint *result) {
//  (void) args;
//  (void) nargs;
//  (void) result;
//
//}


lbm_value index_list(lbm_value l, unsigned int n) {
  lbm_value curr = l;
  while ( lbm_type_of(curr) == LBM_PTR_TYPE_CONS &&
          n > 0) {
    curr = lbm_cdr(curr);
    n --;
  }
  if (lbm_type_of(curr) == LBM_PTR_TYPE_CONS) {
    return lbm_car(curr);
  } else {
    return lbm_enc_sym(SYM_NIL);
  }
}

lbm_value lbm_fundamental(lbm_value* args, lbm_uint nargs, lbm_value op) {

  lbm_uint result = lbm_enc_sym(SYM_EERROR);
  int cmp_res = -1;

  switch (lbm_dec_sym(op)) {
  case SYM_IX:
    if (nargs == 2 && lbm_is_number(args[0])) {
      result = index_list(args[1], lbm_dec_as_u(args[0]));
    }
  break;
  case SYM_DECODE:
    if (nargs == 1 && (lbm_is_number(args[0]) ||
                       lbm_is_char(args[0]))) {
      switch (lbm_type_of(args[0])) {
      case LBM_VAL_TYPE_CHAR:
        /*fall through*/
      case LBM_VAL_TYPE_I:
        /* fall through */
      case LBM_VAL_TYPE_U: {
        lbm_uint v = lbm_dec_as_u(args[0]);
        result = lbm_cons(lbm_enc_u(v & 0xFF), lbm_enc_sym(SYM_NIL));
        result = lbm_cons(lbm_enc_u(v >> 8 & 0xFF), result);
        result = lbm_cons(lbm_enc_u(v >> 16 & 0xFF), result);
        result = lbm_cons(lbm_enc_u(v >> 24 & 0xF), result);
      } break;
      case LBM_PTR_TYPE_BOXED_F: {
        lbm_float tmp = lbm_dec_F(args[0]);
        lbm_uint  v;
        memcpy(&v, &tmp, sizeof(lbm_uint));
        result = lbm_cons(lbm_enc_u(v & 0xFF), lbm_enc_sym(SYM_NIL));
        result = lbm_cons(lbm_enc_u(v >> 8 & 0xFF), result);
        result = lbm_cons(lbm_enc_u(v >> 16 & 0xFF), result);
        result = lbm_cons(lbm_enc_u(v >> 24 & 0xFF), result);
      } break;
      case LBM_PTR_TYPE_BOXED_I:
        /* fall through */
      case LBM_PTR_TYPE_BOXED_U: {
        lbm_uint v = lbm_dec_as_u(args[0]);
        result = lbm_cons(lbm_enc_u(v & 0xFF), lbm_enc_sym(SYM_NIL));
        result = lbm_cons(lbm_enc_u(v >> 8 & 0xFF), result);
        result = lbm_cons(lbm_enc_u(v >> 16 & 0xFF), result);
        result = lbm_cons(lbm_enc_u(v >> 24 & 0xFF), result);
      } break;
      } // close if
    }break;
  /// Encode a list of up to 4 bytes as an i32
  case SYM_ENCODE_I32:
    if (nargs == 1 && lbm_type_of(args[0]) == LBM_PTR_TYPE_CONS) {
      lbm_value curr = args[0];
      lbm_uint r = 0;
      int n = 4;
      while (lbm_type_of(curr) == LBM_PTR_TYPE_CONS && n > 0) {
        if (n < 4) r = r << 8;
        if (lbm_is_number(lbm_car(curr))) {
          uint32_t v = lbm_dec_as_u(lbm_car(curr));
          r |= v;
          n --;
          curr = lbm_cdr(curr);
        } else {
          break;
        }
      }
      result = lbm_enc_I((lbm_int)r);
    }
    break;
  /// Encode a list of up to 4 bytes as an U32
  case SYM_ENCODE_U32:
      if (nargs == 1 && lbm_type_of(args[0]) == LBM_PTR_TYPE_CONS) {
        lbm_value curr = args[0];
        lbm_uint r = 0;
        int n = 4;
        while (lbm_type_of(curr) == LBM_PTR_TYPE_CONS && n > 0) {
          if (n < 4) r = r << 8;
          if (lbm_is_number(lbm_car(curr))) {
            uint32_t v = lbm_dec_as_u(lbm_car(curr));
            r |= v;
            n --;
            curr = lbm_cdr(curr);
          } else {
            break;
          }
        }
        result = lbm_enc_U(r);
      }
      break;
  /// Encode a list of up to 4 bytes as an U32
  case SYM_ENCODE_FLOAT:
    if (nargs == 1 && lbm_type_of(args[0]) == LBM_PTR_TYPE_CONS) {
      lbm_value curr = args[0];
      lbm_uint r = 0;
      lbm_float f;
      int n = 4;
      while (lbm_type_of(curr) == LBM_PTR_TYPE_CONS && n > 0) {
        if (n < 4) r = r << 8;
        if (lbm_is_number(lbm_car(curr))) {
          uint32_t v = lbm_dec_as_u(lbm_car(curr));
          r |= v;
          n --;
          curr = lbm_cdr(curr);
        } else {
          break;
        }
      }
      memcpy(&f,&r, sizeof(lbm_uint));
      result = lbm_enc_F(f);
    }
    break;
  case SYM_IS_FUNDAMENTAL:
    if (nargs < 1 ||
        lbm_type_of(args[0]) != LBM_VAL_TYPE_SYMBOL)
      result = lbm_enc_sym(SYM_NIL);
    else if (lbm_is_fundamental(args[0]))
      result = lbm_enc_sym(SYM_TRUE);
    else
      result = lbm_enc_sym(SYM_NIL);
    break;

  case SYM_SYMBOL_TO_STRING: {
    if (nargs < 1 ||
        lbm_type_of(args[0]) != LBM_VAL_TYPE_SYMBOL)
      return lbm_enc_sym(SYM_NIL);
    lbm_value sym = args[0];
    const char *sym_str = lbm_get_name_by_symbol(lbm_dec_sym(sym));
    if (sym_str == NULL) return lbm_enc_sym(SYM_NIL);
    size_t len = strlen(sym_str);

    lbm_value v;
    if (lbm_heap_allocate_array(&v, len+1, LBM_VAL_TYPE_CHAR)) {
      lbm_array_header_t *arr = (lbm_array_header_t*)lbm_car(v);
      if (!arr) return lbm_enc_sym(SYM_MERROR);
      char *data = (char *)arr+8;
      memset(data,0,len+1);
      memcpy(data,sym_str,len);
    } else {
      return lbm_enc_sym(SYM_MERROR);
    }
    result = v;
    break;
  }
  case SYM_STRING_TO_SYMBOL: {
    result = lbm_enc_sym(SYM_EERROR);
    if (nargs < 1 ||
        lbm_type_of(args[0] != LBM_PTR_TYPE_ARRAY))
      break;
    lbm_array_header_t *arr = (lbm_array_header_t *)lbm_car(args[0]);
    if (arr->elt_type != LBM_VAL_TYPE_CHAR)
      break;
    char *str = (char *)arr+8;
    lbm_uint sym;
    if (lbm_get_symbol_by_name(str, &sym)) {
      result = lbm_enc_sym(sym);
    } else if (lbm_add_symbol(str, &sym)) {
      result = lbm_enc_sym(sym);
    }
    break;
  }
  case SYM_SYMBOL_TO_UINT: {
    lbm_value s = args[0];
    if (lbm_type_of(s) == LBM_VAL_TYPE_SYMBOL)
      result = lbm_enc_u(lbm_dec_sym(s));
    else
      result = lbm_enc_sym(SYM_EERROR);
    break;
  }
  case SYM_UINT_TO_SYMBOL: {
    lbm_value s = args[0];
    if (lbm_type_of(s) == LBM_VAL_TYPE_U)
      result = lbm_enc_sym(lbm_dec_u(s));
    else
      result = lbm_enc_sym(SYM_EERROR);
    break;
  }
  case SYM_SET_CAR:
    if (nargs == 2) {
      if (lbm_set_car(args[0],args[1])) {
        result = lbm_enc_sym(SYM_TRUE);
      } else {
        result = lbm_enc_sym(SYM_NIL);
      }
    }
    break;
  case SYM_SET_CDR:
    if (nargs == 2) {
      if (lbm_set_cdr(args[0],args[1])) {
        result = lbm_enc_sym(SYM_TRUE);
      } else {
        result = lbm_enc_sym(SYM_NIL);
      }
    }
    break;
  case SYM_CONS: {
    lbm_uint a = args[0];
    lbm_uint b = args[1];
    result = lbm_cons(a,b);
    break;
  }
  case SYM_CAR: {
    result = lbm_car(args[0]);
    break;
  }
  case SYM_CDR: {
    result = lbm_cdr(args[0]);
    break;
  }
  case SYM_LIST: {
    result = lbm_enc_sym(SYM_NIL);
    for (lbm_uint i = 1; i <= nargs; i ++) {
      result = lbm_cons(args[nargs-i], result);
      if (lbm_type_of(result) == LBM_VAL_TYPE_SYMBOL)
        break;
    }
    break;
  }
  case SYM_APPEND: {
    if (nargs != 2) break;

    lbm_value a = args[0];
    lbm_value b = args[1];

    result = b;
    lbm_value curr = a;
    int n = 0;
    while (lbm_type_of(curr) == LBM_PTR_TYPE_CONS) {
      n++;
      curr = lbm_cdr(curr);
    }

    for (int i = n-1; i >= 0; i --) {
      result = lbm_cons(index_list(a,(unsigned int)i), result);
      if (lbm_type_of(result) == LBM_VAL_TYPE_SYMBOL)
        break;
    }
    break;
  }
  case SYM_ADD: {
    lbm_uint sum = lbm_enc_u(0);
    for (lbm_uint i = 0; i < nargs; i ++) {
      sum = add2(sum, args[i]);
      if (lbm_type_of(sum) == LBM_VAL_TYPE_SYMBOL) {
        break;
      }
    }
    result = sum;
    break;
  }
  case SYM_SUB: {
    lbm_uint res = nargs == 0 ? lbm_enc_u(0) : args[0];

    if (nargs == 1) {
      res = negate(res);
    } else {
      for (lbm_uint i = 1; i < nargs; i ++) {
        res = sub2(res, args[i]);
        if (lbm_type_of(res) == LBM_VAL_TYPE_SYMBOL)
          break;
      }
    }
    result = res;
    break;
  }
  case SYM_MUL: {
    lbm_uint prod = lbm_enc_u(1);
    for (lbm_uint i = 0; i < nargs; i ++) {
      prod = mul2(prod, args[i]);
      if (lbm_type_of(prod) == LBM_VAL_TYPE_SYMBOL) {
        break;
      }
    }
    result = prod;
    break;
  }
  case SYM_DIV:  {
    if (nargs >= 1) {
      lbm_uint res = args[0];
      for (lbm_uint i = 1; i < nargs; i ++) {
        res = div2(res, args[i]);
        if (lbm_type_of(res) == LBM_VAL_TYPE_SYMBOL) {
          break;
        }
      }
      result = res;
    } else {
      result = lbm_enc_sym(SYM_EERROR);
    }
    break;
  }
  case SYM_MOD: {
    lbm_uint res = args[0];
    for (lbm_uint i = 1; i < nargs; i ++) {
      res = mod2(res, args[i]);
      if (lbm_type_of(res) == LBM_VAL_TYPE_SYMBOL) {
        break;
      }
    }
    result = res;
    break;
  }
  case SYM_EQ: {
    lbm_uint a = args[0];
    lbm_uint b;
    bool r = true;

    for (lbm_uint i = 1; i < nargs; i ++) {
      b = args[i];
      r = r && struct_eq(a, b);
    }
    if (r) {
      result = lbm_enc_sym(SYM_TRUE);
    } else {
      result = lbm_enc_sym(SYM_NIL);
    }
    break;
  }
  case SYM_NUMEQ:
    cmp_res = 0;
    /* fall through */
  case SYM_GT:
    if (lbm_dec_sym(op) == SYM_GT) cmp_res = 1;
    /* fall through */
  case SYM_LT: {
    lbm_uint a = args[0];
    lbm_uint b;
    bool r = true;
    bool ok = true;

    if (!lbm_is_number(a)) {
      result = lbm_enc_sym(SYM_TERROR);
      break;
    }
    for (lbm_uint i = 1; i < nargs; i ++) {
      b = args[i];
      if (!lbm_is_number(b)) {
        ok = false;
        break;
      }
      r = r && (compare(a, b) == cmp_res);
    }
    if (ok) {
      if (r) {
        result = lbm_enc_sym(SYM_TRUE);
      } else {
        result = lbm_enc_sym(SYM_NIL);
      }
    } else {
      result = lbm_enc_sym(SYM_TERROR);
    }
    break;
  }
  case SYM_NOT: {
    if (nargs == 0) {
      return lbm_enc_sym(SYM_NIL);
      break;
    }
    lbm_uint a = args[0];
    if (lbm_type_of(a) == LBM_VAL_TYPE_SYMBOL &&
        lbm_dec_sym(a) == SYM_NIL) {
      result = lbm_enc_sym(SYM_TRUE);
      break;
    }
    result = lbm_enc_sym(SYM_NIL);
    break;
  }
  case SYM_ARRAY_READ:
    array_read(args, nargs, &result);
    break;
  case SYM_ARRAY_WRITE:
    array_write(args, nargs, &result);
    break;
//  case SYM_ARRAY_CREATE:
//    array_create(args, nargs, &result);
//    break;
  case SYM_TYPE_OF:
    if (nargs != 1) return lbm_enc_sym(SYM_NIL);
    lbm_value val = args[0];
    switch(lbm_type_of(val)) {
    case LBM_PTR_TYPE_CONS:
      return lbm_enc_sym(SYM_TYPE_LIST);
    case LBM_PTR_TYPE_ARRAY:
      return lbm_enc_sym(SYM_TYPE_ARRAY);
    case LBM_PTR_TYPE_BOXED_I:
      return lbm_enc_sym(SYM_TYPE_I32);
    case LBM_PTR_TYPE_BOXED_U:
      return lbm_enc_sym(SYM_TYPE_U32);
    case LBM_PTR_TYPE_BOXED_F:
      return lbm_enc_sym(SYM_TYPE_FLOAT);
    case LBM_VAL_TYPE_I:
      return lbm_enc_sym(SYM_TYPE_I28);
    case LBM_VAL_TYPE_U:
      return lbm_enc_sym(SYM_TYPE_U28);
    case LBM_VAL_TYPE_CHAR:
      return lbm_enc_sym(SYM_TYPE_CHAR);
    case LBM_VAL_TYPE_SYMBOL:
      return lbm_enc_sym(SYM_TYPE_SYMBOL);
    default:
      return lbm_enc_sym(SYM_TERROR);
    }
    break;
  default:
//    printf("fundamental unknown\n");
    result = lbm_enc_sym(SYM_EERROR);
    break;
  }

  return result;
}
