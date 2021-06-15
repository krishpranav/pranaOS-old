/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <LibArchive/Zip.h>

namespace Archive {

bool Zip::find_end_of_central_directory_offset(const ReadonlyBytes& buffer, size_t& offset)
{
    for (size_t backwards_offset = 0; backwards_offset <= UINT16_MAX; backwards_offset++) // the file may have a trailing comment of an arbitrary 16 bit length
    {
        if (buffer.size() < (sizeof(EndOfCentralDirectory) - sizeof(u8*)) + backwards_offset)
            return false;

        auto signature_offset = (buffer.size() - (sizeof(EndOfCentralDirectory) - sizeof(u8*)) - backwards_offset);
        if (memcmp(buffer.data() + signature_offset, end_of_central_directory_signature, sizeof(end_of_central_directory_signature)) == 0) {
            offset = signature_offset;
            return true;
        }
    }
    return false;
}

Optional<Zip> Zip::try_create(const ReadonlyBytes& buffer)
{
    size_t end_of_central_directory_offset;
    if (!find_end_of_central_directory_offset(buffer, end_of_central_directory_offset))
        return {};

    EndOfCentralDirectory end_of_central_directory {};
    if (!end_of_central_directory.read(buffer.slice(end_of_central_directory_offset)))
        return {};

    if (end_of_central_directory.disk_number != 0 || end_of_central_directory.central_directory_start_disk != 0 || end_of_central_directory.disk_records_count != end_of_central_directory.total_records_count)
        return {}; 

    size_t member_offset = end_of_central_directory.central_directory_offset;
    for (size_t i = 0; i < end_of_central_directory.total_records_count; i++) {
        CentralDirectoryRecord central_directory_record {};
        if (member_offset > buffer.size())
            return {};
        if (!central_directory_record.read(buffer.slice(member_offset)))
            return {};
        if (central_directory_record.general_purpose_flags & 1)
            return {}; 
        if (central_directory_record.general_purpose_flags & 3)
            return {}; 
        if (central_directory_record.compression_method != ZipCompressionMethod::Store && central_directory_record.compression_method != ZipCompressionMethod::Deflate)
            return {}; 
        if (central_directory_record.compression_method == ZipCompressionMethod::Store && central_directory_record.uncompressed_size != central_directory_record.compressed_size)
            return {};
        if (central_directory_record.start_disk != 0)
            return {};
        if (memchr(central_directory_record.name, 0, central_directory_record.name_length) != nullptr)
            return {};
        LocalFileHeader local_file_header {};
        if (central_directory_record.local_file_header_offset > buffer.size())
            return {};
        if (!local_file_header.read(buffer.slice(central_directory_record.local_file_header_offset)))
            return {};
        if (buffer.size() - (local_file_header.compressed_data - buffer.data()) < central_directory_record.compressed_size)
            return {};
        member_offset += central_directory_record.size();
    }

    Zip zip;
    zip.m_input_data = buffer;
    zip.member_count = end_of_central_directory.total_records_count;
    zip.members_start_offset = end_of_central_directory.central_directory_offset;
    return zip;
}

bool Zip::for_each_member(Function<IterationDecision(const ZipMember&)> callback)
{
    size_t member_offset = members_start_offset;
    for (size_t i = 0; i < member_count; i++) {
        CentralDirectoryRecord central_directory_record {};
        VERIFY(central_directory_record.read(m_input_data.slice(member_offset)));
        LocalFileHeader local_file_header {};
        VERIFY(local_file_header.read(m_input_data.slice(central_directory_record.local_file_header_offset)));

        ZipMember member;
        char null_terminated_name[central_directory_record.name_length + 1];
        memcpy(null_terminated_name, central_directory_record.name, central_directory_record.name_length);
        null_terminated_name[central_directory_record.name_length] = 0;
        member.name = String { null_terminated_name };
        member.compressed_data = { local_file_header.compressed_data, central_directory_record.compressed_size };
        member.compression_method = static_cast<ZipCompressionMethod>(central_directory_record.compression_method);
        member.uncompressed_size = central_directory_record.uncompressed_size;
        member.crc32 = central_directory_record.crc32;
        member.is_directory = central_directory_record.external_attributes & zip_directory_external_attribute || member.name.ends_with('/'); // FIXME: better directory detection

        if (callback(member) == IterationDecision::Break)
            return false;

        member_offset += central_directory_record.size();
    }
    return true;
}

}