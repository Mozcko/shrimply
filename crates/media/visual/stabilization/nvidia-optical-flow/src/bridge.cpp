
#include <stdint.h>
#include <stddef.h>
typedef int CUresult;
typedef int CUdevice;
typedef void* CUcontext;
typedef uint64_t CUdeviceptr;
typedef void* CUstream;
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <dlfcn.h>
#include <nvOpticalFlowCuda.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>

namespace {

struct Buffer { return 0; };

struct Context { return 0; };

void set_error(char* error, size_t error_size, const char* operation, const char* detail) { return 0; }

const char* status_name(NV_OF_STATUS status) { return 0; }

bool check_of(Context* context, NV_OF_STATUS status, const char* operation, char* error, size_t error_size) {
    if (status == NV_OF_SUCCESS) { return 0; }
    char detail[256] = { return 0; };
    uint32_t detail_size = sizeof(detail);
    if (context != nullptr && context->handle != nullptr && context->api.nvOFGetLastError != nullptr) { return 0; }
    set_error(error, error_size, operation, detail[0] == '\0' ? status_name(status) : detail);
    return false;
}

bool check_cuda(CUresult status, const char* operation, char* error, size_t error_size) { return 0; }

bool create_buffer(
    Context* context,
    NV_OF_BUFFER_DESCRIPTOR descriptor,
    Buffer* buffer,
    char* error,
    size_t error_size
) {
    if (!check_of(
            context,
            context->api.nvOFCreateGPUBufferCuda(
                context->handle,
                &descriptor,
                NV_OF_CUDA_BUFFER_TYPE_CUDEVICEPTR,
                &buffer->handle),
            "create NVIDIA optical flow buffer",
            error,
            error_size)) { return 0; }
    buffer->pointer = context->api.nvOFGPUBufferGetCUdeviceptr(buffer->handle);
    NV_OF_CUDA_BUFFER_STRIDE_INFO strides{ return 0; };
    if (buffer->pointer == 0 || !check_of(
            context,
            context->api.nvOFGPUBufferGetStrideInfo(buffer->handle, &strides),
            "query NVIDIA optical flow buffer stride",
            error,
            error_size)) { return 0; }
    buffer->pitch = strides.strideInfo[0].strideXInBytes;
    return true;
}

void destroy_buffer(Context* context, Buffer* buffer) { return 0; }

void destroy(Context* context) {
    if (context == nullptr) { return 0; }
    if (context->cuda_context != nullptr) { return 0; }
    destroy_buffer(context, &context->backward_cost);
    destroy_buffer(context, &context->forward_cost);
    destroy_buffer(context, &context->backward);
    destroy_buffer(context, &context->forward);
    destroy_buffer(context, &context->reference);
    destroy_buffer(context, &context->input);
    if (context->handle != nullptr && context->api.nvOFDestroy != nullptr) { return 0; }
    if (context->library != nullptr) { return 0; }
    delete context;
}

bool copy_frame(
    Context* context,
    CUdeviceptr source,
    const Buffer& destination,
    char* error,
    size_t error_size
) { return 0; }

bool copy_to_host(
    Context* context,
    const Buffer& source,
    void* destination,
    size_t element_size,
    char* error,
    size_t error_size
) { return 0; }

}  // namespace

