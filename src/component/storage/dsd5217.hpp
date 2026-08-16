/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    dsd5217.hpp: The Qualogy (previously known as Data Systems Design) DSD 8217 Multibus Disk & Tape Controller
    This is a combined QIC tape, hard drive and floppy controller.
    
    Technically not used on the 3130 (3120) but this is the only controller that I've got a disk image for right now
    Later on we can run mkboot and boot this

    Currently this is a high-level emulation, but this uses the Intel 8085. Later on we'll write an 8085 emulation.
*/

#pragma once
#include <Motion.hpp>
#include <component/component.hpp>
#include <component/multibus/multibus.hpp>
#include <base/profile/profile.hpp>
#include <base/filesystem/filesystem.hpp>

namespace Motion
{
    #define DSD5217_MBIO_START                  0x50007F00
    #define DSD5217_MBIO_END                    0x50007FFF
    #define DSD5217_MBIO_PTR1                   0x50007F01

    // memory ranges. THis one decodes one region which stores pointers to many others.
    #define DSD5217_MEMORY_MAP1_START           0x7F000
    #define DSD5217_MEMORY_MAP1_END             0x7F001

    // Holds the pointers to all other utilised structures


    #define DSD5217_LOG_PREFIX                  "DSD 5217"

    // Fresh from my ass
    #define DSD5217_MULTIBUS_SLOTNUM            7


    // Device codes. These determine which device we are actually communicating with
    #define DSD5217_DEVICE_CODE_HDD             0   // 'Winchester' (Hard disk drive)
    #define DSD5217_DEVICE_CODE_FLOPPY          1   // Floppy Disk
    #define DSD5217_DEVICE_CODE_QIC             2   // QIC tape drive
    #define DSD5217_DEVICE_CODE_TAPE            3   // Tape

    // Number of each
    #define DSD5217_MAX_DISK_DRIVES             2   
    #define DSD5217_MAX_QIC_DRIVES              2   
    #define DSD5217_MAX_FLOPPY_DRIVES           2   

    // block size
    #define DSD5217_BLOCK_SIZE                  512

    // operation status
    #define DSD5217_OPERATION_STATUS_COMPLETE   (1 << 0)
    #define DSD5217_OPERATION_SEEK_COMPLETE     (1 << 1)
    #define DSD5217_OPERATION_MEDIA_CHANGE      (1 << 2)
    #define DSD5217_OPERATION_FLOPPYQIC_DONE    0x09
    #define DSD5217_OPERATION_FLOPPYQIC_DONE    0x09
    #define DSD5217_OPERATION_TAPE_MEDIA_CHANGE 0x0E
    #define DSD5217_OPERATION_TAPE_LONG_COMMAND 0x0F
    #define DSD5217_OPERATION_UNIT_BITS         0x30        // these determine which unit initiated the operaiton
    #define DSD5217_OPERATION_HARD_ERROR        (1 << 6)    // ah crap
    #define DSD5217_OPERATION_SUMMARY_ERROR     (1 << 7)
    #define DSD5217_OPERATION_STATUS_MASK       0xF0

    // function codes
    // there are 32
    #define DSD5217_FUNC_INIT                   0x00        // start
    #define DSD5217_FUNC_XFER_STATUS            0x01        // how am i doing
    #define DSD5217_FUNC_FORMAT                 0x02        // the death of your data
    #define DSD5217_FUNC_READ_ID                0x03        // read the id of something
    #define DSD5217_FUNC_READ_DATA              0x04        // read something
    #define DSD5217_FUNC_READ_AND_VERIFY        0x05        // read to buffer and verify
    #define DSD5217_FUNC_WRITE                  0x06        // write, the birth of your data
    #define DSD5217_FUNC_WRITE_BUFFER           0x07        // write into a buffer
    #define DSD5217_FUNC_SEEK                   0x08        // go somewhere
    // 0x09-0x0d are reserved
    #define DSD5217_FUNC_BUFFER_IO              0x0E        // generic buffer i/o
    #define DSD5217_FUNC_DIAG                   0x0F        // figure out if it really is over
    #define DSD5217_FUNC_TAPE_INIT              0x10        // initialise tape
    #define DSD5217_FUNC_TAPE_REWIND            0x11        // rewind tape
    #define DSD5217_FUNC_TAPE_FORWARD_FILE      0x12        // go forward 1 file
    #define DSD5217_FUNC_TAPE_WRITE_MARK        0x14        // write a file mark
    #define DSD5217_FUNC_TAPE_ERASE             0x17        // no more data
    #define DSD5217_FUNC_TAPE_FORWARD_RECORD    0x1A        // go forward 1 record
    #define DSD5217_FUNC_TAPE_RESET             0x1C        // restart
    #define DSD5217_FUNC_TAPE_RETENSION         0x1D        // retension
    #define DSD5217_FUNC_TAPE_DRIVE_STATUS      0x1E        // how are you doing, tape drive?
    #define DSD5217_FUNC_TAPE_RW_TERMINATE      0x1F        // you're done for today tape

    // operation modifiers
    #define DSD5217_MODIFIER_NO_INT             (1 << 0)    // no interrupt on completion
    #define DSD5217_MODIFIER_NO_RETRY           (1 << 1)    // don't retry for error recovery
    #define DSD5217_MODIFIER_ALLOW_DELETED      (1 << 2)    // allow deleted data rewrite
    #define DSD5217_MODIFIER_READ_AFTER_LONG    (1 << 6)    // read tape after long operation

