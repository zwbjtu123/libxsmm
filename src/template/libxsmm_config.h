#ifndef LIBXSMM_CONFIG_H
#define LIBXSMM_CONFIG_H

#if !defined(LIBXSMM_DEFAULT_CONFIG) && defined(_WIN32)
#define LIBXSMM_DEFAULT_CONFIG
#endif

#if !defined(LIBXSMM_DEFAULT_CONFIG)

#define LIBXSMM_CONFIG_VERSION "$VERSION"
#define LIBXSMM_CONFIG_BRANCH "$BRANCH"
#define LIBXSMM_CONFIG_VERSION_MAJOR $MAJOR
#define LIBXSMM_CONFIG_VERSION_MINOR $MINOR
#define LIBXSMM_CONFIG_VERSION_UPDATE $UPDATE
#define LIBXSMM_CONFIG_VERSION_PATCH $PATCH
#define LIBXSMM_CONFIG_BUILD_DATE $DATE

#define LIBXSMM_CONFIG_CACHELINE $CACHELINE
#define LIBXSMM_CONFIG_ALIGNMENT $CACHELINE
#define LIBXSMM_CONFIG_MALLOC $MALLOC
#define LIBXSMM_CONFIG_ILP64 $ILP64
#define LIBXSMM_CONFIG_SYNC $SYNC
#define LIBXSMM_CONFIG_JIT $JIT

#define LIBXSMM_CONFIG_PREFETCH $PREFETCH
#define LIBXSMM_CONFIG_MAX_MNK $MAX_MNK
#define LIBXSMM_CONFIG_MAX_DIM $MAX_DIM
#define LIBXSMM_CONFIG_AVG_DIM $AVG_DIM
#define LIBXSMM_CONFIG_MAX_M $MAX_M
#define LIBXSMM_CONFIG_MAX_N $MAX_N
#define LIBXSMM_CONFIG_MAX_K $MAX_K
#define LIBXSMM_CONFIG_FLAGS $FLAGS
#define LIBXSMM_CONFIG_ALPHA $ALPHA
#define LIBXSMM_CONFIG_BETA $BETA
#define LIBXSMM_CONFIG_WRAP $WRAP
$LIBXSMM_OFFLOAD_BUILD

$MNK_PREPROCESSOR_LIST

#else

#define LIBXSMM_CONFIG_VERSION "unconfigured"
#define LIBXSMM_CONFIG_BRANCH "unconfigured"
#define LIBXSMM_CONFIG_VERSION_MAJOR INT_MAX
#define LIBXSMM_CONFIG_VERSION_MINOR INT_MAX
#define LIBXSMM_CONFIG_VERSION_UPDATE INT_MAX
#define LIBXSMM_CONFIG_VERSION_PATCH INT_MAX
#define LIBXSMM_CONFIG_BUILD_DATE INT_MAX

#define LIBXSMM_CONFIG_CACHELINE 64
#define LIBXSMM_CONFIG_ALIGNMENT LIBXSMM_CONFIG_CACHELINE
#define LIBXSMM_CONFIG_MALLOC 0
#define LIBXSMM_CONFIG_ILP64 0
#define LIBXSMM_CONFIG_SYNC 1
#define LIBXSMM_CONFIG_JIT 1

#define LIBXSMM_CONFIG_PREFETCH -1
#define LIBXSMM_CONFIG_MAX_MNK 262144
#define LIBXSMM_CONFIG_MAX_DIM 64
#define LIBXSMM_CONFIG_AVG_DIM 32
#define LIBXSMM_CONFIG_MAX_M 64
#define LIBXSMM_CONFIG_MAX_N 64
#define LIBXSMM_CONFIG_MAX_K 64
#define LIBXSMM_CONFIG_FLAGS 0
#define LIBXSMM_CONFIG_ALPHA 1
#define LIBXSMM_CONFIG_BETA 1
#define LIBXSMM_CONFIG_WRAP 1

#endif

#endif

