#pragma once

// includes
#include <AK/Types.h>
#include <Kernel/FileSystem/FileSystem.h>
#include <Kernel/FileSystem/Inode.h>

namespace Kernel {

class SlavePTY;
class DevPtsFSInode;

class DevPtsFS final : public FS {
    friend class DevPtsFSInode;

public:
    virtual ~DevPtsFS() override;
    static NonnullRefPtr<DevPtsFS> create();

    virtual bool initialize() override;
    virtual const char* class_name() const override { return "DevPtsFS"; }

    virtual NonnullRefPtr<Inode> root_inode() const override;

    static void register_slave_pty(SlavePTY&);
    static void unregister_slave_pty(SlavePTY&);

private:
    DevPtsFS();
    RefPtr<Inode> get_inode(InodeIdentifier) const;

    RefPtr<DevPtsFSInode> m_root_inode;
};

class DevPtsFSInode final : public Inode {
    friend class DevPtsFS;

public:
    virtual ~DevPtsFSInode() override;

private:
    DevPtsFSInode(DevPtsFS&, InodeIndex, SlavePTY*);

    // ^Inode
    virtual ssize_t read_bytes(off_t, ssize_t, UserOrKernelBuffer& buffer, FileDescription*) const override;
    virtual InodeMetadata metadata() const override;
    virtual KResult traverse_as_directory(Function<bool(const FS::DirectoryEntryView&)>) const override;
    virtual RefPtr<Inode> lookup(StringView name) override;
    virtual void flush_metadata() override;
    virtual ssize_t write_bytes(off_t, ssize_t, const UserOrKernelBuffer& buffer, FileDescription*) override;
    virtual KResultOr<NonnullRefPtr<Inode>> create_child(const String& name, mode_t, dev_t, uid_t, gid_t) override;
    virtual KResult add_child(Inode&, const StringView& name, mode_t) override;
    virtual KResult remove_child(const StringView& name) override;
    virtual KResultOr<size_t> directory_entry_count() const override;
    virtual KResult chmod(mode_t) override;
    virtual KResult chown(uid_t, gid_t) override;

    WeakPtr<SlavePTY> m_pty;
    InodeMetadata m_metadata;
};

}
