////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT NovAtel Inc, 2022. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////////////////
//                            DESCRIPTION
//
//! \file jsonreader.hpp
//! \brief Class to read a JSON UI DB.
////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Recursive Inclusion
//-----------------------------------------------------------------------
#ifndef JSONREADER_HPP
#define JSONREADER_HPP

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using nlohmann::json;

namespace novatel::edie {

//============================================================================
//! \class JsonReaderFailure
//! \brief Exception to be thrown when JsonReader fails to parse a JSON.
//============================================================================
class JsonReaderFailure : public std::exception
{
  private:
    const char* func;
    const char* file;
    int32_t line;
    std::filesystem::path clFilePath;
    const char* failure;
    char acWhatString[256];

  public:
    JsonReaderFailure(const char* func_, const char* file_, int32_t line_, const std::filesystem::path& json_file_, const char* failure_)
        : func(func_), file(file_), line(line_), clFilePath(json_file_), failure(failure_), acWhatString{}
    {
    }

    const char* what() const throw() override
    {
        sprintf(const_cast<char*>(acWhatString), "In file \"%s\" : %s() (Line %d)\n\t\"%s: %s.\"", file, func, line,
                clFilePath.generic_string().c_str(), failure);
        return acWhatString;
    }
};

//-----------------------------------------------------------------------
//! \enum CONVERSION_STRING
//! \brief Conversion string represented as an enum.
//-----------------------------------------------------------------------
enum class CONVERSION_STRING
{
    // Signed & Unsigned Integers
    d,
    ld,
    hd,
    lld,
    u,
    lu,
    hu,
    llu,
    // Single Chars/Bytes & Hexadecimal
    c,
    uc,
    Z,
    B,
    UB,
    XB,
    x,
    X,
    lx,
    ucb,
    // Floating Point & Scientific Notation
    f,
    lf,
    k,
    lk,
    e,
    le,
    g,
    // Strings & Bytestream
    P,
    s,
    S,
    // NovAtel types
    id, // SATELLITEID
    R,  // RXCONFIG
    m,  // MessageName
    T,  // GPSTime value (<int value>/1000.0)
    UNKNOWN
};

//!< Mapping from String to data type enums.
static std::unordered_map<std::string, CONVERSION_STRING> const ConversionStringEnumLookup = {
    {"%d", CONVERSION_STRING::d},   {"%ld", CONVERSION_STRING::ld},   {"%hd", CONVERSION_STRING::hd}, {"%lld", CONVERSION_STRING::lld},
    {"%u", CONVERSION_STRING::u},   {"%lu", CONVERSION_STRING::lu},   {"%hu", CONVERSION_STRING::hu}, {"%llu", CONVERSION_STRING::llu},
    {"%c", CONVERSION_STRING::c},   {"%uc", CONVERSION_STRING::uc},   {"%B", CONVERSION_STRING::B},   {"%UB", CONVERSION_STRING::UB},
    {"%XB", CONVERSION_STRING::XB}, {"%Z", CONVERSION_STRING::Z},     {"%x", CONVERSION_STRING::x},   {"%X", CONVERSION_STRING::X},
    {"%lx", CONVERSION_STRING::lx}, {"%ucb", CONVERSION_STRING::ucb}, {"%f", CONVERSION_STRING::f},   {"%lf", CONVERSION_STRING::lf},
    {"%k", CONVERSION_STRING::k},   {"%lk", CONVERSION_STRING::lk},   {"%e", CONVERSION_STRING::e},   {"%le", CONVERSION_STRING::le},
    {"%g", CONVERSION_STRING::g},   {"%P", CONVERSION_STRING::P},     {"%s", CONVERSION_STRING::s},   {"%S", CONVERSION_STRING::S},
    {"%id", CONVERSION_STRING::id}, {"%R", CONVERSION_STRING::R},     {"%m", CONVERSION_STRING::m},   {"%T", CONVERSION_STRING::T}};

//-----------------------------------------------------------------------
//! \enum DATA_TYPE_NAME
//! \brief Data type name string represented as an enum.
//-----------------------------------------------------------------------
enum class DATA_TYPE_NAME
{
    BOOL,
    HEXBYTE,
    CHAR,
    UCHAR,
    SHORT,
    USHORT,
    INT,
    UINT,
    LONG,
    ULONG,
    LONGLONG,
    ULONGLONG,
    FLOAT,
    DOUBLE,
    EMBEDDED_HEADER,
    EMBEDDED_BODY,
    SATELLITEID,
    UNKNOWN
};

//!< Mapping from String to data type enums.
static std::unordered_map<std::string, DATA_TYPE_NAME> const DataTypeEnumLookup = {
    {"BOOL", DATA_TYPE_NAME::BOOL},   {"HEXBYTE", DATA_TYPE_NAME::HEXBYTE},   {"CHAR", DATA_TYPE_NAME::CHAR},
    {"UCHAR", DATA_TYPE_NAME::UCHAR}, {"SHORT", DATA_TYPE_NAME::SHORT},       {"USHORT", DATA_TYPE_NAME::USHORT},
    {"INT", DATA_TYPE_NAME::INT},     {"UINT", DATA_TYPE_NAME::UINT},         {"LONG", DATA_TYPE_NAME::LONG},
    {"ULONG", DATA_TYPE_NAME::ULONG}, {"LONGLONG", DATA_TYPE_NAME::LONGLONG}, {"ULONGLONG", DATA_TYPE_NAME::ULONGLONG},
    {"FLOAT", DATA_TYPE_NAME::FLOAT}, {"DOUBLE", DATA_TYPE_NAME::DOUBLE},     {"UNKNOWN", DATA_TYPE_NAME::UNKNOWN}};

//-----------------------------------------------------------------------
//! \enum FIELD_TYPE
//! \brief Field type string represented as an enum.
//-----------------------------------------------------------------------
enum class FIELD_TYPE
{
    SIMPLE,                //!< Simple type.
    ENUM,                  //!< Enum type.
    BITFIELD,              //!< BitField type.
    FIXED_LENGTH_ARRAY,    //!< Fixed-sized array.
    VARIABLE_LENGTH_ARRAY, //!< Variable-length array.
    STRING,                //!< String type.
    FIELD_ARRAY,           //!< Array of other fields.
    RESPONSE_ID,           //!< Fabricated response ID field.
    RESPONSE_STR,          //!< Fabricated response string field.
    RXCONFIG_HEADER,       //!< Fabricated RXCONFIG header field.
    RXCONFIG_BODY,         //!< Fabricated RXCONFIG body field.
    UNKNOWN                //!< Unknown.
};

//!< Mapping from String to field type enums.
static std::unordered_map<std::string, FIELD_TYPE> const FieldTypeEnumLookup = {{"SIMPLE", FIELD_TYPE::SIMPLE},
                                                                                {"ENUM", FIELD_TYPE::ENUM},
                                                                                {"BITFIELD", FIELD_TYPE::BITFIELD},
                                                                                {"FIXED_LENGTH_ARRAY", FIELD_TYPE::FIXED_LENGTH_ARRAY},
                                                                                {"VARIABLE_LENGTH_ARRAY", FIELD_TYPE::VARIABLE_LENGTH_ARRAY},
                                                                                {"STRING", FIELD_TYPE::STRING},
                                                                                {"FIELD_ARRAY", FIELD_TYPE::FIELD_ARRAY},
                                                                                {"UNKNOWN", FIELD_TYPE::UNKNOWN}};

//-----------------------------------------------------------------------
//! \struct EnumDataType
//! \brief Enum Data Type representing contents of UI DB.
//-----------------------------------------------------------------------
struct EnumDataType
{
    uint32_t value{0};
    std::string name{};
    std::string description{};

