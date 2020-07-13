#ifndef _RIVE_COMPONENT_BASE_HPP_
#define _RIVE_COMPONENT_BASE_HPP_
#include "core.hpp"
#include "core/field_types/core_int_type.hpp"
#include "core/field_types/core_string_type.hpp"
#include <string>
namespace rive
{
	class ComponentBase : public Core
	{
	public:
		static const int typeKey = 10;

		// Helper to quickly determine if a core object extends another without RTTI
		/// at runtime.
		bool inheritsFrom(int typeKey) override { return false; }

		int coreType() const override { return typeKey; }

		static const int namePropertyKey = 4;
		static const int parentIdPropertyKey = 5;

	private:
		std::string m_Name;
		int m_ParentId = 0;
	public:
		std::string name() const { return m_Name; }
		void name(std::string value) { m_Name = value; }

		int parentId() const { return m_ParentId; }
		void parentId(int value) { m_ParentId = value; }

		bool deserialize(int propertyKey, BinaryReader& reader) override
		{
			switch (propertyKey)
			{
				case namePropertyKey:
					m_Name = CoreStringType::deserialize(reader);
					return true;
				case parentIdPropertyKey:
					m_ParentId = CoreIntType::deserialize(reader);
					return true;
			}
			return false;
		}
	};
} // namespace rive

#endif