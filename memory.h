#ifndef _INCL_DEFINE
    #include "globals.h"
#endif

class Memory {
    public:
        static constexpr u32 MAX_MEM = 1024 * 64; 
        BYTE Data[MAX_MEM];

        // Operator for reading 1 Byte
        BYTE operator[](u32 Address) const {
            // todo :: assert that Addres is < MAX_MEM
            return Data[Address];
        }

        void Initialize();
};