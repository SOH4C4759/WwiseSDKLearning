// AkType Wwise程序的数据类型，甚至包含优先级数值、BandIO吞吐量的设定，见AkType.h

#pragma region 初始化

//  必须首先初始化Memory Manager
#include <AK\SoundEngine\Common\AkMemoryMgr.h> // 内存管理器接口
#include <AK\SoundEngine\Common\AkModule.h> // 默认内存管理器
#include <cassert>

bool InitSoundEngine()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	#pragma region 自定义内存管理器初始化
	memSettings.bUseDeviceMemAlways = true;
	// 在适配的平台，使所有分配使用设备内存

	memSettings.pfDebugMalloc;
	// 设备页面分配Hook

	memSettings.pfAllocVM;
	// 虚拟页面分配Hook

	memSettings.pfDebugFree;
	// (可选） 内存分配调试Hook，用于跟踪对pfFree的调用

	memSettings.pfDebugMalign;
	// (可选）内存分配调试钩子。用于跟踪对pfMalign的调用

	memSettings.pfDebugMalloc;
	// (可选）内存分配调试钩子。用来跟踪对pfRealloc的调用

	memSettings.pfDebugReallocAligned;
	// (可选）内存分配调试钩子。用来跟踪对pfReallocAligned

	memSettings.pfFree;
	// (可选）内存分配钩子

	memSettings.pfFreeDevice;
	// 硬件页面分配钩子

	memSettings.pfFreeVM;
	// 虚拟界面分配钩子

	memSettings.pfInitForThread;
	// (可选）针对线程的分配器初始化钩子

	memSettings.pfMalign;
	// (可选）内存分配钩子

	memSettings.pfMalloc;
	// (可选）内存分配钩子

	memSettings.pfReallocAligned;
	// (可选） 内存分配钩子

	memSettings.pfSizeOfMemory;
	// (可选） 内存分配统计钩子

	memSettings.pfTermForThread;
	// (可选） 针对线程的分配器终止钩子

	memSettings.pfTotalReservedMemorySize;
	// (可选） 内存分配统计钩子

	memSettings.uDevicePageSize = 0;
	// 设备内存页面尺寸，默认为0即自动检测

	memSettings.uMemAllocationSizeLimit = 0;
	// 默认0为禁用，1调试启用，2 Stomp allocator enabled,3 Stomp allocator and debug enabled.
	// 用户的实现可以通过使用多个非零值来提供不同的功能。

	memSettings.uVMPageSize = 0;
	// 虚拟内存页面大小，默认为0即自动检测

	#pragma endregion 

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"无法创建内存管理器");
		return false;
	}
}


#include <AK/SoundEngine/Common/IAkStreamMgr.h> // 流管理器
#include <AK/Tools\Common/AkPlatformFuncs.h> // 线程定义
#include <SoundEngine\POSIX\AkFilePackageLowLevelIOBlocking.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

bool InitSoundEngine()
{	
	// 默认Low-Level I/O 实现，文件包扩展名为
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	// 创建并初始化默认流管理器实例，可以使用自己的流管理器覆盖默认流管理器
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	// 自定义 流管理器的设置

	#pragma region 自定义流管理器初始化

	#pragma region AkStreamMgrSettings
	// High-level IO Device initialization settings.
	// \sa
	// - AK::IAkStreamMgr
	// - AK::StreamMgr::CreateDevice()
	// - \ref streamingmanager_settings
	#pragma endregion

	#pragma region g_LowLevleIO
	// CAkFilePackageLowLevelIOBlocking g_lowLevelIO 
	// 此类用于实现AK::StreamMgr::IAkFileLocationResolver和AK::StreamMgr::IAkIOHookBlocking接口
	// 能够加载由File Packager 实用程序生成的文件包
	#pragma endregion

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"无法创建流管理器");
		return false;
	}
	AkDeviceSettings deviceSettings;

	#pragma region AkDeviceSettings

	deviceSettings.bUseStreamCache = false;
	// 如果为true，设备会尝试重新使用已经从磁盘流过的IO缓冲区，这在传输小的循环声音使特别有用。
	// 缺点是在分配内存时，CPU会收到小的冲击，并且在StreamManager池中的内存占用会略大。

	deviceSettings.ePoolAttributes;
	// IO内存的属性，在此指的是分配类型（AkMemType_Device，类推），它将被直接传递给AK::MemoryMgr::Malign()

	deviceSettings.fTargetAutoStmBufferLength = 1024;
	// 以BufferLength = 1024为例，指的是目标的自动流缓冲区长度（ms)，当一个流达到该缓冲时
	// 它将被停止调度为IO，除非调度器为闲置

	deviceSettings.pIOMemory;
	// 可被用户分配的0内存的指针，如果你想让内存通过AK::MemoryMgr::Malign()分配，则传递Null。
	// 如果特别指定，uIOMemorySize,uIOMemoryAlignment,ePoolAttributes将被忽略

	deviceSettings.threadProperties;
	// 调度器线程属性

	deviceSettings.uGranularity;
	// IO请求的颗粒度（典型的字节/请求）

	deviceSettings.uIOMemoryAlignment = true;
	// IO内存对齐，他将直接传递给AK::MemoryMgr::Malign()

	deviceSettings.uIOMemorySize;
	// 用于IO的内存大小，在被四舍五入为uGranularity的倍数后，直接传递给AK::Memory::Malign()

	deviceSettings.uMaxCachePinnedBytes;
	// 使用AK::SoundEngine::PinEventInStreamCache()或inFileInCache()时可以使用的最大“pinned”数量

	deviceSettings.uMaxConcurrentIO;
	// 可以同时向Low-Level IO 发送的最大传输数量（仅适用于AK_SCHEDULER_DEFERRED_LINED_UP设备）

	deviceSettings.uSchedulerTypeFlags;
	// 调度器类型标志

	#pragma endregion

	#pragma endregion

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// 自定义流播放设备设置
	// CAkFilePackageLowLevelIOBlocking::Init() 在流管理器中创建流播放设备，并将其自身注册为File Location Resolver

	// Note:2022.11.29 需要复制一份cpp和头文件至C++项目里，否则会出现错误；

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"无法创建流播放设备以及Low_Level IO 系统");
		return false;
	}
}