    constexpr EnumDataType() = default;
};

//-----------------------------------------------------------------------
//! \struct EnumDefinition
//! \brief Enum Definition representing contents of UI DB.
//-----------------------------------------------------------------------
struct EnumDefinition
{
    std::string _id{};
    std::string name{};
    std::vector<novatel::edie::EnumDataType> enumerators{};

    constexpr EnumDefinition() = default;

    using Ptr = std::shared_ptr<novatel::edie::EnumDefinition>;
    using ConstPtr = std::shared_ptr<const novatel::edie::EnumDefinition>;
};

//-----------------------------------------------------------------------
//! \struct BaseDataType
//! \brief Struct containing basic elements of data type fields in the
//! UI DB.
//-----------------------------------------------------------------------
struct BaseDataType
{
    DATA_TYPE_NAME name{DATA_TYPE_NAME::UNKNOWN};
    uint16_t length{0};
    std::string description{};

    constexpr BaseDataType() = default;
};

//-----------------------------------------------------------------------
//! \struct SimpleDataType
//! \brief Struct containing elements of simple data type fields in the
//! UI DB.
//-----------------------------------------------------------------------
struct SimpleDataType : BaseDataType
{
    std::unordered_map<int32_t, novatel::edie::EnumDataType> enums;
};

//-----------------------------------------------------------------------
//! \struct BaseField
//! \brief Struct containing elements of basic fields in the UI DB.
//-----------------------------------------------------------------------
struct BaseField
{
    std::string name;
    FIELD_TYPE type{FIELD_TYPE::UNKNOWN};
    std::string description;
    std::string conversion;
    CONVERSION_STRING conversionStripped{CONVERSION_STRING::UNKNOWN};
    int32_t conversionBeforePoint{0};
    int32_t conversionAfterPoint{0};
    SimpleDataType dataType;

