// AkType Wwise������������ͣ������������ȼ���ֵ��BandIO���������趨����AkType.h

#pragma region ��ʼ��

//  �������ȳ�ʼ��Memory Manager
#include <AK\SoundEngine\Common\AkMemoryMgr.h> // �ڴ�������ӿ�
#include <AK\SoundEngine\Common\AkModule.h> // Ĭ���ڴ������
#include <cassert>

bool InitSoundEngine()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	#pragma region �Զ����ڴ��������ʼ��
	memSettings.bUseDeviceMemAlways = true;
	// �������ƽ̨��ʹ���з���ʹ���豸�ڴ�

	memSettings.pfDebugMalloc;
	// �豸ҳ�����Hook

	memSettings.pfAllocVM;
	// ����ҳ�����Hook

	memSettings.pfDebugFree;
	// (��ѡ�� �ڴ�������Hook�����ڸ��ٶ�pfFree�ĵ���

	memSettings.pfDebugMalign;
	// (��ѡ���ڴ������Թ��ӡ����ڸ��ٶ�pfMalign�ĵ���

	memSettings.pfDebugMalloc;
	// (��ѡ���ڴ������Թ��ӡ��������ٶ�pfRealloc�ĵ���

	memSettings.pfDebugReallocAligned;
	// (��ѡ���ڴ������Թ��ӡ��������ٶ�pfReallocAligned

	memSettings.pfFree;
	// (��ѡ���ڴ���乳��

	memSettings.pfFreeDevice;
	// Ӳ��ҳ����乳��

	memSettings.pfFreeVM;
	// ���������乳��

	memSettings.pfInitForThread;
	// (��ѡ������̵߳ķ�������ʼ������

	memSettings.pfMalign;
	// (��ѡ���ڴ���乳��

	memSettings.pfMalloc;
	// (��ѡ���ڴ���乳��

	memSettings.pfReallocAligned;
	// (��ѡ�� �ڴ���乳��

	memSettings.pfSizeOfMemory;
	// (��ѡ�� �ڴ����ͳ�ƹ���

	memSettings.pfTermForThread;
	// (��ѡ�� ����̵߳ķ�������ֹ����

	memSettings.pfTotalReservedMemorySize;
	// (��ѡ�� �ڴ����ͳ�ƹ���

	memSettings.uDevicePageSize = 0;
	// �豸�ڴ�ҳ��ߴ磬Ĭ��Ϊ0���Զ����

	memSettings.uMemAllocationSizeLimit = 0;
	// Ĭ��0Ϊ���ã�1�������ã�2 Stomp allocator enabled,3 Stomp allocator and debug enabled.
	// �û���ʵ�ֿ���ͨ��ʹ�ö������ֵ���ṩ��ͬ�Ĺ��ܡ�

	memSettings.uVMPageSize = 0;
	// �����ڴ�ҳ���С��Ĭ��Ϊ0���Զ����

	#pragma endregion 

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"�޷������ڴ������");
		return false;
	}
}


#include <AK/SoundEngine/Common/IAkStreamMgr.h> // ��������
#include <AK/Tools\Common/AkPlatformFuncs.h> // �̶߳���
#include <SoundEngine\POSIX\AkFilePackageLowLevelIOBlocking.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

