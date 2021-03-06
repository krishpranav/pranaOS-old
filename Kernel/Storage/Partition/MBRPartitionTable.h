#pragma once

// includes
#include <AK/ByteBuffer.h>
#include <AK/RefPtr.h>
#include <AK/Result.h>
#include <AK/Vector.h>
#include <Kernel/Storage/Partition/PartitionTable.h>

namespace Kernel {

class MBRPartitionTable : public PartitionTable {
public:
    struct [[gnu::packed]] Entry {
        u8 status;
        u8 chs1[3];
        u8 type;
        u8 chs2[3];
        u32 offset;
        u32 length;
    };
    struct [[gnu::packed]] Header {
        u8 code1[218];
        u16 ts_zero;
        u8 ts_drive;
        u8 ts_seconds;
        u8 ts_minutes;
        u8 ts_hours;
        u8 code2[216];
        u32 disk_signature;
        u16 disk_signature_zero;
        Entry entry[4];
        u16 mbr_signature;
    };

public:
    ~MBRPartitionTable();

    static Result<NonnullOwnPtr<MBRPartitionTable>, PartitionTable::Error> try_to_initialize(const StorageDevice&);
    static OwnPtr<MBRPartitionTable> try_to_initialize(const StorageDevice&, u32 start_lba);
    explicit MBRPartitionTable(const StorageDevice&);
    MBRPartitionTable(const StorageDevice&, u32 start_lba);

    bool is_protective_mbr() const;
    bool contains_ebr() const;
    virtual bool is_valid() const override { return m_valid; };

protected:
    const Header& header() const;
    bool is_header_valid() const { return m_header_valid; };

private:
    bool read_boot_record();
    bool initialize();
    bool m_valid { false };
    bool m_header_valid { false };
    const u32 m_start_lba;
    ByteBuffer m_cached_header;
};
}
