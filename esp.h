#include <Windows.h>
#include <iostream>
#include <D3D11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>
#include "classes.h"
#include "chams.h"

namespace settings
{
	int folder;

	namespace Visuals
	{
		int enabled = true;
		int boxes = false;
		int name = false; //
		int health = false; //
		int distance = false; //
		int skeleton = false; //
		int max_distance = 300;
		int held_item = false;
		int chams = false;
		int hotbar = false;
		int boxescorner = false;
		int filledbox = false;
		float BoxColor[3] = { 255, 255, 255 };
		float testespcolor[3] = { 255, 255, 255 };
		float Boxfilled[3] = { 255, 255, 255 };
		float namecolor[3] = { 255, 255, 255 };
		float BoxcornerColor[3] = { 255, 255, 255 };
		float SkeletonColor[3] = { 0, 255, 230 };
	}

	namespace ore
	{
		int enabled = true;
		int name = true;
		int distance = false;
		int max_distance = 200;
		int stone = false;
		int metal = false;
		int sulfur = false;
	}

	namespace collectable
	{
		int enabled = true;
		int name = true;
		int distance = false;
		int max_distance = 200;
		int hemp = false;
		int collectables = false;
		int food = false;
	}

	namespace dropped
	{
		int enabled = true;
		int name = true;
		int distance = false;
		int show_weapons = false;
		int show_ammo = false;
		int show_scrap = false;
		int show_all = false;
		int max_distance = 200;
	}

	namespace otheresp
	{
		namespace stashes
		{
			int enabled = true;
			int max_distance = 200;
		}

		namespace vehicles
		{
			int enabled = true;
			int max_distance = 450;
		}

		namespace crates
		{
			int enabled = true;
			int max_distance = 200;
		}

		namespace barrels
		{
			int enabled = true;
			int max_distance = 200;
		}
	}

	namespace misc
	{
		int testing_esp = false;
		int spider = false;
		int infinatejump = false;
		int debug_cam = false;
		int always_eoka = false;
		int always_chainsaw = false;
		int fast_bow = false;
		int fast_reload = false;
		int no_spread = false;
		int automatic = false;
		int always_day = false;
		int gravity_enabled = false;
		int water_walk = false;
		int rapid_fire = false;
		int no_recoil = false;
		int gravity1 = false;
		int fake_lag = false;
		int spin_bot = false;
		int run_and_gun = false;
		int runandshoot = false;
		float gravity = 2.5f;
		int longneck = false;
		float longneckupwards = 1.5f;
	}

	namespace aimbot
	{
		int enabled = true;
		int fov = 100;
		int max_distance = 300;
		int shoot_head = false;
		int use_prediction = false;
		int aim_key = 0x43;
		int silent_key = 1;
		int silent = false;
		int use_mouse = false;
		int smooth = 10;
		int teamcheck = false;
		int crosshair = true;
	}

	int spacer;
}

uint64_t	GABaseAddress;
uint64_t	UnityBaseAddress;
uint64_t	GOM;
uint64_t	BaseNetworkable;
uint64_t	TOD_Sky;
BasePlayer	LocalPlayer;
BasePlayer	closestPlayer;
Vector3		LocalPosition;
Matrix4x4	ViewMatrix;

std::vector<long> TeamMateList;

char className[64];
char draw_buffer[0x50];

float FOV = 300, curFOV;

void Normalize(float& Yaw, float& Pitch)
{
	if (Pitch < -89)
		Pitch = -89;

	else if (Pitch > 89)
		Pitch = 89;

	if (Yaw < -360)
		Yaw += 360;

	else if (Yaw > 360)
		Yaw -= 360;
}

Vector3 Cross(Vector3 first_vec, Vector3 second_vec)
{
	Vector3 returnme = Vector3();
	returnme.x = first_vec.y * second_vec.z - first_vec.z * second_vec.y;
	returnme.y = first_vec.z * second_vec.x - first_vec.x * second_vec.z; //first_vec.x * second_vec.z - first_vec.z * second_vec.x;
	returnme.z = first_vec.x * second_vec.y - first_vec.y * second_vec.x;
	return returnme;
}

