//
// Created by cwb on 2022/3/3.
//
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <stack>
#include <string>

#define WRITER reinterpret_cast<PrettyWriter<StringBuffer>*>(m_writer)
#define STREAM reinterpret_cast<StringBuffer*>(m_stream)

using namespace rapidjson;

class JsonWriter
{
public:
    JsonWriter() :
        m_writer(), m_stream()
    {
        m_stream = new StringBuffer;
        m_writer = new PrettyWriter<StringBuffer>(*STREAM);
    }
    ~JsonWriter()
    {
        delete WRITER;
        delete STREAM;
    }
    JsonWriter(JsonWriter&) = delete;
    JsonWriter& operator=(JsonWriter&) = delete;

    const char* getString() const
    {
        return STREAM->GetString();
    }

    explicit operator bool() const { return true; }

    JsonWriter& startObject()
    {
        WRITER->StartObject();
        return *this;
    }

    JsonWriter& endObject()
    {
        WRITER->EndObject();
        return *this;
    }

    JsonWriter& member(const char* name)
    {
        WRITER->String(name, static_cast<SizeType>(strlen(name)));
        return *this;
    }

    bool hasMember(const char* name)
    {
        // This function is for JsonReader only.
        assert(false);
        return false;
    }

    JsonWriter& startArray(size_t* size = 0)
    {
        WRITER->StartArray();
        return *this;
    }

    JsonWriter& EndArray()
    {
        WRITER->EndArray();
        return *this;
    }

    JsonWriter& operator&(bool& b)
    {
        WRITER->Bool(b);
        return *this;
    }

    JsonWriter& operator&(unsigned& u)
    {
        WRITER->Uint(u);
        return *this;
    }

    JsonWriter& operator&(int& i)
    {
        WRITER->Int(i);
        return *this;
    }
    JsonWriter& operator&(double& d)
    {
        WRITER->Double(d);
        return *this;
    }
    JsonWriter& operator&(std::string& s)
    {
        WRITER->String(s.c_str(), static_cast<SizeType>(s.size()));
        return *this;
    }

    JsonWriter& SetNull()
    {
        WRITER->Null();
        return *this;
    }

    static const bool m_isReader = false;
    static const bool m_isWriter = !m_isReader;

private:
    void* m_writer;
    void* m_stream;
};

struct JsonReaderStackItem
{
    enum State
    {
        BeforeStart,
        Started,
        Closed
    };
    JsonReaderStackItem(const Value* value, State state) :
        m_value(value), m_state(state) {}

    const Value* m_value;
    State m_state;
    SizeType m_index = 0;
};

using JsonReaderStack = std::stack<JsonReaderStackItem>;

#define DOCUMENT reinterpret_cast<Document*>(m_document)
#define STACK (reinterpret_cast<JsonReaderStack*>(m_stack))
#define TOP (STACK->top())
#define CURRENT (*TOP.m_value)

class JsonReader
{
public:
    JsonReader(const char* json)
    {
        m_document = new Document;
        DOCUMENT->Parse(json);
        if (DOCUMENT->HasParseError())
        {
            m_error = true;
        }
        else
        {
            m_stack = new JsonReaderStack;
            reinterpret_cast<JsonReaderStack*>(m_stack)->push(JsonReaderStackItem(DOCUMENT, JsonReaderStackItem::BeforeStart));
        }
    }
    ~JsonReader()
    {
        delete DOCUMENT;
        delete STACK;
    }
    JsonReader(const JsonReader&) = delete;
    JsonReader& operator=(const JsonReader&) = delete;
    explicit operator bool() const { return !m_error; }

