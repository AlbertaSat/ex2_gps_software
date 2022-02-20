
#include <stdint.h>
#define uint32 uint32_t
#define uint8 uint8_t
/** @def scilinREG
 *   @brief SCILIN (LIN - Compatibility Mode)  Register Frame Pointer
 *
 *   This pointer is used by the SCI driver to access the sci module registers.
 */
#define scilinREG ((sciBASE_t *)0xFFF7E400U)

typedef volatile struct sciBase { uint32 hello; } sciBASE_t;
// function definiton copied from HalCoGen
void sciSend(sciBASE_t *sci, uint32 length, uint8 *data);