#include <AK\SoundEngine\Common\AkSoundEngine.h>

bool InitSoundEngine()
{
	// 使用默认初始化参数创建声音引擎
	AkInitSettings initSetting;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSetting);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	// 初始化后，可以通过更改参数满足游戏的需求
	// 使用默认参数可确保代码向下兼容，SDK的未来版本可能会在结构中添加更多的初始化参数

	#pragma region 声音引擎自定义初始化设置
	initSetting.bDebugOutOfRangeCheckEnabled = true;
	// 调试设置，在处理代码中启用对超范围（和NAN）浮点数的检查。性能影响：小，大多数情况可以使用，如果在管线的不同点发现勿笑的值，将在日志中打印错误信息

	initSetting.bEnableGameSyncPreparation = true;
	// 设置RTPC Preparation 是否可用

	initSetting.BGMCallback;
	// 应用程序定义的背景音乐音源变化事件的回调函数

	initSetting.BGMCallbackCookie;
	// 用于背景音乐音源变化事件的回调函数的应用程序定义的用户数据

	initSetting.bUseLEngineThread = false;
	// 使用一个单独的线程来处理音频，如果设置为false,音频将在RenderAudio（）中进行处理，优化CPU的使用

	initSetting.bUseSoundBankMgrThread = false;
	// 使用一个单独的线程加载Soundbank，允许异步操作

	initSetting.eFloorPlane;
	// 用于定义地板平面相对于XYZ轴的方向，以及哪些轴代表侧向、前向、上向，作为Wwise中旋转的基础。
	// AkFloorPlane被用于确定Wwise中的游戏对象3D浏览器的方向，以及在Wwise空间音频中的几何体实例的转换。

	initSetting.fDebugOutOfRangeLimit = 16;
	// 调试设置，只在bDebugOutRangeCheckEnabled为true时使用。这定义样本归一化的峰值限制在+1/-1之间，允许短暂地超出范围。

	initSetting.fGameUnitsToMeters = 1;
	// 每一米对应地游戏单位数量
	// Note：在某些游戏的尺度变化上，这个参数很重要

	initSetting.fnProfilerPopTimer;
	// 外部（可选）功能，用于跟踪声音引擎的性能，当定时器停止时被调用。（仅在Debug和二进制文件中调用）

	initSetting.fnProfilerPostMarker;
	// 外部（可选）功能，用于跟踪声音引擎中的重要事件，作为一个标记或通知（仅在Debug和二进制文件中调用）

	initSetting.fnProfilerPushTimer;
	// 外部（可选）功能，用于跟踪声音引擎的性能，当定时器停止时被调用。（仅在Debug和二进制文件中调用）

	initSetting.pfnAssertHook;
	// 外部的断言处理功能（可选）

	initSetting.settingsJobManager;
	// 设置配置声音引擎内部任务的行为

	initSetting.settingsMainOutput;
	// 主输出设备设置

	initSetting.szPluginDLLPath;
	// 当使用DLL插件时，指定其路径，如果DLL与游戏可执行文件在同一文件夹，则留空

	initSetting.uBankReadBufferSize;
	// 当新数据需要在序列化过程中从磁盘加载时，BankReader读取的字节数。
	// 增加这个数值将增大内存的使用，反之，将增加磁盘读取的需求

	initSetting.uCommandQueueSize;
	// 命令队列的大小，单位：字节

	initSetting.uContinuousPlaybackLookAhead = 1;
	// 默认为1个音频量子，即音频帧，它的大小=AkInitSettings::uNumSamplesPerFrane/AkPlatformInitSettings::uSampleRate
	// 在许多平台，其默认值可以重写为1024，48KHz，这将默认的21.3ms的音频帧，如果你有一个基于RAM的流媒体在20ms内完成传输，那就够了
	// 前瞻1个音频帧的情况下，由序列容器产生的声音更可能在需要播放时准备好，从而提高整个音频调度的精度
	// 一般来说前瞻的音频帧的时长要始终大于流媒体需要加载的事件

	initSetting.uCpuMonitorQueueMaxSize;
	// 每个线程的CPU监控队列的最大尺寸，单位：字节（发布版本不可用）

	initSetting.uMaxHardwareTimeoutMs = 2000;
	// 等待硬件设备触发音频中断的时间量，如果超过该时间还未中断，声音引擎将恢复至Slient Mode继续执行任务，直至硬件设备返回信息。
	// 默认值：2000 单位：ms

	initSetting.uMaxNumPaths = 128;
	// 定位的最大位置数量

	initSetting.uMonitorQueuePoolSize;
	// 监测对流额的大小，以字节为单位，不用于发布版本的构建

	initSetting.uNumSamplesPerFrame = 1024;
	// 每一音频帧的采样数量

	#pragma region AkPlatformInitSettings
	platformInitSettings.hWnd;
	// 与音频关联的Windows的句柄，所有游戏都要指定应用程序的HWND以便检测设备。
	// GetDefaultPlatformInitSettings返回的值时声音引擎初始化时的前台HWND，可能不是你游戏的正确值。
	// 每个游戏必须提供正确HWND来使用。

	platformInitSettings.uMaxSystemAudioObjects = 128;
	// 决定由System Links保持多少Microsoft Spatial Sound动态对象，在Window上会禁用其他正在运行的进程以保持这些对象。
	// 设置为0，禁用System Audio Object，默认值为128

	platformInitSettings.bEnableAvxSupport = true;
	// 启用引擎和插件中的AVX和AVX2 SIMD支持的运行时间检测。禁用可以提高CPU时钟速度，提高CPU性能。

	platformInitSettings.uSampleRate = 48000;
	// 例如低品质可以使用24000Hz，但是使用奇数或一个特别低的采样率可能会导致声音引擎故障

	platformInitSettings.uNumRefillsInVoice = 4;
	// 声部缓冲区的充填缓冲区数，2 = 双缓冲，默认设置为4

	platformInitSettings.threadMonitor;
	// 监控线程属性（默认优先级为AK_THREAD_PRIORITY_ABOVENORMAL)发布版本不会使用

	platformInitSettings.threadBankManager;
	// Bank管理器的线程属性（默认优先级为AK_THREAD_PRIORITY_NORMAL)

	platformInitSettings.threadOutputMgr;
	// 输出的线程属性

	platformInitSettings.threadLEngine;
	// 底层引擎的线程属性


	#pragma endregion

	#pragma endregion

	if (AK::SoundEngine::Init(&initSetting, &platformInitSettings) != AK_Success)
	{
		assert(!"无法初始化声音引擎");
		return false;
	}
}
// 建议根据游戏需要自定义以上的初始化值，有些默认值可能会导致内存用量过高