bool InitSoundEngine()
{	
	// Ĭ��Low-Level I/O ʵ�֣��ļ�����չ��Ϊ
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	// ��������ʼ��Ĭ����������ʵ��������ʹ���Լ���������������Ĭ����������
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	// �Զ��� ��������������

	#pragma region �Զ�������������ʼ��

	#pragma region AkStreamMgrSettings
	// High-level IO Device initialization settings.
	// \sa
	// - AK::IAkStreamMgr
	// - AK::StreamMgr::CreateDevice()
	// - \ref streamingmanager_settings
	#pragma endregion

	#pragma region g_LowLevleIO
	// CAkFilePackageLowLevelIOBlocking g_lowLevelIO 
	// ��������ʵ��AK::StreamMgr::IAkFileLocationResolver��AK::StreamMgr::IAkIOHookBlocking�ӿ�
	// �ܹ�������File Packager ʵ�ó������ɵ��ļ���
	#pragma endregion

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"�޷�������������");
		return false;
	}
	AkDeviceSettings deviceSettings;

	#pragma region AkDeviceSettings

	deviceSettings.bUseStreamCache = false;
	// ���Ϊtrue���豸�᳢������ʹ���Ѿ��Ӵ���������IO�����������ڴ���С��ѭ������ʹ�ر����á�
	// ȱ�����ڷ����ڴ�ʱ��CPU���յ�С�ĳ����������StreamManager���е��ڴ�ռ�û��Դ�

	deviceSettings.ePoolAttributes;
	// IO�ڴ�����ԣ��ڴ�ָ���Ƿ������ͣ�AkMemType_Device�����ƣ���������ֱ�Ӵ��ݸ�AK::MemoryMgr::Malign()

	deviceSettings.fTargetAutoStmBufferLength = 1024;
	// ��BufferLength = 1024Ϊ����ָ����Ŀ����Զ������������ȣ�ms)����һ�����ﵽ�û���ʱ
	// ������ֹͣ����ΪIO�����ǵ�����Ϊ����

	deviceSettings.pIOMemory;
	// �ɱ��û������0�ڴ��ָ�룬����������ڴ�ͨ��AK::MemoryMgr::Malign()���䣬�򴫵�Null��
	// ����ر�ָ����uIOMemorySize,uIOMemoryAlignment,ePoolAttributes��������

	deviceSettings.threadProperties;
	// �������߳�����

	deviceSettings.uGranularity;
	// IO����Ŀ����ȣ����͵��ֽ�/����

	deviceSettings.uIOMemoryAlignment = true;
	// IO�ڴ���룬����ֱ�Ӵ��ݸ�AK::MemoryMgr::Malign()

	deviceSettings.uIOMemorySize;
	// ����IO���ڴ��С���ڱ���������ΪuGranularity�ı�����ֱ�Ӵ��ݸ�AK::Memory::Malign()

	deviceSettings.uMaxCachePinnedBytes;
	// ʹ��AK::SoundEngine::PinEventInStreamCache()��inFileInCache()ʱ����ʹ�õ����pinned������

	deviceSettings.uMaxConcurrentIO;
	// ����ͬʱ��Low-Level IO ���͵��������������������AK_SCHEDULER_DEFERRED_LINED_UP�豸��

	deviceSettings.uSchedulerTypeFlags;
	// ���������ͱ�־

	#pragma endregion

	#pragma endregion

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// �Զ����������豸����
	// CAkFilePackageLowLevelIOBlocking::Init() �����������д����������豸������������ע��ΪFile Location Resolver

	// Note:2022.11.29 ��Ҫ����һ��cpp��ͷ�ļ���C++��Ŀ��������ִ���

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"�޷������������豸�Լ�Low_Level IO ϵͳ");
		return false;
	}
}


#include <AK\SoundEngine\Common\AkSoundEngine.h>

