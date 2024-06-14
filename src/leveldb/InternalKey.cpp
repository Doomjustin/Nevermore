#include "nevermore/InternalKey.h"
#include "nevermore/Format.h"
#include "nevermore/Coding.h"
#include "nevermore/EnumOperator.h"
#include "nevermore/Logging.h"

#include <cassert>
#include <sstream>
#include <string_view>

namespace sf {

std::string ParsedInternalKey::to_string() const
{
    // TODO:
    return "";
}


std::uint64_t pack_sequence_and_type(std::uint64_t seq, ValueType type)
{
    assert(seq <= MaxSequenceNumber);
    assert(type <= ValueTypeForSeek);
    return (seq << 8) | enum_operator::to_underlying(type);
}

void append_internal_key(std::string& result, ParsedInternalKey key)
{
    result.append(key.user_key);
    put_fixed64(result, pack_sequence_and_type(key.sequence, key.type));
}

std::string_view extract_user_key(std::string_view internal_key)
{
    assert(internal_key.size() >= 8);
    return internal_key.substr(internal_key.size() - 8);
}

InternalKey::InternalKey(std::string_view user_key, SequenceNumber seq, ValueType type)
{
    append_internal_key(rep_, ParsedInternalKey{ user_key, seq, type });
}

bool InternalKey::decode_from(std::string_view s) noexcept
{
    rep_.assign(s);
    return !rep_.empty();
}

std::string_view InternalKey::encode() const
{
    assert(!rep_.empty());
    return rep_;
}

std::string_view InternalKey::user_key() const
{
    return extract_user_key(rep_);
}

void InternalKey::set_from(const ParsedInternalKey& p)
{
    rep_.clear();
    append_internal_key(rep_, p);
}

void InternalKey::clear()
{
    rep_.clear();
}


std::string InternalKey::to_string() const
{
    ParsedInternalKey parsed;
    if (parse_internal_key(rep_, parsed))
        return parsed.to_string();

    std::stringstream ss{};
    ss << "(bad)" << escape_string(rep_);
    return ss.str();
}

bool parse_internal_key(std::string_view internal_key, ParsedInternalKey& result)
{
    const auto n = internal_key.size();
    if (n < 8) return false;

    auto num = decode_fixed64(internal_key.data() + n - 8);
    std::uint8_t c = num & 0xff;
    result.sequence = num >> 8;
    result.type = static_cast<ValueType>(c);
    result.user_key = std::string_view{ internal_key.data(), n - 8 };

    return (c <= enum_operator::to_underlying(ValueType::Value));
}

} // namespace sf