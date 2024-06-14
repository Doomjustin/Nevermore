#include "nevermore/WriteBatch.h"
#include "nevermore/Coding.h"
#include "nevermore/Status.h"
#include "nevermore/Format.h"

#include <cassert>
#include <string_view>

namespace sf {

static constexpr int HeaderSize = 12;

struct WriteBatchInternal {
    static void set_count(WriteBatch& batch, int n)
    {
        encode_fixed32(batch.rep_.data(), n);
    }

    static int count(const WriteBatch& batch)
    {
        return decode_fixed32(batch.rep_.data() + 8);
    }

    static SequenceNumber sequence(const WriteBatch& batch)
    {
        return static_cast<SequenceNumber>(decode_fixed64(batch.rep_.data()));
    }

    static void set_sequence(WriteBatch& batch, SequenceNumber seq)
    {
        encode_fixed64(&batch.rep_[0], seq);
    }

    static std::string_view contents(const WriteBatch& batch)
    {
        return std::string_view{ batch.rep_ };
    }

    static void set_contents(WriteBatch& batch, const std::string_view contents)
    {
        assert(contents.size() >= HeaderSize);
        batch.rep_.assign(contents.data(), contents.size());
    }

    static void append(WriteBatch& dst, const WriteBatch& src)
    {
        set_count(dst, count(dst) + count(src));
        assert(src.rep_.size() >= HeaderSize);
        dst.rep_.append(src.rep_.data() + HeaderSize, src.rep_.size() - HeaderSize);
    }

};

// WriteBatch::rep_ :=
//    sequence: fixed64
//    count: fixed32
//    data: record[count]
// record :=
//    kTypeValue varstring varstring         |
//    kTypeDeletion varstring
// varstring :=
//    len: varint32
//    data: uint8[len]
WriteBatch::WriteBatch() 
{ 
    clear(); 
}

void WriteBatch::put(const std::string_view key, const std::string_view value)
{
    WriteBatchInternal::set_count(*this, WriteBatchInternal::count(*this) + 1);
    rep_.push_back(static_cast<char>(ValueType::Value));
    put_length_prefixed_slice(rep_, key);
    put_length_prefixed_slice(rep_, value);
}

void WriteBatch::remove(const std::string_view key)
{
    WriteBatchInternal::set_count(*this, WriteBatchInternal::count(*this) + 1);
    rep_.push_back(static_cast<char>(ValueType::Deletion));
    put_length_prefixed_slice(rep_, key);
}

void WriteBatch::clear()
{
    rep_.clear();
    rep_.resize(HeaderSize);
}

std::size_t WriteBatch::approximate_size()
{
    return rep_.size();
}

void WriteBatch::append(const WriteBatch& other)
{
    WriteBatchInternal::append(*this, other);
}

Status WriteBatch::iterator(Handler* handler) const
{
    std::string_view input{ rep_ };

    if (input.size() < HeaderSize)
        return Status::corruption("malformed WriteBatch (too small)");

    input.remove_prefix(HeaderSize);
    std::string_view key;
    std::string_view value;
    int found = 0;
    while (!input.empty()) {
        ++found;
        auto tag = static_cast<ValueType>(input[0]);
        switch (tag) {
        using enum ValueType;
        case Value:
            if (get_length_prefixed_slice(input, key) &&
                get_length_prefixed_slice(input, value)) {
                handler->put(key, value);
            }
            else {
                return Status::corruption("bad WriteBatch put");
            }
        case Deletion:
            if (get_length_prefixed_slice(input, key))
                handler->remove(key);
            else
                return Status::corruption("bad WriteBatch remove");
        default:
            return Status::corruption("unknown WriteBatch tag");
        }
    }

    if (found != WriteBatchInternal::count(*this))
        return Status::corruption("WriteBatch has wrong count");

    return Status::ok();
}

} // namespace sf