
#include <stdlib.h>
#include <stdio.h>

#include "heap.h"
#include "symrepr.h"


int main(int argc, char **argv) {

  int res = 1;

  lbm_cons_t *heap_storage = NULL;
  
  unsigned int heap_size = 1024 * 1024; 
  uint32_t cell;

  res = lbm_symrepr_init();
  if (!res) {
    printf("Error initializing symrepr\n");
    return 0;
  }
  printf("Initialized symrepr: OK\n"); 

  heap_storage = (lbm_cons_t*)malloc(sizeof(lbm_cons_t) * heap_size);
  if (heap_storage == NULL) {
    return 0;
  }
  
  res = lbm_heap_init(heap_storage,heap_size);
  if (!res) {
    printf("Error initializing heap\n"); 
    return 0;
  }

  printf("Initialized heap: OK\n"); 
  
  for (int i = 0; i < heap_size; i ++) {
    cell = lbm_heap_allocate_cell(LBM_PTR_TYPE_CONS);
    if (!lbm_is_ptr(cell)) {
      printf("Error allocating cell %d\n", i); 
      return 0;
    }
  }
  printf("Allocated %d heap cells: OK\n", heap_size);

  for (int i = 0; i < 34; i ++) {
    cell = lbm_heap_allocate_cell(LBM_PTR_TYPE_CONS);
    if (lbm_is_ptr(cell)) {
      printf("Error allocation succeeded on empty heap\n"); 
      return 0;
    } else if (lbm_type_of(cell) != LBM_VAL_TYPE_SYMBOL ||
	       lbm_dec_sym(cell) != SYM_MERROR) {
      printf("Error Incorrect return value at cell allocation on full heap\n");
      return 0; 
    }
  }

  printf("HEAP allocation when full test: OK\n");
  return 1; 
  
}
