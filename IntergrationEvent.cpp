#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "Wwise_IDs.h"

void IntergrationEventExample()
{
	const AkGameObjectID WM_CM_SFX_WALK = 1000;
	AK::SoundEngine::RegisterGameObj(WM_CM_SFX_WALK, "WM_CM_SFX_WALK");

	// 使用EventName
	AK::SoundEngine::PostEvent(L"WM_CM_SFX_WALK", WM_CM_SFX_WALK);

	// 使用EventID
	AK::SoundEngine::PostEvent(AK::EVENTS::WM_CM_SFX_WALK, WM_CM_SFX_WALK);

	// 调用AK::SoundEngine::RenderAudio之前，不会处理任何AkEvent
	// 以游戏帧的速率调用RenderAudio是合理的做法

	// GameFrame(Previous)------------------------------------------------------------->>>GameFrame(Next)
	// RenderAudio(Previous)---->PostEvent----->Update Position、Switch、State、RTPC----->RenderAudio(Next)
	// Streaming------------------------流媒体播放声音不会受RenderAudio函数影响-------------------------->>

	// 用于AKEvetn回调的标记可以通过外部波形编辑器创建（例如SoundForge/Adobe Audition），实际目前通过Wwise自身的标记添加已经极为便利
	// TODO 除非可以利用外部的宿主软件对一批次音频进行标记的批量添加的需求或者工具

	// 可以通过强制调用RenderAudio许可音频（EventManager）线程使其尽快执行最近发送的AkEvent。
	// TODO 这可能用于一些极需要时间准确性或同步需求的音频或者游戏玩法
	// 该线程与平台音频的处理速率同步。只有当音频输出模块已处理完缓冲区
	// 并因此空出其环形缓冲区的一段以供写入时才能开始“重新填满音频数据”的过程

	// Windows 中初始化声音引擎时，您可以使用 AkPlatformInitSettings::uNumRefillsInVoice 在平台专用参数中指定环形缓冲区的大小。
	// 这将给出声部缓冲区中的“填充缓冲区”（refill buffer）数量，其中 2 代表双缓冲，默认值为 4。
	// 选择此数字是在减少延迟（小缓冲）和提高系统对内存匮乏的抵抗性（大缓冲）之间做出平衡的结果。
	// TODO 可以针对此做一些相关性能优化的调试功能代码

	// “填充缓冲区”通常又被称为“音频帧”（audio frame），是由采样率与频率的比值所确定的。
	// 因此，在 Windows 的高质量模式中，这通常为：（1, 024 个采样） / (48, 000 Hz) = 21.3 毫秒。
	// 如果我们使用 AkInitSettings::uNumSamplesPerFrame 将采样设为 512，则音频帧为：（512 个采样） / (48, 000 Hz) = 10.6 毫秒。
	// TODO 实时显示当前音频帧与帧单位时间以及SoundEngine延迟时间的工具
		// SoundEngine Ping的计算
		// PostEvent--->RenderAudio t_ping = 1000ms/GameFrameRate(游戏帧率）
		// 调用RenderAudio时，距离下一音频帧的时间 t_ping2 = AkPlatFormInitSettings::uNumSamplePerFrame / SampleRate 例如：1024/48000 = 21.3ms 
		// IO Suffer ,双缓冲为：t_ping3 = AkPlatForInitSettings::uNumRefillsinVoice * tping2
		// 最小延迟 maxPing = t_ping3
		// 最大延迟 minPing = t_ping + t_ping2 + t_ping3
	// TODO 可以制作一个实时显示音频帧的工具

	// 流媒体播放（不向内存加载任何内容）需要加上IO延迟，可以使用Zero Latency（零延迟）避免IO延迟。
	// 缓冲大小由Prefetch Length（预读长度）决定，默认为100ms

#pragma region EventProcessExample
	/*
	PostEvent( Play_Sound1, GameObj_X )
	PostEvent( Stop_Sound1, GameObj_X)
	RenderAudio()
	结果：不会播放任何内容。

	PostEvent( Stop_Sound1, GameObj_X )
	PostEvent( Play_Sound1, GameObj_X )
	RenderAudio()
	结果:播放Sound1

	SetSwitch( Grass, GameObj_X )
	PostEvent( Play_SwitchFootStep, GameObj_X )
	SetSwitch( Concrete, GameObj_X )
	PostEvent( Play_SwitchFootStep, GameObj_X )
	RenderAudio()
	Result: Grass and concrete sounds will both play

	SetSwitch( Grass, GameObj_X )
	SetSwitch( Concrete, GameObj_X )
	PostEvent( Play_SwitchFootStep, GameObj_X )
	PostEvent( Play_SwitchFootStep, GameObj_X )
	RenderAudio()
	Result: Concrete sounds will play twice
	 */
#pragma endregion

	 // 支持同步渲染视频和音频内容的游戏必须查询声音的播放位置才能正确渲染游戏画面
	 // 如需逐帧查询（对口型和对白同步），播放位置查询比标记（Maker）更适合，且无需编辑源文件。

	 // 传递标志AK_EnableGetSourcePlayPosition至PostEvent
	static AkPlayingID g_markersPlayingID = 0;
	const AkGameObjectID GAME_OBJECT_ID_MAKERS = 100;
	g_markersPlayingID = AK::SoundEngine::PostEvent(AK::EVENTS::WM_CM_SFX_DASH, GAME_OBJECT_ID_MAKERS,AK_EnableGetSourcePlayPosition,NULL,NULL);
	AkTimeMs uPosition = 0;
	// 在游戏循环中某个位置（周期性发生）：
	AK::SoundEngine::GetSourcePlayPosition(g_markersPlayingID, &uPosition);
	// 现在通过使用 uPosition 进行对嘴形
	// TODO 做对嘴型相关的Demo或者研究
	/*
	•当事件包含多个播放动作时，GetSourcePlayPosition() 返回在时间 0 时发生的第一个播放动作的目标所播放的第一个源的位置。 当此源停止时，其返回的播放位置将为未定义。
	•连续（随机或序列）容器返回当前正在播放的声音的位置。当有许多声音时，其返回的播放位置将为未定义。 在交叉淡变或精确到采样点的过渡中，只有当上一个源完成播放时下一个源才会返回它的位置。
	•当结合外插法使用时，循环播放源可能会在循环发生时短暂超出循环结束点。
	•当优先级低于特定阈值时，某些虚声部会被终止。发生这种情况时，您可能会从 AK::SoundEngine::GetSourcePlayPosition() 收到错误码。在调用 AK::SoundEngine::GetSourcePlayPosition() 时应总是检查返回值。
	•动态更改音高可能导致在过渡期间播放位置出现短暂轻微偏移。
	*/
	// TODO 请针对以上内容做测试视频
};