bool InitSoundEngine()
{
	// ʹ��Ĭ�ϳ�ʼ������������������
	AkInitSettings initSetting;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSetting);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	// ��ʼ���󣬿���ͨ�����Ĳ���������Ϸ������
	// ʹ��Ĭ�ϲ�����ȷ���������¼��ݣ�SDK��δ���汾���ܻ��ڽṹ����Ӹ���ĳ�ʼ������

	#pragma region ���������Զ����ʼ������
	initSetting.bDebugOutOfRangeCheckEnabled = true;
	// �������ã��ڴ�����������öԳ���Χ����NAN���������ļ�顣����Ӱ�죺С��������������ʹ�ã�����ڹ��ߵĲ�ͬ�㷢����Ц��ֵ��������־�д�ӡ������Ϣ

	initSetting.bEnableGameSyncPreparation = true;
	// ����RTPC Preparation �Ƿ����

	initSetting.BGMCallback;
	// Ӧ�ó�����ı���������Դ�仯�¼��Ļص�����

	initSetting.BGMCallbackCookie;
	// ���ڱ���������Դ�仯�¼��Ļص�������Ӧ�ó�������û�����

	initSetting.bUseLEngineThread = false;
	// ʹ��һ���������߳���������Ƶ���������Ϊfalse,��Ƶ����RenderAudio�����н��д����Ż�CPU��ʹ��

	initSetting.bUseSoundBankMgrThread = false;
	// ʹ��һ���������̼߳���Soundbank�������첽����

	initSetting.eFloorPlane;
	// ���ڶ���ذ�ƽ�������XYZ��ķ����Լ���Щ��������ǰ��������ΪWwise����ת�Ļ�����
	// AkFloorPlane������ȷ��Wwise�е���Ϸ����3D������ķ����Լ���Wwise�ռ���Ƶ�еļ�����ʵ����ת����

	initSetting.fDebugOutOfRangeLimit = 16;
	// �������ã�ֻ��bDebugOutRangeCheckEnabledΪtrueʱʹ�á��ⶨ��������һ���ķ�ֵ������+1/-1֮�䣬������ݵس�����Χ��

	initSetting.fGameUnitsToMeters = 1;
	// ÿһ�׶�Ӧ����Ϸ��λ����
	// Note����ĳЩ��Ϸ�ĳ߶ȱ仯�ϣ������������Ҫ

	initSetting.fnProfilerPopTimer;
	// �ⲿ����ѡ�����ܣ����ڸ���������������ܣ�����ʱ��ֹͣʱ�����á�������Debug�Ͷ������ļ��е��ã�

	initSetting.fnProfilerPostMarker;
	// �ⲿ����ѡ�����ܣ����ڸ������������е���Ҫ�¼�����Ϊһ����ǻ�֪ͨ������Debug�Ͷ������ļ��е��ã�

	initSetting.fnProfilerPushTimer;
	// �ⲿ����ѡ�����ܣ����ڸ���������������ܣ�����ʱ��ֹͣʱ�����á�������Debug�Ͷ������ļ��е��ã�

	initSetting.pfnAssertHook;
	// �ⲿ�Ķ��Դ����ܣ���ѡ��

	initSetting.settingsJobManager;
	// �����������������ڲ��������Ϊ

	initSetting.settingsMainOutput;
	// ������豸����

	initSetting.szPluginDLLPath;
	// ��ʹ��DLL���ʱ��ָ����·�������DLL����Ϸ��ִ���ļ���ͬһ�ļ��У�������

	initSetting.uBankReadBufferSize;
	// ����������Ҫ�����л������дӴ��̼���ʱ��BankReader��ȡ���ֽ�����
	// ���������ֵ�������ڴ��ʹ�ã���֮�������Ӵ��̶�ȡ������

	initSetting.uCommandQueueSize;
	// ������еĴ�С����λ���ֽ�

	initSetting.uContinuousPlaybackLookAhead = 1;
	// Ĭ��Ϊ1����Ƶ���ӣ�����Ƶ֡�����Ĵ�С=AkInitSettings::uNumSamplesPerFrane/AkPlatformInitSettings::uSampleRate
	// �����ƽ̨����Ĭ��ֵ������дΪ1024��48KHz���⽫Ĭ�ϵ�21.3ms����Ƶ֡���������һ������RAM����ý����20ms����ɴ��䣬�Ǿ͹���
	// ǰհ1����Ƶ֡������£�������������������������������Ҫ����ʱ׼���ã��Ӷ����������Ƶ���ȵľ���
	// һ����˵ǰհ����Ƶ֡��ʱ��Ҫʼ�մ�����ý����Ҫ���ص��¼�

	initSetting.uCpuMonitorQueueMaxSize;
	// ÿ���̵߳�CPU��ض��е����ߴ磬��λ���ֽڣ������汾�����ã�

	initSetting.uMaxHardwareTimeoutMs = 2000;
	// �ȴ�Ӳ���豸������Ƶ�жϵ�ʱ���������������ʱ�仹δ�жϣ��������潫�ָ���Slient Mode����ִ������ֱ��Ӳ���豸������Ϣ��
	// Ĭ��ֵ��2000 ��λ��ms

	initSetting.uMaxNumPaths = 128;
	// ��λ�����λ������

	initSetting.uMonitorQueuePoolSize;
	// ��������Ĵ�С�����ֽ�Ϊ��λ�������ڷ����汾�Ĺ���

	initSetting.uNumSamplesPerFrame = 1024;
	// ÿһ��Ƶ֡�Ĳ�������

	#pragma region AkPlatformInitSettings
	platformInitSettings.hWnd;
	// ����Ƶ������Windows�ľ����������Ϸ��Ҫָ��Ӧ�ó����HWND�Ա����豸��
	// GetDefaultPlatformInitSettings���ص�ֵʱ���������ʼ��ʱ��ǰ̨HWND�����ܲ�������Ϸ����ȷֵ��
	// ÿ����Ϸ�����ṩ��ȷHWND��ʹ�á�

	platformInitSettings.uMaxSystemAudioObjects = 128;
	// ������System Links���ֶ���Microsoft Spatial Sound��̬������Window�ϻ���������������еĽ����Ա�����Щ����
	// ����Ϊ0������System Audio Object��Ĭ��ֵΪ128

	platformInitSettings.bEnableAvxSupport = true;
	// ��������Ͳ���е�AVX��AVX2 SIMD֧�ֵ�����ʱ���⡣���ÿ������CPUʱ���ٶȣ����CPU���ܡ�

	platformInitSettings.uSampleRate = 48000;
	// �����Ʒ�ʿ���ʹ��24000Hz������ʹ��������һ���ر�͵Ĳ����ʿ��ܻᵼ�������������

	platformInitSettings.uNumRefillsInVoice = 4;
	// �����������ĳ����������2 = ˫���壬Ĭ������Ϊ4

	platformInitSettings.threadMonitor;
	// ����߳����ԣ�Ĭ�����ȼ�ΪAK_THREAD_PRIORITY_ABOVENORMAL)�����汾����ʹ��

	platformInitSettings.threadBankManager;
	// Bank���������߳����ԣ�Ĭ�����ȼ�ΪAK_THREAD_PRIORITY_NORMAL)

	platformInitSettings.threadOutputMgr;
	// ������߳�����

	platformInitSettings.threadLEngine;
	// �ײ�������߳�����


	#pragma endregion

	#pragma endregion

	if (AK::SoundEngine::Init(&initSetting, &platformInitSettings) != AK_Success)
	{
		assert(!"�޷���ʼ����������");
		return false;
	}
}
// ���������Ϸ��Ҫ�Զ������ϵĳ�ʼ��ֵ����ЩĬ��ֵ���ܻᵼ���ڴ���������



#include <AK\MusicEngine\Common\AkMusicEngine.h>
bool InitSoundEngine()
{
	// ʹ��Ĭ�ϳ�ʼ�����������������ʼ��
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	#pragma region �Զ������������ʼ��
	musicInit.fStreamingLookAheadRatio;
	// ������ý��ǰհ������ֵ�ĳ˷�ϵ����
	#pragma endregion

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"�޷���ʼ����������");
	}
}


#include <AK\SpatialAudio\Common\AkSpatialAudio.h>

bool InitSoundEngine()
{
	// ʹ��Ĭ�ϵĳ�ʼ��������ʼ���ռ���Ƶ
	AkSpatialAudioInitSettings spatialSettings;

	#pragma region �Զ���ռ���Ƶ��ʼ��
	spatialSettings.bCalcEmitterVirtualPosition = 8;
	// ����׷��������ʹ�õ���Ҫ���ߵ��������ϴ����������߻������ҵ����������·���Ļ��ᣬ���ᵼ�½ϸߵ�CPUʹ���ʡ�

	spatialSettings.uMaxSoundPropagationDepth = 2;
	// �������Դ���������Ż�����������С�ڻ����AK_MAX_SOUND_PROPAGATION_DEPTH

	spatialSettings.uMaxReflectionOrder = 2;
	// ��������[1��4]��������·���еķ������������ߵķ������������Ⱦ����ϸ�ڣ��������Ǹ��ߵ�CPUʹ���ʡ�

	spatialSettings.uMaxDiffractionOrder = 2;
	// ����������[1,8]������·���е��������������ߵ���������������ɸ����ӵļ�����״�������Ǹ��ߵ�CPUʹ���ʡ�
	// ��������ڼ����������ã��Ա�Ѱ������·�����ο�AkGeometryParams��������Ϊ0���Խ������м������ϵ����䡣
	// �ò��������˴�ListenerͶ������������ɨ�軷���ĵݹ���ȡ�Ϊ�Ż�CPU��ʹ�ã����԰�������ΪԤ�Ʊ��谭�ļ����������Ե����
	// ���磺ʹ�����ͼ����壬����Ԥ����Emitter��Listener֮��ֻ��һ���䣬��uMaxDiffractionOrder = 2����.
	// ����·�����������ڿ�ʼ������������������ʱ��·��ĩ�˺�Emitter֮���ʣ�༸���彫�����ԡ�
	// ����������£���������ڵ���Emitter֮ǰ����ֹ������ϵ�������͹������Ǹ��ݲ���·�����㣬�������κ�ʣ��ļ����塣

	spatialSettings.uLoadBalancingSpread = 1;
	// ��uLoadBalancingSpread֡�Ϸ�ɢ����·��
	// ��Ϊ1ʱ�������и��ؾ��⣬����1��ֵ��ʾ·���ļ��㽫��ɢ�����������֡�ϡ�������ζ��·�����������ӳ٣�

	spatialSettings.uDiffractionOnReflectionsOrder = 0;
	// ����·���������������·���������⽫����Listener��Emitter��������������Ӱ��ʱ��������������Ȼ�ĵ�����
	// ��������ʱ���������߻��Listener�����䣬��Listener��һ�������Ƕ��������䡣
	// ��������ڼ����������ã������ҵ�����ķ��䣨AkGeometryParam����Ϊ0���ֹ�Է�������䡣

	spatialSettings.fMovementThreshold = 1;
	// Emitter��Listener���´�������/��������¼�����ƶ�����ֵ���ϴ���Լ���CPU���ɣ������ǽ��;��ȡ�
	// �����Ʋ�õ�λΪPosition_Attenuation�ĵ�λ

	spatialSettings.fMaxPathLength = 100;
	// ��һϵ���߶λ�������ɵ�·�����ܳ��Ȳ��ܳ�����������·�����ȡ��ϸ�ֵ���Լ��������·����������CPU�ɱ���
	// ÿ���������������յ������˥�������Ӱ�죬��Spatial Audio�����ķ�����������������ᳬ�����������˥�����롣
	// ���������Զ�ĵ��ڿ�����ֵ֮�ϣ�˥������Ϊ���޴�

	spatialSettings.fCPULimitPercentage = 0;
	// �����������߼�������������ʱ�䣬Ϊ��ǰ��Ƶ֡ʱ��İٷֱȣ�
	// ����׷�����潫��̬���������ߣ������䡢���䣩���������Դﵽָ���ļ���ʱ��ֵ��
	// ��̬����������������Χ���ᳬ��AkSpatialAudioInitSetting::uNumberOfPrimaryRays��
	// ��ֵΪ0ʱ����ʾ���ö�̬�����������������̶���

	spatialSettings.bEnableGeometricDiffractionAndTransmission = true;
	// ��������Wwise���Ա༭��Positionѡ�������Diffraction��Transmission����Դ���������������͸��ļ��㡣
	// ��������Ϊfalse��������ֻ���ڷ�����㣬��ѡ�����ȼ�����Wwise�����ж��������͸����趨��
	// Wwise����Ϊtrue����ֵΪfalseʱ��������ͨ��Portal���䣬������Լ����塣
	// ����Ϸ�У�ִ��������ϰ������ʱ������ø����ã����������Իᱻ���ݸ��ռ���Ƶ���з�����㡣

	spatialSettings.bCalcEmitterVirtualPosition = true;
	// ͨ��Portal�򼸺�����Χ��Emitter�����ɿռ���Ƶ������ʵ�ʻ�����λ�ã������ݸ��������档

	#pragma region FuncAkSptialAudioInitSetting() Ĭ�ϲ���
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

	if (AK::SpatialAudio::Init(spatialSettings) != AK_Success) // ԭ�ֲ�Ϊ"AK::SpatialAudio::Init(&setting)
	{
		assert(!"�޷���ʼ���ռ���Ƶ");
		return false;
	}
}