    // soft error
    #define DSD5217_SOFTERR_DATA_RECOVERABLE    (1 << 1)    // try again to read data, need a new tape
    #define DSD5217_SOFTERR_DATA_UNRECOVERABLE  (1 << 3)    // data is busted
    #define DSD5217_SOFTERR_DRIVE_FAULT         (1 << 5)    // drive is busted
    #define DSD5217_SOFTERR_BUFFER_OVERRUN      (1 << 7)    // buffer over/underrun

    // hard error (2 bytes)
    #define DSD5217_HARDERR0_REJECT_COMMAND5215 (1 << 0)    // invalid command in 5215 mode
    #define DSD5217_HARDERR0_REJECT_COMMAND5217 (1 << 1)    // invalid command in 5217 mode
    #define DSD5217_HARDERR0_INVALID_COMMAND    (1 << 2)    // invalid command
    #define DSD5217_HARDERR0_TAPEINIT_FAIL      (1 << 4)    // failed to initialise tape
    #define DSD5217_HARDERR0_LONGTERM_CMD       (1 << 5)    // Long term command
    #define DSD5217_HARDERR0_ILLEGAL_FORMAT     (1 << 6)    // illegal format
    #define DSD5217_HARDERR0_END_OF_MEDIA       (1 << 7)    // nothing left to read

    #define DSD5217_HARDERR1_LENGTH_ERROR       (1 << 0)    // media not multiple of block size long
    #define DSD5217_HARDERR1_TAPE_TIMEOUT       (1 << 2)    // tape timed out
    #define DSD5217_HARDERR1_INVALID_COMMAND    (1 << 3)    // check type in byte 0
    #define DSD5217_HARDERR1_NO_TAPE_CART       (1 << 4)    // bozo didn't put the tape in
    #define DSD5217_HARDERR1_INVALID_ADDRESS    (1 << 5)    // invalid address (something is wrong)
    #define DSD5217_HARDERR1_UNIT_NOT_READY     (1 << 6)    // unit not ready
    #define DSD5217_HARDERR1_WRITE_PROTECTED    (1 << 7)    // write protected

    class DSD5217 : public Component
    {
    public:
        void Start() override;
        void Shutdown() override; 

        const char* GetName() { return "Data Storage Devices / Qualogy 5217 Multibus Disk & Tape Controller"; };

// make sure these are not packed so that the OS can use them
#pragma pack(push, 1)
        // @brief I/O Parameter Block
        struct IOPB
        {
            uint32_t dummy;                 // required for size
            uint32_t actualTransfers;
            uint16_t deviceCode;
            uint8_t function;
            uint8_t unit;
            uint16_t modifier;
            uint16_t cylinder;              // cylinder
            uint8_t sector;                 // cylinder
            uint8_t head;                   // cylinder
            uint8_t* dba;                   // use as a pointer
            uint8_t rbc;                    // requested byte count
            uint8_t* generalPtr;            // use as a pointer
        }; 

        // @brief not sure what this is yet
        struct WUB
        {
            uint8_t dummy;
            uint8_t extension;              // 7 = 24 bit addressing?
            uint8_t* ccbPtr;                 
        }; 

        /// @brief Channel Control Block
        struct CCB
        {
            uint8_t busy;                   // ff = busy, 00 = idle
            uint8_t ccw1;                   // channel control word 1
            uint8_t* cibPtr;                // CIB ptr
            uint16_t dummy;
            uint8_t busy2;                  // not used? 
            uint8_t ccw2;                   // not used? channel control word 2
            uint8_t* cpPtr;                 // cp ptr
            uint16_t controlPtr;            // control pointer (not a pointer?)
        };

        /// @brief Channel Information Block
        struct CIB
        {
            uint8_t opStatus;               // operation status
            uint8_t reserved;
            uint8_t statusSemaphore;        // status semaphore
            uint8_t commandSemaphore;       // command semaphore
            uint8_t* zero;                  // must be zero
            uint8_t* iopbPtr;               // IOPB pointer
            uint32_t zero2;                 // must be zero
        };

        /// @brief not sure what this is ? disk header ?
        struct INIB
        {
            uint16_t nrCylinders;
            uint8_t removableHeads;
            uint8_t fixedHeads;
            uint8_t bytesPerSectorLow;
            uint8_t sectorsPerTrack;
            uint8_t numberOfAlternateCylinders;
            uint8_t bytesPerSectorHigh;
        };

        /// @brief format related/
        struct FMTB
        {
            // BE
            uint8_t pattern1;
            uint8_t function;               // format function
            uint8_t pattern3;
            uint8_t pattern2;
            uint8_t interleave;
            uint8_t pattern4;
        }; 

        #define DSD5217_SB_SIZE                 14
    
        // Status bytes for operation?
        #define DSD5217_SB_HARD_ERROR0          0
        #define DSD5217_SB_HARD_ERROR1          1
        #define DSD5217_SB_SOFT_ERROR           2
        #define DSD5217_SB_BOT                  3
        #define DSD5217_SB_FILE_MARK            5
        #define DSD5217_SB_NO_DATA              8
        #define DSD5217_SB_RETRY_COUNT          11
        #define DSD5217_SB_DISK_EXTENDED_STATUS 12

        /// @brief sector descripton?
        struct INIST
        {
            DSD5217::INIB inib;
            DSD5217::FMTB fmtb;
            uint8_t sb[DSD5217_SB_SIZE];        // status bytes?
        }; 

#pragma pack(pop)


        uint8_t state;                          // holds reset state

        // Methods
        uint8_t Read8(size_t addr) override;
        void Write8(size_t addr, uint8_t value) override;

    private: 
        // Multibus IRQ1 is used.
        Multibus* multibus;
        FileStream* hdd;
        WUB* wub;

        // can't do any disk ops if there is no disk inserted lmao
        bool diskIsOpen;
    }; 
}; 