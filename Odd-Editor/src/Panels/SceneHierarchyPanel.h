#pragma once
#include "Odd/Core/Core.h"
#include "Odd/Core/Log.h"
#include "Odd/Scene/Scene.h"
#include "Odd/Scene/Entity.h"
#include "Odd/Scene/Components.h"


namespace Odd
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }
		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}