Vector4 QuaternionLookRotation(Vector3 forward, Vector3 upDirection)
{
	Vector3 up = upDirection;

	forward.Normalize();

	Vector3 first_vec = forward.Normalize();;
	Vector3 cross_value = Cross(up, first_vec);
	Vector3 second_vec = cross_value.Normalize();
	Vector3 third_vec = Cross(first_vec, second_vec);

	float m00 = second_vec.x;
	float m01 = second_vec.y;
	float m02 = second_vec.z;

	float m10 = third_vec.x;
	float m11 = third_vec.y;
	float m12 = third_vec.z;

	float m20 = first_vec.x;
	float m21 = first_vec.y;
	float m22 = first_vec.z;

	float num8 = (m00 + m11) + m22;

	Vector4 quaternion = Vector4();

	if (num8 > 0.f)
	{
		float num = sqrtf(num8 + 1.f);
		quaternion.w = num * 0.5f;
		num = 0.5f / num;
		quaternion.x = (m12 - m21) * num;
		quaternion.y = (m20 - m02) * num;
		quaternion.z = (m01 - m10) * num;
		return quaternion;
	}

	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = sqrtf(((1.f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		quaternion.x = 0.5f * num7;
		quaternion.y = (m01 + m10) * num4;
		quaternion.z = (m02 + m20) * num4;
		quaternion.w = (m12 - m21) * num4;
		return quaternion;
	}
	if (m11 > m22)
	{
		float num6 = sqrtf(((1.f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		quaternion.x = (m10 + m01) * num3;
		quaternion.y = 0.5f * num6;
		quaternion.z = (m21 + m12) * num3;
		quaternion.w = (m20 - m02) * num3;
		return quaternion;
	}

	float num5 = sqrtf(((1.f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	quaternion.x = (m20 + m02) * num2;
	quaternion.y = (m21 + m12) * num2;
	quaternion.z = 0.5f * num5;
	quaternion.w = (m01 - m10) * num2;
	return quaternion;
}

bool WorldToScreen(const Vector3& EntityPos, Vector2& ScreenPos)
{
	Vector3 TransVec = Vector3(ViewMatrix._14, ViewMatrix._24, ViewMatrix._34);
	Vector3 RightVec = Vector3(ViewMatrix._11, ViewMatrix._21, ViewMatrix._31);
	Vector3 UpVec = Vector3(ViewMatrix._12, ViewMatrix._22, ViewMatrix._32);
	float w = Math::Dot(TransVec, EntityPos) + ViewMatrix._44;
	if (w < 0.098f) return false;
	float y = Math::Dot(UpVec, EntityPos) + ViewMatrix._42;
	float x = Math::Dot(RightVec, EntityPos) + ViewMatrix._41;
	ScreenPos = Vector2((Globals::rWidth/ 2) * (1.f + x / w), (Globals::rHeight / 2) * (1.f - y / w));
	return true;
}

std::string get_class_name(std::uint64_t class_object)
{
	const auto object_unk = mem.Read(class_object);

	if (!object_unk)
		return {};

	auto classname_ptr = mem.Read(object_unk + 0x10);
	if (!classname_ptr)
		return {};

	return read_ascii(classname_ptr, 64);
}

Vector3 get_object_pos(std::uint64_t entity)
{
	auto player_visual = mem.Read(entity + 0x8);

	if (!player_visual)
		return {};

	auto visual_state = mem.Read(player_visual + 0x38);

	if (!visual_state)
		return {};

	return mem.Read<Vector3>(visual_state + 0x90);
}

bool IsSameTeam(BasePlayer bp)
{
	auto uid = bp.GetUID();
	for (long userid : TeamMateList)
	{
		if (userid == uid)
		{
			//std::cout << ws2s(bp.GetName()).c_str() << " is on the same team!" << std::endl;
			return true;
		}
	}
	//std::cout << ws2s(bp.GetName()).c_str() << " is NOT on the same team!" << std::endl;
	return false;
}

inline float distance_cursor(Vector2 vec)
{
	POINT p;
	if (GetCursorPos(&p))
	{
		float ydist = (vec.y - p.y);
		float xdist = (vec.x - p.x);
		float ret = sqrt(pow(ydist, 2) + pow(xdist, 2));
		return ret;
	}
}

void call_misc_hacks()
{
	if (!LocalPlayer.Player)
		return;

	if (settings::misc::debug_cam)
		LocalPlayer.SetAdminFlag();

	if (settings::misc::no_recoil)
	{
		LocalPlayer.DoNoRecoil();
	}

	if (settings::misc::spider)
		LocalPlayer.DoSpider();

	if (settings::misc::longneck)
		LocalPlayer.LongNeckUpwards();

	if (settings::misc::water_walk)
		LocalPlayer.DoWaterWalk();
	else
		LocalPlayer.UndoWaterWalk();

	auto HeldItem = LocalPlayer.GetHeldItem();
	if (HeldItem.Item)
	{
		if (settings::misc::rapid_fire)
		{
			HeldItem.SetRapidFire();
		}

		if (settings::misc::automatic)
		{
			HeldItem.SetAutomatic();
		}

		if (settings::misc::always_eoka)
		{
			HeldItem.SetAlwaysEoka();
		}

		if (settings::misc::fast_bow)
		{
			HeldItem.SetFastBow();
		}

		if (settings::misc::no_spread)
		{
			if (HeldItem.IsItemGun())
			{
				auto imp = HeldItem.GetItemModProjectile();
				imp.set_projectile_spread(0.f);
				imp.set_projectile_velocity_spread(0.f); // = 0;
			}
		}
	}
}

void render_entities_test(ImDrawList* draw_list)
{
	//std::cout << "GOM" << std::endl;
	GOM = mem.Read((uint64_t)GetModuleHandle(L"UnityPlayer.dll") + 0x17C1F18);
	if (!GOM || GOM == NULL)
	{
		std::cout << "!tagged_objects" << std::endl;
		return;
	}

	//std::cout << "Tagged Object" << std::endl;
	auto tagged_objects = mem.Read(GOM + 0x8);

	if (!tagged_objects)
	{
		std::cout << "!tagged_objects" << std::endl;
		return;
	}

	//std::cout << "Game Object" << std::endl;
	auto game_object = mem.Read(tagged_objects + 0x10);

	if (!game_object)
	{
		std::cout << "!tagged_objects" << std::endl;
		return;
	}

	//std::cout << "Object Class" << std::endl;
	auto object_class = mem.Read(game_object + 0x30);

	if (!object_class)
	{
		std::cout << "!tagged_objects" << std::endl;
		return;
	}

	//std::cout << "Ent" << std::endl;
	auto ent = mem.Read(object_class + 0x18);

	if (!ent)
	{
		std::cout << "!tagged_objects" << std::endl;
		return;
	}

	//std::cout << "ViewMatrix" << std::endl;
	ViewMatrix = mem.Read<Matrix4x4>(ent + 0xDC);

	uint64_t client_entities;

	auto get_client_entities = [&]() {
		client_entities = il2cpp::value(("BaseNetworkable"), ("clientEntities"), false);
	};

	if (!client_entities)
	{
		std::cout << "Updating Client Entities!" << std::endl;
		//get_client_entities();
		std::cout << "Updated Client Entities!" << std::endl;
	}

	if (!client_entities)
	{
		std::cout << "!client_entities" << std::endl;
		return;
	}

	//std::cout << "Size: " << size << std::endl;
	return;
}

BasePlayer get_closest_to_crosshair(uint64_t buffer_list)
{
	if (!LocalPlayer.Player)
		return BasePlayer();

	BasePlayer closestEntity;
	closestEntity.Player = NULL;

	auto object_list = mem.Read(buffer_list + 0x18);
	if (!object_list)
	{
		//std::cout << "!object_list" << std::endl;
		return BasePlayer();
	}
	//printf("object_list: 0x%x\n", object_list);

	//std::cout << "object_list_size" << std::endl;
	auto object_list_size = mem.Read<std::uint32_t>(buffer_list + 0x10);
	if (!object_list_size)
	{
		//std::cout << "!object_list_size" << std::endl;
		return BasePlayer();
	}

	for (auto i = 0; i < object_list_size; i++)
	{
		//std::cout << "current_object" << std::endl;
		auto current_object = mem.Read(object_list + (0x20 + (i * 8)));

		if (!current_object)
			continue;

		//std::cout << "base_object" << std::endl;
		auto base_object = mem.Read(current_object + 0x10);

		if (!base_object)
			continue;

		//std::cout << "object" << std::endl;
		auto object = mem.Read(base_object + 0x30);

		if (!object)
			continue;

		std::string class_name = get_class_name(current_object);
		//std::cout << "Class Name: " << class_name << std::endl;
		if (class_name == "BasePlayer")
		{
			auto name_pointer = mem.Read(object + 0x60);

			if (!name_pointer)
				continue;

			ZeroMemory(&className, sizeof(className));
			if (!mem.Read(name_pointer, &className, sizeof(className)))
			{
				continue;
			}

			auto object_class = mem.Read(object + 0x30);

			if (!object_class)
				continue;

			auto entity = mem.Read(object_class + 0x18);

			if (!entity)
				continue;

			auto the_player = mem.Read(entity + 0x28);

			if (!the_player)
				continue;

			BasePlayer player;
			player.Player = the_player;
			player.ObjectClass = object_class;

			Vector3 pos = player.GetVisualPosition();

			if (strcmp(className, "LocalPlayer") != 0)
			{
				player.IsLocalPlayer = false;
			}
			else
			{
				player.IsLocalPlayer = true;
				LocalPlayer = player;
				LocalPosition = pos;
				//TeamMateList = player.GetTeamList();
				continue;
			}

			auto player_health = player.GetHealth();
			if (!(player_health > 0))
			{
				//std::cout << "Player Dead!" << std::endl;
				continue;
			}

			if (player.IsSleeping())
			{
				continue;
			}

			//std::cout << "Player Health: " << player_health << std::endl;

			auto Distance = Math::Calc3D_Dist(LocalPosition, pos);
			if (Distance > settings::Visuals::max_distance)
				continue;

			pos = player.GetBonePosition(neck);
			if (pos == Vector3{ 0,0,0 })
				continue;

			Vector2 ScreenPos;

			if (WorldToScreen(pos, ScreenPos) && !player.IsSleeping())
			{
				curFOV = Math::Calc2D_Dist(Vector2(Globals::rWidth / 2, Globals::rHeight / 2), ScreenPos);
				if (FOV > curFOV && !player.IsLocalPlayer)
				{
					FOV = curFOV;
					closestEntity = player;
				}
			}
		}
	}

	return closestEntity;
}

auto Prediction(const Vector3& LP_Pos, BasePlayer Player)
{
	Vector3 BonePos = Player.GetBonePosition(neck);

	if (!settings::aimbot::shoot_head)
		BonePos = Player.GetBonePosition(spine1);

	auto helditem = LocalPlayer.GetHeldItem();
	if (!helditem.Item)
		return BonePos;

	float Dist = Math::Calc3D_Dist(LP_Pos, BonePos);

	if (Dist > 0.001f)
	{
		float BulletTime = Dist / LocalPlayer.GetHeldItem().GetWeaponSpeed();
		Vector3 vel = Player.GetVelocity();
		Vector3 PredictVel = vel * BulletTime * 0.75f;
		BonePos += PredictVel; BonePos.y += (4.905f * BulletTime * BulletTime);
	}
	return BonePos;
}

void render_entities(uintptr_t buffer_list, ImDrawList* draw_list)
{
	closestPlayer = BasePlayer{};
	closestPlayer.Player = NULL;

	//std::cout << "object_list" << std::endl;
	auto object_list = mem.Read(buffer_list + 0x18);
	if (!object_list)
	{
		//std::cout << "!object_list" << std::endl;
		return;
	}
	//printf("object_list: 0x%x\n", object_list);

	//std::cout << "object_list_size" << std::endl;
	auto object_list_size = mem.Read<std::uint32_t>(buffer_list + 0x10);
	if (!object_list_size)
	{
		//std::cout << "!object_list_size" << std::endl;
		return;
	}
	//printf("object_list_size: 0x%x\n", object_list_size);

	try
	{
		for (auto i = 0; i < object_list_size; i++)
		{
			//std::cout << "current_object" << std::endl;
			auto current_object = mem.Read(object_list + (0x20 + (i * 8)));

			if (!current_object)
				continue;

			//std::cout << "base_object" << std::endl;
			auto base_object = mem.Read(current_object + 0x10);

			if (!base_object)
				continue;

			//std::cout << "object" << std::endl;
			auto object = mem.Read(base_object + 0x30);

			if (!object)
				continue;

			//std::cout << "local_element" << std::endl;
			auto local_element = mem.Read(object_list + 0x20);

			if (!local_element)
				continue;

			//std::cout << "local_base_object" << std::endl;
			auto local_base_object = mem.Read(local_element + 0x10);

			if (!local_base_object)
				continue;
			
			//std::cout << "local_player" << std::endl;
			auto local_player = mem.Read(local_base_object + 0x30);

			if (!local_player)
				continue;

			//std::cout << "local_player_object_class" << std::endl;
			auto local_player_object_class = mem.Read(local_player + 0x30);

			if (!local_player_object_class)
				continue;

			//std::cout << "local_player_transform" << std::endl;
			auto local_player_transform = mem.Read(local_player_object_class + 0x8);

			if (!local_player_transform)
				continue;

			//std::cout << "local_player_visual_state" << std::endl;
			auto local_player_visual_state = mem.Read(local_player_transform + 0x38);

			if (!local_player_visual_state)
				continue;

			//std::cout << "LocalPosition" << std::endl;
			LocalPosition = mem.Read<Vector3>(local_player_visual_state + 0x90);

			//std::cout << "class_name" << std::endl;
			std::string class_name = get_class_name(current_object);
			//std::cout << "Class Name: " << class_name << std::endl;
			if (class_name == "BasePlayer")
			{
				auto name_pointer = mem.Read(object + 0x60);

				if (!name_pointer)
					continue;

				ZeroMemory(&className, sizeof(className));
				if (!mem.Read(name_pointer, &className, sizeof(className)))
				{
					continue;
				}

				auto object_class = mem.Read(object + 0x30);

				if (!object_class)
					continue;

				auto entity = mem.Read(object_class + 0x18);

				if (!entity)
					continue;

				auto the_player = mem.Read(entity + 0x28);

				if (!the_player)
					continue;

				BasePlayer player;
				player.Player = the_player;
				player.ObjectClass = object_class;

				Vector3 pos = player.GetVisualPosition();
				Vector3 pos4 = player.GetVisualPosition();

				if (strcmp(className, "LocalPlayer") != 0)
				{
					player.IsLocalPlayer = false;
				}
				else
				{
					player.IsLocalPlayer = true;
					LocalPlayer = player;
					LocalPosition = pos;
					//TeamMateList = player.GetTeamList();
					continue;
				}

				auto player_health = player.GetHealth();
				if (!(player_health > 0))
				{
					//std::cout << "Player Dead!" << std::endl;
					continue;
				}

				//std::cout << "Player Health: " << player_health << std::endl;

				auto Distance = Math::Calc3D_Dist(LocalPosition, pos);
				if (Distance > settings::Visuals::max_distance)
					continue;

				pos = player.GetBonePosition(neck);
				if (pos == Vector3{ 0,0,0 })
					continue;

				pos4 = player.GetBonePosition(head);
				if (pos4 == Vector3{ 0,0,0 })
					continue;

				auto LeftFootPos = player.GetBonePosition(l_foot);
				Vector2 LeftFootScreenPos;
				Vector2 ScreenPos;

				auto BottomScreenPos = player.GetVisualPosition();
				Vector2 UnderBottomScreenPos;

				if (WorldToScreen(LeftFootPos, LeftFootScreenPos) && WorldToScreen(pos, ScreenPos) && !player.IsSleeping())
				{
					int height = LeftFootScreenPos.y - ScreenPos.y;
					int width = height / 2; //RightFootScreenPos.x - LeftFootScreenPos.x;

					ImColor color = ImColor(255, 0, 0);

					int yoffset = height;
					//if (settings::Visuals::name)
					//{
					//	auto name_string = ws2s(player.GetName());
					//	draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y + yoffset), color, name_string.c_str());
					//	yoffset += 15;
					//}

					//if (settings::Visuals::health)
					//{
					//	ZeroMemory(&draw_buffer, sizeof(draw_buffer));
					//	float healthf = nearbyint(player_health);
					//	int health = (int)(healthf);
					//	sprintf(draw_buffer, "[%d HP]", health);
					//	draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y + yoffset), color, draw_buffer);
					//	yoffset += 15;
					//}

					//if (settings::Visuals::distance)
					//{
					//	ZeroMemory(&draw_buffer, sizeof(draw_buffer));
					//	sprintf(draw_buffer, "[%dm]", (int)Distance);
					//	draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y + yoffset), color, draw_buffer);
					//	yoffset += 15;
					//}

					//if (settings::Visuals::held_item)
					//{
					//	//std::cout << "Getting Held Item!" << std::endl;
					//	auto HeldGun = player.GetHeldItem();
					//	//std::cout << "Checking Held Item!" << std::endl;
					//	if (HeldGun.Item && HeldGun.Item != NULL)
					//	{
					//		//std::cout << "Getting Item Name!" << std::endl;
					//		auto gun_name = HeldGun.GetItemName();
					//		//std::cout << "Item Name: " << ws2s(gun_name) << std::endl;
					//		draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y + yoffset), color, ws2s(gun_name).c_str());
					//		//std::cout << "Drew Held Item!" << std::endl;
					//		yoffset += 15;
					//	}
					//	//std::cout << "Done!" << std::endl;
					//}

					if (settings::Visuals::hotbar)
					{
						if (curFOV <= 30)
						{
							for (int i = 0; i < 6; i++)
							{
								auto inventory = player.GetPlayerInventory();
								auto belt = inventory.GetBelt();
								auto helditem = belt.GetItem(i);
								if (!helditem.Item || helditem.Item == NULL)
								{
									continue;
								}

								auto ItemName = helditem.GetItemName();

								draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y + yoffset), color, ws2s(ItemName).c_str());
								yoffset += 15;
							}
						}
					}
				}

				if (WorldToScreen(BottomScreenPos, UnderBottomScreenPos) && WorldToScreen(pos4, ScreenPos) && !player.IsSleeping())
				{

					Vector2 Head;
					Vector2 Spine;
					Vector2 Pelvis;
					Vector2 LKnee;
					Vector2 RKnee;
					Vector2 RFoot;
					Vector2 LFoot;
					Vector2 LUpperArm;
					Vector2 RUpperArm;
					Vector2 LForeArm;
					Vector2 RForeArm;
					Vector2 RHand;
					Vector2 LHand;

					WorldToScreen(player.GetBonePosition(head), Head); // glowa
					WorldToScreen(player.GetBonePosition(spine4), Spine); // kreglosup
					WorldToScreen(player.GetBonePosition(pelvis), Pelvis); // miednica
					WorldToScreen(player.GetBonePosition(l_knee), LKnee); // lewe kolano
					WorldToScreen(player.GetBonePosition(r_knee), RKnee); // prawe kolano
					WorldToScreen(player.GetBonePosition(l_foot), LFoot); // lewa stopa
					WorldToScreen(player.GetBonePosition(r_foot), RFoot); // prawa stopa
					WorldToScreen(player.GetBonePosition(l_upperarm), LUpperArm); // lewe ramie
					WorldToScreen(player.GetBonePosition(r_upperarm), RUpperArm); // prawe ramie
					WorldToScreen(player.GetBonePosition(l_forearm), LForeArm); // lewe przedramie
					WorldToScreen(player.GetBonePosition(r_forearm), RForeArm); // prawe przedramie
					WorldToScreen(player.GetBonePosition(l_hand), RHand); // lewa dlon
					WorldToScreen(player.GetBonePosition(r_hand), LHand); // prawa dlon

					int height = Math::abs(ScreenPos.y - UnderBottomScreenPos.y);
					int width = height / 4;

					ImColor color = ImColor(235, 52, 52);
					ImColor color2 = ImColor(0, 0, 0);
					ImColor color3 = ImColor(0, 255, 0);

					uintptr_t shader;

					//hgealth color
					ImColor color4 = ImColor(0, 155, 0);
					ImColor color5 = ImColor(255, 255, 255);

					Vector2 tempFeet1 = (RFoot + LFoot) / 2.f;
					float Entity_h1 = Head.y - tempFeet1.y;
					float w1 = Entity_h1 / 4;
					float Entity_x1 = tempFeet1.x - w1;
					float Entity_y1 = tempFeet1.y;
					float Entity_w1 = Entity_h1 / 2;

					Vector2 tempFeet = (RFoot + LFoot) / 2.f;
					float Entity_h = Head.y - tempFeet.y;
					float w = Entity_h / 4;
					float Entity_x = tempFeet.x - w;
					float Entity_y = tempFeet.y;
					float Entity_w = Entity_h / 1.8;

					if (settings::Visuals::boxes)
					{

						auto color = ImGui::GetColorU32({ settings::Visuals::BoxColor[0], settings::Visuals::BoxColor[1], settings::Visuals::BoxColor[2], 1.f });
						draw_list->AddRect({ Entity_x, Entity_y }, { Entity_x + Entity_w, Entity_y + Entity_h }, color);
					}

					if (settings::Visuals::health)
					{
						auto HealthDelta = player.GetHealth() / 100.f;
						if (HealthDelta > 1.f) HealthDelta = 1.f;

						auto ColorSpan = 130.f;
						auto Red = 245.f - (ColorSpan * HealthDelta);
						auto Green = 100.f + (ColorSpan * HealthDelta);

						auto healthColor = ImGui::GetColorU32({ Red / 255.f, Green / 255.f, 65.f / 255.f, 0.95f });

						draw_list->AddRectFilled({ (Entity_x - Entity_w / 15.f), Entity_y }, { Entity_x, Entity_y + Entity_h }, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }));
						draw_list->AddRectFilled({ (Entity_x - Entity_w / 15.f), Entity_y }, { Entity_x, Entity_y + Entity_h / 100 * player.GetHealth() }, healthColor);
						draw_list->AddRect({ (Entity_x - Entity_w / 15.f), Entity_y }, { Entity_x, Entity_y + Entity_h / 100 * player.GetHealth() }, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }));
					}

					if (settings::Visuals::boxescorner)
					{
						auto color = ImGui::GetColorU32({ settings::Visuals::BoxcornerColor[0], settings::Visuals::BoxcornerColor[1], settings::Visuals::BoxcornerColor[2], 1.f });
						draw_list->AddLine({ Entity_x, Entity_y }, { Entity_x + Entity_w / 3.5f, Entity_y }, color); //gorny lewy , 
						draw_list->AddLine({ Entity_x, Entity_y }, { Entity_x,Entity_y + Entity_h / 3.5f }, color);
						draw_list->AddLine({ Entity_x + Entity_w, Entity_y }, { (Entity_x + Entity_w) - Entity_w / 3.5f, Entity_y }, color); // plaski dolny lewy
						draw_list->AddLine({ Entity_x + Entity_w, Entity_y }, { Entity_x + Entity_w,Entity_y + Entity_h / 3.5f }, color);
						draw_list->AddLine({ Entity_x + Entity_w, Entity_y + Entity_h }, { (Entity_x + Entity_w) - Entity_w / 3.5f, Entity_y + Entity_h }, color);
						draw_list->AddLine({ Entity_x + Entity_w, Entity_y + Entity_h }, { Entity_x + Entity_w,(Entity_y + Entity_h) - Entity_h / 3.5f }, color);
						draw_list->AddLine({ Entity_x, Entity_y + Entity_h }, { Entity_x + Entity_w / 3.5f, Entity_y + Entity_h }, color);
						draw_list->AddLine({ Entity_x, Entity_y + Entity_h }, { Entity_x,(Entity_y + Entity_h) - Entity_h / 3.5f }, color);
					}

					if (settings::Visuals::filledbox)
					{
						draw_list->AddRectFilled({ Entity_x, Entity_y }, { Entity_x + Entity_w, Entity_y + Entity_h }, ImGui::GetColorU32({ settings::Visuals::Boxfilled[0] , settings::Visuals::Boxfilled[1], settings::Visuals::Boxfilled[2], 0.2f }));
					}

					if (settings::Visuals::skeleton)
					{

						auto color = ImGui::GetColorU32({ settings::Visuals::SkeletonColor[0], settings::Visuals::SkeletonColor[1], settings::Visuals::SkeletonColor[2], 1.f });

						draw_list->AddLine(ImVec2(Head.x, Head.y), ImVec2(Spine.x, Spine.y), color);
						draw_list->AddLine(ImVec2(Spine.x, Spine.y), ImVec2(LUpperArm.x, LUpperArm.y), color);
						draw_list->AddLine(ImVec2(Spine.x, Spine.y), ImVec2(RUpperArm.x, RUpperArm.y), color);
						draw_list->AddLine(ImVec2(LUpperArm.x, LUpperArm.y), ImVec2(LForeArm.x, LForeArm.y), color);
						draw_list->AddLine(ImVec2(RUpperArm.x, RUpperArm.y), ImVec2(RForeArm.x, RForeArm.y), color);
						draw_list->AddLine(ImVec2(LForeArm.x, LForeArm.y), ImVec2(RHand.x, RHand.y), color); // ADDED
						draw_list->AddLine(ImVec2(RForeArm.x, RForeArm.y), ImVec2(LHand.x, LHand.y), color); // ADDED
						draw_list->AddLine(ImVec2(Spine.x, Spine.y), ImVec2(Pelvis.x, Pelvis.y), color);
						draw_list->AddLine(ImVec2(Pelvis.x, Pelvis.y), ImVec2(LKnee.x, LKnee.y), color);
						draw_list->AddLine(ImVec2(Pelvis.x, Pelvis.y), ImVec2(RKnee.x, RKnee.y), color);
						draw_list->AddLine(ImVec2(RKnee.x, RKnee.y), ImVec2(RFoot.x, RFoot.y), color);
						draw_list->AddLine(ImVec2(LKnee.x, LKnee.y), ImVec2(LFoot.x, LFoot.y), color);


					}

					if (settings::Visuals::name)
					{
						auto Name = player.GetName();

						std::wstring NameW(Name);
						std::string NameString(NameW.begin(), NameW.end());
						draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y - 20), ImGui::GetColorU32({ settings::Visuals::namecolor[0] , settings::Visuals::namecolor[1], settings::Visuals::namecolor[2], 1.f }), NameString.c_str());
					}

					if (settings::Visuals::distance)
					{

						ZeroMemory(&draw_buffer, sizeof(draw_buffer));
						sprintf(draw_buffer, "[%dm]", (int)Distance);
						draw_list->AddText(ImVec2(ScreenPos.x + (width / 2), ScreenPos.y - 40), color, draw_buffer);
					}

					int CurPos = 0;

					if (settings::Visuals::held_item)
					{

						auto HeldGun = player.GetHeldItem();
						if (HeldGun.Item && HeldGun.Item != NULL)

						{
							auto gun_name = HeldGun.GetItemName();
							draw_list->AddText(ImVec2((Entity_x + 7), (Entity_y + Entity_h + 100)), ImGui::GetColorU32({ 255.f, 255.f, 255.f, 1.f }), ws2s(gun_name).c_str());
						}

					}

					//if (settings::Visuals::chams) {
					//	auto _multiMesh = mem.Read<uintptr_t>(player.get_player_model() + 0x2B0);
					//	if (_multiMesh) {
					//		auto render = get_Renderers(_multiMesh);
					//		//		printf("\nchamscheck1");
					//		for (int i = 0; i < render->get_size(); i++) {
					//			auto renderer = render->get_value(i);
					//			//		printf("\nchamscheck2");
					//			if (renderer) {
					//				auto material = get_material(renderer);
					//				if (material) {
					//					//		printf("\nchamscheck3");
					//					if (shader != unity::get_shader(material)) {
					//						if (!shader)
					//							shader = Find(_(L"Hidden/Internal-Colored"));
					//						//			printf("\nchamscheck4");
					//						unity::set_shader(material, shader);
					//						//			printf("\nchamscheck5");
					//						SetColor(material, _(L"_Color"), col(1, 0, 0, 1));
					//					}
					//				}
					//			}
					//		}
					//	} BAD BAD BAD BROKEN AND NO FIX
					//}

				}
			}
			else if (class_name.find("DroppedItem") != std::string::npos && settings::dropped::enabled)
			{
				if (!LocalPlayer.Player)
					continue;

				auto object_class = mem.Read(object + 0x30);

				if (!object_class)
					continue;

				auto current_item = mem.Read<DWORD64>(current_object + 0x150);
				if (!current_item)
				{
					continue;
				}

				ItemClass item;
				item.Item = current_item;
				item.ObjectClass = object_class;

				auto item_position = item.GetVisualPosition();

				auto distance = Math::Calc3D_Dist(LocalPosition, item_position);

				if (distance <= settings::dropped::max_distance)
				{

					auto item_name = ws2s(item.GetItemName());

					auto item_amount = item.GetAmount();

					Vector2 screen_position;
					if (WorldToScreen(item_position, screen_position))
					{
						char buffer[0x50]{};
						if (item.IsItemGun() && settings::dropped::show_weapons)
						{
							sprintf(buffer, "[%dm]\n[%d]", (int)distance, item_amount);
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y), ImColor(255, 0, 239), item_name.c_str());
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y + 15), ImColor(255, 0, 239), buffer);
							//delete[] buffer;
						}
						else if (item_name.find("ammo.") != std::string::npos && settings::dropped::show_ammo)
						{
							sprintf(buffer, "[%dm]\n[%d]", (int)distance, item_amount);
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y), ImColor(255, 0, 239), item_name.c_str());
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y + 15), ImColor(255, 0, 239), buffer);
							//delete[] buffer;
						}
						else if (item_name.find("scrap") != std::string::npos && settings::dropped::show_scrap)
						{
							sprintf(buffer, "[%dm]\n[%d]", (int)distance, item_amount);
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y), ImColor(255, 0, 239), item_name.c_str());
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y + 15), ImColor(255, 0, 239), buffer);
							//delete[] buffer;
						}
						else if (settings::dropped::show_all)
						{
							sprintf(buffer, "[%dm]\n[%d]", (int)distance, item_amount);
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y), ImColor(255, 0, 239), item_name.c_str());
							draw_list->AddText(ImVec2(screen_position.x, screen_position.y + 15), ImColor(255, 0, 239), buffer);
							//delete[] buffer;
						}
					}
				}

			}
			else if (class_name.find("OreResource") != std::string::npos && settings::ore::enabled)
			{
			auto unk1 = mem.Read(current_object + 0x10);
			if (!unk1)
				continue;

			auto unk2 = mem.Read(unk1 + 0x30);
			if (!unk2)
				continue;

			auto unk3 = mem.Read(unk2 + 0x30);
			if (!unk3)
				continue;

			if (!current_object)
				continue;

			Vector2 ScreenPos;
			Vector3 OrePos = get_object_pos(unk3);

			auto Distance = Math::Calc3D_Dist(LocalPosition, OrePos);
			if (Distance > settings::ore::max_distance)
				continue;

			auto ore_name_ptr = mem.Read(unk2 + 0x60);
			if (!ore_name_ptr)
				continue;

			auto ore_name = read_ascii(ore_name_ptr, 0x50);

			if (WorldToScreen(OrePos, ScreenPos))
			{
				int yoffset = 0;
				if (settings::ore::name)
				{

					if (ore_name.find("stone-ore") != std::string::npos && settings::ore::stone)
					{
						draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(104, 104, 104), "Stone Node");
						if (settings::ore::distance)
						{
							ZeroMemory(&draw_buffer, sizeof(draw_buffer));
							sprintf(draw_buffer, "[%dm]", (int)Distance);
							draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset + 13), ImColor(104, 104, 104), draw_buffer);

						}
					}
					else if (ore_name.find("metal-ore") != std::string::npos && settings::ore::metal)
					{
						draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(206, 206, 199), "Metal Node");
						if (settings::ore::distance)
						{
							ZeroMemory(&draw_buffer, sizeof(draw_buffer));
							sprintf(draw_buffer, "[%dm]", (int)Distance);
							draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset + 13), ImColor(213, 213, 213), draw_buffer);

						}
					}
					else if (ore_name.find("sulfur-ore") != std::string::npos && settings::ore::sulfur)
					{
						draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(247, 255, 0), "Sulfur Node");
						if (settings::ore::distance)
						{
							ZeroMemory(&draw_buffer, sizeof(draw_buffer));
							sprintf(draw_buffer, "[%dm]", (int)Distance);
							draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset + 13), ImColor(213, 213, 213), draw_buffer);

						}
					}

					yoffset += 15;
				}
				//		if (settings::ore::distance)
				//	{
				//			ZeroMemory(&draw_buffer, sizeof(draw_buffer));
				//			sprintf(draw_buffer, "[%dm]", (int)Distance);
				//			draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(0, 255, 255), draw_buffer);
	//
			}
			//	}
			}
			else if (class_name.find("CollectibleEntity") != std::string::npos && settings::collectable::enabled)
			{
				//std::cout << "Collectible Found!" << std::endl;
				auto unk1 = mem.Read(current_object + 0x10);
				if (!unk1)
					continue;

				auto unk2 = mem.Read(unk1 + 0x30);
				if (!unk2)
					continue;

				auto unk3 = mem.Read(unk2 + 0x30);
				if (!unk3)
					continue;

				Vector2 ScreenPos;
				Vector3 Pos = get_object_pos(unk3);

				auto Distance = Math::Calc3D_Dist(LocalPosition, Pos);
				if (Distance > settings::collectable::max_distance)
					continue;

				auto name_ptr = mem.Read(unk2 + 0x60);
				if (!name_ptr)
					continue;

				auto name = read_ascii(name_ptr, 0x50);
				//std::cout << "Collectible: " << name << std::endl;
				if (WorldToScreen(Pos, ScreenPos))
				{
					int yoffset = 0;
					if (settings::collectable::name)
					{
						if (settings::collectable::hemp)
						{
							if (name.find("hemp") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(0, 255, 26), "Hemp");
							}
						}

						if (settings::collectable::collectables)
						{
							if (name.find("metal-collect") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(206, 206, 199), "Metal Collectable");
							}
							else if (name.find("sulfur-collect") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(247, 255, 0), "Sulfur Collectable");
							}
							else if (name.find("stone-collect") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(104, 104, 104), "Stone Collectable");
							}
							else if (name.find("wood") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(70, 46, 46), "Wood Collectable");
							}
						}

						if (settings::collectable::food)
						{
							if (name.find("mushroom") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(250, 42, 42), "Mushroom");
							}
							else if (name.find("corn") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(250, 42, 42), "Corn");
							}
							else if (name.find("pumpkin") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(250, 42, 42), "Pumpkin");
							}
							else if (name.find("potato") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(250, 42, 42), "Potato");
							}
							else if (name.find("berry-") != std::string::npos)
							{
								draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(250, 42, 42), "Berrys");
							}
						}

						yoffset += 15;
					}
					if (settings::collectable::distance)
					{
						ZeroMemory(&draw_buffer, sizeof(draw_buffer));
						sprintf(draw_buffer, "[%dm]", (int)Distance);
						draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y + yoffset), ImColor(255, 255, 255), draw_buffer);

					}
				}
			}

			else if (class_name.find("StashContainer") != std::string::npos && settings::otheresp::stashes::enabled)
			{
				auto unk1 = mem.Read(current_object + 0x10);
				if (!unk1)
					continue;

				auto unk2 = mem.Read(unk1 + 0x30);
				if (!unk2)
					continue;

				auto unk3 = mem.Read(unk2 + 0x30);
				if (!unk3)
					continue;

				Vector2 ScreenPos;
				Vector3 Pos = get_object_pos(unk3);

				auto Distance = Math::Calc3D_Dist(LocalPosition, Pos);
				if (Distance > settings::otheresp::stashes::max_distance)
					continue;

				auto name_ptr = mem.Read(unk2 + 0x60);
				if (!name_ptr)
					continue;

				auto name = read_ascii(name_ptr, 0x50);

				if (WorldToScreen(Pos, ScreenPos))
				{
					ZeroMemory(&draw_buffer, sizeof(draw_buffer));
					auto distance = Math::Calc3D_Dist(LocalPosition, Pos);
					sprintf(draw_buffer, "Stash\n[%dm]", (int)distance);

					draw_list->AddText(ImVec2(ScreenPos.x, ScreenPos.y), ImColor(255, 255, 255), draw_buffer);
				}
			}
		}

		//std::cout << ws2s(closestPlayer.GetName()) << std::endl;
	}
	catch (...)
	{

	}
}

