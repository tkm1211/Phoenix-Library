#include "pch.h"
#include <imgui.h>
#include "Phoenix/FND/Util.h"
#include "Phoenix/Math/Arithmetic.h"
#include "Phoenix/FrameWork/Component/Camera.h"
#include "Phoenix/FrameWork/Game.h"
#include "Phoenix/FrameWork/GameObject.h"


namespace ys
{
	namespace fwk
	{
		//============================================================================
		// カメラ
		//----------------------------------------------------------------------------
		// 生成時に一度だけ呼ばれる
		void Camera::Awake(IGame* game)
		{
			m_camera.SetPerspective(m_fov, m_aspect, m_near_z, m_far_z);
		}

		// 描画ノード収集
		void Camera::GetRenderables(IGame* game, RenderableCollection& collection, bool force)
		{
			if (game->GetRenderState()->GetRenderView() != RenderView::Scene)
			{
				return;
			}

			std::shared_ptr<GameObject> owner = GetOwner();

			grp::Camera& camera = game->GetGraphicsContext()->GetCamera();
			if (force || IsVisible(game))
			{
				RenderableNode node;
				node.mesh = game->GetShapeLibrary()->GetMesh(ShapeType::Camera);
				node.world_transform = owner->GetWorldTransform();
				node.bone_transforms = nullptr;
				node.bone_transform_count = 0;
				node.material = &m_material;
				node.bounds = owner->GetWorldBounds();
				node.flags = grp::RenderableNodeFlag::None;
				node.object = owner.get();
				node.owner = owner.get();

				grp::RenderFlag flags = grp::RenderFlag::Textured | grp::RenderFlag::Lit;
				collection.Add(node, game, flags, game->GetShaderLibrary()->GetShaderId(SystemShader::Standard));
			}
		}

		// 行列更新処理
		void Camera::UpdateTransform(IGame* game)
		{
			std::shared_ptr<GameObject> owner = GetOwner();

			math::Matrix view;
			math::MatrixInverse(&view, nullptr, &owner->GetWorldTransform());
			m_camera.SetView(view);
			m_camera.Update();

			owner->ExtendLocalBounds(game->GetShapeLibrary()->GetMesh(ShapeType::Camera)->GetBounds());
		}

		// 開始処理
		void Camera::Start(IGame* game)
		{
			game->GetCameraRegistry()->RegisterCamera(this);
		}

		// 破棄時に呼ばれる
		void Camera::OnDestroy(IGame* game)
		{
			game->GetCameraRegistry()->UnregisterCamera(this);
		}

		// シーンGUI表示
		void Camera::OnSceneGUI(IGame* game)
		{
			super::OnSceneGUI(game);

			std::shared_ptr<GameObject> owner = GetOwner();
			std::shared_ptr<ISelectionContext> selection_context = game->GetGameContext();

			if (!selection_context->SelectionContains(owner.get()))
			{
				return;
			}

			const math::Frustum& frustum = m_camera.GetFrustum();

			math::Vector3 corners[8];
			frustum.GetCorners(corners);

			math::AABB bounds;
			bounds.Reset();
			for (u32 i = 0; i < 8; ++i)
			{
				bounds.Extend(corners[i]);
			}

			grp::Camera& camera = game->GetGraphicsContext()->GetCamera();
			if (0 == math::FrustumAABBIntersect(camera.GetFrustum(), owner->GetWorldBounds()))
			{
				return;
			}

			grp::IContext* graphics_context = game->GetGraphicsContext();
			grp::ILineRenderer* line_renderer = game->GetLineRenderer();

			line_renderer->AddVertex(corners[0]);	// 前左下
			line_renderer->AddVertex(corners[1]);	// 前右下

			line_renderer->AddVertex(corners[1]);	// 前右下
			line_renderer->AddVertex(corners[2]);	// 前右上

			line_renderer->AddVertex(corners[2]);	// 前右上
			line_renderer->AddVertex(corners[3]);	// 前左上

			line_renderer->AddVertex(corners[3]);	// 前左上
			line_renderer->AddVertex(corners[0]);	// 前左下

			line_renderer->AddVertex(corners[4]);	// 奥左下
			line_renderer->AddVertex(corners[5]);	// 奥右下

			line_renderer->AddVertex(corners[5]);	// 奥右下
			line_renderer->AddVertex(corners[6]);	// 奥右上

			line_renderer->AddVertex(corners[6]);	// 奥右下
			line_renderer->AddVertex(corners[7]);	// 奥左上

			line_renderer->AddVertex(corners[7]);	// 奥左上
			line_renderer->AddVertex(corners[4]);	// 奥左下

			line_renderer->AddVertex(corners[0]);	// 前左下
			line_renderer->AddVertex(corners[4]);	// 奥左下

			line_renderer->AddVertex(corners[1]);	// 前左下
			line_renderer->AddVertex(corners[5]);	// 奥左下

			line_renderer->AddVertex(corners[2]);	// 前左下
			line_renderer->AddVertex(corners[6]);	// 奥左下

			line_renderer->AddVertex(corners[3]);	// 前左下
			line_renderer->AddVertex(corners[7]);	// 奥左下

			line_renderer->Draw(graphics_context, grp::RenderFlag::DisableDepthTest | grp::RenderFlag::DisableDepthWrite, grp::PrimitiveTopology::LineList);
		}

