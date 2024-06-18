#ifndef NEVERMORE_LEVELDB_WRITE_BATCH_H
#define NEVERMORE_LEVELDB_WRITE_BATCH_H

#include "Varint.h"
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace sf {

enum class ValueType: std::uint8_t {
    Value,
    Deletion
};

/**
 * @brief 一条数据
 * 
 */
class Record {
friend std::ostream& operator<<(std::ostream& os, const Record& record);
    
public:
    Record(std::string_view key);
    Record(std::string_view key, std::string_view value);

    Varint<std::uint32_t> length() const noexcept { return Varint{ static_cast<std::uint32_t>(value_.size()) }; }
    std::string_view value() const noexcept { return value_; }

private:    
    ValueType type_;
    std::string value_;              /// 实际存储的值，根据type不同，Deletion时只存储key，Value时存储key + value
};


/**
 * @brief DB一次存储的最小单位
 *
 * header :=
 *     SequenceNumber: uint64_t
 *     Count: uint32_t
 * records :=
 *     vector<Record>
 */
class WriteBatch {
    /**
     * @brief 将batch里的内容写入os
     *
     * 其输出为二进制且varint也被正确编码
     * 
     * @param os 
     * @param batch 
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream& os, const WriteBatch& batch);
    
public:
    using SizeType = std::uint32_t;
    using SequenceNumberType = std::uint64_t;

    constexpr SizeType size() const noexcept { return records_.size(); }
    SizeType decode_size() const noexcept;
    
    /**
     * @brief 移除key对应的record
     *
     * 不会物理删除数据，只是添加一条标记为deletion的新数据
     * 
     * @param key 
     */
    void erase(std::string_view key);

    /**
     * @brief 添加新数据
     *
     * 如果已经存在相同key的数据，也不会覆盖，而是追加一条新的数据
     * 
     * @param key 
     * @param value 
     */
    void add(std::string_view key, std::string_view value);

    WriteBatch& operator+=(const WriteBatch& other);

    void sequence_number(SequenceNumberType number) noexcept { sequence_number_ = number; }
    constexpr SequenceNumberType sequence_number() const noexcept { return sequence_number_; }

private:
    // TODO: sequence number要怎么确定
    SequenceNumberType sequence_number_ = 0;   /// 每一个write_batch的sequence_number必须是唯一的
    std::vector<Record> records_;
};

} // namespace sf

#endif // NEVERMORE_LEVELDB_WRITE_BATCH_H