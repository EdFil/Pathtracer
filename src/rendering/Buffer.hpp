#pragma once

#include <cstdint>

class Buffer {
public:
	enum class DataLayout { Pos3 = 0, Pos3Norm3, Pos3Norm3Uv2 };
    enum class Type { Array = 0, Indices };
    enum class Usage { Static = 0, Stream, Dynamic };

	struct Params {
		Buffer::DataLayout layout = Buffer::DataLayout::Pos3;
		Buffer::Type type = Buffer::Type::Array;
		Buffer::Usage usage = Buffer::Usage::Static;
		void* data;
		uint32_t size;
	};

    virtual ~Buffer() = default;

    virtual bool init(const Buffer::Params& params) = 0;
	virtual bool updateData(void* data, uint32_t size) = 0;
	virtual void bind() = 0;
	virtual uint32_t id() const = 0;

protected:
	DataLayout _layout{DataLayout::Pos3};
	Type _type{Type::Array};
	Usage _usage{Usage::Static};
};