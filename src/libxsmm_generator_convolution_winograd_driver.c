/******************************************************************************
** Copyright (c) 2016-2019, Intel Corporation                                **
** All rights reserved.                                                      **
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions        **
** are met:                                                                  **
** 1. Redistributions of source code must retain the above copyright         **
**    notice, this list of conditions and the following disclaimer.          **
** 2. Redistributions in binary form must reproduce the above copyright      **
**    notice, this list of conditions and the following disclaimer in the    **
**    documentation and/or other materials provided with the distribution.   **
** 3. Neither the name of the copyright holder nor the names of its          **
**    contributors may be used to endorse or promote products derived        **
**    from this software without specific prior written permission.          **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  **
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
******************************************************************************/
/* Kunal Banerjee (Intel Corp.), Alexander Heinecke (Intel Corp.)
******************************************************************************/
#include <libxsmm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


LIBXSMM_INLINE void print_help(void) {
  printf("\nwrong usage -> exit!\n\n\n");
  printf("Usage (batched-dense*batched-dense=batched-dense):\n");
  printf("    dense, dense_asm\n");
  printf("    filename to append\n");
  printf("    routine name\n");
  printf("    itiles\n");
  printf("    jtiles\n");
  printf("    bimg\n");
  printf("    ur\n");
  printf("    ARCH: knm, knl, skx\n");
  printf("    PREFETCH: nopf, all\n");
  printf("    PRECISION: SP\n");
  printf("    Pass: fwd, bwd, upd");
  printf("    ur_ifm:");
  printf("\n\n\n\n");
}


int main(int argc, char* argv []) {
  libxsmm_convolution_winograd_descriptor l_conv_desc;
  char* l_type;
  char* l_file_out;
  char* l_routine_name;
  char* l_arch;
  char* l_precision;
  const char* l_pass = "fwd";
  int l_itiles = 0;
  int l_jtiles = 0;
  int l_bimg = 0;
  int l_ur   = 0;
  int l_single_precision = 0;
  int flag_ur = 0;
  int l_ur_ifm = 0;
  libxsmm_convolution_prefetch_type l_prefetch;

  /* check argument count for a valid range */
  if (argc != 13) {
    print_help();
    return EXIT_FAILURE;
  }

  /* names of files and routines */
  l_type = argv[1];
  l_file_out = argv[2];
  l_routine_name = argv[3];

  /* xgemm sizes */
  l_itiles = atoi(argv[4]);
  l_jtiles = atoi(argv[5]);
  l_bimg = atoi(argv[6]);
  l_ur = atoi(argv[7]);

  /* arch specific stuff */
  l_arch = argv[8];
  l_precision = argv[10];

  l_pass= argv[11];
  l_ur_ifm = atoi(argv[12]);

  /* some initial parameters checks */
  /* check for sparse / dense only */
  if ( (strcmp(l_type, "dense")     != 0) &&
       (strcmp(l_type, "dense_asm") != 0) ) {
    print_help();
    return EXIT_FAILURE;
  }

  /* set value of prefetch flag */
  if (strcmp("nopf", argv[9]) == 0) {
    l_prefetch = LIBXSMM_CONVOLUTION_PREFETCH_NONE;
  } else if (strcmp("all", argv[9]) == 0) {
    l_prefetch = LIBXSMM_CONVOLUTION_PREFETCH_ALL;
  } else {
    print_help();
    return EXIT_FAILURE;
  }

  /* check value of arch flag */
  if ( (strcmp(l_arch, "knm") != 0) &&
       (strcmp(l_arch, "knl") != 0) &&
       (strcmp(l_arch, "skx") != 0) ) {
    print_help();
    return EXIT_FAILURE;
  }

  /* check and evaluate precision flag */
  if ( strcmp(l_precision, "SP") == 0 ) {
    l_single_precision = 1;
    LIBXSMM_UNUSED(l_single_precision/*TODO*/);
  } else {
    print_help();
    return EXIT_FAILURE;
  }

  /* check value of pass flag */
  if ( (strcmp(l_pass, "fwd") != 0) &&
       (strcmp(l_pass, "bwd") != 0) &&
       (strcmp(l_pass, "upd") != 0) ) {
    print_help();
    return EXIT_FAILURE;
  }

  if ( !flag_ur && (0 != (l_itiles*l_jtiles*l_bimg % l_ur)) ) {
    printf("\n(itiles*jtiles*bimg) = %d must be perfectly divisible by ur %d\n", l_itiles*l_jtiles*l_bimg, l_ur);
    return EXIT_FAILURE;
  }

  l_conv_desc.itiles = l_itiles;
  l_conv_desc.jtiles = l_jtiles;
  l_conv_desc.bimg = l_bimg;
  l_conv_desc.ur = l_ur;
  l_conv_desc.ur_ifm = l_ur_ifm;
  l_conv_desc.prefetch = l_prefetch;

  if ( (strcmp(l_type, "dense")     == 0) ||
       (strcmp(l_type, "dense_asm") == 0) ) {
    if ( flag_ur ) {
      if ( (strcmp(l_arch, "knm") == 0) &&
           (strcmp(l_pass, "upd") == 0) ) {
        l_conv_desc.ur = libxsmm_product_limit(l_conv_desc.itiles * l_conv_desc.jtiles * l_conv_desc.bimg / 4, 26, 0);
      } else {
        l_conv_desc.ur = libxsmm_product_limit(l_conv_desc.itiles * l_conv_desc.jtiles * l_conv_desc.bimg, 26, 0);
      }
    }

    if ( strcmp(l_type, "dense") == 0 ) {
      if ( strcmp(l_pass, "upd") == 0 ) {
        libxsmm_generator_convolution_winograd_weight_update_inlineasm( l_file_out, l_routine_name, &l_conv_desc, l_arch );
      } else {
        libxsmm_generator_convolution_winograd_forward_inlineasm( l_file_out, l_routine_name, &l_conv_desc, l_arch );
      }
    } else {
      if ( strcmp(l_pass, "upd") == 0 ) {
        libxsmm_generator_convolution_winograd_weight_update_directasm( l_file_out, l_routine_name, &l_conv_desc, l_arch );
      } else {
        libxsmm_generator_convolution_winograd_forward_directasm( l_file_out, l_routine_name, &l_conv_desc, l_arch );
      }
    }
  }

  return EXIT_SUCCESS;
}
