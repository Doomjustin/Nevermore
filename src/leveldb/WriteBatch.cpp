#include "nevermore/WriteBatch.h"
#include "nevermore/EnumOperator.h"

#include <algorithm>
#include <iterator>

namespace sf {

WriteBatch::SizeType WriteBatch::decode_size() const noexcept
{
    constexpr auto HeaderSize = sizeof(SequenceNumberType) + sizeof(SizeType);
    SizeType records_size = 0;
    for (auto& record: records_)
        records_size += record.length().value();

    return HeaderSize + records_size;
}


void WriteBatch::erase(std::string_view key)
{
    records_.emplace_back(key);
}

void WriteBatch::add(std::string_view key, std::string_view value)
{
    records_.emplace_back(key, value);
}

WriteBatch& WriteBatch::operator+=(const WriteBatch& other)
{
    std::copy(other.records_.begin(), other.records_.end(),
              std::back_inserter(records_));
    return *this;
}

std::ostream& operator<<(std::ostream& os, const WriteBatch& batch)
{
    os << batch.sequence_number_ << batch.records_.size();
    std::copy(batch.records_.begin(), batch.records_.end(),
              std::ostream_iterator<Record>(os));
              
    return os;
}

std::ostream& operator<<(std::ostream& os, const Record& record)
{
    os << enum_operator::to_underlying(record.type_)
       << record.value_;

    return os;
}

} // namespace sf