		// インスペクターGUI表示
		void Camera::OnInspectorGUI(IGame* game)
		{
			super::OnInspectorGUI(game);

			// カメラタイプ
			static const char* s_camera_type_names[] =
			{
				"Main",
				"Sub"
			};
			if (ImGui::BeginCombo("camera type", s_camera_type_names[static_cast<int>(m_camera_type)]))
			{
				for (int index = 0; index < fnd::ArraySize(s_camera_type_names); ++index)
				{
					bool is_selected = index == static_cast<int>(m_camera_type);
					if (ImGui::Selectable(s_camera_type_names[index], is_selected))
					{
						m_camera_type = static_cast<CameraType>(index);
					}
				}
				ImGui::EndCombo();
			}

			// クリアフラグ
			static const char* s_clear_flags_names[] =
			{
				"SkyBox",
				"SolidColor",
				"Depth",
				"Nothing"
			};
			if (ImGui::BeginCombo("clear flags", s_clear_flags_names[static_cast<int>(m_clear_flags)]))
			{
				for (int index = 0; index < fnd::ArraySize(s_clear_flags_names); ++index)
				{
					bool is_selected = index == static_cast<int>(m_clear_flags);
					if (ImGui::Selectable(s_clear_flags_names[index], is_selected))
					{
						m_clear_flags = static_cast<CameraClearFlags>(index);
					}
				}
				ImGui::EndCombo();
			}

			// 背景色
			if (ImGui::ColorEdit4("background color", m_background_color.v, ImGuiColorEditFlags_Float))
			{
			}

			// カリングマスク
			const char* culling_mask_nothing_name = "Nothing";
			const char* culling_mask_everything_name = "Everything";
			const char* culling_mask_mixed_name = "Mixed..";
			const char* culling_mask_layer_names[32] =
			{
				"Layer0", "Layer1", "Layer2", "Layer3", "Layer4", "Layer5", "Layer6", "Layer7",
				"Layer8", "Layer9", "Layer10", "Layer11", "Layer12", "Layer13", "Layer14", "Layer15",
				"Layer16", "Layer17", "Layer18", "Layer19", "Layer20", "Layer21", "Layer22", "Layer23",
				"Layer24", "Layer25", "Layer26", "Layer27", "Layer28", "Layer29", "Layer30", "Layer31",
			};
			bool culling_mask_nothing = m_culling_mask_flag == CullingMaskFlag::Nothing;
			bool culling_mask_everything = m_culling_mask_flag == CullingMaskFlag::Everything;
			bool culling_mask_layers[32];
			for (int i = 0; i < 32; ++i)
			{
				culling_mask_layers[i] = m_culling_mask_flag.Test(1 << i);
			}

			const char* culling_mask_name = culling_mask_nothing ?
				culling_mask_nothing_name : culling_mask_everything ?
				culling_mask_everything_name :
				culling_mask_mixed_name;


			if (ImGui::BeginCombo("culling mask", culling_mask_name))
			{
				ImGui::OpenPopup("culling mask popup");

				if (ImGui::BeginPopup("culling mask popup"))
				{
					if (ImGui::MenuItem(culling_mask_nothing_name, "", &culling_mask_nothing))
					{
						m_culling_mask_flag = CullingMaskFlag::Nothing;
					}
					if (ImGui::MenuItem(culling_mask_everything_name, "", &culling_mask_everything))
					{
						m_culling_mask_flag = CullingMaskFlag::Everything;
					}
					for (int i = 0; i < 32; ++i)
					{
						if (ImGui::MenuItem(culling_mask_layer_names[i], "", &culling_mask_layers[i]))
						{
							m_culling_mask_flag.Toggle(1 << i);
						}
					}

					ImGui::EndPopup();
				}
				ImGui::EndCombo();
			}


			// 投影方法
			const char* projection_perspective_name = "Perspective";
			const char* projection_orthographic_name = "Orthographic";
			const char* projection_name = m_camera.IsOrtho() ? projection_orthographic_name : projection_perspective_name;
			if (ImGui::BeginCombo("projection", projection_name))
			{
				if (ImGui::Selectable(projection_perspective_name, !m_camera.IsOrtho()))
				{
					m_camera.SetViewType(true);
				}

				if (ImGui::Selectable(projection_orthographic_name, m_camera.IsOrtho()))
				{
					m_camera.SetViewType(false);
				}

				ImGui::EndCombo();
			}

			// 視野角
			if (!m_camera.IsOrtho())
			{
				f32 fov = math::ToDegree(m_camera.GetFovY());
				if (ImGui::SliderFloat("field of view", &fov, 0.01f, 179.0f, "%.3f"))
				{
					m_camera.SetFovY(math::ToRadian(fov));
				}
			}

			// クリッピング面
			f32 clipping_near_plane = m_camera.GetNearZ();
			if (ImGui::InputFloat("clipping near plane", &clipping_near_plane, 0.0f, 0.0f))
			{
				m_camera.SetNearZ(clipping_near_plane);
			}

			f32 clipping_far_plane = m_camera.GetFarZ();
			if (ImGui::InputFloat("clipping far plane", &clipping_far_plane, 0.0f, 0.0f))
			{
				m_camera.SetFarZ(clipping_far_plane);
			}

			// レンダリング順序
			ImGui::InputFloat("depth", &m_depth, 0.0f, 0.0f);

			// ビューポート矩形
			ImGui::InputFloat("viewport x", &m_viewport_rect.x, 0.0f, 0.0f);
			ImGui::InputFloat("viewport y", &m_viewport_rect.y, 0.0f, 0.0f);
			ImGui::InputFloat("viewport w", &m_viewport_rect.w, 0.0f, 0.0f);
			ImGui::InputFloat("viewport h", &m_viewport_rect.h, 0.0f, 0.0f);

		}

	}	// namespace fwk
}	// namespace ys