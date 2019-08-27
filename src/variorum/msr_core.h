#ifndef MSR_CORE_H_INCLUDE
#define MSR_CORE_H_INCLUDE

#include <linux/types.h>
#include <stdint.h>
#include <sys/types.h>

#define X86_IOC_MSR_BATCH _IOWR('c', 0xA2, struct msr_batch_array)
#define MSR_BATCH_DIR "/dev/cpu/msr_batch"
#define FILENAME_SIZE 1024

#ifndef NAME_MAX
#define NAME_MAX 1024
#endif

/// @brief Enum encompassing type of data being read to/written from MSRs.
enum variorum_data_type_e
{
    /// @brief Energy, time, and power measurements of various RAPL power
    /// domains.
    RAPL_DATA = 0,
    /// @brief Units for energy, time, and power across all RAPL power domains.
    RAPL_UNIT = 1,
    /// @brief Fixed-function counter measurements (i.e., instructions retired,
    /// reference clock cycles, CPU cycles).
    FIXED_COUNTERS_DATA = 2,
    /// @brief Controls for fixed-function counters (i.e., instructions retired,
    /// reference clock cycles, CPU cycles).
    FIXED_COUNTERS_CTRL_DATA = 3,
    /// @brief General-purpose performance counter measurements.
    COUNTERS_DATA = 4,
    /// @brief Controls for general-purpose performance counters and
    /// performance event select counter measurements.
    COUNTERS_CTRL = 5,
    /// @brief Clock cycle measurements based on fixed frequency and actual
    /// frequency of the processor.
    CLOCKS_DATA = 6,
    /// @brief Instantaneous operating frequency of the core or socket.
    PERF_DATA = 7,
    /// @brief Thermal status of core.
    THERM_STAT = 8,
    /// @brief Interrupts by thermal monitor when thermal sensor on a core is
    /// tripped.
    THERM_INTERR = 9,
    /// @brief Thermal status of package.
    PKG_THERM_STAT = 10,
    /// @brief Interrupts by thermal monitor when thermal sensor on the package
    /// is tripped.
    PKG_THERM_INTERR = 11,
    /// @brief Current temperature of the package.
    TEMP_TARGET = 12,
    /// @brief Software desired operating frequency of the core or socket.
    PERF_CTRL = 13,
    /// @brief Measured time spent in C-states by the package.
    PKG_CRESIDENCY = 14,
    /// @brief Measured time spent in C-states by the core.
    CORE_CRESIDENCY = 15,
    /// @brief Uncore performance event select counter measurements.
    UNCORE_EVTSEL = 16,
    /// @brief Uncore general-performance counter measurements.
    UNCORE_COUNT = 17,
    /// @brief User-defined batch MSR data.
    USR_BATCH0 = 18,
    /// @brief User-defined batch MSR data.
    USR_BATCH1 = 19,
    /// @brief User-defined batch MSR data.
    USR_BATCH2 = 20,
    /// @brief User-defined batch MSR data.
    USR_BATCH3 = 21,
    /// @brief User-defined batch MSR data.
    USR_BATCH4 = 22,
    /// @brief User-defined batch MSR data.
    USR_BATCH5 = 23,
    /// @brief User-defined batch MSR data.
    USR_BATCH6 = 24,
    /// @brief User-defined batch MSR data.
    USR_BATCH7 = 25,
    /// @brief User-defined batch MSR data.
    USR_BATCH8 = 26,
    /// @brief User-defined batch MSR data.
    USR_BATCH9 = 27,
    /// @brief User-defined batch MSR data.
    USR_BATCH10 = 28,
    PLATFORM_INFO = 29,
};

/// @brief Enum encompassing batch operations.
enum variorum_batch_op_type_e
{
    /// @brief Load batch operation.
    BATCH_LOAD,
    /// @brief Write batch operation.
    BATCH_WRITE,
    /// @brief Read batch operation.
    BATCH_READ,
};

/// @brief Structure holding multiple read/write operations to various MSRs.
struct msr_batch_array
{
    /// @brief Number of operations to execute.
    __u32 numops;
    /// @brief Array of length numops of operations to execute.
    struct msr_batch_op *ops;
};

/// @brief Structure holding information for a single read/write operation to
/// an MSR.
struct msr_batch_op
{
    /// @brief CPU where rdmsr/wrmsr will be executed.
    __u16 cpu;
    /// @brief Identify if operation is rdmsr (non-zero) or wrmsr (0).
    __u16 isrdmsr;
    /// @brief Stores error code.
    __s32 err;
    /// @brief Address of MSR to perform operation.
    __u32 msr;
    /// @brief Stores input to or result from operation.
    __u64 msrdata;
    /// @brief Write mask applied to wrmsr.
    __u64 wmask;
};

// Depending on their scope, MSRs can be written to or read from at either the
// socket (aka package/cpu) or core level, and possibly the hardware thread
// level.
//
//  read/write_msr reads from core 0.
//  read/write_msr_all_cores_v uses a vector of values.
//  write_msr_all_cores writes all cores with a single value.
//  read/write_msr_single_core contains all of the low-level logic.
//	The rest of the functions are wrappers that call these two functions.

/// @brief Validate specific socket exists in the platform configuration.
///
/// @param [in] socket Unique socket/package identifier.
///
/// @param [in] location Line number in source file where error occurred (use
///        standard predefined macro __LINE__).
///
/// @param [in] file Name of source file where error occurred (use standard
///        predefined macro __FILE__).
///
/// @return 0 if successful, else -1 if socket requested is greater than number
/// of sockets in the platform.
int sockets_assert(const unsigned *socket,
                   const int location,
                   const char *file);