#ifndef AK_OPTIMIZED
#include <AK\Comm\AkCommunication.h>
#endif // AK_OPTIMIZED

bool InitSoundEngine()
{
#ifndef AK_OPTIMIZED
	// ��ͨѶ���г�ʼ���������汾�޴˲��裩
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"�޷���ʼ��ͨѶ����");
		return false;
	}
#endif
}
#pragma endregion

#pragma region ����̨���ͨѶ��
// Windows �׽��ֳ�ʼ����
// WSAData wsaData = {0};
// ::WSAStartup(MAKEWORD(2, 2), &wsaData); FIXME �˴��д���

// ��ֹ
// ::WSACleanup(); FIXME �˴��д���
#pragma endregion

#pragma region ͨѶ�˿�
// AkCommSettings::ports ����Wwise����Ӧ�ó�������������֮�����ͨѶ��ʹ�õ�����˿ڣ���WwiseͨѶ��������ʱ����Щ�˿ڽ�����Ϸ�д�

// �̶��˿� Discovery Broadcast
// AkCommSettings::ports::uDiscoveryBroadcast ���Ƕ�̬����������Ϊ0�����ұ�����Project Settings��Networkѡ���ָ����ֵ
	
// ��̬�˿� AkCommSettings::Ports::uCommand ����ϵͳ���Զ�ѡ��һ���˿�
#pragma endregion