namespace hooks
{
	typedef Vector3(__fastcall* GetModifiedAimConeDirection_func)(float, Vector3, bool);
	GetModifiedAimConeDirection_func original_GetModifiedAimConeDirection;

	typedef void* (__fastcall* create_projectile_func)(void*, void*, Vector3, Vector3, Vector3);
	create_projectile_func original_create_projectile;

	Vector3 __fastcall hk_GetModifiedAimConeDirection(float aimCone, Vector3 inputVector, bool anywhereInside)
	{
		std::cout << "Called GetModifiedAimConeDirection" << std::endl;
		if (LocalPlayer.Player != NULL && closestPlayer.Player != NULL)
		{
			std::cout << "Silent Aim" << std::endl;
			auto localpos = LocalPlayer.GetBonePosition(spine1);
			auto targetpos = closestPlayer.GetBonePosition(spine1);

			Vector3 direction = Vector3(targetpos.x - localpos.x, targetpos.y - localpos.y, targetpos.z - localpos.z);

			return original_GetModifiedAimConeDirection(aimCone, direction, anywhereInside);
		}
		else
		{
			return original_GetModifiedAimConeDirection(aimCone, inputVector, anywhereInside);
		}
	}

	void* __fastcall hk_create_projectile(void* ecx, void* prefab_path, Vector3 pos, Vector3 forward, Vector3 velocity)
	{
		std::cout << "Create Projectile Called!" << std::endl;
		void* projectile;
		projectile = original_create_projectile(ecx, prefab_path, pos, forward, velocity);
		if (false) //Disabled for testing purposes.
		{
			mem.Write<float>((uintptr_t)projectile + 0x2C, 0.5f);
		}
		return projectile;
	}
}