#include <AK\MusicEngine\Common\AkMusicEngine.h>
bool InitSoundEngine()
{
	// 使用默认初始化参数对音乐引擎初始化
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	#pragma region 自定义音乐引擎初始化
	musicInit.fStreamingLookAheadRatio;
	// 所有流媒体前瞻触发数值的乘法系数；
	#pragma endregion

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"无法初始化音乐引擎");
	}
}


#include <AK\SpatialAudio\Common\AkSpatialAudio.h>

bool InitSoundEngine()
{
	// 使用默认的初始化参数初始化空间音频
	AkSpatialAudioInitSettings spatialSettings;

	#pragma region 自定义空间音频初始化
	spatialSettings.bCalcEmitterVirtualPosition = 8;
	// 射线追踪引擎中使用的主要射线的数量，较大数量的射线会增加找到反射和衍射路径的机会，但会导致较高的CPU使用率。

	spatialSettings.uMaxSoundPropagationDepth = 2;
	// 声音可以传播的最大门户数量，必须小于或等于AK_MAX_SOUND_PROPAGATION_DEPTH

	spatialSettings.uMaxReflectionOrder = 2;
	// 最大反射阶数[1，4]，反射声路径中的反射数量，更高的反射次数可以渲染更多细节，但代价是更高的CPU使用率。

	spatialSettings.uMaxDiffractionOrder = 2;
	// 最大衍射阶数[1,8]，衍射路径中的衍射数量，更高的衍射阶数可以容纳更复杂的几何形状，代价是更高的CPU使用率。
	// 衍射必须在几何体上启用，以便寻找衍射路径（参考AkGeometryParams），设置为0可以禁用所有几何体上的衍射。
	// 该参数限制了从Listener投出的衍射射线扫描环境的递归深度。为优化CPU的使用，可以把它设置为预计被阻碍的几何体的最大边缘数。
	// 例如：使用箱型几何体，并且预计在Emitter和Listener之间只有一个箱，将uMaxDiffractionOrder = 2即可.
	// 衍射路径搜索从听众开始，当超过最大衍射阶数时，路径末端和Emitter之间的剩余几何体将被忽略。
	// 在这种情况下，如果搜索在到达Emitter之前被终止，衍射系数将被低估，他是根据部分路径计算，忽略了任何剩余的几何体。

	spatialSettings.uLoadBalancingSpread = 1;
	// 在uLoadBalancingSpread帧上分散计算路径
	// 当为1时，不进行负载均衡，大于1的值表示路径的计算将分散在这个数量的帧上。（这意味着路径计算会产生延迟）

	spatialSettings.uDiffractionOnReflectionsOrder = 0;
	// 反射路径两端衍射点的最大路径数量。这将允许Listener或Emitter进出反射声的阴影区时，反射声可以自然的淡化。
	// 当大于零时，衍射射线会从Listener处发射，从Listener的一个或多个角度搜索反射。
	// 衍射必须在几何体上启用，才能找到衍射的反射（AkGeometryParam），为0则禁止对反射的衍射。

	spatialSettings.fMovementThreshold = 1;
	// Emitter或Listener重新触发反射/衍射的重新计算的移动量阈值。较大可以减少CPU负荷，代价是降低精度。
	// 个人推测该单位为Position_Attenuation的单位

	spatialSettings.fMaxPathLength = 100;
	// 由一系列线段或射线组成的路径的总长度不能超过定义的最大路径长度。较高值可以计算更长的路径，但增加CPU成本。
	// 每个单独的声音会收到其最大衰减距离的影响，在Spatial Audio里计算的反射和衍射历经，不会超过声音的最大衰减距离。
	// 如果声音最远的点在可听阈值之上，衰减被认为无限大。

	spatialSettings.fCPULimitPercentage = 0;
	// 定义分配给射线检测引擎的最大计算时间，为当前音频帧时间的百分比；
	// 射线追踪引擎将动态调整主射线（非衍射、反射）的数量，以达到指定的计算时间值。
	// 动态调整的射线数量范围不会超过AkSpatialAudioInitSetting::uNumberOfPrimaryRays。
	// 当值为0时，表示禁用动态调整，主射线数量固定。

	spatialSettings.bEnableGeometricDiffractionAndTransmission = true;
	// 对所有在Wwise属性编辑的Position选项卡中启用Diffraction和Transmission的声源启动几何体衍射和透射的计算。
	// 将其设置为false，几何体只用于反射计算，该选项优先级高于Wwise工程中对于衍射和透射的设定。
	// Wwise工程为true，此值为false时，声音会通过Portal衍射，但会忽略几何体。
	// 在游戏中，执行另外的障碍物计算时，会禁用该设置，但几何体仍会被传递给空间音频进行反射计算。

	spatialSettings.bCalcEmitterVirtualPosition = true;
	// 通过Portal或几何体周围的Emitter，将由空间音频计算其实际或虚拟位置，并传递给声音引擎。

	#pragma region FuncAkSptialAudioInitSetting() 默认参数
		/*uMaxSoundPropagationDepth(AK_MAX_SOUND_PROPAGATION_DEPTH)
		fMovementThreshold(AK_DEFAULT_MOVEMENT_THRESHOLD)
		uNumberOfPrimaryRays(100)
		uMaxReflectionOrder(1)
		uMaxDiffractionOrder(8)
		uDiffractionOnReflectionsOrder(2)
		fMaxPathLength(10000.0f)
		fCPULimitPercentage(0.0f)
		uLoadBalancingSpread(1)
		bEnableGeometricDiffractionAndTransmission(true)
		bCalcEmitterVirtualPosition(true)*/
	#pragma endregion
	
	#pragma endreigon

	if (AK::SpatialAudio::Init(spatialSettings) != AK_Success) // 原手册为"AK::SpatialAudio::Init(&setting)
	{
		assert(!"无法初始化空间音频");
		return false;
	}
}

