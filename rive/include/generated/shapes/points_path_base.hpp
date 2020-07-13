#ifndef _RIVE_POINTS_PATH_BASE_HPP_
#define _RIVE_POINTS_PATH_BASE_HPP_
#include "core/field_types/core_bool_type.hpp"
#include "shapes/path.hpp"
namespace rive
{
	class PointsPathBase : public Path
	{
	public:
		static const int typeKey = 16;

		// Helper to quickly determine if a core object extends another without RTTI
		/// at runtime.
		bool inheritsFrom(int typeKey) override
		{
			switch (typeKey)
			{
				case PathBase::typeKey:
				case NodeBase::typeKey:
				case ContainerComponentBase::typeKey:
				case ComponentBase::typeKey:
					return true;
				default:
					return false;
			}
		}

		int coreType() const override { return typeKey; }

		static const int isClosedPropertyKey = 32;

	private:
		bool m_IsClosed = false;
	public:
		bool isClosed() const { return m_IsClosed; }
		void isClosed(bool value) { m_IsClosed = value; }

		bool deserialize(int propertyKey, BinaryReader& reader) override
		{
			switch (propertyKey)
			{
				case isClosedPropertyKey:
					m_IsClosed = CoreBoolType::deserialize(reader);
					return true;
			}
			return Path::deserialize(propertyKey, reader);
		}
	};
} // namespace rive

#endif