    JsonReader& startObject()
    {
        if (!m_error)
        {
            if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::BeforeStart)
            {
                TOP.m_state = JsonReaderStackItem::Started;
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& endObject()
    {
        if (!m_error)
        {
            if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::Started)
            {
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    void next()
    {
        if (!m_error)
        {
            assert(!STACK->empty());
            STACK->pop();
            if (!STACK->empty() && CURRENT.IsArray())
            {
                if (TOP.m_state == JsonReaderStackItem::Started)
                {
                    if (TOP.m_index < CURRENT.Size() - 1)
                    {
                        const Value* value = &CURRENT[++TOP.m_index];
                        STACK->push(JsonReaderStackItem(value, JsonReaderStackItem::BeforeStart));
                    }
                    else
                    {
                        TOP.m_state = JsonReaderStackItem::Closed;
                    }
                }
                else
                {
                    m_error = true;
                }
            }
        }
    }

    JsonReader& member(const char* name)
    {
        if (!m_error)
        {
            if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::Started)
            {
                auto member = CURRENT.FindMember(name);
                if (member != CURRENT.MemberEnd())
                {
                    STACK->push(JsonReaderStackItem(&member->value, JsonReaderStackItem::BeforeStart));
                }
                else
                {
                    m_error = true;
                }
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    bool hasMember(const char* name) const
    {
        if (!m_error && CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::Started)
        {
            return CURRENT.HasMember(name);
        }
        return false;
    }

    JsonReader& startArray(size_t* size)
    {
        if (!m_error)
        {
            if (CURRENT.IsArray() && TOP.m_state == JsonReaderStackItem::BeforeStart)
            {
                TOP.m_state == JsonReaderStackItem::Started;
                if (size)
                {
                    *size = CURRENT.Size();
                }
                if (!CURRENT.Empty())
                {
                    const Value* value = &CURRENT[TOP.m_index];
                    STACK->push(JsonReaderStackItem(value, JsonReaderStackItem::BeforeStart));
                }
                else
                {
                    TOP.m_state = JsonReaderStackItem::Closed;
                }
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& endArray()
    {
        if (!m_error)
        {
            if (CURRENT.IsArray() && TOP.m_state == JsonReaderStackItem::Closed)
            {
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& operator&(bool& b)
    {
        if (!m_error)
        {
            if (CURRENT.IsBool())
            {
                b = CURRENT.GetBool();
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& operator&(unsigned& u)
    {
        if (!m_error)
        {
            if (CURRENT.IsUint())
            {
                u = CURRENT.GetUint();
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& operator&(int& i)
    {
        if (!m_error)
        {
            if (CURRENT.IsInt())
            {
                i = CURRENT.GetInt();
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& operator&(double& d)
    {
        if (!m_error)
        {
            if (CURRENT.IsDouble())
            {
                d = CURRENT.GetDouble();
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& operator&(std::string& s)
    {
        if (!m_error)
        {
            if (CURRENT.IsString())
            {
                s = CURRENT.GetString();
                next();
            }
            else
            {
                m_error = true;
            }
        }
        return *this;
    }

    JsonReader& setNull()
    {
        // This function is for JsonWriter only.
        m_error = true;
        return *this;
    }

    static const bool m_isReader = true;
    static const bool m_isWriter = !m_isReader;

private:
    // PIMPL
    void* m_document = nullptr; ///< DOM result of parsing.
    void* m_stack = nullptr;    ///< Stack for iterating the DOM
    bool m_error = false;       ///< Whether an error has occurred.
};

struct Student
{
    Student() :
        m_name(), m_age(), m_height(), m_canSwim() {}
    Student(const std::string name, unsigned age, double height, bool canSwim) :
        m_name(name), m_age(age), m_height(height), m_canSwim(canSwim) {}
    ~Student() = default;

    std::string m_name;
    unsigned m_age;
    double m_height;
    bool m_canSwim;
};

std::ostream& operator<<(std::ostream& os, const Student& s)
{
    return os << "[" << s.m_name << ", " << s.m_age << ", " << s.m_height << ", " << (s.m_canSwim ? "true" : "false") << "]";
}

template <typename T>
T& operator&(T& ar, Student& s)
{
    ar.startObject();
    ar.member("name") & s.m_name;
    ar.member("age") & s.m_age;
    ar.member("height") & s.m_height;
    ar.member("canSwim") & s.m_canSwim;
    return ar.endObject();
}

void rapidjsonTest()
{
    std::string json;
    {
        Student s("william", 26, 170.0, true);
        JsonWriter writer;
        writer& s;
        json = writer.getString();
        std::cout << json << std::endl;
    }
    {
        Student s;
        JsonReader reader(json.c_str());
        reader& s;
        std::cout << s << std::endl;
        JsonWriter writer;
        writer& s;
        std::cout << writer.getString() << std::endl;
    }
}