    BaseField() = default;

    virtual ~BaseField() = default;

    virtual BaseField* clone() { return new novatel::edie::BaseField(*this); }

    void setConversion(const std::string& sConversion)
    {
        conversion = sConversion;
        std::string strStrippedConversionString;
        parseConversion(strStrippedConversionString, conversionBeforePoint, conversionAfterPoint);
        const auto itrConversionStringMapping = ConversionStringEnumLookup.find(strStrippedConversionString);
        conversionStripped =
            itrConversionStringMapping != ConversionStringEnumLookup.end() ? itrConversionStringMapping->second : CONVERSION_STRING::UNKNOWN;
    }

    void parseConversion(std::string& strStrippedConversionString_, int32_t& iBeforePoint_, int32_t& AfterPoint_) const
    {
        bool bIsBeforePoint = true;
        const char* sConvertString = conversion.c_str();
        int32_t iBytesRead;

        while (*sConvertString)
        {
            // "0x" could be a prefix on a hex field (0x%...) or a valid conversion string (%0x).
            // Prevent these two cases from occurring at the same time.
            if ((0 == memcmp(sConvertString, "0x", 2)) && (0 != strcmp(sConvertString, "0x"))) { sConvertString += 2; }

            // If the value "10" or greater is found from the conversion string, two bytes would
            // need to be consumed from the string to move past that value.  Otherwise only one byte
            // is necessary to consume.
            if ((*sConvertString >= '0') && (*sConvertString <= '9'))
            {
                if (bIsBeforePoint) // before point
                {
                    sscanf(sConvertString, "%d.", &iBeforePoint_);
                    if (iBeforePoint_ > 9)
                        iBytesRead = 2;
                    else
                        iBytesRead = 1;
                    sConvertString = sConvertString + iBytesRead; // Skip the numerals
                }
                else
                {
                    sscanf(sConvertString, "%d", &AfterPoint_);
                    if (AfterPoint_ > 9)
                        iBytesRead = 2;
                    else
                        iBytesRead = 1;
                    sConvertString = sConvertString + iBytesRead; // Skip the numerals
                }
            }

            if (std::isalpha(*sConvertString) || *sConvertString == '%')
            {
                strStrippedConversionString_.push_back(sConvertString[0]);
                sConvertString++;
            }

            if (*sConvertString == '.')
            {
                bIsBeforePoint = false; // Found the decimal
                sConvertString++;
            }
        }
    }

    using Ptr = std::shared_ptr<novatel::edie::BaseField>;
    using ConstPtr = std::shared_ptr<const novatel::edie::BaseField>;
};

//-----------------------------------------------------------------------
//! \struct EnumField
//! \brief Struct containing elements of enum fields in the UI DB.
//-----------------------------------------------------------------------
struct EnumField : novatel::edie::BaseField
{
    std::string enumID;
    novatel::edie::EnumDefinition::Ptr enumDef{nullptr};
    uint32_t length{0};

    EnumField() = default;

