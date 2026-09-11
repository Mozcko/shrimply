
#include <stdint.h>
#include <stddef.h>
typedef int CUresult;
typedef int CUdevice;
typedef int CUmemLocation;
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <stddef.h>
#include <stdint.h>

CUresult shrimply_cuda_init(unsigned flags) { return 0; }
CUresult shrimply_cuda_device_get(CUdevice *device, int ordinal) { return 0; }
CUresult shrimply_cuda_device_uuid(unsigned char *uuid, CUdevice device) { return 0; }
CUresult shrimply_cuda_primary_retain(void **context, CUdevice device) { return 0; }
CUresult shrimply_cuda_primary_release(CUdevice device) { return 0; }
CUresult shrimply_cuda_primary_get_state(CUdevice device, unsigned *flags,
                                         int *active) { return 0; }
CUresult shrimply_cuda_primary_set_flags(CUdevice device, unsigned flags) { return 0; }
CUresult shrimply_cuda_context_get_current(void **context) { return 0; }
CUresult shrimply_cuda_context_set_current(void *context) { return 0; }
CUresult shrimply_cuda_context_synchronize(void) { return 0; }
CUresult shrimply_cuda_context_push(void *context) { return 0; }
CUresult shrimply_cuda_context_pop(void **context) { return 0; }
CUresult shrimply_cuda_stream_create(void **stream) { return 0; }
CUresult shrimply_cuda_stream_destroy(void *stream) { return 0; }
CUresult shrimply_cuda_stream_synchronize(void *stream) { return 0; }
CUresult shrimply_cuda_stream_wait_event(void *stream, void *event) { return 0; }
CUresult shrimply_cuda_stream_wait_event_flags(void *stream, void *event,
                                               unsigned flags) { return 0; }
CUresult shrimply_cuda_event_create(void **event, unsigned flags) { return 0; }
CUresult shrimply_cuda_event_destroy(void *event) { return 0; }
CUresult shrimply_cuda_event_record(void *event, void *stream) { return 0; }
CUresult shrimply_cuda_event_synchronize(void *event) { return 0; }
CUresult shrimply_cuda_event_elapsed(float *milliseconds, void *start,
                                     void *end) { return 0; }
CUresult shrimply_cuda_module_load(void **module, const void *image) { return 0; }
CUresult shrimply_cuda_module_unload(void *module) { return 0; }
CUresult shrimply_cuda_module_function(void **function, void *module,
                                       const char *name) { return 0; }
CUresult shrimply_cuda_launch(void *function, unsigned gx, unsigned gy,
                              unsigned gz, unsigned bx, unsigned by,
                              unsigned bz, unsigned shared, void *stream,
                              void **arguments) { return 0; }
CUresult shrimply_cuda_mem_alloc(uint64_t *pointer, size_t bytes) { return 0; }
CUresult shrimply_cuda_mem_free(uint64_t pointer) { return 0; }
CUresult shrimply_cuda_memcpy_htod_async(uint64_t destination,
                                         const void *source, size_t bytes,
                                         void *stream) { return 0; }
CUresult shrimply_cuda_memcpy_htod(uint64_t destination, const void *source,
                                   size_t bytes) { return 0; }
CUresult shrimply_cuda_memcpy_dtoh_async(void *destination, uint64_t source,
                                         size_t bytes, void *stream) { return 0; }
CUresult shrimply_cuda_memcpy_dtod_async(uint64_t destination, uint64_t source,
                                         size_t bytes, void *stream) { return 0; }
CUresult shrimply_cuda_memset_async(uint64_t destination, unsigned char value,
                                    size_t bytes, void *stream) { return 0; }
CUresult shrimply_cuda_mem_alloc_managed(uint64_t *pointer, size_t bytes,
                                         unsigned flags) { return 0; }
CUresult shrimply_cuda_mem_get_info(size_t *free_bytes, size_t *total_bytes) { return 0; }

CUresult shrimply_cuda_memcpy_2d(const void *descriptor) { return 0; }
CUresult shrimply_cuda_memcpy_2d_async(const void *descriptor, void *stream) { return 0; }
CUresult shrimply_cuda_pointer_get_attribute(void *data, unsigned attribute,
                                             uint64_t pointer) { return 0; }
CUresult shrimply_cuda_import_external_memory(void **memory,
                                              const void *descriptor) { return 0; }
CUresult shrimply_cuda_external_memory_get_buffer(uint64_t *pointer,
                                                  void *memory,
                                                  const void *descriptor) { return 0; }
CUresult
shrimply_cuda_external_memory_get_mipmapped_array(void **array, void *memory,
                                                  const void *descriptor) { return 0; }
CUresult shrimply_cuda_destroy_external_memory(void *memory) { return 0; }
CUresult shrimply_cuda_mipmapped_array_get_level(void **array, void *mipmapped,
                                                 unsigned level) { return 0; }
CUresult shrimply_cuda_mipmapped_array_destroy(void *mipmapped) { return 0; }
CUresult shrimply_cuda_import_external_semaphore(void **semaphore,
                                                 const void *descriptor) { return 0; }
CUresult shrimply_cuda_wait_external_semaphores(const void *semaphores,
                                                const void *parameters,
                                                unsigned count, void *stream) { return 0; }
CUresult shrimply_cuda_destroy_external_semaphore(void *semaphore) { return 0; }
CUresult shrimply_cuda_graphics_map(unsigned count, void **resources,
                                    void *stream) { return 0; }
CUresult shrimply_cuda_graphics_unmap(unsigned count, void **resources,
                                      void *stream) { return 0; }
CUresult shrimply_cuda_graphics_mapped_array(void **array, void *resource,
                                             unsigned array_index,
                                             unsigned mip_level) { return 0; }
CUresult shrimply_cuda_graphics_unregister(void *resource) { return 0; }
CUresult shrimply_cuda_error_name(CUresult result, const char **name) { return 0; }
CUresult shrimply_cuda_error_string(CUresult result, const char **description) { return 0; }
