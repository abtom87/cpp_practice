#include <cstddef> // For size_t
#include <cstring> // Required for memset
#include <iostream>
#include <stdexcept> // For runtime_error
#include <vector>

// --- 1. Define the Buffer Size ---
// We will allocate 1MB (1024 * 1024 bytes) of memory upfront.
constexpr size_t BUFFER_SIZE = 1024 * 1024;

static char m_buffer[BUFFER_SIZE];

/**
 * @brief A simple Monotonic Buffer Allocator.
 * * This allocator allocates memory linearly from a single large buffer.
 * It is fast (O(1) allocation) but does not support individual deallocation.
 * Memory is only reclaimed when the allocator object itself is destroyed.
 */
class MonoAllocator {
public:
  MonoAllocator() : m_current(m_buffer) {
    std::cout << "MonoAllocator initialized with " << BUFFER_SIZE
              << " bytes.\n";

    std::printf("%p\n", m_current);
  }

  // Since we manage the memory locally, we need to disable copying
  MonoAllocator(const MonoAllocator &) = delete;
  MonoAllocator &operator=(const MonoAllocator &) = delete;

  /**
   * @brief Allocates memory from the buffer.
   * @param size The number of bytes requested.
   * @param alignment The required alignment for the memory block (e.g., 4, 8,
   * 16).
   * @return A pointer to the allocated memory block.
   */
  void *allocate(size_t size, size_t alignment = 8) {
    // --- 1. Calculate Required Alignment Offset ---
    // This is necessary to ensure the returned pointer satisfies the alignment
    // requirement for the data type being stored (e.g., int, double).
    size_t current_address = reinterpret_cast<size_t>(m_current);

    size_t alignment_offset =
        (alignment - (current_address % alignment)) % alignment;

    // Calculate the address after alignment
    char *aligned_ptr = m_current + alignment_offset;

    // --- Optional Zeroing of Padding ---
    // If the offset is greater than zero, we have padding.
    if (alignment_offset > 0) {
      // memset(start_address, byte_value, number_of_bytes);
      // We write '0' into the bytes between m_current (start of padding)
      // and aligned_ptr (end of padding).
      std::memset(m_current, 0, alignment_offset);
      std::cout << "  - Padded and zeroed " << alignment_offset << " bytes.\n";
    }

    // Calculate the new 'next' pointer location
    char *next_ptr = aligned_ptr + size;

    // --- 2. Check for Overrun ---
    if (next_ptr > m_buffer + BUFFER_SIZE) {
      throw std::runtime_error("MonoAllocator: Out of memory in buffer!");
    }

    // --- 3. Allocate and Advance Pointer (The O(1) step) ---
    // Note: The allocated 'size' block itself is not zeroed here,
    // only the padding is.
    m_current = next_ptr;

    std::cout << "  - Allocated " << size
              << " bytes. Next address: " << reinterpret_cast<void *>(m_current)
              << "\n";

    return aligned_ptr;
  }

  /**
   * @brief Deallocation is a NO-OP (No-Operation).
   * * Since this is a monotonic allocator, it doesn't support individual
   * deallocation. The entire buffer is reset when the allocator object goes out
   * of scope.
   */
  void deallocate(void *ptr, size_t size, size_t alignment = 8) {
    // This function is required by the standard allocator interface,
    // but its body is intentionally left empty for this type of allocator.
    // The memory is only freed when the MonoAllocator object is destroyed.
  }

  /**
   * @brief Resets the allocator pointer to the start of the buffer.
   * This effectively "frees" all previously allocated memory instantly.
   */
  void reset() {
    m_current = m_buffer;
    std::cout
        << "\nMonoAllocator reset. All previous memory is now available.\n";
  }

private:
  // The pre-allocated memory buffer on the stack (or static memory).
  // static char m_buffer[BUFFER_SIZE];

  // Pointer to the next available byte in the buffer.
  char *m_current;
};

// --- Main Demonstration ---

int main() {
  try {
    // Create an instance of our custom allocator
    MonoAllocator allocator;
    std::printf("%p\n", &allocator);

    std::cout << "\n--- Phase 1: Allocating multiple objects ---\n";

    // Allocation 1: 40 bytes. Assumed to be naturally aligned at the start.
    int *data1 =
        static_cast<int *>(allocator.allocate(10 * sizeof(int), alignof(int)));
    data1[0] = 42;
    std::cout << "  - Data 1 (int[10]) stored: " << data1[0] << "\n";

    // Allocation 2: 40 bytes (5 doubles). This is allocated immediately after
    // Allocation 1 (40 bytes). If m_current was at address 1000, it's now at
    // 1040. 1040 is aligned for a double (8-byte alignment).
    double *data2 = static_cast<double *>(
        allocator.allocate(5 * sizeof(double), alignof(double)));
    data2[0] = 3.14;
    std::cout << "  - Data 2 (double[5]) stored: " << data2[0] << "\n";

    // Allocation 3: A small struct (1 byte).
    // This is allocated at 1080 (which is aligned for 1 byte).
    struct TinyStruct {
      char c;
    };
    TinyStruct *data3 = static_cast<TinyStruct *>(
        allocator.allocate(sizeof(TinyStruct), alignof(TinyStruct)));
    data3->c = 'A';
    std::cout << "  - Data 3 (TinyStruct) stored: " << data3->c << "\n";

    // Allocation 4: An unaligned request followed by an aligned object.
    // We simulate an unaligned gap by allocating 17 bytes.
    std::cout << "\n--- Simulating Unaligned Allocation (17 bytes) ---\n";
    allocator.allocate(
        17, 1); // Allocation of 17 bytes, 1-byte alignment (no padding)

    // Allocation 5: A double (8 bytes) after the 17-byte block.
    // The allocator will now introduce 7 bytes of padding and zero them out.
    std::cout << "\n--- Allocating Aligned object after 17 bytes ---\n";
    double *data5 = static_cast<double *>(
        allocator.allocate(sizeof(double), alignof(double)));
    data5[0] = 7.77;
    std::cout << "  - Data 5 (double[1]) stored: " << data5[0] << "\n";

    std::cout << "\n--- Phase 2: Instant Reset ---\n";

    // This is the power of the monotonic allocator: clearing all memory
    // instantly.
    allocator.reset();

    std::cout << "\n--- Phase 3: Allocating after reset ---\n";

    // The pointer is back at the start, and we can reuse the memory.
    int *data4 =
        static_cast<int *>(allocator.allocate(20 * sizeof(int), alignof(int)));
    data4[0] = 100;
    std::cout << "  - Data 4 (int[20]) stored: " << data4[0] << "\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }

  return 0;
}
