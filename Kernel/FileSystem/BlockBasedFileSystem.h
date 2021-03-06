#pragma once

// includes
#include <Kernel/FileSystem/FileBackedFileSystem.h>

namespace Kernel {

class BlockBasedFS : public FileBackedFS {
public:
    TYPEDEF_DISTINCT_ORDERED_ID(u64, BlockIndex);

    virtual ~BlockBasedFS() override;

    size_t logical_block_size() const { return m_logical_block_size; };

    virtual void flush_writes() override;
    void flush_writes_impl();

protected:
    explicit BlockBasedFS(FileDescription&);

    KResult read_block(BlockIndex, UserOrKernelBuffer*, size_t count, size_t offset = 0, bool allow_cache = true) const;
    KResult read_blocks(BlockIndex, unsigned count, UserOrKernelBuffer&, bool allow_cache = true) const;

    bool raw_read(BlockIndex, UserOrKernelBuffer&);
    bool raw_write(BlockIndex, const UserOrKernelBuffer&);

    bool raw_read_blocks(BlockIndex index, size_t count, UserOrKernelBuffer&);
    bool raw_write_blocks(BlockIndex index, size_t count, const UserOrKernelBuffer&);

    KResult write_block(BlockIndex, const UserOrKernelBuffer&, size_t count, size_t offset = 0, bool allow_cache = true);
    KResult write_blocks(BlockIndex, unsigned count, const UserOrKernelBuffer&, bool allow_cache = true);

    size_t m_logical_block_size { 512 };

private:
    DiskCache& cache() const;
    void flush_specific_block_if_needed(BlockIndex index);

    mutable OwnPtr<DiskCache> m_cache;
};

}

template<>
struct AK::Formatter<Kernel::BlockBasedFS::BlockIndex> : AK::Formatter<FormatString> {
    void format(FormatBuilder& builder, Kernel::BlockBasedFS::BlockIndex value)
    {
        return AK::Formatter<FormatString>::format(builder, "{}", value.value());
    }
};