// Wwise��Ƶ�������̣���ʼ��->�ظ�����ִ����Ƶ����ÿ��Ƶ֡һ�Σ�
#pragma region �������洦��
void ProcessAudio()
{
	// ������Ƶ�������¼���λ�á�RTPC��
	AK::SoundEngine::RenderAudio();
}
#pragma endregion


#pragma region ��ֹ���������ģ��
void TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	
	AK::Comm::Term();
	// ��ֹͨѶ����
	
	// AK::SpatialAudio::Term();
	// Wwise SDK 2022.1.0 �ú���������

	AK::MusicEngine::Term();
	// ��ֹ��������

	AK::SoundEngine::Term();
	// ��ֹ�������汾��

	// ��ֹ��ý�岥���豸�������Ź�����
	// CAkFilePackageLowLevelIOBlocking::Term() ���� Stream Manager ��
    // ������ص��������豸����ע���Լ��� File Location Resolver ��ݡ�

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	// ��ֹ�ڴ������
	AK::MemoryMgr::Term();

#endif // !AK_OPTIMIZED
}
#pragma endregion

#pragma region ����ϵͳר���¼�

// 1.��ͣ������Ƶ��Ⱦ bool AK::SoundEngine::Suspend(false);
// 2.������ͣ�����������¼���������Ⱦ��Ƶ�� bool AK::SoundEngine::Suspend(true);
// 3������������Ƶ����ִ�в���

bool AK::SoundEngine::Suspend(false);

// �����Ҫ��������������̨����Ҫ���øú������ᵽǰ̨ʹ��WakeupFromSuspend.
// SoundEngineֻ��RenderAudio����������ʱ����API��Ϣ�����磺PostEvent��SetSwitch��
// ���齫in_bRenderAnyway����������Ϸ���������ֵ���Ϊ��ƥ�䡣
// ��������Ϸ���ں�̨���У��ұ���SoundEngine����Ϸ֮�����ϰ���Ǿ�������Ⱦ��
// ������ں�̨��CPU������ͣ���ô�ɲ�Ҫ������Ⱦ��Ҳ��Ҫ����Ϸ�е���RenderAudio����
// ��ͣ������Ƶ��Ⱦʱ����Ϸ���õ����κ�SoundEngine Api�������¼��Ȳ�������Ч�һ�����ϵͳ����ʱͬʱ���ţ�����ע��������������
// Ϊ�˼���ִ����Ƶ�����ڴ�ϵͳ�յ���ȷ֪ͨʱ�����ȵ��� AK::SoundEngine::WakeupFromSuspend()��Ȼ���ٵ��� RenderAudio()�������ڵ��� RenderAudio() ֮ǰ���������¼��ŶӵȺ�

// HWND������Ƶ��صĴ��ڵľ����ÿ����Ϸ������ָ��Ӧ�ó����HWND���Ա�����豸��⡣
// GetDefaultPlatformInitSettings���ص�ֵ�����������ʼ��ʱ��ǰ̨HWND�����ܲ�������Ϸ����ȷֵ��ÿ����Ϸ�����ṩ��ȷ��HWND��ʹ��

// Ӧ��ƽ̨����ǰ̨ʱ��ý��侲����
// �ɷֱ��� CoreApplication.EnteredBackground �� CoreApplication.LeavingBackground �¼���������е��� Suspend() �� WakeupFromSuspend()��

#pragma endregion