    EnumField* clone() override { return new novatel::edie::EnumField(*this); }

    using Ptr = std::shared_ptr<novatel::edie::EnumField>;
    using ConstPtr = std::shared_ptr<const novatel::edie::EnumField>;
};

//-----------------------------------------------------------------------
//! \struct ArrayField
//! \brief Struct containing elements of array fields in the UI DB.
//-----------------------------------------------------------------------
struct ArrayField : novatel::edie::BaseField
{
    uint32_t arrayLength{0};

    ArrayField() = default;

    ArrayField* clone() override { return new novatel::edie::ArrayField(*this); }

    using Ptr = std::shared_ptr<novatel::edie::ArrayField>;
    using ConstPtr = std::shared_ptr<const novatel::edie::ArrayField>;
};

//-----------------------------------------------------------------------
//! \struct FieldArrayField
//! \brief Struct containing elements of field array fields in the UI DB.
//-----------------------------------------------------------------------
struct FieldArrayField : novatel::edie::BaseField
{
    uint32_t arrayLength{0}, fieldSize{0};
    std::vector<std::shared_ptr<novatel::edie::BaseField>> fields;

    FieldArrayField() = default;

    FieldArrayField(const FieldArrayField& that) : BaseField(that)
    {
        for (const auto& field : that.fields) { fields.push_back(std::shared_ptr<BaseField>(field->clone())); }

        arrayLength = that.arrayLength;
        fieldSize = that.fieldSize;
    }

    FieldArrayField& operator=(FieldArrayField that)
    {
        if (this != &that)
        {
            BaseField::operator=(that);

            fields.clear();
            for (const auto& field : that.fields) { fields.push_back(std::shared_ptr<BaseField>(field->clone())); }

            arrayLength = that.arrayLength;
            fieldSize = that.fieldSize;
        }

        return *this;
    }

    FieldArrayField* clone() override { return new FieldArrayField(*this); }

    using Ptr = std::shared_ptr<novatel::edie::FieldArrayField>;
    using ConstPtr = std::shared_ptr<const novatel::edie::FieldArrayField>;
};

//-----------------------------------------------------------------------
//! \struct MessageDefinition
//! \brief Struct containing elements of message definitions in the UI
//! DB.
//-----------------------------------------------------------------------
struct MessageDefinition
{
    std::string _id;
    uint32_t logID{0};
    std::string name;
    std::string description;
    std::map<uint32_t, std::vector<novatel::edie::BaseField::Ptr>> fields; // map of crc keys to field defs
    uint32_t latestMessageCrc{0};

    MessageDefinition() = default;
    MessageDefinition(const MessageDefinition& that)
    {
        for (const auto& fielddefs : that.fields)
        {
            uint32_t key = fielddefs.first;
            // Ensure a 0-length vector exists for this key in the case the message has no fields.
            fields[key].clear();
            for (const auto& field : fielddefs.second) { fields[key].push_back(std::shared_ptr<BaseField>(field->clone())); }
        }

        _id = that._id;
        logID = that.logID;
        name = that.name;
        description = that.description;
        latestMessageCrc = that.latestMessageCrc;
    }

    MessageDefinition& operator=(MessageDefinition that)
    {
        if (this != &that)
        {
            fields.clear();
            for (const auto& fielddefs : that.fields)
            {
                uint32_t key = fielddefs.first;
                // Ensure a 0-length vector exists for this key in the case the message has no fields.
                fields[key].clear();
                for (const auto& field : fielddefs.second) { fields[key].push_back(std::shared_ptr<BaseField>(field->clone())); }
            }

            _id = that._id;
            logID = that.logID;
            name = that.name;
            description = that.description;
            latestMessageCrc = that.latestMessageCrc;
        }

        return *this;
    }

