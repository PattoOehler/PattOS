#ifndef __PATTOS__GDT_H
#define __PATTOS__GDT_H

#include <common/types.h>

namespace pattos
{


    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {
                private:
                    pattos::common::uint16_t limit_lo;
                    pattos::common::uint16_t base_lo;
                    pattos::common::uint8_t base_hi;
                    pattos::common::uint8_t type;
                    pattos::common::uint8_t limit_hi;
                    pattos::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(pattos::common::uint32_t base, pattos::common::uint32_t limit, pattos::common::uint8_t type);
                    pattos::common::uint32_t Base();
                    pattos::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            pattos::common::uint16_t CodeSegmentSelector();
            pattos::common::uint16_t DataSegmentSelector();
    };

}

#endif