/// @brief Validate specific thread exists in the platform configuration.
///
/// @param [in] thread Unique thread identifier.
///
/// @param [in] location Line number in source file where error occurred (use
///        standard predefined macro __LINE__).
///
/// @param [in] file Name of source file where error occurred (use standard
///        predefined macro __FILE__).
///
/// @return 0 if successful, else -1 if thread requested is greater than number
/// of threads per core in the platform.
int threads_assert(const unsigned *thread,
                   const int location,
                   const char *file);

/// @brief Validate specific core exists in the platform configuration.
///
/// @param [in] core Unique core identifier.
///
/// @param [in] location Line number in source file where error occurred (use
///        standard predefined macro __LINE__).
///
/// @param [in] file Name of source file where error occurred (use standard
///        predefined macro __FILE__).
///
/// @return 0 if successful, else -1 if core requested is greater than number
/// of cores per socket in the platform.
int cores_assert(const unsigned *core,
                 const int location,
                 const char *file);

/// @brief Check status of a file.
///
/// @param [in] filename File to check status of.
///
/// @param [in] kerneltype OS privilege level (ring 0-ring 3).
///
/// @param [in] dev_idx Unique logical processor index.
///
/// @return 0 if successful, else -1 if can't find any msr module with RW
/// permissions.
int stat_module(char *filename,
                int *kerneltype,
                int *dev_idx);

/// @brief Open the MSR module file descriptors exposed in the /dev filesystem.
///
/// @return 0 if initialization was a success, else -1 if could not stat file
/// descriptors or open any msr module.
int init_msr(void);

/// @brief Close the MSR module file descriptors exposed in the /dev
/// filesystem.
///
/// @return 0 if finalization was a success, else -1 if could not close file
/// descriptors.
int finalize_msr(void);

/// @brief Write a new value to an MSR based on the coordinates of a core or
/// thread.
///
/// A user can request to write to core 4 on socket 1, instead of having to map
/// this core to a continuous value based on the the number of cores on socket
/// 0. For a dual socket system with 8 cores, socket 0 would have cores 0-7,
/// core 0 on socket 1 would be mapped to index 8.
///
/// @param [in] socket Unique socket/package identifier.
///
/// @param [in] core Unique core identifier.
///
/// @param [in] thread Unique thread identifier.
///
/// @param [in] msr Address of register to write.
///
/// @param [in] val Value to write to MSR.
///
/// @return 0 if write_msr_by_idx() was a success, else -1 if the file
/// descriptor was NULL or if the number of bytes written was not the size of
/// uint64_t.
int write_msr_by_coord(unsigned socket,
                       unsigned core,
                       unsigned thread,
                       off_t msr,
                       uint64_t val);

/// @brief Read current value of an MSR based on the coordinates of a core or
/// thread.
///
/// A user can request to read from core 4 on socket 1, instead of having to map
/// this core to a continuous value based on the the number of cores on socket
/// 0. For a dual socket system with 8 cores, socket 0 would have cores 0-7,
/// core 0 on socket 1 would be mapped to index 8.
///
/// @param [in] socket Unique socket/package identifier.
///
/// @param [in] core Unique core identifier.
///
/// @param [in] thread Unique thread identifier.
///
/// @param [in] msr Address of register to read.
///
/// @param [out] val Value read from MSR.
///
/// @return 0 if read_msr_by_idx() was a success, else -1 if the file
/// descriptor was NULL or if the number of bytes read was not the size of
/// uint64_t.
int read_msr_by_coord(unsigned socket,
                      unsigned core,
                      unsigned thread,
                      off_t msr,
                      uint64_t *val);

/// @brief Read current value of an MSR based on the index of a core or
/// thread.
///
/// A user can request to read from index 8, which is core 0 on socket 1 in a
/// dual socket system with 8 cores per socket.  This index is a continuous
/// value based on the the number of cores on socket 0.
///
/// @param [in] dev_idx Unique device identifier.
///
/// @param [in] msr Address of register to read.
///
/// @param [out] val Value read from MSR.
///
/// @return 0 if successful, else -1 if file descriptor was NULL or if the
/// number of bytes read was not the size of uint64_t.
int read_msr_by_idx(int dev_idx,
                    off_t msr,
                    uint64_t *val);

/// @brief Write new value to an MSR based on the index of a core or thread.
///
/// A user can request to read from index 8, which is core 0 on socket 1 in a
/// dual socket system with 8 cores per socket.  This index is a continuous
/// value based on the the number of cores on socket 0.
///
/// @param [in] dev_idx Unique device identifier.
///
/// @param [in] msr Address of register to read.
///
/// @param [out] val Value read from MSR.
///
/// @return 0 if successful, else -1 if file descriptor was NULL or if the
/// number of bytes read was not the size of uint64_t.
int write_msr_by_idx(int dev_idx,
                     off_t msr,
                     uint64_t val);

int write_msr_by_coord(unsigned socket,
                       unsigned core,
                       unsigned thread,
                       off_t msr,
                       uint64_t val);

int load_thread_batch(off_t msr,
                      uint64_t **val,
                      int batchnum);

int load_socket_batch(off_t msr,
                      uint64_t **val,
                      int batchnum);

int allocate_batch(int batchnum,
                   size_t bsize);

int read_batch(const int batchnum);

int write_batch(const int batchnum);

int create_batch_op(off_t msr,
                    uint64_t cpu,
                    uint64_t **dest,
                    const int batchnum);

#endif