    using Ptr = std::shared_ptr<novatel::edie::MessageDefinition>;
    using ConstPtr = std::shared_ptr<const novatel::edie::MessageDefinition>;
};

// Forward declaration of from_json
void from_json(const json& j, novatel::edie::EnumDataType& f);
void from_json(const json& j, novatel::edie::BaseDataType& f);
void from_json(const json& j, novatel::edie::SimpleDataType& f);
void from_json(const json& j, novatel::edie::BaseField& f);
void from_json(const json& j, novatel::edie::EnumField& f);
void from_json(const json& j, novatel::edie::ArrayField& fd);
void from_json(const json& j, novatel::edie::FieldArrayField& fd);
void from_json(const json& j, MessageDefinition& md);
void from_json(const json& j, EnumDefinition& ed);

// Forward declaration of parse_fields and parse_enumerators
uint32_t parse_fields(const json& j, std::vector<novatel::edie::BaseField::Ptr>& vFields);
void parse_enumerators(const json& j, std::vector<novatel::edie::EnumDataType>& vEnumerators);

} // namespace novatel::edie

//============================================================================
//! \class JsonReader
//! \brief Responsible for translating the Json representation of the
//! NovAtel UI DB.
//============================================================================
class JsonReader
{
    std::vector<novatel::edie::MessageDefinition::Ptr> vMessageDefinitions;
    std::vector<novatel::edie::EnumDefinition::Ptr> vEnumDefinitions;
    std::unordered_map<std::string, novatel::edie::MessageDefinition::Ptr> mMessageName;
    std::unordered_map<int32_t, novatel::edie::MessageDefinition::Ptr> mMessageID;
    std::unordered_map<std::string, novatel::edie::EnumDefinition::Ptr> mEnumName;
    std::unordered_map<std::string, novatel::edie::EnumDefinition::Ptr> mEnumID;

  public:
    //----------------------------------------------------------------------------
    //! \brief A constructor for the JsonReader class.
    //----------------------------------------------------------------------------
    JsonReader() = default;

    //----------------------------------------------------------------------------
    //! \brief A copy constructor for the JsonReader class.
    //
    //! \param [in] that The JsonReader object to copy.
    //----------------------------------------------------------------------------
    JsonReader(const JsonReader& that)
    {
        // TODO Verify it's calling the copy constructor for the messages
        vEnumDefinitions = that.vEnumDefinitions;
        vMessageDefinitions = that.vMessageDefinitions;
        GenerateMappings();
    }

    //----------------------------------------------------------------------------
    //! \brief Overloaded assignment operator for the JsonReader class.
    //
    //! \param [in] that The JsonReader object to assign.
    //----------------------------------------------------------------------------
    JsonReader& operator=(JsonReader that)
    {
        if (this != &that)
        {
            vEnumDefinitions = that.vEnumDefinitions;
            vMessageDefinitions = that.vMessageDefinitions;
            GenerateMappings();
        }

        return *this;
    }

    //----------------------------------------------------------------------------
    //! \brief Destructor for the JsonReader class.
    //----------------------------------------------------------------------------
    ~JsonReader() = default;

    //----------------------------------------------------------------------------
    //! \brief Load a Json DB from the provided filepath.
    //
    //! \param [in] filePath_ The filepath to the Json file.
    //----------------------------------------------------------------------------
    template <typename T> void LoadFile(T filePath_);

    //----------------------------------------------------------------------------
    //! \brief Append a message Json DB from the provided filepath.
    //
    //! \param [in] filePath_ The filepath to the Json file.
    //----------------------------------------------------------------------------
    template <typename T> void AppendMessages(T filePath_);

    //----------------------------------------------------------------------------
    //! \brief Append an enumeration Json DB from the provided filepath.
    //
    //! \param [in] filePath_ The filepath to the Json file.
    //----------------------------------------------------------------------------
    template <typename T> void AppendEnumerations(T filePath_);

    //----------------------------------------------------------------------------
    //! \brief Append an message Json DB from the provided filepath.
    //
    //! \param [in] iMsgId_ The message Id
    //----------------------------------------------------------------------------
    template <typename T> void RemoveMessage(T iMsgId_, bool bGenerateMappings_ = true);

    //----------------------------------------------------------------------------
    //! \brief Append an enumeration Json DB from the provided filepath.
    //
    //! \param [in] strEnumeration_ The enumeration name
    //----------------------------------------------------------------------------
    template <typename T> void RemoveEnumeration(T strEnumeration_, bool bGenerateMappings_ = true);

    //----------------------------------------------------------------------------
    //! \brief Parse the Json string provided.
    //
    //! \param [in] strJsonData_ A string containing Json objects.
    //----------------------------------------------------------------------------
    void ParseJson(const std::string& strJsonData_);