#ifndef AK_OPTIMIZED
#include <AK\Comm\AkCommunication.h>
#endif // AK_OPTIMIZED

bool InitSoundEngine()
{
#ifndef AK_OPTIMIZED
	// 对通讯进行初始化（发布版本无此步骤）
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"无法初始化通讯代码");
		return false;
	}
#endif
}
#pragma endregion

#pragma region 控制台相关通讯库
// Windows 套接字初始化：
// WSAData wsaData = {0};
// ::WSAStartup(MAKEWORD(2, 2), &wsaData); FIXME 此处有错误

// 终止
// ::WSACleanup(); FIXME 此处有错误
#pragma endregion

#pragma region 通讯端口
// AkCommSettings::ports 代表Wwise创作应用程序与声音引擎之间进行通讯所使用的网络端口，当Wwise通讯功能启用时，这些端口将在游戏中打开

// 固定端口 Discovery Broadcast
// AkCommSettings::ports::uDiscoveryBroadcast 不是动态（不能设置为0），且必须是Project Settings的Network选项卡中指定的值
	
// 动态端口 AkCommSettings::Ports::uCommand 操作系统不自动选择一个端口
#pragma endregion

// Wwise音频处理流程：初始化->重复调用执行音频处理（每音频帧一次）
#pragma region 声音引擎处理
void ProcessAudio()
{
	// 处理音频包请求、事件、位置、RTPC等
	AK::SoundEngine::RenderAudio();
}
#pragma endregion