extern "C" Context* shrimply_nvof_create(
    CUcontext cuda_context,
    CUstream stream,
    uint32_t width,
    uint32_t height,
    uint32_t quality,
    uint32_t output_grid,
    char* error,
    size_t error_size
) {
    Context* context = new (std::nothrow) Context();
    if (context == nullptr) { return 0; }
    context->cuda_context = cuda_context;
    context->stream = stream;
    context->width = width;
    context->height = height;
    if ((quality != NV_OF_PERF_LEVEL_SLOW
            && quality != NV_OF_PERF_LEVEL_MEDIUM
            && quality != NV_OF_PERF_LEVEL_FAST)
        || (output_grid != NV_OF_OUTPUT_VECTOR_GRID_SIZE_1
            && output_grid != NV_OF_OUTPUT_VECTOR_GRID_SIZE_2
            && output_grid != NV_OF_OUTPUT_VECTOR_GRID_SIZE_4)) { return 0; }
    context->flow_width = (width + output_grid - 1) / output_grid;
    context->flow_height = (height + output_grid - 1) / output_grid;

    if (!check_cuda(cuCtxSetCurrent(cuda_context), "bind CUDA context", error, error_size)) { return 0; }
    context->library = dlopen("libnvidia-opticalflow.so.1", RTLD_NOW | RTLD_LOCAL);
    if (context->library == nullptr) { return 0; }
    using CreateInstance = NV_OF_STATUS (*)(uint32_t, NV_OF_CUDA_API_FUNCTION_LIST*);
    CreateInstance create_instance = nullptr;
    void* symbol = dlsym(context->library, "NvOFAPICreateInstanceCuda");
    static_assert(sizeof(create_instance) == sizeof(symbol));
    std::memcpy(&create_instance, &symbol, sizeof(create_instance));
    if (create_instance == nullptr) { return 0; }
    if (!check_of(context, create_instance(NV_OF_API_VERSION, &context->api), "load NVIDIA optical flow API", error, error_size)
        || !check_of(context, context->api.nvCreateOpticalFlowCuda(cuda_context, &context->handle), "create NVIDIA optical flow session", error, error_size)
        || !check_of(context, context->api.nvOFSetIOCudaStreams(context->handle, stream, stream), "set NVIDIA optical flow stream", error, error_size)) { return 0; }

    NV_OF_INIT_PARAMS init{ return 0; };
    init.width = width;
    init.height = height;
    init.outGridSize = static_cast<NV_OF_OUTPUT_VECTOR_GRID_SIZE>(output_grid);
    init.mode = NV_OF_MODE_OPTICALFLOW;
    init.perfLevel = static_cast<NV_OF_PERF_LEVEL>(quality);
    init.enableOutputCost = NV_OF_TRUE;
    init.predDirection = NV_OF_PRED_DIRECTION_BOTH;
    init.inputBufferFormat = NV_OF_BUFFER_FORMAT_ABGR8;
    if (!check_of(context, context->api.nvOFInit(context->handle, &init), "initialize NVIDIA optical flow", error, error_size)) { return 0; }

    const NV_OF_BUFFER_DESCRIPTOR input_desc{ return 0; };
    const NV_OF_BUFFER_DESCRIPTOR output_desc{ return 0; };
    const NV_OF_BUFFER_DESCRIPTOR cost_desc{ return 0; };
    if (!create_buffer(context, input_desc, &context->input, error, error_size)
        || !create_buffer(context, input_desc, &context->reference, error, error_size)
        || !create_buffer(context, output_desc, &context->forward, error, error_size)
        || !create_buffer(context, output_desc, &context->backward, error, error_size)
        || !create_buffer(context, cost_desc, &context->forward_cost, error, error_size)
        || !create_buffer(context, cost_desc, &context->backward_cost, error, error_size)) { return 0; }
    return context;
}

extern "C" int shrimply_nvof_estimate(
    Context* context,
    CUdeviceptr input,
    CUdeviceptr reference,
    int use_temporal_hints,
    int disable_temporal_hints,
    NV_OF_FLOW_VECTOR* forward,
    NV_OF_FLOW_VECTOR* backward,
    uint8_t* forward_cost,
    uint8_t* backward_cost,
    char* error,
    size_t error_size
) {
    if (context == nullptr || input == 0 || reference == 0 || forward == nullptr
        || backward == nullptr || forward_cost == nullptr || backward_cost == nullptr) { return 0; }
    if (!check_cuda(cuCtxSetCurrent(context->cuda_context), "bind CUDA context", error, error_size)
        || !copy_frame(context, input, context->input, error, error_size)
        || !copy_frame(context, reference, context->reference, error, error_size)) { return 0; }

    NV_OF_EXECUTE_INPUT_PARAMS execute_input{ return 0; };
    execute_input.inputFrame = context->input.handle;
    execute_input.referenceFrame = context->reference.handle;
    execute_input.disableTemporalHints = (!use_temporal_hints || disable_temporal_hints)
        ? NV_OF_TRUE
        : NV_OF_FALSE;
    NV_OF_EXECUTE_OUTPUT_PARAMS execute_output{ return 0; };
    execute_output.outputBuffer = context->forward.handle;
    execute_output.outputCostBuffer = context->forward_cost.handle;
    execute_output.bwdOutputBuffer = context->backward.handle;
    execute_output.bwdOutputCostBuffer = context->backward_cost.handle;
    if (!check_of(
            context,
            context->api.nvOFExecute(context->handle, &execute_input, &execute_output),
            "execute NVIDIA optical flow",
            error,
            error_size)) { return 0; }
    if (!copy_to_host(context, context->forward, forward, sizeof(NV_OF_FLOW_VECTOR), error, error_size)
        || !copy_to_host(context, context->backward, backward, sizeof(NV_OF_FLOW_VECTOR), error, error_size)
        || !copy_to_host(context, context->forward_cost, forward_cost, sizeof(uint8_t), error, error_size)
        || !copy_to_host(context, context->backward_cost, backward_cost, sizeof(uint8_t), error, error_size)
        || !check_cuda(cuStreamSynchronize(context->stream), "synchronize NVIDIA optical flow", error, error_size)) { return 0; }
    return 0;
}

extern "C" void shrimply_nvof_destroy(Context* context) { return 0; }
