#include "shapes/path_composer.hpp"
#include "renderer.hpp"
#include "shapes/path.hpp"
#include "shapes/shape.hpp"

using namespace rive;

PathComposer::~PathComposer()
{
	delete m_LocalPath;
	delete m_WorldPath;
}

void PathComposer::onAddedClean(CoreContext* context)
{
	// Find the shape.
	for (auto currentParent = parent(); currentParent != nullptr; currentParent = currentParent->parent())
	{
		if (currentParent->is<Shape>())
		{
			m_Shape = currentParent->as<Shape>();
			m_Shape->pathComposer(this);
			return;
		}
	}
}

void PathComposer::buildDependencies()
{
	assert(m_Shape != nullptr);
	m_Shape->addDependent(this);
	for (auto path : m_Shape->paths())
	{
		path->addDependent(this);
	}
}

void PathComposer::update(ComponentDirt value)
{
	if (hasDirt(value, ComponentDirt::Path))
	{
		auto space = m_Shape->pathSpace();
		if ((space & PathSpace::Local) == PathSpace::Local)
		{
			if (m_LocalPath == nullptr)
			{
				m_LocalPath = makeRenderPath();
			}
			else
			{
				m_LocalPath->reset();
			}
			auto world = m_Shape->worldTransform();
			Mat2D inverseWorld;
			if (!Mat2D::invert(inverseWorld, world)) {
				Mat2D::identity(inverseWorld);
			}
			// Get all the paths into local shape space.
			for (auto path : m_Shape->paths()) 
			{
				Mat2D localTransform;
				auto transform = path->pathTransform();
				Mat2D::multiply(localTransform, inverseWorld, transform);
				m_LocalPath->addPath(path->renderPath(), localTransform);
			}
		}
		if ((space & PathSpace::World) == PathSpace::World)
		{
			if (m_WorldPath == nullptr)
			{
				m_WorldPath = makeRenderPath();
			}
			else
			{
				m_WorldPath->reset();
			}
			for (auto path : m_Shape->paths()) 
			{
				const Mat2D& transform = path->pathTransform();
				m_WorldPath->addPath(path->renderPath(), transform);
			}
		}
	}
}