    //----------------------------------------------------------------------------
    //! \brief Get a UI DB message definition for the provided message name.
    //
    //! \param [in] strMsgName_ A string containing the message name.
    //----------------------------------------------------------------------------
    novatel::edie::MessageDefinition::ConstPtr GetMsgDef(const std::string& strMsgName_);

    //----------------------------------------------------------------------------
    //! \brief Get a UI DB message definition for the provided message ID.
    //
    //! \param [in] iMsgID_ The message ID.
    //----------------------------------------------------------------------------
    novatel::edie::MessageDefinition::ConstPtr GetMsgDef(int32_t iMsgID_);

    //----------------------------------------------------------------------------
    //! \brief Get a UI DB enum definition for the provided enum ID or name.
    //
    //! \param [in] sEnumNameOrID_ The enum ID or name.
    //----------------------------------------------------------------------------
    novatel::edie::EnumDefinition::Ptr GetEnumDef(const std::string& sEnumNameOrID_);

  private:
    void GenerateMappings()
    {
        for (auto& enm : vEnumDefinitions)
        {
            mEnumName[enm->name] = enm;
            mEnumID[enm->_id] = enm;
        }

        for (auto& msg : vMessageDefinitions)
        {
            mMessageName[msg->name] = msg;
            mMessageID[msg->logID] = msg;

            for (const auto& field : msg->fields) { MapMessageEnumFields(field.second); }
        }
    }

    void MapMessageEnumFields(const std::vector<std::shared_ptr<novatel::edie::BaseField>>& vMsgDefFields_)
    {
        for (const auto& field : vMsgDefFields_)
        {
            if (field->type == novatel::edie::FIELD_TYPE::ENUM)
            {
                std::dynamic_pointer_cast<novatel::edie::EnumField>(field)->enumDef =
                    GetEnumDef(std::dynamic_pointer_cast<novatel::edie::EnumField>(field)->enumID);
            }
            else if (field->type == novatel::edie::FIELD_TYPE::FIELD_ARRAY)
            {
                MapMessageEnumFields(std::dynamic_pointer_cast<novatel::edie::FieldArrayField>(field)->fields);
            }
        }
    }

    void RemoveMessageMapping(novatel::edie::MessageDefinition& msg)
    {
        // Check string against name map
        auto itName = mMessageName.find(msg.name);
        if (itName != mMessageName.end()) mMessageName.erase(itName);

        auto itId = mMessageID.find(msg.logID);
        if (itId != mMessageID.end()) mMessageID.erase(itId);
    }

    void RemoveEnumerationMapping(novatel::edie::EnumDefinition& enm)
    {
        // Check string against name map
        auto itName = mEnumName.find(enm.name);
        if (itName != mEnumName.end()) mEnumName.erase(itName);

        auto itId = mEnumID.find(enm.name);
        if (itId != mEnumID.end()) mEnumID.erase(itId);
    }

    std::vector<novatel::edie::MessageDefinition::Ptr>::iterator GetMessageIt(uint32_t iMsgId_)
    {
        return find_if(vMessageDefinitions.begin(), vMessageDefinitions.end(),
                       [iMsgId_](novatel::edie::MessageDefinition::ConstPtr elem) { return (elem->logID == iMsgId_); });
    }

    std::vector<novatel::edie::MessageDefinition::Ptr>::iterator GetMessageIt(const std::string& strMessage_)
    {
        return find_if(vMessageDefinitions.begin(), vMessageDefinitions.end(),
                       [strMessage_](novatel::edie::MessageDefinition::ConstPtr elem) { return (elem->name == strMessage_); });
    }

    std::vector<novatel::edie::EnumDefinition::Ptr>::iterator GetEnumIt(const std::string& strEnumeration_)
    {
        return find_if(vEnumDefinitions.begin(), vEnumDefinitions.end(),
                       [strEnumeration_](novatel::edie::EnumDefinition::ConstPtr elem) { return (elem->name == strEnumeration_); });
    }

  public:
    using Ptr = std::shared_ptr<JsonReader>;
    using ConstPtr = std::shared_ptr<const JsonReader>;
};

#endif
