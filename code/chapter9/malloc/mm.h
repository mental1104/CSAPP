#ifdef __cplusplus 
extern "C" {
#endif
    int mm_init (void);
    void *mm_malloc (size_t size);
    void mm_free (void *ptr);
    void *mm_realloc(void *ptr, size_t size);
    int get_fragment();
    int cal_throughput();
#ifdef __cplusplus
}
#endif