#pragma region 终止声音引擎的模块
void TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	
	AK::Comm::Term();
	// 终止通讯服务
	
	// AK::SpatialAudio::Term();
	// Wwise SDK 2022.1.0 该函数已弃用

	AK::MusicEngine::Term();
	// 终止音乐引擎

	AK::SoundEngine::Term();
	// 终止声音引擎本身

	// 终止流媒体播放设备和流播放管理器
	// CAkFilePackageLowLevelIOBlocking::Term() 销毁 Stream Manager 中
    // 与其相关的流播放设备，并注销自己的 File Location Resolver 身份。

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	// 终止内存管理器
	AK::MemoryMgr::Term();

#endif // !AK_OPTIMIZED
}
#pragma endregion

#pragma region 处理系统专用事件

// 1.暂停所有音频渲染 bool AK::SoundEngine::Suspend(false);
// 2.部分暂停（继续处理事件，但不渲染音频） bool AK::SoundEngine::Suspend(true);
// 3，继续播放音频：不执行操作

bool AK::SoundEngine::Suspend(false);

// 如果需要声音处理移至后台，需要调用该函数，会到前台使用WakeupFromSuspend.
// SoundEngine只在RenderAudio（）被调用时处理API消息（例如：PostEvent和SetSwitch）
// 建议将in_bRenderAnyway参数与你游戏的其他部分的行为相匹配。
// 如果你的游戏仍在后台运行，且保持SoundEngine与游戏之间的练习，那就允许渲染。
// 如果想在后台将CPU降到最低，那么旧不要允许渲染，也不要在游戏中调用RenderAudio（）
// 暂停所有音频渲染时，游戏不得调用任何SoundEngine Api，发布事件等操作仍有效且积累至系统唤醒时同时播放！（请注意该情况！！！）
// 为了继续执行音频处理，在从系统收到正确通知时，请先调用 AK::SoundEngine::WakeupFromSuspend()，然后再调用 RenderAudio()。可以在调用 RenderAudio() 之前，让其他事件排队等候。

// HWND是与音频相关的窗口的句柄。每个游戏都必须指定应用程序的HWND，以便进行设备检测。
// GetDefaultPlatformInitSettings返回的值是声音引擎初始化时的前台HWND，可能不是你游戏的正确值。每个游戏必须提供正确的HWND来使用

// 应用平台不在前台时最好将其静音。
// 可分别在 CoreApplication.EnteredBackground 和 CoreApplication.LeavingBackground 事件处理程序中调用 Suspend() 和 WakeupFromSuspend()。

#pragma endregion




