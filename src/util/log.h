#ifndef _UTIL_LOG_H
#define _UTIL_LOG_H

/* Dumb logger utils (tuned for simplicity). Notes:
 * - must set callback/defaults to print anything
 * - mainly used to print info for users, like format detected but wrong size
 *   (don't clutter by logging code that happens most of the time, since log may be shared with other plugins)
 * - callbacks receive formed string for simplicity (to be adjusted)
 * - DO NOT put logs in tight loops (like decoders), slow fn calls and clutter
 *   (metas are usually fine but also consider cluttering)
 * - as compiler must support variable args, needs to pass VGM_LOG_OUTPUT flag to enable
 * - debug logs are removed unless VGM_DEBUG_OUTPUT is passed to compiler args
 *   (passing either of them works)
 * - callback should be thread-safe (no internal checks and only one log ATM)
 * - still WIP, some stuff not working ATM or may change
 */

// void (*callback)(int level, const char* str);
void vgm_log_set_callback(void* ctx_p, int level, int type, void* callback);

#if defined(VGM_LOG_OUTPUT) || defined(VGM_DEBUG_OUTPUT)
    void vgm_logi(/*void* ctx,*/ const char* fmt, ...);
    //void vgm_logi_once(/*void* ctx, int* once_flag, */ const char* fmt, ...);
#else
    #define vgm_logi(...) /* nothing */
#endif

#ifdef VGM_DEBUG_OUTPUT
    void vgm_logd(/*void* ctx,*/ const char* fmt, ...);
    #define VGM_LOG(...) do { vgm_logd(__VA_ARGS__); } while (0)
#else
    #define vgm_logd(...) /* nothing */
    #define VGM_LOG(...) /* nothing */
#endif


/* original stdout logging for debugging and regression testing purposes, may be removed later.
 * Needs C99 variadic macros, uses do..while to force ";" as statement */
#ifdef VGM_DEBUG_OUTPUT

/* equivalent to printf when condition is true */
#define VGM_ASSERT(condition, ...) \
    do { if (condition) {printf(__VA_ARGS__);} } while (0)

#define VGM_ASSERT_ONCE(condition, ...) \
    do { static int written; if (!written) { if (condition) {printf(__VA_ARGS__); written = 1;} }  } while (0)

/* equivalent to printf */
//#define VGM_LOG(...) do { printf(__VA_ARGS__); } while (0)

#define VGM_LOG_ONCE(...) \
    do { static int written; if (!written) { printf(__VA_ARGS__); written = 1; } } while (0)

/* prints file/line/func */
//#define VGM_LOGF()  do { printf("%s:%i '%s'\n",  __FILE__, __LINE__, __func__); } while (0)

/* prints to a file */
#define VGM_LOGT(txt, ...) \
    do { FILE *fl = fopen(txt,"a+"); if(fl){fprintf(fl,__VA_ARGS__); fflush(fl);} fclose(fl); } while(0)

/* prints a buffer/array */
#define VGM_LOGB(buf, buf_size, bytes_per_line) \
    do { \
        int i; \
        for (i=0; i < buf_size; i++) { \
            printf("%02x",buf[i]); \
            if (bytes_per_line && (i+1) % bytes_per_line == 0) printf("\n"); \
        } \
        printf("\n"); \
    } while (0)

#else/*VGM_DEBUG_OUTPUT*/

#define VGM_ASSERT(condition, ...) /* nothing */
#define VGM_ASSERT_ONCE(condition, ...) /* nothing */
//#define VGM_LOG(...) /* nothing */
#define VGM_LOG_ONCE(...) /* nothing */
//#define VGM_LOGF() /* nothing */
#define VGM_LOGT() /* nothing */
#define VGM_LOGB(buf, buf_size, bytes_per_line) /* nothing */

#endif/*VGM_DEBUG_OUTPUT*/

#endif
