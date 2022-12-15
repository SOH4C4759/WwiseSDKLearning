#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>

#pragma region AkGameObjectID
void GameObejctExample()
{
	// GameObject必须先进行注册后才能用于PostEvent、SetPosition等。
	// 可以使用预定义ID注册游戏对象，自由选用任一 64 位无符号整数 ID (0 ~ 0x?FFFFFFFFFFFFFFDF)
	// 比如转换为 AkGameObjectID 的递增计数器或对象指针
	const AkGameObjectID GAME_OBJECT_ID_CAR = 100;
	const AkGameObjectID GAME_OBJECT_ID_HUMAN = 200;
	const AkGameObjectID GAME_OBJECT_ID_MARKERS = 300;
	// 某些全局游戏对象可能会在初始化时注册，例如：DefaultListener
	// 大多数GameObject会在游戏动态注册/销毁，例如：技能音效、可摧毁的物体的声音
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_CAR, "Car");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_HUMAN, "Human");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_MARKERS, "MarkerTest");
	// GameObject销毁时，实体从游戏中移除，应当注销并释放相关联的资源：
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_CAR);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_HUMAN);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_MARKERS);
	// TODO 可以通过RegisterGameObj(in_gameObjectID,const char * in_pzzObjName)的重载写一个UE中检测正在播放音频的工具

	// GameObject 是链接UI、Sound和Trigger等元素的Instance。
	// GameObject被定义为AkGameObjectID(uint64)可以自由选择 ID，范围为 0 ~ 0x?FFFFFFFFFFFFFFDF（作为有符号整数时为 -33）。 
	// 游戏对象 ID 范围 0x?FFFFFFFFFFFFFFE0?（-32）到 0x?FFFFFFFFFFFFFFFF (-1) 是保留给内部使用的。
	// transport 游戏对象，由设计工具注册，现在使用的游戏对象 ID 是 0x?FFFFFFFFFFFFFFFE?（-2）。

	// 游戏对象可能有相关的 3D 位置、针对各个切换开关组的切换开关、针对各个 RTPC 的 RTPC 值，以及在特定项目（音效声、角色混音器、总线等）上为特定游戏对象设置的多个值（例如音量）。
	// 声音引擎一直存储着这些信息，直到与这些值相关的游戏对象被注销。
	
	// UnregisterGameObj将对GameObject设置的参数清理，移除GameObject的关联
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_MARKERS);
	AK::SoundEngine::UnregisterAllGameObj();
	// TODO 可以通过该函数实现UE中的AudioGM功能
}
#pragma endregion


